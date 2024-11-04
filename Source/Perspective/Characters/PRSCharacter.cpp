// Copyright Anton Vasserman, All Rights Reserved.

#include "PRSCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Perspective/Core/Interactables/PRSInteractableActor.h"
#include "Perspective/Subsystems/PRSModeWorldSubsystem.h"

APRSCharacter::APRSCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// Setup character movement
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Setup Spring Arm Component
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 200.0f;
	SpringArmComp->SocketOffset = FVector(0.f, 75.f, 70.f);
	SpringArmComp->bUsePawnControlRotation = true;

	// Setup Camera Component
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	CameraComp->SetRelativeRotation(FRotator(-5.f, -4.f, 0.f));
	CameraComp->bUsePawnControlRotation = false;

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

		SpringArmComp->SocketOffset = FVector(0.f, 0.f, 0.f);
		SpringArmComp->bUsePawnControlRotation = false;
		SpringArmComp->SetRelativeRotation(UKismetMathLibrary::MakeRotFromX(ForwardVectorOverride));
		SpringArmComp->AddRelativeRotation(FRotator(0.f, -90.f, 0.f));
		SpringArmComp->bInheritYaw = false;

		CameraComp->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
		CameraComp->SetProjectionMode(ECameraProjectionMode::Orthographic);
		CameraComp->SetOrthoWidth(1024.0f); // Increase the Orthographic width, we have to do it here only after the projection mode has changed
	}
	else if (NewPerspectiveMode == EPerspectiveMode::ThreeDimensional)
	{
		CameraComp->SetProjectionMode(ECameraProjectionMode::Perspective);
		CameraComp->SetRelativeRotation(FRotator(-5.f, -4.f, 0.f));

		SpringArmComp->bUsePawnControlRotation = true;
		SpringArmComp->bInheritYaw = true;
		SpringArmComp->SocketOffset = FVector(0.f, 75.f, 70.f);
		
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
