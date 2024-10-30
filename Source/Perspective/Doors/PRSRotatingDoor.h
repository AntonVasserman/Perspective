// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PRSDoor.h"

#include "PRSRotatingDoor.generated.h"

UENUM(BlueprintType)
enum class ERotatingDoorState : uint8
{
	Static,
	Rotating,
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRotatingDoorStateChanged, ERotatingDoorState, NewState);

UCLASS()
class PERSPECTIVE_API APRSRotatingDoor : public APRSDoor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnRotatingDoorStateChanged OnRotationStateChanged;

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
	ERotatingDoorState CurrentState = ERotatingDoorState::Static;
	
	virtual void BeginPlay() override;

	virtual void OnButtonPressed() override;
};
