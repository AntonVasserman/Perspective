#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EDoorColor : uint8
{
	EDC_None	UMETA(DisplayName = "None"),
	EDC_Red		UMETA(DisplayName = "Red"),
	EDC_Green	UMETA(DisplayName = "Green"),
	EDC_Blue	UMETA(DisplayName = "Blue"),
	EDC_Yellow	UMETA(DisplayName = "Yellow"),
};