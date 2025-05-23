// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Interactables/PRSInteractableActor.h"
#include "GameFramework/Actor.h"

#include "PRSInteractableButton.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnResetButtonRequested);

UCLASS()
class PERSPECTIVE_API APRSInteractableButton : public APRSInteractableActor
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnResetButtonRequested OnResetButtonRequested;
	APRSInteractableButton();

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void ResetButton() const { OnResetButtonRequested.Broadcast(); }

protected:
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Config")
	bool bRepressable = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config | Sound")
	USoundBase* ButtonClickSound = nullptr;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Config")
	TArray<class APRSOperatableActor*> OperatableActors;
	
	UFUNCTION(BlueprintCallable)
	void TryEnableInteraction();

private:
	UFUNCTION()
	void OnOperationStateChanged(const bool bOperatable);

	//~ APRSInteractableActor Begin
public:
	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaSeconds) override;
protected:
	virtual void Interact_Implementation() override;
	//~ APRSInteractableActor End
};
