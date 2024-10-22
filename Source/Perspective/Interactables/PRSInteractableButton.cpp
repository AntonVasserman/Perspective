// Copyright Anton Vasserman, All Rights Reserved.

#include "PRSInteractableButton.h"

APRSInteractableButton::APRSInteractableButton()
{
	PrimaryActorTick.bCanEverTick = true;

}

void APRSInteractableButton::Interact()
{
	if (IsInteractable())
	{
		bPressed = true;
		OnButtonPressed.Broadcast();
	}
}
