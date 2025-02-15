// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Interactables/PRSInteractableActor.h"
#include "Core/Utility/PRSSoundStatics.h"
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

	virtual void Highlight() override;
	virtual void UnHighlight() override;
	virtual void PostInitializeComponents() override;
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void ResetButton() const { OnResetButtonRequested.Broadcast(); }
	virtual void Tick(float DeltaSeconds) override;

protected:
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Config")
	bool bRepressable = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config | Sound")
	USoundCue* ButtonClickSound = nullptr;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Config")
	TArray<class APRSOperatableActor*> OperatableActors;

	virtual void Interact_Implementation() override;

	UFUNCTION(BlueprintCallable)
	void TryEnableInteraction();

	UFUNCTION(BlueprintImplementableEvent)
	void Highlight_Implementation();

	UFUNCTION(BlueprintImplementableEvent)
	void UnHighlight_Implementation();
	
private:
	UFUNCTION()
	void OnOperationStateChanged(const bool bOperatable);
};
