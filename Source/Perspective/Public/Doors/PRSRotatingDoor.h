// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PRSDoor.h"
#include "Core/Utility/PRSSoundStatics.h"
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

	virtual void Operate_Implementation() override;

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
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config | Sound")
	class USoundCue* OpenCloseSound = UPRSSoundStatics::GetRotatingDoorOpenCloseSoundCue();
	
	UPROPERTY(BlueprintReadWrite)
	ERotatingDoorState CurrentState = ERotatingDoorState::Static;
	
	virtual void BeginPlay() override;
};
