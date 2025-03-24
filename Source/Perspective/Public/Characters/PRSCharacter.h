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
	FORCEINLINE bool IsSprinting() const { return bSprinting; }
	void Sprint();
	void StopSprint();

protected:
	UPROPERTY(BlueprintReadWrite)
	class APRSInteractableActor* InteractableActor = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config | Animation")
	UAnimMontage* InteractionAnimMontage;
	FOnMontageEnded MontageEndedDelegate;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Config | Character Movement")
	float CanWalkOffLedgesHeight = 1100.f;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	bool bInteracting = false;
	bool bSprinting = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> SpringArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> CameraComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation | Motion Warping", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UMotionWarpingComponent> MotionWarpingComp;
	
	UPROPERTY(EditDefaultsOnly, Category = "Config | Character Movement", meta = (AllowPrivateAccess = "true"))
	float MinWalkSpeed = 20.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Config | Character Movement", meta = (AllowPrivateAccess = "true"))
	float RunSpeed = 500.f;

	UPROPERTY(EditDefaultsOnly, Category = "Config | Character Movement", meta = (AllowPrivateAccess = "true"))
	float SprintSpeed = 1000.f;
	
	void LineTraceForInteractableActor();
	void LineTraceForLedges();
	UFUNCTION()
	void OnPerspectiveModeChanged(const struct FPerspectiveModeChangedArgs& PerspectiveModeArgs);
	void OnMontageEnded(UAnimMontage* AnimMontage, bool bInterrupted);
	UFUNCTION()
	void OnNotifyBeginReceived(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);
	bool PlayInteractionMontage();
};

