// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Logging/LogMacros.h"

#include "PRSCharacter.generated.h"

UCLASS(config=Game)
class APRSCharacter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* CameraComp;

public:
	APRSCharacter();
	
	FVector GetActorForwardVector() const;
	FVector GetActorRightVector() const;
	void Interact();
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
	class APRSInteractableActor* InteractableActor = nullptr;
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

	void LineTraceForInteractableActor();
	UFUNCTION()
	void OnPerspectiveModeChanged(EPerspectiveMode NewPerspectiveMode);
	void OnMontageEnded(UAnimMontage* AnimMontage, bool bInterrupted);
	UFUNCTION()
	void OnNotifyBeginReceived(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);
	bool PlayInteractionMontage();
};

