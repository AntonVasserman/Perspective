// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PRSDoor.h"
#include "GameFramework/Actor.h"
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
	UPROPERTY(BlueprintReadWrite)
	FRotator OriginalRotation = FRotator::ZeroRotator;

	UPROPERTY(BlueprintReadWrite)
	FRotator NextRotation = FRotator::ZeroRotator;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	bool bSupportFullRotation = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	bool bCounterClockwise = false;

	UPROPERTY(BlueprintReadOnly)
	FRotator RotationDelta = FRotator::ZeroRotator;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config | Sound")
	USoundBase* OpenCloseSound = nullptr;
	
	UPROPERTY(BlueprintReadWrite)
	ERotatingDoorState CurrentState = ERotatingDoorState::Static;
	
	//~ APRSDoor Begin
public:
	virtual void Operate_Implementation() override;
protected:
	virtual void BeginPlay() override;
	//~ APRSDoor End
};
