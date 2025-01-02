// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PerspectiveMode.h"

#include "PerspectiveModeChangedArgs.generated.h"

enum class EPerspectiveMode : uint8;

USTRUCT(BlueprintType)
struct FPerspectiveModeChangedArgs
{
	GENERATED_BODY()

	FPerspectiveModeChangedArgs() = default;

	FPerspectiveModeChangedArgs(const EPerspectiveMode& Mode, const FRotator& NewControlRotation)
		: Mode(Mode), NewControlRotation(NewControlRotation) { }

	UPROPERTY()
	EPerspectiveMode Mode = EPerspectiveMode::ThreeDimensional;

	UPROPERTY()
	FRotator NewControlRotation = FRotator::ZeroRotator;
};
