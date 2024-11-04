// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Sound/SoundCue.h"

#include "PRSStatics.generated.h"

UCLASS(MinimalAPI)
class UPRSStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	FORCEINLINE static UStaticMesh* GetCubeStaticMesh()
	{
		return LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cube.Cube"));
	}

	FORCEINLINE static USoundCue* GetModeChangedSoundCue()
	{
		return LoadObject<USoundCue>(nullptr, TEXT("/Game/Perspective/Core/Audio/A_PRSModeChanged_Cue.A_PRSModeChanged_Cue"));
	}
	
	FORCEINLINE static UMaterialInterface* GetPanelGreenMaterial()
	{
		return LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/Perspective/WorldModeChangers/M_Panel_Green.M_Panel_Green"));
	}

	FORCEINLINE static UMaterialInterface* GetPanelYellowMaterial()
	{
		return LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/Perspective/WorldModeChangers/M_Panel_Yellow.M_Panel_Yellow"));
	}

	FORCEINLINE static UMaterialInterface* GetPanelRedMaterial()
	{
		return LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/Perspective/WorldModeChangers/M_Panel_Red.M_Panel_Red"));
	}
};
