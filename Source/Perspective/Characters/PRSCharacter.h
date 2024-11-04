// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Logging/LogMacros.h"

#include "PRSCharacter.generated.h"

class APRSInteractableActor;
class IInteractable;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

UCLASS(config=Game)
class APRSCharacter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

public:
	APRSCharacter();
	
	FVector GetActorForwardVector() const;
	FVector GetActorRightVector() const;
	void Interact();
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	UFUNCTION(BlueprintPure)
	FORCEINLINE bool CanMove() const { return !IsInteracting(); }
	UFUNCTION(BlueprintPure)
	FORCEINLINE bool CanRotate() const { return !IsInteracting(); }
	UFUNCTION(BlueprintPure)
	FORCEINLINE bool IsInteracting() const { return bInteracting; }
	FORCEINLINE bool IsMoving() const { return IsValid(GetCharacterMovement()) && (GetCharacterMovement()->Velocity.X != 0.f || GetCharacterMovement()->Velocity.Y != 0.f); }
	FORCEINLINE void SetForwardVectorOverride (const FVector& ForwardVector) { ForwardVectorOverride = ForwardVector; }

protected:
	UPROPERTY(BlueprintReadWrite)
	APRSInteractableActor* InteractableActor = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	UAnimMontage* InteractionAnimMontage;
	FOnMontageEnded MontageEndedDelegate;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector ForwardVectorOverride = FVector(1.0f, 0.0f, 0.0f);

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	bool bShouldUseForwardVectorOverride = false;
	bool bIsPerspectiveChangedRequiresHandling = false;
	bool bInteracting = false;

	UFUNCTION()
	void OnPerspectiveModeChanged(EPerspectiveMode NewPerspectiveMode);
	void OnMontageEnded(UAnimMontage* AnimMontage, bool bInterrupted);
	UFUNCTION()
	void OnNotifyBeginReceived(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);
	bool PlayInteractionMontage();
};

