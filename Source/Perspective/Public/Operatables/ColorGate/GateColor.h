// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EGateColor : uint8
{
	EGC_None	UMETA(DisplayName = "None", Description = "This should be used in case the Gate Color system wasn't yet enabled"),
	EGC_Red		UMETA(DisplayName = "Red"),
	EGC_Green	UMETA(DisplayName = "Green"),
	EGC_Blue	UMETA(DisplayName = "Blue"),
	EGC_Yellow	UMETA(DisplayName = "Yellow"),
};