// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Materials/MaterialParameterCollection.h"

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

	FORCEINLINE static UMaterialInterface* GetPanelGreenMaterial()
	{
		return LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/Perspective/Operatables/Gates/M_Gate_Green.M_Gate_Green"));
	}

	FORCEINLINE static UMaterialInterface* GetPanelYellowMaterial()
	{
		return LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/Perspective/Operatables/Gates/M_Gate_Yellow.M_Gate_Yellow"));
	}

	FORCEINLINE static UMaterialInterface* GetPanelRedMaterial()
	{
		return LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/Perspective/Operatables/Gates/M_Gate_Red.M_Gate_Red"));
	}

	FORCEINLINE static UMaterialParameterCollection* GetMaskingMaterialParameterCollection()
	{
		return LoadObject<UMaterialParameterCollection>(nullptr, TEXT("/Game/Perspective/Art/Shared/Materials/MPC_Masking.MPC_Masking"));
	}

	FORCEINLINE static FName GetMaskingMaterialParameterCollectionMaskParameterName() { return FName(TEXT("Mask")); }
};
