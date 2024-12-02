// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PRSDoor.h"
#include "Perspective/Core/Operations/Interfaces/PRSOperatableInterface.h"
#include "PRSRotatingDoor.generated.h"

UENUM(BlueprintType)
enum class ERotatingDoorState : uint8
{
	Static,
	Rotating,
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRotatingDoorStateChanged, ERotatingDoorState, NewState);

UCLASS()
class PERSPECTIVE_API APRSRotatingDoor : public APRSDoor, public IPRSOperatableInterface
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnRotatingDoorStateChanged OnRotationStateChanged;

	virtual void Operate() override;

protected:
	UPROPERTY(BlueprintReadWrite)
	FRotator OriginalRotation = FRotator::ZeroRotator;

	UPROPERTY(BlueprintReadWrite)
	FRotator NewRotation = FRotator::ZeroRotator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	bool bSupportFullRotation = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	bool bCounterClockwise = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	float YawRotationDelta = 0.f;
	
	UPROPERTY(BlueprintReadWrite)
	ERotatingDoorState CurrentState = ERotatingDoorState::Static;
	
	virtual void BeginPlay() override;
};
