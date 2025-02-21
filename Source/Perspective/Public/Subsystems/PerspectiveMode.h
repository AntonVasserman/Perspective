// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EPerspectiveMode : uint8
{
	TwoDimensional = 0 UMETA(DisplayName = "Two Dimensional"),
	ThreeDimensional = 1 UMETA(DisplayName = "Three Dimensional"),
};
