// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/Interactables/PRSInteractableActor.h"
#include "GameFramework/Actor.h"
#include "PRSInteractableButton.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnButtonPressed);

UCLASS()
class PERSPECTIVE_API APRSInteractableButton : public APRSInteractableActor
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnButtonPressed OnButtonPressed;

	APRSInteractableButton();
	
	virtual void Interact() override;
	FORCEINLINE virtual bool IsInteractable() override { return Super::IsInteractable() && !bPressed; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void ResetButton() { bPressed = false; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bRepressable = false;

private:
	bool bPressed = false;
};
