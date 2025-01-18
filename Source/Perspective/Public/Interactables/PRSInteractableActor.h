// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "PRSInteractableActor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInteractionSucceeded);

UCLASS(Abstract)
class PERSPECTIVE_API APRSInteractableActor : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable)
	FOnInteractionSucceeded OnInteractionSucceeded;
	
	APRSInteractableActor();
	
	void Interact();
	UFUNCTION(BlueprintCallable, BlueprintPure)
	virtual bool IsInteractable() { return bInteractable; }

	virtual void Highlight() { }
	virtual void UnHighlight() { }

protected:
	UFUNCTION(BlueprintCallable)
	void SetInteractable(const bool bInInteractable);

	UFUNCTION()
	virtual void Interact_Implementation() { }

private:
	bool bInteractable = true;
};
