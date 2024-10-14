// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interactable.generated.h"

UINTERFACE(MinimalAPI)
class UInteractable : public UInterface
{
	GENERATED_BODY()
};

class PERSPECTIVE_API IInteractable
{
	GENERATED_BODY()

public:
	virtual void DisableInteraction() = 0;
	virtual void EnableInteraction() = 0;
	virtual void Interacted() = 0;
	virtual bool IsInteractable() = 0;
};