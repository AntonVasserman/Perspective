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