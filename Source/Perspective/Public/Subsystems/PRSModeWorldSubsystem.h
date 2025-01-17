// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PerspectiveMode.h"
#include "Subsystems/WorldSubsystem.h"

#include "PRSModeWorldSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPerspectiveModeChanged, const struct FPerspectiveModeChangedArgs&, NewPerspectiveArgs);

UCLASS()
class PERSPECTIVE_API UPRSModeWorldSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable, Category="Perspective")
	FPerspectiveModeChanged OnPerspectiveModeChanged;
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE EPerspectiveMode GetMode() const { return Mode; }

	UFUNCTION(BlueprintCallable)
	void Switch(const FRotator& NewControlRotation = FRotator::ZeroRotator,
		const bool bOverridePlayerCharacterX = false, const float PlayerCharacterXOverride = 0.f,
		const bool bOverridePlayerCharacterY = false, const float PlayerCharacterYOverride = 0.f);

private:
	EPerspectiveMode Mode = EPerspectiveMode::ThreeDimensional;
};
