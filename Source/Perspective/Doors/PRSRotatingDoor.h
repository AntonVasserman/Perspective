// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PRSRotatingDoor.generated.h"

UENUM(BlueprintType)
enum class ERotationState : uint8
{
	Static,
	Rotating,
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRotationStateChanged, ERotationState, NewRotationState);

class APRSInteractableButton;

// TODO: Consolidate this and Sliding Door into one parent class
UCLASS()
class PERSPECTIVE_API APRSRotatingDoor : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnRotationStateChanged OnRotationStateChanged;
	
	APRSRotatingDoor();

protected:
	UPROPERTY(BlueprintReadWrite)
	ERotationState CurrentState = ERotationState::Static;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	APRSInteractableButton* InteractableButton = nullptr;
	
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnButtonPressed();
};
