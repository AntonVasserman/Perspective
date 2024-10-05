// Fill out your copyright notice in the Description page of Project Settings.


#include "PerspectivePlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "PerspectiveCharacter.h"
#include "PerspectiveModeWorldSubsystem.h"

void APerspectivePlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}

	UPerspectiveModeWorldSubsystem* PerspectiveModeWorldSubsystem = GetWorld()->GetSubsystem<UPerspectiveModeWorldSubsystem>();
	PerspectiveModeWorldSubsystem->OnPerspectiveModeChanged.AddDynamic(this, &APerspectivePlayerController::OnPerspectiveModeChanged);
}

void APerspectivePlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APerspectivePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APerspectivePlayerController::RequestLookAction);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APerspectivePlayerController::RequestMoveAction);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &APerspectivePlayerController::RequestMoveActionCompleted);
}

void APerspectivePlayerController::RequestMoveAction(const FInputActionValue& InputActionValue)
{
	const FVector2D MovementVector = InputActionValue.Get<FVector2D>();
	const float MovementVectorX = bIsPerspectiveChangedRequiresHandling ? FMath::Abs(MovementVector.X) : MovementVector.X;
	const float MovementVectorY = MovementVector.Y;

	if (APerspectiveCharacter* ControllerCharacter = Cast<APerspectiveCharacter>(GetCharacter()))
	{
		if (bEnableYInput)
		{
			ControllerCharacter->AddMovementInput(ControllerCharacter->GetForwardVector(), MovementVectorY);
		}
		
		ControllerCharacter->AddMovementInput(ControllerCharacter->GetRightVector(), MovementVectorX);
	}
}

void APerspectivePlayerController::RequestMoveActionCompleted()
{
	if (bIsPerspectiveChangedRequiresHandling)
	{
		switch (GetWorld()->GetSubsystem<UPerspectiveModeWorldSubsystem>()->GetMode())
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

void APerspectivePlayerController::RequestLookAction(const FInputActionValue& InputActionValue)
{
	const FVector2D LookVector = InputActionValue.Get<FVector2D>();
	
	AddYawInput(LookVector.X * BaseLookRightRate * GetWorld()->GetDeltaSeconds());
	AddPitchInput(LookVector.Y * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void APerspectivePlayerController::OnPerspectiveModeChanged(EPerspectiveMode NewPerspectiveMode)
{
	bIsPerspectiveChangedRequiresHandling = true;

	switch (NewPerspectiveMode)
	{
	case EPerspectiveMode::TwoDimensional:
			// Save previous Pitch rotation to restore it upon exiting 2D mode
			PreviousControllerPitchRotation = GetControlRotation().Pitch;
		break;
	case EPerspectiveMode::ThreeDimensional:
			// Restore the previous Pitch rotation
			SetControlRotation(FRotator(PreviousControllerPitchRotation, 0.f, 0.f));
		break;
	}
}
