// Copyright Epic Games, Inc. All Rights Reserved.

#include "PerspectiveCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "PerspectiveModeWorldSubsystem.h"

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
		const EPerspectiveMode CurrentPerspectiveMode = GetWorld()->GetSubsystem<UPerspectiveModeWorldSubsystem>()->GetMode();
		const bool bIsCharacterMoving = GetCharacterMovement()->Velocity.X == 0.f && GetCharacterMovement()->Velocity.Y == 0.f; 

		if (bIsCharacterMoving && CurrentPerspectiveMode == EPerspectiveMode::ThreeDimensional)
		{
			bEnableYInput = true;
			bShouldUseForwardVectorOverride = false;
			bIsPerspectiveChanged = false;
		}
		else if (bIsCharacterMoving && CurrentPerspectiveMode == EPerspectiveMode::TwoDimensional)
		{
			bEnableYInput = false;
			bIsPerspectiveChanged = false;
		}
	}
}

void APerspectiveCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Add Input Mapping Context
	if (const APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APerspectiveCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APerspectiveCharacter::Look);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void APerspectiveCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	const FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		FVector ForwardVector;
		FVector RightVector;

		if (bShouldUseForwardVectorOverride)
		{
			ForwardVector = ForwardVectorOverride;
			RightVector = ForwardVectorOverride;
		}
		else
		{
			// find out which way is forward
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);
			const FRotationMatrix YawRotationMatrix(YawRotation);

			// get forward vector
			ForwardVector = YawRotationMatrix.GetUnitAxis(EAxis::X);
	
			// get right vector 
			RightVector = YawRotationMatrix.GetUnitAxis(EAxis::Y);
		}
		
		if (bEnableYInput)
		{
			AddMovementInput(ForwardVector, MovementVector.Y);
		}
		
		AddMovementInput(RightVector, MovementVector.X);
	}
}

void APerspectiveCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void APerspectiveCharacter::OnPerspectiveModeChanged(EPerspectiveMode NewPerspectiveMode)
{
	bIsPerspectiveChanged = true;

	if (NewPerspectiveMode == EPerspectiveMode::TwoDimensional)
	{
		bShouldUseForwardVectorOverride = true;

		CameraBoom->bUsePawnControlRotation = false;
		CameraBoom->AddRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
		FollowCamera->SetProjectionMode(ECameraProjectionMode::Orthographic);
		FollowCamera->SetOrthoWidth(1024.0f);
	}
	else if (NewPerspectiveMode == EPerspectiveMode::ThreeDimensional)
	{
		CameraBoom->bUsePawnControlRotation = true;
		CameraBoom->AddRelativeRotation(FRotator(0.0f, +90.0f, 0.0f));
		FollowCamera->SetProjectionMode(ECameraProjectionMode::Perspective);
	}
}
