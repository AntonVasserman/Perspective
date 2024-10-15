// Fill out your copyright notice in the Description page of Project Settings.

#include "PRSSlidingDoor.h"

#include "Perspective/PRSInteractableButton.h"

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
