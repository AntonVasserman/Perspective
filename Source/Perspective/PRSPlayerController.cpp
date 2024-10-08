// Fill out your copyright notice in the Description page of Project Settings.


#include "PRSPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "PRSCharacter.h"
#include "PRSModeWorldSubsystem.h"

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

void APRSPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	PossessedCharacter = Cast<APRSCharacter>(aPawn);
}

void APRSPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APRSPlayerController::RequestLookAction);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APRSPlayerController::RequestMoveAction);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &APRSPlayerController::RequestMoveActionCompleted);
	EnhancedInputComponent->BindAction(InteractionAction, ETriggerEvent::Started, this, &APRSPlayerController::RequestInteractionAction);
}

void APRSPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APRSPlayerController::RequestMoveAction(const FInputActionValue& InputActionValue)
{
	const FVector2D MovementVector = InputActionValue.Get<FVector2D>();
	const float MovementVectorX = bIsPerspectiveChangedRequiresHandling ? FMath::Abs(MovementVector.X) : MovementVector.X;
	const float MovementVectorY = MovementVector.Y;
	
	if (bEnableYInput)
	{
		PossessedCharacter->AddMovementInput(PossessedCharacter->GetForwardVector(), MovementVectorY);
	}
	
	PossessedCharacter->AddMovementInput(PossessedCharacter->GetRightVector(), MovementVectorX);
}

void APRSPlayerController::RequestMoveActionCompleted()
{
	if (bIsPerspectiveChangedRequiresHandling)
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
}

void APRSPlayerController::RequestLookAction(const FInputActionValue& InputActionValue)
{
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
