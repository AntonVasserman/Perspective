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
		return LoadObject<USoundCue>(nullptr, TEXT("/Game/Perspective/Interactables/Audio/A_PRSButtonClick_Cue.A_PRSButtonClick_Cue"));
	}
	
	FORCEINLINE static USoundCue* GetModeChangedSoundCue()
	{
		return LoadObject<USoundCue>(nullptr, TEXT("/Game/Perspective/Core/Audio/A_PRSModeChanged_Cue.A_PRSModeChanged_Cue"));
	}

	static void PlaySoundAtLocation(const UObject* WorldContextObject, class USoundBase* Sound,
		FVector Location, FRotator Rotation);
};
