// Copyright Anton Vasserman, All Rights Reserved.

#include "PRSCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/LocalPlayer.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "Kismet/KismetMathLibrary.h"
#include "Perspective/Core/Interactables/PRSInteractableActor.h"
#include "Perspective/Subsystems/PRSModeWorldSubsystem.h"

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

	MontageEndedDelegate.BindUObject(this, &APRSCharacter::OnMontageEnded);
}

FVector APRSCharacter::GetActorForwardVector() const
{
	if (bShouldUseForwardVectorOverride)
	{
		return ForwardVectorOverride;
	}

	return ACharacter::GetActorForwardVector();
}

FVector APRSCharacter::GetActorRightVector() const
{
	if (bShouldUseForwardVectorOverride)
	{
		return ForwardVectorOverride;
	}

	return ACharacter::GetActorRightVector();
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

void APRSCharacter::OnPerspectiveModeChanged(EPerspectiveMode NewPerspectiveMode)
{
	bIsPerspectiveChangedRequiresHandling = true;

	if (NewPerspectiveMode == EPerspectiveMode::TwoDimensional)
	{
		bShouldUseForwardVectorOverride = true;
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;

		CameraBoom->SocketOffset = FVector(0.f, 0.f, 0.f);
		CameraBoom->bUsePawnControlRotation = false;
		CameraBoom->SetRelativeRotation(UKismetMathLibrary::MakeRotFromX(ForwardVectorOverride));
		CameraBoom->AddRelativeRotation(FRotator(0.f, -90.f, 0.f));
		CameraBoom->bInheritYaw = false;

		FollowCamera->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
		FollowCamera->SetProjectionMode(ECameraProjectionMode::Orthographic);
		FollowCamera->SetOrthoWidth(1024.0f); // Increase the Orthographic width, we have to do it here only after the projection mode has changed
	}
	else if (NewPerspectiveMode == EPerspectiveMode::ThreeDimensional)
	{
		FollowCamera->SetProjectionMode(ECameraProjectionMode::Perspective);
		FollowCamera->SetRelativeRotation(FRotator(-5.f, -4.f, 0.f));

		CameraBoom->bUsePawnControlRotation = true;
		CameraBoom->bInheritYaw = true;
		CameraBoom->SocketOffset = FVector(0.f, 75.f, 70.f);
		
		GetCharacterMovement()->bOrientRotationToMovement = false;
		bUseControllerRotationYaw = true;
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
