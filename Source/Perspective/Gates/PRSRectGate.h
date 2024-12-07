// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PRSPanel.h"
#include "GameFramework/Actor.h"
#include "Perspective/Core/Operations/Interfaces/PRSOperatableInterface.h"
#include "Perspective/Core/Utility/PRSSoundStatics.h"
#include "PRSRectGate.generated.h"

class USoundCue;

UCLASS()
class PERSPECTIVE_API APRSRectGate : public AActor, public IPRSOperatableInterface
{
	GENERATED_BODY()
	
public:	
	APRSRectGate();

	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	virtual void Operate() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config")
	bool bEnabled = true;
	
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Config")
	bool bCloseOnPassthrough = false;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Config | Sound")
	USoundCue* PerspectiveModeChangedSoundCue = UPRSSoundStatics::GetModeChangedSoundCue();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config | Sound")
	USoundCue* EnableDisableSound = UPRSSoundStatics::GetRectGateEnableDisableSoundCue();
	
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Config | Panels")
	bool bFrontPanelEnabled = true;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Config | Panels")
	bool bBackPanelEnabled = true;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Config | Panels")
	bool bRightPanelEnabled = true;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Config | Panels")
	bool bLeftPanelEnabled = true;

	UPROPERTY()
	TArray<UPRSPanel*> Panels;

	virtual void BeginPlay() override;

	UFUNCTION()
	void CenterRectCompOnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void CenterRectCompOnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);
	UFUNCTION()
	void PanelOnPlayerEndOverlap(UPRSPanel* OverlappedPanel, class APRSCharacter* PlayerCharacter);

private:
	const float PanelLength = 50.f;
	bool bIsTouchingInsideRectangle = false;
	UPROPERTY()
	UPRSPanel* EnterPanel = nullptr;
	UPROPERTY()
	UPRSPanel* ExitPanel = nullptr;

	UPROPERTY(VisibleDefaultsOnly)
	class UBoxComponent* CenterRectComp;
};
