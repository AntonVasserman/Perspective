// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Core/Interactables/PRSInteractableActor.h"
#include "Core/Utility/PRSSoundStatics.h"
#include "GameFramework/Actor.h"

#include "PRSInteractableButton.generated.h"

UCLASS()
class PERSPECTIVE_API APRSInteractableButton : public APRSInteractableActor
{
	GENERATED_BODY()

public:
	APRSInteractableButton();
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void ResetButton() { EnableInteraction(); }

protected:
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Config")
	bool bRepressable = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config | Sound")
	class USoundCue* ButtonClickSound = UPRSSoundStatics::GetButtonClickSoundCue();

	virtual void Interact_Implementation() override;
};
