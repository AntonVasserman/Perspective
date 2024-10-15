// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PRSSlidingDoor.generated.h"

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

// TODO: Consolidate this and Sliding Door into one parent class
UCLASS()
class PERSPECTIVE_API APRSSlidingDoor : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnDoorStateChanged OnDoorStateChanged;
	
	APRSSlidingDoor();

protected:
	UPROPERTY(BlueprintReadWrite)
	EDoorState CurrentState = EDoorState::Closed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	APRSInteractableButton* InteractableButton = nullptr;

	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnButtonPressed();
};
