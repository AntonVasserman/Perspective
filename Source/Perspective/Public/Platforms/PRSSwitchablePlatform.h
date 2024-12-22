// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Core/Operations/Interfaces/PRSOperatableInterface.h"
#include "Core/Utility/PRSSoundStatics.h"
#include "Engine/StaticMeshActor.h"
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
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Config")
	bool bEnabled = true;

	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config | Sound")
	class USoundCue* SwitchSound = UPRSSoundStatics::GetSwitchablePlatformSwitchSoundCue();
private:
	void UpdatePlatform();
};
