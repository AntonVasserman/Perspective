// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GateColor.h"
#include "Subsystems/WorldSubsystem.h"
#include "PRSColoredGateWorldSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGateColorChanged, EGateColor, NewGateColor);

UCLASS()
class PERSPECTIVE_API UPRSColoredGateWorldSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FGateColorChanged OnGateColorChanged;

	UFUNCTION(BlueprintCallable)
	FORCEINLINE EGateColor GetCurrentColor() const { return CurrentGateColor; }
	
	UFUNCTION(BlueprintCallable)
	void SetColor(EGateColor NewGateColor);
	
private:
	EGateColor CurrentGateColor = EGateColor::EGC_None;
};
