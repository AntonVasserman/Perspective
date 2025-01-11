// Copyright Anton Vasserman, All Rights Reserved.

#include "Characters/PRSCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Core/Interactables/PRSInteractableActor.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetInputLibrary.h"
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
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
	GetCharacterMovement()->bCanWalkOffLedges = true;
	GetCharacterMovement()->bUseFlatBaseForFloorChecks = true;
	GetCharacterMovement()->SetCrouchedHalfHeight(72.5f);
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;
	GetCharacterMovement()->SetWalkableFloorAngle(46.f);
	
	// Setup Spring Arm Component
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArmComp->SetupAttachment(RootComponent);
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
}

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
	LineTraceForInteractableActor();
}

void APRSCharacter::Interact()
{
	if (InteractableActor != nullptr && InteractableActor->IsInteractable())
	{
		bInteracting = true;
		
		// In case the interaction montage fails allow interaction 
		if(!PlayInteractionMontage())
		{
			bInteracting = false;
		}
	}
}

void APRSCharacter::LineTraceForInteractableActor()
{
	// Get Current Game Perspective Mode
	const EPerspectiveMode CurrentPerspectiveMode = GetWorld()->GetSubsystem<UPRSModeWorldSubsystem>()->GetMode();
	
	// Get the start and end points for the line trace
	const float TraceAxisOffset = GetCapsuleComponent()->GetScaledCapsuleRadius();;

	float TraceUnits;
	FVector Start;
	FVector End;

	switch (CurrentPerspectiveMode)
	{
		case EPerspectiveMode::TwoDimensional:
			TraceUnits = 10000.f;
			Start = GetActorLocation() + FVector(0.f, TraceAxisOffset, TraceAxisOffset);
			End = Start + CameraComp->GetForwardVector() * TraceUnits;
			break;
		case EPerspectiveMode::ThreeDimensional:
			TraceUnits = 150.f;
			Start = GetActorLocation() + FVector(0.f, 0.f, TraceAxisOffset);
			End = Start + GetActorForwardVector() * TraceUnits;
			break;
		default:
			break;
	}
	
	// Line trace parameters
	FCollisionQueryParams TraceParams(FName(TEXT("Trace")), true, this);
	TraceParams.bReturnPhysicalMaterial = false;
	TraceParams.bTraceComplex = true;

	// Perform the line trace
	if (FHitResult HitResult; GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, TraceParams))
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
					InteractableActor = HitInteractableActor;
					InteractableActor->EnableInteraction();
					return;
				}
			}
		}
	}

	if (InteractableActor != nullptr)
	{
		InteractableActor->DisableInteraction();
		InteractableActor = nullptr;
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

void APRSCharacter::OnMontageEnded(UAnimMontage* AnimMontage, bool bInterrupted)
{
	GetMesh()->GetAnimInstance()->OnPlayMontageNotifyBegin.RemoveDynamic(this, &APRSCharacter::OnNotifyBeginReceived);
}

void APRSCharacter::OnNotifyBeginReceived(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	if (NotifyName == FName("Interacted"))
	{
		// We do the check again, in case the player somehow moved away from the interactable actor
		if (InteractableActor != nullptr)
		{
			InteractableActor->Interact();
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
