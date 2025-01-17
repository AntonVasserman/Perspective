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

	FPerspectiveModeChangedArgs(const EPerspectiveMode& Mode, const FRotator& NewControlRotation,
		const bool bOverridePlayerCharacterX, const float PlayerCharacterXOverride,
		const bool bOverridePlayerCharacterY, const float PlayerCharacterYOverride)
		: Mode(Mode), NewControlRotation(NewControlRotation),
		bOverridePlayerCharacterX(bOverridePlayerCharacterX), PlayerCharacterXOverride(PlayerCharacterXOverride),
		bOverridePlayerCharacterY(bOverridePlayerCharacterY), PlayerCharacterYOverride(PlayerCharacterYOverride)
	{ }

	UPROPERTY()
	EPerspectiveMode Mode = EPerspectiveMode::ThreeDimensional;

	UPROPERTY()
	FRotator NewControlRotation = FRotator::ZeroRotator;

	UPROPERTY()
	bool bOverridePlayerCharacterX = false;
	
	UPROPERTY()
	float PlayerCharacterXOverride = 0.f;

	UPROPERTY()
	bool bOverridePlayerCharacterY = false;
	
	UPROPERTY()
	float PlayerCharacterYOverride = 0.f;
};
