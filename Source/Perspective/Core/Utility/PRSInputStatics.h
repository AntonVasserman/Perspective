// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "InputMappingContext.h"

#include "PRSInputStatics.generated.h"

UCLASS(MinimalAPI)
class UPRSInputStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	FORCEINLINE static UInputMappingContext* GetDefaultInputMappingContext()
	{
		return LoadObject<UInputMappingContext>(nullptr, TEXT("/Game/Perspective/Core/Inputs/IMC_Default.IMC_Default"));
	}

	FORCEINLINE static UInputAction* GetCrouchInputAction()
	{
		return LoadObject<UInputAction>(nullptr, TEXT("/Game/Perspective/Core/Inputs/Actions/IA_Crouch.IA_Crouch"));
	}

	FORCEINLINE static UInputAction* GetInteractInputAction()
	{
		return LoadObject<UInputAction>(nullptr, TEXT("/Game/Perspective/Core/Inputs/Actions/IA_Interact.IA_Interact"));
	}

	FORCEINLINE static UInputAction* GetLookInputAction()
	{
		return LoadObject<UInputAction>(nullptr, TEXT("/Game/Perspective/Core/Inputs/Actions/IA_Look.IA_Look"));
	}

	FORCEINLINE static UInputAction* GetMoveInputAction()
	{
		return LoadObject<UInputAction>(nullptr, TEXT("/Game/Perspective/Core/Inputs/Actions/IA_Move.IA_Move"));
	}

	FORCEINLINE static UInputAction* GetSprintInputAction()
	{
		return LoadObject<UInputAction>(nullptr, TEXT("/Game/Perspective/Core/Inputs/Actions/IA_Sprint.IA_Sprint"));
	}
};