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
	
	void Interact();
	UFUNCTION(BlueprintPure)
	FORCEINLINE bool CanMove() const { return !IsInteracting(); }
	UFUNCTION(BlueprintPure)
	FORCEINLINE bool CanRotate() const { return !IsInteracting(); }
	FORCEINLINE bool CanInteract() const { return !IsInteracting(); }
	UFUNCTION(BlueprintPure)
	FORCEINLINE bool IsInteracting() const { return bInteracting; }
	FORCEINLINE bool IsMoving() const { return IsValid(GetCharacterMovement()) && (GetCharacterMovement()->Velocity.X != 0.f || GetCharacterMovement()->Velocity.Y != 0.f); }
	FORCEINLINE void Sprint() const { GetCharacterMovement()->MaxWalkSpeed *= SprintSpeedMultiplier; }
	FORCEINLINE void StopSprint() const { GetCharacterMovement()->MaxWalkSpeed /= SprintSpeedMultiplier; }

protected:
	UPROPERTY(BlueprintReadWrite)
	class APRSInteractableActor* InteractableActor = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config | Animation")
	UAnimMontage* InteractionAnimMontage;
	FOnMontageEnded MontageEndedDelegate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config | Character Movement")
	float CanWalkOffLedgesHeight = 2500.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config | Character Movement")
	float SprintSpeedMultiplier = 2.f;
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	bool bInteracting = false;
	float WalkSpeed = 0.f;

	void LineTraceForInteractableActor();
	void LineTraceForLedges();
	UFUNCTION()
	void OnPerspectiveModeChanged(const struct FPerspectiveModeChangedArgs& PerspectiveModeArgs);
	void OnMontageEnded(UAnimMontage* AnimMontage, bool bInterrupted);
	UFUNCTION()
	void OnNotifyBeginReceived(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);
	bool PlayInteractionMontage();
};

