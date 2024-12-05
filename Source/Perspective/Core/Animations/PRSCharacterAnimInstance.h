// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PRSCharacterAnimInstance.generated.h"

UCLASS()
class PERSPECTIVE_API UPRSCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	class APRSCharacter* OwnerCharacter;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float Direction = 0.f;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	float Speed = 0.f;
	
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	bool bInteracting = false;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	bool bRotating = false;
	FRotator PrevRotation;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	bool bFalling = false;
	
	void EvaluateDirection();
	void EvaluateFalling();
	void EvaluateSpeed();
	void EvaluateInteracting();
	void EvaluateRotating();
};
