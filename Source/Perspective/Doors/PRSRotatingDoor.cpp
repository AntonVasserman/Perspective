// Fill out your copyright notice in the Description page of Project Settings.


#include "PRSRotatingDoor.h"

#include "Perspective/PRSInteractableButton.h"

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