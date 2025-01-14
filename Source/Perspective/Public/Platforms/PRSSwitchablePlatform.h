// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Core/PRSOperatableActor.h"
#include "Core/Utility/PRSSoundStatics.h"
#include "PRSSwitchablePlatform.generated.h"

UCLASS()
class PERSPECTIVE_API APRSSwitchablePlatform : public APRSOperatableActor
{
	GENERATED_BODY()

public:
	APRSSwitchablePlatform();
	
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void Operate_Implementation() override;

protected:
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Config")
	bool bEnabled = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config | Sound")
	class USoundCue* SwitchSound = UPRSSoundStatics::GetSwitchablePlatformSwitchSoundCue();
private:
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UStaticMeshComponent> StaticMeshComp = nullptr;
	
	void UpdatePlatform();
};
