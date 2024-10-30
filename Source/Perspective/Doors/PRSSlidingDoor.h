// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PRSDoor.h"

#include "PRSSlidingDoor.generated.h"

UENUM(BlueprintType)
enum class ESlidingDoorState : uint8
{
	Closed,
	Opening,
	Open,
	Closing
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSlidingDoorStateChanged, ESlidingDoorState, NewState);

UCLASS()
class PERSPECTIVE_API APRSSlidingDoor : public APRSDoor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnSlidingDoorStateChanged OnDoorStateChanged;

protected:
	UPROPERTY(BlueprintReadWrite)
	ESlidingDoorState CurrentState = ESlidingDoorState::Closed;

	virtual void OnButtonPressed() override;
};
