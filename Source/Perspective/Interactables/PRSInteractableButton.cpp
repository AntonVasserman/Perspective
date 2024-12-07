// Copyright Anton Vasserman, All Rights Reserved.

#include "PRSInteractableButton.h"

#include "Perspective/Core/Utility/PRSSoundStatics.h"
#include "Sound/SoundCue.h"

APRSInteractableButton::APRSInteractableButton()
{
	PrimaryActorTick.bCanEverTick = true;
}

void APRSInteractableButton::Interact()
{
	if (IsInteractable())
	{
		bPressed = true;
		UPRSSoundStatics::PlaySoundAtLocation(GetWorld(), ButtonClickSound, GetActorLocation(), GetActorRotation());
		OnButtonPressed.Broadcast();
	}
}
