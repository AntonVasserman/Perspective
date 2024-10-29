// Copyright Anton Vasserman, All Rights Reserved.

#include "PRSPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Perspective/Characters/PRSCharacter.h"
#include "Perspective/Subsystems/PRSModeWorldSubsystem.h"

// TODO (Refactor #14): Gamepad and Keyboard rotation requires fix. The rotation in the Gamepad is faster using 'Movement' than it is using the Gamepad look function.
void APRSPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}

	UPRSModeWorldSubsystem* PerspectiveModeWorldSubsystem = GetWorld()->GetSubsystem<UPRSModeWorldSubsystem>();
	PerspectiveModeWorldSubsystem->OnPerspectiveModeChanged.AddDynamic(this, &APRSPlayerController::OnPerspectiveModeChanged);
}

void APRSPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	PossessedCharacter = Cast<APRSCharacter>(InPawn);
}

void APRSPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APRSPlayerController::RequestLookAction);
	EnhancedInputComponent->BindAction(MoveXAction, ETriggerEvent::Triggered, this, &APRSPlayerController::RequestMoveXAction);
	EnhancedInputComponent->BindAction(MoveXAction, ETriggerEvent::Completed, this, &APRSPlayerController::RequestMoveActionCompleted);
	EnhancedInputComponent->BindAction(MoveYAction, ETriggerEvent::Triggered, this, &APRSPlayerController::RequestMoveYAction);
	EnhancedInputComponent->BindAction(MoveYAction, ETriggerEvent::Completed, this, &APRSPlayerController::RequestMoveActionCompleted);
	EnhancedInputComponent->BindAction(InteractionAction, ETriggerEvent::Started, this, &APRSPlayerController::RequestInteractionAction);
}

void APRSPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!PossessedCharacter->IsMoving())
	{
		bMovingByX = false;
		bMovingByY = false;
	}
}

void APRSPlayerController::RequestMoveXAction(const FInputActionValue& InputActionValue)
{
	if (!PossessedCharacter->CanMove())
	{
		return;
	}

	const float MovementValue = InputActionValue.Get<float>();

	if (bEnableYInput) // 3D
	{
		if (bMovingByY)
		{
			HandlePerspectiveChanged();
			bMovingByY = false;
		}
		
		AddYawInput(MovementValue * BaseLookRightRate * LookMultiplierForMoveControls * GetWorld()->GetDeltaSeconds());
	}
	else // 2D
	{
		bMovingByX = true;
		const float NormalizedMovementValue = bIsPerspectiveChangedRequiresHandling ? FMath::Abs(MovementValue) : MovementValue;
		PossessedCharacter->AddMovementInput(PossessedCharacter->GetActorRightVector(), NormalizedMovementValue);
	}
}

void APRSPlayerController::RequestMoveYAction(const FInputActionValue& InputActionValue)
{
	if (!PossessedCharacter->CanMove())
	{
		return;
	}

	if (bEnableYInput) // 3D
	{
		bMovingByY = true;
		const float MovementValue = InputActionValue.Get<float>();
		const float InputToApply = MovementValue >= 0.f ? MovementValue : MovementValue * BackwardMovementMultiplier;
		PossessedCharacter->AddMovementInput(PossessedCharacter->GetActorForwardVector(), InputToApply);
	}
	else
	{
		if (bMovingByX)
		{
			HandlePerspectiveChanged();
			bMovingByX = false;
		}
	}
}

void APRSPlayerController::RequestMoveActionCompleted()
{
	if (bIsPerspectiveChangedRequiresHandling)
	{
		HandlePerspectiveChanged();
	}
}

void APRSPlayerController::RequestLookAction(const FInputActionValue& InputActionValue)
{
	if (!PossessedCharacter->CanRotate() || !bEnableYInput)
	{
		return;
	}

	const FVector2D LookVector = InputActionValue.Get<FVector2D>();
	
	AddYawInput(LookVector.X * BaseLookRightRate * GetWorld()->GetDeltaSeconds());
	AddPitchInput(LookVector.Y * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void APRSPlayerController::RequestInteractionAction()
{
	UE_LOG(LogTemp, Warning, TEXT("APRSPlayerController::RequestInteractionAction"));
	PossessedCharacter->Interact();
}

void APRSPlayerController::OnPerspectiveModeChanged(EPerspectiveMode NewPerspectiveMode)
{
	bIsPerspectiveChangedRequiresHandling = true;

	if (NewPerspectiveMode == EPerspectiveMode::TwoDimensional)
	{
		// Save previous Pitch rotation to restore it upon exiting 2D mode
		PreviousControllerPitchRotation = GetControlRotation().Pitch;
	}
	else
	{
		// Restore the previous Pitch rotation
		SetControlRotation(FRotator(PreviousControllerPitchRotation, PossessedCharacter->GetActorRotation().Yaw, 0.f));
	}
}

void APRSPlayerController::HandlePerspectiveChanged()
{
	switch (GetWorld()->GetSubsystem<UPRSModeWorldSubsystem>()->GetMode())
	{
	case EPerspectiveMode::TwoDimensional:
		bEnableYInput = false;
		break;
	case EPerspectiveMode::ThreeDimensional:
		bEnableYInput = true;
		break;
	}

	bIsPerspectiveChangedRequiresHandling = false;
}