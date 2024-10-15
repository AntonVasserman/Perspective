// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PRSDoor.generated.h"

UENUM(BlueprintType)
enum class EDoorState : uint8
{
	Closed,
	Opening,
	Open,
	Closing
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDoorStateChanged, EDoorState, NewDoorState);

class APRSInteractableButton;

UCLASS()
class PERSPECTIVE_API APRSDoor : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnDoorStateChanged OnDoorStateChanged;
	
	APRSDoor();

protected:
	UPROPERTY(BlueprintReadWrite)
	EDoorState CurrentState = EDoorState::Closed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	APRSInteractableButton* InteractableButton = nullptr;

	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnButtonPressed();
};
