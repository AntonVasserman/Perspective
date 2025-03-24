// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PRSDoor.h"
#include "GameFramework/Actor.h"
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
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config | Sound")
	USoundBase* OpenCloseSound = nullptr;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Config")
	bool bCloseOnOpened = false;

	UPROPERTY(BlueprintReadWrite)
	ESlidingDoorState CurrentState = ESlidingDoorState::Closed;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Config")
	bool bOpenOnGameStart = false;
	
	//~ APRSDoor Begin
public:
	virtual void Operate_Implementation() override;
protected:
	virtual void BeginPlay() override;
	//~ APRSDoor End
};
