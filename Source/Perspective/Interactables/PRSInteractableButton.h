// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Perspective/Core/Interactables/PRSInteractableActor.h"
#include "Perspective/Core/Utility/PRSSoundStatics.h"

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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config")
	bool bRepressable = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config | Sound")
	class USoundCue* ButtonClickSound = UPRSSoundStatics::GetButtonClickSoundCue();

private:
	bool bPressed = false;
};
