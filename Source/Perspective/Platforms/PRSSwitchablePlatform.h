// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "Perspective/Core/Operations/Interfaces/PRSOperatableInterface.h"
#include "PRSSwitchablePlatform.generated.h"

UCLASS()
class PERSPECTIVE_API APRSSwitchablePlatform : public AStaticMeshActor, public IPRSOperatableInterface
{
	GENERATED_BODY()

public:
	APRSSwitchablePlatform();
	
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void Operate() override;

protected:
	UPROPERTY(EditAnywhere)
	bool bEnabled = true;

private:
	void UpdatePlatform();
};
