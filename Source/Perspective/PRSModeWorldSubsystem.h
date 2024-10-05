// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enums/PerspectiveMode.h"
#include "Subsystems/WorldSubsystem.h"
#include "PRSModeWorldSubsystem.generated.h"

UCLASS()
class PERSPECTIVE_API UPRSModeWorldSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPerspectiveModeChanged, const EPerspectiveMode, NewPerspectiveMode);
	UPROPERTY(BlueprintAssignable, Category="Perspective")
	FPerspectiveModeChanged OnPerspectiveModeChanged;
	
	UFUNCTION(BlueprintCallable)
	EPerspectiveMode GetMode() const;

	UFUNCTION(BlueprintCallable)
	void Switch();

private:
	EPerspectiveMode Mode = EPerspectiveMode::ThreeDimensional;
};
