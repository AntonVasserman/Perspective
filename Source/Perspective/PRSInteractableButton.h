// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Interactable.h"
#include "PRSInteractableButton.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnButtonPressed);

UCLASS()
class PERSPECTIVE_API APRSInteractableButton : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable)
	FOnButtonPressed OnButtonPressed;

	APRSInteractableButton();
	virtual void Interacted() override;
};
