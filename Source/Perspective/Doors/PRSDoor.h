// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "PRSDoor.Generated.h"

UCLASS()
class PERSPECTIVE_API APRSDoor : public AActor
{
	GENERATED_BODY()

public:
	APRSDoor();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Interaction")
	class APRSInteractableButton* InteractableButton = nullptr;

	virtual void BeginPlay() override;
	
	UFUNCTION()
	virtual void OnButtonPressed() { }
};