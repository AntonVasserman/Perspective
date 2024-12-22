// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "PRSInteractableActor.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FInteractionStateChangedDelegate, bool);

UCLASS()
class PERSPECTIVE_API APRSInteractableActor : public AActor
{
	GENERATED_BODY()
	
public:
	FInteractionStateChangedDelegate OnInteractionStateChanged;
	
	APRSInteractableActor();
	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	virtual void DisableInteraction();
	UFUNCTION(BlueprintCallable)
	virtual void EnableInteraction();
	virtual void Interact();
	UFUNCTION(BlueprintCallable)
	virtual bool IsInteractable();

protected:
	bool bInteractable = false;
};
