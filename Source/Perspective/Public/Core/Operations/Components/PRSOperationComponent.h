// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PRSOperationComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PERSPECTIVE_API UPRSOperationComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPRSOperationComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config | Interaction")
	TArray<class APRSInteractableButton*> InteractableButtons;

	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void ExecuteOperation();
};
