// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "PRSInteractableActor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteractionStateChanged, const bool, bInteractable);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInteractionStarted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteractionEnded, const bool, bInteractionSucceeded);

UCLASS(Abstract)
class PERSPECTIVE_API APRSInteractableActor : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable)
	FOnInteractionStateChanged OnInteractionStateChanged;
	UPROPERTY(BlueprintAssignable)
	FOnInteractionStarted OnInteractionStarted;
	UPROPERTY(BlueprintAssignable)
	FOnInteractionEnded OnInteractionEnded;
	
	APRSInteractableActor();
	
	void Interact();
	UFUNCTION(BlueprintCallable)
	virtual bool IsInteractable() { return bInteractable; }

protected:
	bool bInteractable = true;

	UFUNCTION(BlueprintCallable)
	void DisableInteraction();

	UFUNCTION(BlueprintCallable)
	void EnableInteraction();
	
	UFUNCTION()
	virtual void Interact_Implementation() { }
};
