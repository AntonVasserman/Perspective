// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Interactable.h"
#include "PRSInteractableButton.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnButtonPressed);

class UUserWidget;

UCLASS()
class PERSPECTIVE_API APRSInteractableButton : public AActor, public IInteractable
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnButtonPressed OnButtonPressed;

	APRSInteractableButton();
	
	virtual void TickActor(float DeltaTime, ELevelTick TickType, FActorTickFunction& ThisTickFunction) override;
	
	virtual void DisableInteraction() override;
	virtual void EnableInteraction() override;
	virtual void Interacted() override;
	FORCEINLINE virtual bool IsInteractable() override { return bInteractable && !bPressed; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void ResetButton() { bPressed = false; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bRePressable = false;
	
private:
	bool bInteractable = false;
	bool bPressed = false;
};
