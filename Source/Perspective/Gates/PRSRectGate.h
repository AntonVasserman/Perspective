// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PRSPanel.h"
#include "GameFramework/Actor.h"
#include "Perspective/Core/Operations/Interfaces/PRSOperatableInterface.h"
#include "Perspective/Core/Utility/PRSStatics.h"
#include "PRSRectGate.generated.h"

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Config | Sound")
	USoundCue* PerspectiveModeChangedSoundCue = UPRSStatics::GetModeChangedSoundCue();

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Config | Panels")
	bool bFrontPanelEnabled = true;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Config | Panels")
	bool bBackPanelEnabled = true;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Config | Panels")
	bool bRightPanelEnabled = true;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Config | Panels")
	bool bLeftPanelEnabled = true;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Config")
	bool bCloseOnPassthrough = false;

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
