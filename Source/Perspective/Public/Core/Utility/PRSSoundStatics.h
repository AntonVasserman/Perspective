// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Sound/SoundCue.h"

#include "PRSSoundStatics.generated.h"

UCLASS(MinimalAPI)
class UPRSSoundStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static void PlaySoundAtLocation(const UObject* WorldContextObject, USoundBase* Sound, const FVector& Location, const FRotator& Rotation,
		const float VolumeMultiplier = 1.f, const float PitchMultiplier = 1.f);
};
