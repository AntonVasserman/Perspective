// Copyright Anton Vasserman, All Rights Reserved.

#include "PRSDoor.h"

#include "Perspective/Interactables/PRSInteractableButton.h"

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