// Copyright Epic Games, Inc. All Rights Reserved.

#include "PRSCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/LocalPlayer.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "Interfaces/Interactable.h"
#include "Kismet/KismetMathLibrary.h"
#include "PRSModeWorldSubsystem.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

static TAutoConsoleVariable<bool> CVarDisplayTraceLine(
	TEXT("Perspective.Character.Debug.DisplayTraceLine"),
	false,
	TEXT("Display Trace Line"),
	ECVF_Default);

APRSCharacter::APRSCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 200.0f; // The camera follows at this distance behind the character
	CameraBoom->SocketOffset = FVector(0.f, 75.f, 70.f);
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->SetRelativeRotation(FRotator(-5.f, -4.f, 0.f));
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
}

void APRSCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	UPRSModeWorldSubsystem* PerspectiveModeWorldSubsystem = GetWorld()->GetSubsystem<UPRSModeWorldSubsystem>();
	PerspectiveModeWorldSubsystem->OnPerspectiveModeChanged.AddDynamic(this, &APRSCharacter::OnPerspectiveModeChanged);
}

void APRSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	LineTraceForInteractableActor();
	
	if (bIsPerspectiveChangedRequiresHandling && !IsMoving())
	{
		if (const EPerspectiveMode PerspectiveMode = GetWorld()->GetSubsystem<UPRSModeWorldSubsystem>()->GetMode();
			PerspectiveMode == EPerspectiveMode::TwoDimensional)
		{
			bIsPerspectiveChangedRequiresHandling = false;
		}
		else if (PerspectiveMode == EPerspectiveMode::ThreeDimensional)
		{
			bShouldUseForwardVectorOverride = false;
			bIsPerspectiveChangedRequiresHandling = false;
		}
	}
}

FVector APRSCharacter::GetForwardVector() const
{
	if (bShouldUseForwardVectorOverride)
	{
		return ForwardVectorOverride;
	}

	// find out which way is forward
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);
	const FRotationMatrix YawRotationMatrix(YawRotation);

	// get forward vector
	return YawRotationMatrix.GetUnitAxis(EAxis::X);
}

FVector APRSCharacter::GetRightVector() const
{
	if (bShouldUseForwardVectorOverride)
	{
		return ForwardVectorOverride;
	}

	// find out which way is forward
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);
	const FRotationMatrix YawRotationMatrix(YawRotation);

	// get right vector 
	return YawRotationMatrix.GetUnitAxis(EAxis::Y);
}

void APRSCharacter::Interact()
{
	UE_LOG(LogTemp, Warning, TEXT("APRSCharacter::Interact"));
	if (InteractableActor != nullptr)
	{
		// TODO: Disable Controller Input while interacting (Maybe enable input, once interacted notified)
		// TODO: Add Blend between Interaction and Movement, so we can move after interacted notified with normal animation
		PlayInteractionMontage();
	}
}

void APRSCharacter::OnPerspectiveModeChanged(EPerspectiveMode NewPerspectiveMode)
{
	bIsPerspectiveChangedRequiresHandling = true;

	switch (NewPerspectiveMode)
	{
	case EPerspectiveMode::TwoDimensional:
		bShouldUseForwardVectorOverride = true;
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;

		CameraBoom->SocketOffset = FVector(0.f, 0.f, 0.f);
		CameraBoom->bUsePawnControlRotation = false;
		CameraBoom->SetRelativeRotation(UKismetMathLibrary::MakeRotFromX(ForwardVectorOverride));
		CameraBoom->AddRelativeRotation(FRotator(0.f, -90.f, 0.f));
		CameraBoom->bInheritYaw = false;

		FollowCamera->SetProjectionMode(ECameraProjectionMode::Orthographic);
		FollowCamera->SetOrthoWidth(1024.0f); // Increase the Orthographic width, we have to do it here only after the projection mode has changed
		break;
	case EPerspectiveMode::ThreeDimensional:
		FollowCamera->SetProjectionMode(ECameraProjectionMode::Perspective);

		CameraBoom->bUsePawnControlRotation = true;
		CameraBoom->bInheritYaw = true;
		CameraBoom->SocketOffset = FVector(0.f, 75.f, 70.f);
		
		GetCharacterMovement()->bOrientRotationToMovement = false;
		bUseControllerRotationYaw = true;
		break;
	}
}

void APRSCharacter::LineTraceForInteractableActor()
{
	// Get the start and end points for the line trace
	constexpr float TraceUnits = 150.f;
	constexpr float TraceZOffset = 50.f;
	const FVector Start = GetActorLocation() + FVector(0.f, 0.f, TraceZOffset);
	const FVector End = Start + GetActorForwardVector() * TraceUnits; // Trace 150 units forward

	FHitResult HitResult;

	// Line trace parameters
	FCollisionQueryParams TraceParams(FName(TEXT("Trace")), true, this);
	TraceParams.bReturnPhysicalMaterial = false;
	TraceParams.bTraceComplex = true;

	// Perform the line trace
	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, TraceParams))
	{
		if (AActor* HitActor = HitResult.GetActor();
			IsValid(HitActor) && HitActor->Implements<UInteractable>())
		{
#if ENABLE_DRAW_DEBUG
			if (CVarDisplayTraceLine->GetBool())
			{
				DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 0.f, 0, 1.f);
			}
#endif
			InteractableActor = Cast<IInteractable>(HitActor);
			return;
		}

#if ENABLE_DRAW_DEBUG
		if (CVarDisplayTraceLine->GetBool())
		{
			DrawDebugLine(GetWorld(), Start, End, FColor::Yellow, false, 0.f, 0, 1.f);
		}
#endif
	}
	else
	{
#if ENABLE_DRAW_DEBUG
		if (CVarDisplayTraceLine->GetBool())
		{
			DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 0.f, 0, 1.f);
		}
#endif
	}

	if (InteractableActor != nullptr)
	{
		InteractableActor = nullptr;
	}
}

void APRSCharacter::OnMontageEnded(UAnimMontage* AnimMontage, bool bInterrupted)
{
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		AnimInstance->OnPlayMontageNotifyBegin.RemoveDynamic(this, &APRSCharacter::OnNotifyBeginReceived);
	}
}

void APRSCharacter::OnNotifyBeginReceived(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	if (NotifyName == FName("Interacted"))
	{
		// We do the check again, in case the player somehow moved away from the interactable actor
		if (InteractableActor != nullptr)
		{
			InteractableActor->Interacted();
		}
	}
}

bool APRSCharacter::PlayInteractionMontage()
{
	const bool bPlayedSuccessfully = PlayAnimMontage(InteractionAnimMontage, 1.f) > 0.f;

	if (bPlayedSuccessfully)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

		if (!MontageEndedDelegate.IsBound())
		{
			// TODO (Refactor): Try to move this to Begin play
			MontageEndedDelegate.BindUObject(this, &APRSCharacter::OnMontageEnded);
		}

		AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, InteractionAnimMontage);
		
		AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &APRSCharacter::OnNotifyBeginReceived);
	}

	return bPlayedSuccessfully;
}
