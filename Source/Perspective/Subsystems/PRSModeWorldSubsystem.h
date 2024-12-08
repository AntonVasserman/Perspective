// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PerspectiveMode.h"
#include "Subsystems/WorldSubsystem.h"

#include "PRSModeWorldSubsystem.generated.h"

UCLASS()
class PERSPECTIVE_API UPRSModeWorldSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPerspectiveModeChanged, const struct FPerspectiveModeChangedArgs&, NewPerspectiveArgs);
	UPROPERTY(BlueprintAssignable, Category="Perspective")
	FPerspectiveModeChanged OnPerspectiveModeChanged;
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE EPerspectiveMode GetMode() const { return Mode; }

	UFUNCTION(BlueprintCallable)
	void Switch(const FRotator& NewControlRotation = FRotator::ZeroRotator);

private:
	EPerspectiveMode Mode = EPerspectiveMode::ThreeDimensional;
};
