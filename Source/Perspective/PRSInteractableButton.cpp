// Fill out your copyright notice in the Description page of Project Settings.


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
