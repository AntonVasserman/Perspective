// Copyright Anton Vasserman, All Rights Reserved.

#include "Core/Interactables/PRSInteractableActor.h"

APRSInteractableActor::APRSInteractableActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void APRSInteractableActor::Interact()
{
	OnInteractionStarted.Broadcast();

	if (IsInteractable())
	{
		Interact_Implementation();
		OnInteractionSucceeded.Broadcast();
	}

	OnInteractionFailed.Broadcast();
}

void APRSInteractableActor::DisableInteraction()
{
	bInteractable = false;
	OnInteractionStateChanged.Broadcast(bInteractable);
}

void APRSInteractableActor::EnableInteraction()
{
	bInteractable = true;
	OnInteractionStateChanged.Broadcast(bInteractable);
}
