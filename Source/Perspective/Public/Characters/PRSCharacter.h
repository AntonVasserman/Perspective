// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Logging/LogMacros.h"

#include "PRSCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInteracted);

UCLASS(config=Game)
class APRSCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	FOnInteracted OnInteracted;
	
	APRSCharacter();
	FORCEINLINE virtual bool CanCrouch() const override { return !IsFalling() && !IsInteracting() && Super::CanCrouch(); }
	UFUNCTION(BlueprintPure)
	FORCEINLINE bool CanMove() const { return !IsInteracting(); }
	UFUNCTION(BlueprintPure)
	FORCEINLINE bool CanRotate() const { return !IsInteracting(); }
	FORCEINLINE bool CanInteract() const { return !IsInteracting() && !IsFalling(); }
	void Interact();
	UFUNCTION(BlueprintPure)
	FORCEINLINE bool IsFalling() const { return IsValid(GetCharacterMovement()) && GetCharacterMovement()->IsFalling(); }
	UFUNCTION(BlueprintPure)
	FORCEINLINE bool IsInteracting() const { return bInteracting; }
	FORCEINLINE bool IsMoving() const { return UKismetMathLibrary::VSizeXY(GetVelocity()) != 0.f; }
	FORCEINLINE void Sprint() const { GetCharacterMovement()->MaxWalkSpeed *= SprintSpeedMultiplier; }
	FORCEINLINE void StopSprint() const { GetCharacterMovement()->MaxWalkSpeed /= SprintSpeedMultiplier; }

protected:
	UPROPERTY(BlueprintReadWrite)
	class APRSInteractableActor* InteractableActor = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config | Animation")
	UAnimMontage* InteractionAnimMontage;
	FOnMontageEnded MontageEndedDelegate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config | Character Movement")
	float CanWalkOffLedgesHeight = 1100.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config | Character Movement")
	float SprintSpeedMultiplier = 2.f;
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	bool bInteracting = false;
	float WalkSpeed = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> SpringArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> CameraComp;
	
	void LineTraceForInteractableActor();
	void LineTraceForLedges();
	UFUNCTION()
	void OnPerspectiveModeChanged(const struct FPerspectiveModeChangedArgs& PerspectiveModeArgs);
	void OnMontageEnded(UAnimMontage* AnimMontage, bool bInterrupted);
	UFUNCTION()
	void OnNotifyBeginReceived(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);
	bool PlayInteractionMontage();
};

