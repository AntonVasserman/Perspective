// Fill out your copyright notice in the Description page of Project Settings.


#include "PRSInteractableButton.h"

APRSInteractableButton::APRSInteractableButton()
{
	PrimaryActorTick.bCanEverTick = true;

}

void APRSInteractableButton::Interacted()
{
	UE_LOG(LogTemp, Warning, TEXT("APRSInteractableButton::Interacted"));
	OnButtonPressed.Broadcast();
}
