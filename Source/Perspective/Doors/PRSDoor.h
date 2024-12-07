// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "PRSDoor.Generated.h"

UCLASS()
class PERSPECTIVE_API APRSDoor : public AActor
{
	GENERATED_BODY()

public:
	APRSDoor();

protected:
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Config")
	float DoorOpenDuration = 2.5f;
};
