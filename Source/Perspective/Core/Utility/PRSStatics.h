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
	FORCEINLINE static FName GetCollisionProfileNameBlockAll() { return FName("BlockAll"); }
	FORCEINLINE static FName GetCollisionProfileNameNoCollision() { return FName("NoCollision"); }
	
	FORCEINLINE static UStaticMesh* GetCubeStaticMesh()
	{
		return LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cube.Cube"));
	}

	FORCEINLINE static USoundCue* GetModeChangedSoundCue()
	{
		return LoadObject<USoundCue>(nullptr, TEXT("/Game/Perspective/Core/Audio/A_PRSModeChanged_Cue.A_PRSModeChanged_Cue"));
	}

	FORCEINLINE static UMaterialInterface* GetPanelMaterial()
	{
		return LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/Perspective/Gates/M_Gate.M_Gate"));
	}
	
	FORCEINLINE static UMaterialInterface* GetPanelGreenMaterial()
	{
		return LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/Perspective/Gates/M_Gate_Green.M_Gate_Green"));
	}

	FORCEINLINE static UMaterialInterface* GetPanelYellowMaterial()
	{
		return LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/Perspective/Gates/M_Gate_Yellow.M_Gate_Yellow"));
	}

	FORCEINLINE static UMaterialInterface* GetPanelRedMaterial()
	{
		return LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/Perspective/Gates/M_Gate_Red.M_Gate_Red"));
	}

	FORCEINLINE static UMaterialInterface* GetPlatformEnabledMaterial()
	{
		return LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/Perspective/Platforms/M_Platform_Enabled.M_Platform_Enabled"));
	}

	FORCEINLINE static UMaterialInterface* GetPlatformDisabledMaterial()
	{
		return LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/Perspective/Platforms/M_Platform_Disabled.M_Platform_Disabled"));
	}
};
