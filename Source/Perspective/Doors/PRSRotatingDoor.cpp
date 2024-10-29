// Copyright Anton Vasserman, All Rights Reserved.

#include "PRSRotatingDoor.h"

#include "Perspective/Interactables/PRSInteractableButton.h"

APRSRotatingDoor::APRSRotatingDoor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void APRSRotatingDoor::BeginPlay()
{
	Super::BeginPlay();

	if (InteractableButton != nullptr)
	{
		InteractableButton->OnButtonPressed.AddDynamic(this, &APRSRotatingDoor::OnButtonPressed);
	}

	YawRotationDelta = bCounterClockwise ? -90.f : 90.f;
	OriginalRotation = GetActorRotation();
	NewRotation = bSupportFullRotation ?
		OriginalRotation :
		FRotator(OriginalRotation.Pitch, OriginalRotation.Yaw + YawRotationDelta, OriginalRotation.Roll);
}

void APRSRotatingDoor::OnButtonPressed()
{
	switch (CurrentState)
	{
	case ERotationState::Static:
		CurrentState = ERotationState::Rotating;
		break;
	case ERotationState::Rotating:
		break;
	default:
		break;
	}

	OnRotationStateChanged.Broadcast(CurrentState);
}