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
	FORCEINLINE static USoundCue* GetButtonClickSoundCue()
	{
		return LoadObject<USoundCue>(nullptr, TEXT("/Game/Perspective/Interactables/Audio/A_Button_Click_Cue.A_Button_Click_Cue"));
	}
	
	FORCEINLINE static USoundCue* GetModeChangedSoundCue()
	{
		return LoadObject<USoundCue>(nullptr, TEXT("/Game/Perspective/Core/Audio/A_ModeChanged_Cue.A_ModeChanged_Cue"));
	}

	FORCEINLINE static USoundCue* GetRotatingDoorOpenCloseSoundCue()
	{
		return LoadObject<USoundCue>(nullptr, TEXT("/Game/Perspective/Doors/Audio/A_RotatingDoor_OpenClose_Cue.A_RotatingDoor_OpenClose_Cue"));
	}

	FORCEINLINE static USoundCue* GetSlidingDoorOpenCloseSoundCue()
	{
		return LoadObject<USoundCue>(nullptr, TEXT("/Game/Perspective/Doors/Audio/A_SlidingDoor_OpenClose_Cue.A_SlidingDoor_OpenClose_Cue"));
	}

	FORCEINLINE static USoundCue* GetSwitchablePlatformSwitchSoundCue()
	{
		return LoadObject<USoundCue>(nullptr, TEXT("/Game/Perspective/Platforms/Audio/A_SwitchablePlatform_Switch_Cue.A_SwitchablePlatform_Switch_Cue"));
	}
	
	static void PlaySoundAtLocation(const UObject* WorldContextObject, USoundBase* Sound, const FVector& Location, const FRotator& Rotation,
		const float VolumeMultiplier = 1.f, const float PitchMultiplier = 1.f);
};
