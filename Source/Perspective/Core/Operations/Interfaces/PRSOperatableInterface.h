// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PRSOperatableInterface.generated.h"

UINTERFACE(MinimalAPI)
class UPRSOperatableInterface : public UInterface
{
	GENERATED_BODY()
};

class PERSPECTIVE_API IPRSOperatableInterface
{
	GENERATED_BODY()

public:
	virtual void Operate() = 0;
};
