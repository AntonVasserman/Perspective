// Copyright Anton Vasserman, All Rights Reserved.

#include "PRSInteractableActor.h"

APRSInteractableActor::APRSInteractableActor()
{
	PrimaryActorTick.bCanEverTick = true;

}

void APRSInteractableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (IsInteractable())
	{
		DrawDebugString(GetWorld(), FVector::Zero(), TEXT("Press 'E' to Interact"), this, FColor::Blue, 0.f);
	}
}

void APRSInteractableActor::DisableInteraction()
{
	bInteractable = false;
	OnInteractionStateChanged.Broadcast(false);
}

void APRSInteractableActor::EnableInteraction()
{
	bInteractable = true;
	OnInteractionStateChanged.Broadcast(true);
}

void APRSInteractableActor::Interact()
{
}

bool APRSInteractableActor::IsInteractable()
{
	return bInteractable;
}
