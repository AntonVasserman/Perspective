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

void APRSInteractableActor::Highlight_Implementation()
{
}

void APRSInteractableActor::UnHighlight_Implementation()
{
}

void APRSInteractableActor::SetInteractable(const bool bInInteractable)
{
	bInteractable = bInInteractable;
}
