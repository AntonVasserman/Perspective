// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "PRSSwitchablePlatform.generated.h"

UCLASS()
class PERSPECTIVE_API APRSSwitchablePlatform : public AStaticMeshActor
{
	GENERATED_BODY()

public:
	APRSSwitchablePlatform();
	
	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	UPROPERTY(EditAnywhere)
	bool bEnabled = true;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Interaction)
	class APRSInteractableButton* InteractableButton = nullptr;
	
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnButtonPressed();
};
