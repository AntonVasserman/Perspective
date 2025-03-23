// Copyright Anton Vasserman, All Rights Reserved.

#include "Core/Controllers/PRSPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Characters/PRSCharacter.h"
#include "Subsystems/PerspectiveModeChangedArgs.h"
#include "Subsystems/PRSModeWorldSubsystem.h"

void APRSPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bAwaitingPerspectiveChangeHandling && (!PossessedCharacter->IsMoving() || bMovementInputChanged))
	{
		bAwaitingPerspectiveChangeHandling = false;
	}
}

void APRSPlayerController::BeginPlay()
{
	Super::BeginPlay();

	check(DefaultMappingContext);
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}

	PerspectiveModeWorldSubsystem = GetWorld()->GetSubsystem<UPRSModeWorldSubsystem>();
	PerspectiveModeWorldSubsystem->OnPerspectiveModeChanged.AddDynamic(this, &APRSPlayerController::OnPerspectiveModeChanged);
}

void APRSPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	PossessedCharacter = Cast<APRSCharacter>(InPawn);

	PossessedCharacter->OnInteracted.AddDynamic(this, &APRSPlayerController::OnPlayerCharacterInteracted);
}

void APRSPlayerController::OnUnPossess()
{
	Super::OnUnPossess();

	PossessedCharacter = nullptr;
}

void APRSPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

	check(LookAction);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APRSPlayerController::RequestLookAction);
	check(MoveAction);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APRSPlayerController::RequestMoveAction);
	check(InteractAction);
	EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &APRSPlayerController::RequestInteractionAction);
	check(SprintAction);
	EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &APRSPlayerController::RequestSprintAction);
}

void APRSPlayerController::RequestInteractionAction()
{
	if (!PossessedCharacter->CanInteract())
	{
		return;
	}
	
	PossessedCharacter->Interact();
}

void APRSPlayerController::RequestLookAction(const FInputActionValue& InputActionValue)
{
	if (!PossessedCharacter->CanRotate() || PerspectiveModeWorldSubsystem->GetMode() == EPerspectiveMode::TwoDimensional)
	{
		return;
	}

	const FVector2D LookVector = InputActionValue.Get<FVector2D>();
	
	AddYawInput(LookVector.X * BaseLookRightRate * GetWorld()->GetDeltaSeconds());
	AddPitchInput(LookVector.Y * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void APRSPlayerController::RequestMoveAction(const FInputActionValue& InputActionValue)
{
	if (!PossessedCharacter->CanMove())
	{
		return;
	}

	const FVector2D MovementValue = InputActionValue.Get<FVector2D>();
	if (PreviousMovementInput != MovementValue)
	{
		PreviousMovementInput = MovementValue;
		bMovementInputChanged = true;
	}
	else
	{
		bMovementInputChanged = false;
	}
	
	const FRotator ControllerRotation = GetControlRotation();
	const FRotationMatrix YawControllerRotationMatrix = FRotationMatrix(FRotator(0.f, ControllerRotation.Yaw, 0.f));

	if (PerspectiveModeWorldSubsystem->GetMode() == EPerspectiveMode::TwoDimensional)
	{
		if (bAwaitingPerspectiveChangeHandling)
		{
			PossessedCharacter->AddMovementInput(YawControllerRotationMatrix.GetUnitAxis(EAxis::X), FMath::Abs(MovementValue.Y));
			PossessedCharacter->AddMovementInput(YawControllerRotationMatrix.GetUnitAxis(EAxis::X), FMath::Abs(MovementValue.X));
		}
		else
		{
			PossessedCharacter->AddMovementInput(YawControllerRotationMatrix.GetUnitAxis(EAxis::X), MovementValue.X);	
		}
	}
	else if (PerspectiveModeWorldSubsystem->GetMode() == EPerspectiveMode::ThreeDimensional)
	{
		if (bAwaitingPerspectiveChangeHandling)
		{
			PossessedCharacter->AddMovementInput(YawControllerRotationMatrix.GetUnitAxis(EAxis::X), FMath::Abs(MovementValue.X));
		}
		else
		{
			PossessedCharacter->AddMovementInput(YawControllerRotationMatrix.GetUnitAxis(EAxis::X), MovementValue.Y);
			PossessedCharacter->AddMovementInput(YawControllerRotationMatrix.GetUnitAxis(EAxis::Y), MovementValue.X);
		}
	}
}

void APRSPlayerController::RequestSprintAction(const FInputActionValue& InputActionValue)
{
	if (PossessedCharacter->IsSprinting())
	{
		PossessedCharacter->StopSprint();
	}
	else
	{
		PossessedCharacter->Sprint();
	}
}

void APRSPlayerController::OnPerspectiveModeChanged(const FPerspectiveModeChangedArgs& NewPerspectiveArgs)
{
	if (NewPerspectiveArgs.Mode == EPerspectiveMode::TwoDimensional)
	{
		// Save previous Pitch rotation to restore it upon exiting 2D mode
		PreviousPitchRotation = GetControlRotation().Pitch;
		SetControlRotation(NewPerspectiveArgs.NewControlRotation);
		if (NewPerspectiveArgs.bOverridePlayerCharacterX || NewPerspectiveArgs.bOverridePlayerCharacterY)
		{
			const FVector& CharacterCurrentLocation = PossessedCharacter->GetActorLocation();
			const FVector& NewCharacterLocation = FVector(
				NewPerspectiveArgs.bOverridePlayerCharacterX ? NewPerspectiveArgs.PlayerCharacterXOverride : CharacterCurrentLocation.X,
				NewPerspectiveArgs.bOverridePlayerCharacterY ? NewPerspectiveArgs.PlayerCharacterYOverride : CharacterCurrentLocation.Y,
				CharacterCurrentLocation.Z);
			PossessedCharacter->SetActorLocation(NewCharacterLocation);
		}
	}
	else if (NewPerspectiveArgs.Mode == EPerspectiveMode::ThreeDimensional)
	{
		// Restore the previous Pitch rotation, and use new Yaw rotation
		SetControlRotation(FRotator(PreviousPitchRotation, NewPerspectiveArgs.NewControlRotation.Yaw, 0.f));
	}

	bAwaitingPerspectiveChangeHandling = !bAwaitingPerspectiveChangeHandling;
	// No need to check the feedback here for nullptr, as it is allowed to be nullptr in ClientPlayForceFeedback.
	ClientPlayForceFeedback(PerspectiveChangedForceFeedbackEffect);
}

void APRSPlayerController::OnPlayerCharacterInteracted()
{
	// No need to check the feedback here for nullptr, as it is allowed to be nullptr in ClientPlayForceFeedback.
	ClientPlayForceFeedback(InteractForceFeedbackEffect);
}
