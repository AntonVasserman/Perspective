// Copyright Epic Games, Inc. All Rights Reserved.

#include "PerspectiveCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "PerspectiveModeWorldSubsystem.h"
#include "Kismet/KismetMathLibrary.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

APerspectiveCharacter::APerspectiveCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
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
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void APerspectiveCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	UPerspectiveModeWorldSubsystem* PerspectiveModeWorldSubsystem = GetWorld()->GetSubsystem<UPerspectiveModeWorldSubsystem>();
	PerspectiveModeWorldSubsystem->OnPerspectiveModeChanged.AddDynamic(this, &APerspectiveCharacter::OnPerspectiveModeChanged);
}

void APerspectiveCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsPerspectiveChanged)
	{
		if (IsMoving())
		{
			switch (GetWorld()->GetSubsystem<UPerspectiveModeWorldSubsystem>()->GetMode())
			{
			case EPerspectiveMode::TwoDimensional:
				bIsPerspectiveChanged = false;
				break;
			case EPerspectiveMode::ThreeDimensional:
				bShouldUseForwardVectorOverride = false;
				bIsPerspectiveChanged = false;
				break;
			}
		}
	}
}

FVector APerspectiveCharacter::GetForwardVector() const
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

FVector APerspectiveCharacter::GetRightVector() const
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

void APerspectiveCharacter::OnPerspectiveModeChanged(EPerspectiveMode NewPerspectiveMode)
{
	bIsPerspectiveChanged = true;

	switch (NewPerspectiveMode)
	{
	case EPerspectiveMode::TwoDimensional:
		bShouldUseForwardVectorOverride = true;

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
		break;
	}
}
