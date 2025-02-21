// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Operatables/ColorGate/GateColor.h"
#include "Subsystems/WorldSubsystem.h"
#include "PRSBagColorWorldSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGateColorChanged, EGateColor, NewGateColor);

UCLASS()
class PERSPECTIVE_API UPRSBagColorWorldSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FGateColorChanged OnGateColorChanged;

	UFUNCTION(BlueprintCallable)
	void SetColor(EGateColor NewGateColor);
	
private:
	EGateColor CurrentGateColor = EGateColor::EGC_None;
};
