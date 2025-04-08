// Copyright Anton Vasserman, All Rights Reserved.

#include "Characters/PRSCharacter.h"

#include "MotionWarpingComponent.h"
#include "Animation/PRSCharacterAnimationConstants.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Interactables/PRSInteractableActor.h"
#include "Subsystems/PerspectiveModeChangedArgs.h"
#include "Subsystems/PRSModeWorldSubsystem.h"

APRSCharacter::APRSCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(25.f, 96.0f);
		
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Setup character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
	GetCharacterMovement()->MinAnalogWalkSpeed = MinWalkSpeed;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
	GetCharacterMovement()->bCanWalkOffLedges = true;
	GetCharacterMovement()->bUseFlatBaseForFloorChecks = true;
	GetCharacterMovement()->SetWalkableFloorAngle(46.f);
	
	// Setup Spring Arm Component
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArmComp->SetupAttachment(GetRootComponent());
	SpringArmComp->TargetArmLength = 300.0f;
	SpringArmComp->SocketOffset = FVector(0.f, 75.f, 70.f);
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->bEnableCameraLag = true;
	SpringArmComp->CameraLagSpeed = 16.f;

	// Setup Camera Component
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	CameraComp->SetRelativeRotation(FRotator(-5.f, -4.f, 0.f));
	CameraComp->bUsePawnControlRotation = false;

	// Setup Motion Warping Component
	MotionWarpingComp = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("Motion Warping"));
}

void APRSCharacter::Interact()
{
	if (InteractionTarget != nullptr && InteractionTarget->IsInteractable())
	{
		bInteracting = true;
		
		// In case the interaction montage fails allow interaction 
		if(!PlayInteractionMontage())
		{
			bInteracting = false;
		}
	}
}

void APRSCharacter::Sprint()
{
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	bSprinting = true;
}

void APRSCharacter::StopSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
	bSprinting = false;
}

void APRSCharacter::SetInteractionTarget(APRSInteractableActor* InInteractionTarget)
{
	if (InteractionTarget != nullptr && InteractionTarget != InInteractionTarget)
	{
		UnsetInteractionTarget(InteractionTarget);
	}
	
	InteractionTarget = InInteractionTarget;
	InteractionTarget->Highlight();
	MotionWarpingComp->AddOrUpdateWarpTargetFromLocation(FPRSCharacterAnimationConstants::InteractionTargetWarpTargetName, InteractionTarget->GetActorLocation());
}

void APRSCharacter::UnsetInteractionTarget(APRSInteractableActor* InInteractionTarget)
{
	if (InteractionTarget != InInteractionTarget)
	{
		return;
	}
	
	InteractionTarget->UnHighlight();
	MotionWarpingComp->RemoveWarpTarget(FPRSCharacterAnimationConstants::InteractionTargetWarpTargetName);
	InteractionTarget = nullptr;
}

void APRSCharacter::LineTraceForInteractionTarget()
{
	// We do not detect interactables using Line Traces in 3D mode, only 2D mode
	if (const EPerspectiveMode CurrentPerspectiveMode = GetWorld()->GetSubsystem<UPRSModeWorldSubsystem>()->GetMode();
		CurrentPerspectiveMode == EPerspectiveMode::ThreeDimensional)
	{
		return;
	}
	
	// Get the start and end points for the line trace
	const float TraceAxisOffset = GetCapsuleComponent()->GetScaledCapsuleRadius();;

	constexpr float TraceUnits = 10000.f;
	const FVector Start = GetActorLocation() + FVector(0.f, TraceAxisOffset, TraceAxisOffset);
	const FVector End = Start + CameraComp->GetForwardVector() * TraceUnits;

	// Line trace parameters
	FCollisionQueryParams TraceParams(FName(TEXT("Trace")), true, this);
	TraceParams.bReturnPhysicalMaterial = false;
	TraceParams.bTraceComplex = true;

	// Perform the line trace
	if (FHitResult HitResult;
		GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, TraceParams))
	{
		if (AActor* HitActor = HitResult.GetActor();
			IsValid(HitActor))
		{
			// We do the next calculation as we don't want to be able to interact with interactables that don't face us!
			const FVector HitActorToImpactDirection = (HitResult.ImpactPoint - HitActor->GetActorLocation()).GetSafeNormal();
			const float DotProduct = FVector::DotProduct(HitActor->GetActorForwardVector(), HitActorToImpactDirection);

			// Convert dot product to angle in degrees (everything in (0, 90) will result in the interactable facing us)
			if (FMath::RadiansToDegrees(FMath::Acos(DotProduct)) < 90.f)
			{
				if (APRSInteractableActor* HitInteractableActor = Cast<APRSInteractableActor>(HitActor);
					IsValid(HitInteractableActor))
				{
					if (HitInteractableActor != InteractionTarget)
					{
						SetInteractionTarget(HitInteractableActor);
					}
					
					return;
				}
			}

			if (InteractionTarget != nullptr)
			{
				UnsetInteractionTarget(InteractionTarget);
			}
		}
	}
	else
	{
		if (InteractionTarget != nullptr)
		{
			UnsetInteractionTarget(InteractionTarget);
		}
	}
}

