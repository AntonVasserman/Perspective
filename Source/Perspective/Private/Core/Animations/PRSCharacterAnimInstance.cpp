// Copyright Anton Vasserman, All Rights Reserved.

#include "Core/Animations/PRSCharacterAnimInstance.h"

#include "Kismet/KismetMathLibrary.h"
#include "Characters/PRSCharacter.h"

void UPRSCharacterAnimInstance::EvaluateCrouching()
{
	bCrouching = OwnerCharacter->GetCharacterMovement()->IsCrouching();
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

//~ UAnimInstance Begin

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
		EvaluateCrouching();
		EvaluateDirection();
		EvaluateFalling();
		EvaluateInteracting();
		EvaluateSpeed();
	}
}

//~ UAnimInstance End

