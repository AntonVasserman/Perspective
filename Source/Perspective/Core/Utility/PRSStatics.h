// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "PRSStatics.generated.h"

UCLASS(MinimalAPI)
class UPRSStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// TODO (79): Move to a dedicated shared Module
	FORCEINLINE static FName GetCollisionProfileNameBlockAll() { return FName("BlockAll"); }
	FORCEINLINE static FName GetCollisionProfileNameNoCollision() { return FName("NoCollision"); }
	FORCEINLINE static FName GetCollisionProfileNameOverlapAllDynamic() { return FName("OverlapAllDynamic"); }

	FORCEINLINE static UStaticMesh* GetCubeStaticMesh()
	{
		return LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cube.Cube"));
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
		return LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/Perspective/Platforms/MI_SwitchablePlatform_Enabled.MI_SwitchablePlatform_Enabled"));
	}

	FORCEINLINE static UMaterialInterface* GetPlatformDisabledMaterial()
	{
		return LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/Perspective/Platforms/MI_SwitchablePlatform_Disabled.MI_SwitchablePlatform_Disabled"));
	}
};
