// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Operatables/PRSOperatableActor.h"
#include "PRSSwitchablePlatform.generated.h"

UCLASS()
class PERSPECTIVE_API APRSSwitchablePlatform : public APRSOperatableActor
{
	GENERATED_BODY()

public:
	APRSSwitchablePlatform();

protected:
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Config")
	bool bEnabled = true;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config | Sound")
	USoundBase* SwitchSound = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config | Material")
	TObjectPtr<UMaterialInterface> PlatformEnabledMaterial = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config | Material")
	TObjectPtr<UMaterialInterface> PlatformDisabledMaterial = nullptr;
	

private:
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UStaticMeshComponent> StaticMeshComp = nullptr;
	
	void UpdatePlatform();

	//~ APRSOperatableActor Begin
public:
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void Operate_Implementation() override;
protected:
	virtual void BeginPlay() override;
	//~ APRSOperatableActor End
};
