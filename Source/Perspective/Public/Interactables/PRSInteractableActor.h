// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "PRSInteractableActor.generated.h"

class UBoxComponent;

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
	FORCEINLINE bool IsInteractable() const { return bInteractable; }
	void Highlight();
	UFUNCTION(BlueprintImplementableEvent, DisplayName = "Highlight")
	void Highlight_BP();
	void UnHighlight();
	UFUNCTION(BlueprintImplementableEvent, DisplayName = "UnHighlight")
	void UnHighlight_BP();
	FORCEINLINE bool IsHighlighted() const { return bHighlighted; }

protected:
	UFUNCTION(BlueprintCallable)
	void SetInteractable(const bool bInInteractable);

	// TODO: see if there is a more correct way to implement this...
	UFUNCTION()
	virtual void Interact_Implementation() { }

private:
	bool bInteractable = true;
	bool bHighlighted = false;
	
	UPROPERTY(VisibleInstanceOnly, meta=(AllowPrivateAccess=true))
	TObjectPtr<USceneComponent> DefaultRootSceneComp;
	
	UPROPERTY(VisibleInstanceOnly, meta=(AllowPrivateAccess=true))
	TObjectPtr<UBoxComponent> BoxCollisionComp;

	UFUNCTION()
	void OnBoxCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnBoxCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	//~ AActor Begin
public:
	virtual void PostInitializeComponents() override;
	//~ AActor End
};
