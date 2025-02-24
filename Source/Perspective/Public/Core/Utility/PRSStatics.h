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
	FORCEINLINE static UMaterialParameterCollection* GetMaskingMaterialParameterCollection()
	{
		return LoadObject<UMaterialParameterCollection>(nullptr, TEXT("/Game/Perspective/Art/Shared/Materials/MPC_Masking.MPC_Masking"));
	}

	FORCEINLINE static FName GetMaskingMaterialParameterCollectionMaskParameterName() { return FName(TEXT("Mask")); }
};
