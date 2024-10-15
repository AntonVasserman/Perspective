// Fill out your copyright notice in the Description page of Project Settings.


#include "PRSDoor.h"

#include "PRSInteractableButton.h"

APRSDoor::APRSDoor()
{
	PrimaryActorTick.bCanEverTick = true;

}

void APRSDoor::BeginPlay()
{
	Super::BeginPlay();

	if (InteractableButton != nullptr)
	{
		InteractableButton->OnButtonPressed.AddDynamic(this, &APRSDoor::OnButtonPressed);
	}
}

void APRSDoor::OnButtonPressed()
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
