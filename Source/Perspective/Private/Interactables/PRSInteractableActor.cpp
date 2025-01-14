// Copyright Anton Vasserman, All Rights Reserved.

#include "Interactables/PRSInteractableActor.h"

APRSInteractableActor::APRSInteractableActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void APRSInteractableActor::Interact()
{
	if (IsInteractable())
	{
		Interact_Implementation();
		OnInteractionSucceeded.Broadcast();
	}
}

void APRSInteractableActor::SetInteractable(const bool bInInteractable)
{
	bInteractable = bInInteractable;
}
