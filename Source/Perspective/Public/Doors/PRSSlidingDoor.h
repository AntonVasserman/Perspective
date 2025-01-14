// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PRSDoor.h"
#include "Core/Utility/PRSSoundStatics.h"
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

	virtual void Operate_Implementation() override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config | Sound")
	class USoundCue* OpenCloseSound = UPRSSoundStatics::GetSlidingDoorOpenCloseSoundCue();
	
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Config")
	bool bCloseOnOpened = false;

	UPROPERTY(BlueprintReadWrite)
	ESlidingDoorState CurrentState = ESlidingDoorState::Closed;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Config")
	bool bOpenOnGameStart = false;
};
