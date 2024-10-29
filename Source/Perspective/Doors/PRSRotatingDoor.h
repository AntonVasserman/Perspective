// Copyright Anton Vasserman, All Rights Reserved.

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

// TODO (Refactor #15): Consolidate this and Sliding Door into one parent class
UCLASS()
class PERSPECTIVE_API APRSRotatingDoor : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnRotationStateChanged OnRotationStateChanged;
	
	APRSRotatingDoor();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator OriginalRotation = FRotator::ZeroRotator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator NewRotation = FRotator::ZeroRotator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bSupportFullRotation = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCounterClockwise = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float YawRotationDelta = 0.f;
	
	UPROPERTY(BlueprintReadWrite)
	ERotationState CurrentState = ERotationState::Static;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	APRSInteractableButton* InteractableButton = nullptr;
	
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnButtonPressed();
private:
	// TODO
};
