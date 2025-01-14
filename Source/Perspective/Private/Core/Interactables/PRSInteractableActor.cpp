// Copyright Anton Vasserman, All Rights Reserved.

#include "Core/Interactables/PRSInteractableActor.h"

APRSInteractableActor::APRSInteractableActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void APRSInteractableActor::Interact()
{
	OnInteractionStarted.Broadcast();

	const bool bCurrentIsInteractable = IsInteractable();

	if (bCurrentIsInteractable)
	{
		Interact_Implementation();
	}

	OnInteractionEnded.Broadcast(bCurrentIsInteractable);
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
