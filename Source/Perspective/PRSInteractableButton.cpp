// Fill out your copyright notice in the Description page of Project Settings.


#include "PRSInteractableButton.h"

APRSInteractableButton::APRSInteractableButton()
{
	PrimaryActorTick.bCanEverTick = true;

}

void APRSInteractableButton::TickActor(float DeltaTime, ELevelTick TickType, FActorTickFunction& ThisTickFunction)
{
	Super::TickActor(DeltaTime, TickType, ThisTickFunction);

	if (bInteractable)
	{
		// TODO: Move this into an InteractableActor that will also support a proper widget instead of text, for keyboard/controller (both PS & Xbox)
		DrawDebugString(GetWorld(), FVector::Zero(), TEXT("Press 'E' to Interact"), this, FColor::Blue, 0.f);
	}
}

// TODO: Remove once switched to trigger boxes
void APRSInteractableButton::DisableInteraction()
{
	bInteractable = false;
}

// TODO: Remove once switched to trigger boxes
void APRSInteractableButton::EnableInteraction()
{
	bInteractable = true;
}

void APRSInteractableButton::Interacted()
{
	if (bInteractable && !bPressed)
	{
		bPressed = true;
		OnButtonPressed.Broadcast();
	}
}
