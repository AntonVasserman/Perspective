// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Operatables//PRSOperatableActor.h"

#include "PRSDoor.Generated.h"

UCLASS(Abstract)
class PERSPECTIVE_API APRSDoor : public APRSOperatableActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Config")
	float DoorOpenDuration = 2.5f;
};
