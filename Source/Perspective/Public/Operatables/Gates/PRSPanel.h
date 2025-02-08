// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "Core/Utility/PRSStatics.h"
#include "PRSPanel.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPlayerBeginOverlap, UPRSPanel*, OverlappedPanel, APRSCharacter*, PlayerCharacter);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPlayerEndOverlap, UPRSPanel*, OverlappedPanel, APRSCharacter*, PlayerCharacter);

UCLASS()
class PERSPECTIVE_API UPRSPanel : public UStaticMeshComponent
{
	GENERATED_BODY()

public:
	FOnPlayerBeginOverlap OnPlayerBeginOverlap;
	FOnPlayerEndOverlap OnPlayerEndOverlap;
	
	UPRSPanel();
	
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	
	void SetOpen();
	void SetPending();
	void SetClosed();
	void SetEnabled(bool bEnabled);

protected:
	enum class EState : uint8
	{
		Open,
		Pending,
		Closed
	};
	
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Config")
	bool bPanelEnabled = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config | Material")
	TObjectPtr<UMaterialInterface> PanelOpenMaterial = UPRSStatics::GetPanelGreenMaterial();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config | Material")
	TObjectPtr<UMaterialInterface> PanelPendingMaterial = UPRSStatics::GetPanelYellowMaterial();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config | Material")
	TObjectPtr<UMaterialInterface> PanelClosedMaterial = UPRSStatics::GetPanelRedMaterial();
	
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	                    bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	EState CurrentState = EState::Open;
	FVector BeginOverlapLocation = FVector::ZeroVector;
};
