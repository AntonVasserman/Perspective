// Copyright Anton Vasserman, All Rights Reserved.

#include "Interactables/PRSInteractableButton.h"

#include "Core/Utility/PRSSoundStatics.h"
#include "Sound/SoundCue.h"

APRSInteractableButton::APRSInteractableButton()
{
	PrimaryActorTick.bCanEverTick = true;
}

void APRSInteractableButton::Interact_Implementation()
{
	UPRSSoundStatics::PlaySoundAtLocation(GetWorld(), ButtonClickSound, GetActorLocation(), GetActorRotation());
	DisableInteraction();
}
