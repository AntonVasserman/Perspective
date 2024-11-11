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

	for (APRSInteractableButton* Button : InteractableButtons)
	{
		if (Button == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("Door: %s, has a null button set"), *this->GetName());
			continue;
		}
		
		Button->OnButtonPressed.AddDynamic(this, &APRSDoor::OnButtonPressed);
	}
}
