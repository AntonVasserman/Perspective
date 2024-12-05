// Copyright Anton Vasserman, All Rights Reserved.


#include "PRSCharacterAnimInstance.h"

#include "Kismet/KismetMathLibrary.h"
#include "Perspective/Characters/PRSCharacter.h"

void UPRSCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	OwnerCharacter = Cast<APRSCharacter>(TryGetPawnOwner());
}

void UPRSCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (OwnerCharacter)
	{
		EvaluateDirection();
		EvaluateFalling();
		EvaluateInteracting();
		EvaluateRotating();
		EvaluateSpeed();
	}
}

void UPRSCharacterAnimInstance::EvaluateDirection()
{
	Direction = FMath::Sign(
		FVector::DotProduct(
			Cast<AActor>(OwnerCharacter)->GetActorForwardVector().GetSafeNormal(),
			OwnerCharacter->GetVelocity().GetSafeNormal()));
}

void UPRSCharacterAnimInstance::EvaluateFalling()
{
	bFalling = OwnerCharacter->GetCharacterMovement()->IsFalling();
}

void UPRSCharacterAnimInstance::EvaluateSpeed()
{
	Speed = UKismetMathLibrary::VSizeXY(OwnerCharacter->GetVelocity());
}

void UPRSCharacterAnimInstance::EvaluateInteracting()
{
	bInteracting = OwnerCharacter->IsInteracting();
}

void UPRSCharacterAnimInstance::EvaluateRotating()
{
	if (const FRotator CurrRotation = OwnerCharacter->GetActorRotation();
		PrevRotation.Equals(CurrRotation))
	{
		bRotating = false;
	}
	else
	{
		bRotating = true;
		PrevRotation = CurrRotation;
	}
}
