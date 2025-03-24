// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DoorColor.h"
#include "Subsystems/WorldSubsystem.h"
#include "PRSColoredDoorWorldSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnKeyCountChanged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBagColorChanged, EDoorColor, NewBagColor);

UCLASS()
class PERSPECTIVE_API UPRSColoredDoorWorldSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnBagColorChanged OnBagColorChanged;

	UPROPERTY(BlueprintAssignable)
	FOnKeyCountChanged OnKeyCountChanged;
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE EDoorColor GetCurrentBagColor() const { return CurrentBagColor; }
	
	UFUNCTION(BlueprintCallable)
	void SetBagColor(EDoorColor NewBagColor);

	UFUNCTION(BlueprintCallable)
	void AddKey(EDoorColor KeyColor);

	UFUNCTION(BlueprintCallable)
	bool UseKey(EDoorColor KeyColor);

	UFUNCTION(BlueprintCallable)
	bool HasKey(EDoorColor KeyColor);

	UFUNCTION(BlueprintCallable)
	int32 GetKeyCount(EDoorColor KeyColor);

private:
	EDoorColor CurrentBagColor = EDoorColor::EDC_None;
	TMap<EDoorColor, uint32> KeyColorCountMap;
	
	//~ UWorldSubsystem Begin
public:
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	//~ UWorldSubsystem End
};
