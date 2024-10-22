// Copyright Anton Vasserman, All Rights Reserved.

#include "PRSSlidingDoor.h"

#include "Perspective/Interactables/PRSInteractableButton.h"

APRSSlidingDoor::APRSSlidingDoor()
{
	PrimaryActorTick.bCanEverTick = true;

}

void APRSSlidingDoor::BeginPlay()
{
	Super::BeginPlay();

	if (InteractableButton != nullptr)
	{
		InteractableButton->OnButtonPressed.AddDynamic(this, &APRSSlidingDoor::OnButtonPressed);
	}
}

void APRSSlidingDoor::OnButtonPressed()
{
	switch (CurrentState)
	{
	case EDoorState::Closed:
		CurrentState = EDoorState::Opening;
		break;
	case EDoorState::Closing:
		break;
	case EDoorState::Open:
		CurrentState = EDoorState::Closing;
	case EDoorState::Opening:
		break;
	default:
		break;
	}

	OnDoorStateChanged.Broadcast(CurrentState);
}