void APRSCharacter::LineTraceForLedges()
{
	const FVector CharacterLocation = GetActorLocation();
	const FVector Forward = GetActorForwardVector();
	const FVector Right = GetActorRightVector();

	TArray<FVector> Directions = {
		Forward,
		-Forward,
		Right,
		-Right,
		(Forward + Right).GetSafeNormal(),
		(Forward - Right).GetSafeNormal(),
		(-Forward + Right).GetSafeNormal(),
		(-Forward - Right).GetSafeNormal()
	};

	for (const FVector& Direction : Directions)
	{
		constexpr float TraceCharacterOffset = 50.f;
		FVector Start = CharacterLocation + TraceCharacterOffset * Direction;
		FVector End = Start - FVector(0, 0, CanWalkOffLedgesHeight);

		FHitResult HitResult;
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(this);

		if (const bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, CollisionParams);
			!bHit)
		{
			GetCharacterMovement()->bCanWalkOffLedges = false;
			return;
		}
	}

	GetCharacterMovement()->bCanWalkOffLedges = true;
}

void APRSCharacter::OnPerspectiveModeChanged(const FPerspectiveModeChangedArgs& NewPerspectiveArgs)
{
	if (NewPerspectiveArgs.Mode == EPerspectiveMode::TwoDimensional)
	{
		SpringArmComp->SocketOffset = FVector(0.f, 0.f, 0.f);
		SpringArmComp->bInheritYaw = false;
		SpringArmComp->bUsePawnControlRotation = false;
		SpringArmComp->SetRelativeRotation(NewPerspectiveArgs.NewControlRotation);
		SpringArmComp->AddRelativeRotation(FRotator(0.f, -90.f, 0.f));

		CameraComp->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
		CameraComp->SetProjectionMode(ECameraProjectionMode::Orthographic);
		CameraComp->SetOrthoWidth(1024.0f); // Increase the Orthographic width, we have to do it here only after the projection mode has changed
	}
	else if (NewPerspectiveArgs.Mode == EPerspectiveMode::ThreeDimensional)
	{
		CameraComp->SetProjectionMode(ECameraProjectionMode::Perspective);
		CameraComp->SetRelativeRotation(FRotator(-5.f, -4.f, 0.f));

		SpringArmComp->bUsePawnControlRotation = true;
		SpringArmComp->bInheritYaw = true;
		SpringArmComp->SocketOffset = FVector(0.f, 75.f, 70.f);
	}
}

// TODO: Why do we have to add/remove this event.
// Why not add it once on begin play and let it run for all MontageNotifies...
// This should be fine since APRSCharacter::OnNotifyBeginReceived is generic
void APRSCharacter::OnMontageEnded(UAnimMontage* AnimMontage, bool bInterrupted)
{
	GetMesh()->GetAnimInstance()->OnPlayMontageNotifyBegin.RemoveDynamic(this, &APRSCharacter::OnNotifyBeginReceived);
}

void APRSCharacter::OnNotifyBeginReceived(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	if (NotifyName == FPRSCharacterAnimationConstants::InteractedNotifyName)
	{
		// We do the check again, in case the player somehow moved away from the interactable actor
		if (InteractionTarget != nullptr)
		{
			InteractionTarget->Interact();
			OnInteracted.Broadcast();
		}

		bInteracting = false;
	}
}

bool APRSCharacter::PlayInteractionMontage()
{
	const bool bPlayedSuccessfully = PlayAnimMontage(InteractionAnimMontage, 1.f) > 0.f;

	if (bPlayedSuccessfully)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, InteractionAnimMontage);
		AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &APRSCharacter::OnNotifyBeginReceived);
	}

	return bPlayedSuccessfully;
}

//~ ACharacter Begin

void APRSCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	UPRSModeWorldSubsystem* PerspectiveModeWorldSubsystem = GetWorld()->GetSubsystem<UPRSModeWorldSubsystem>();
	PerspectiveModeWorldSubsystem->OnPerspectiveModeChanged.AddDynamic(this, &APRSCharacter::OnPerspectiveModeChanged);
	
	MontageEndedDelegate.BindUObject(this, &APRSCharacter::OnMontageEnded);
}

void APRSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	LineTraceForLedges();
	LineTraceForInteractionTarget();

	// TODO: This should be part of the highlight logic, and I believe it should be in the InteractableActor, instead of here...
	if (InteractionTarget != nullptr && InteractionTarget->IsInteractable())
	{
		if (!InteractionTarget->IsHighlighted())
		{
			InteractionTarget->Highlight();
		}
		DrawDebugString(GetWorld(), FVector(0.f, 0.f, 50.f), TEXT("Press 'E' to Interact"), this, FColor::Red, 0.f);
	}
	if (InteractionTarget != nullptr && !InteractionTarget->IsInteractable())
	{
		if (InteractionTarget->IsHighlighted())
		{
			InteractionTarget->UnHighlight();
		}
	}

	// If sprinting and current speed is lower than regular running speed minus some delta then turn off sprinting
	if (bSprinting && UKismetMathLibrary::VSizeXY(GetVelocity()) < 500.f - 0.1f)
	{
		StopSprint();
	}
}

//~ ACharacter End
