// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Core/Operations/Interfaces/PRSOperatableInterface.h"
#include "Core/Utility/PRSSoundStatics.h"
#include "GameFramework/Actor.h"
#include "PRSGate.generated.h"

class UBoxComponent;

UCLASS()
class PERSPECTIVE_API APRSGate : public AActor, public IPRSOperatableInterface
{
	GENERATED_BODY()

	enum class EGateSide : uint8
	{
		None,
		Back,
		Front
	};
	
public:
	APRSGate();
	virtual void PostInitializeComponents() override;
	virtual void Operate() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config | Sound")
	USoundCue* PerspectiveModeChangedSoundCue = UPRSSoundStatics::GetModeChangedSoundCue();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config | Sound")
	USoundCue* EnableDisableSound = UPRSSoundStatics::GetGateEnableDisableSoundCue();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config | Collision")
	TObjectPtr<UBoxComponent> BackBoxComp = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config | Collision")
	TObjectPtr<UBoxComponent> FrontBoxComp = nullptr;
	
private:
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<USceneComponent> RootComp = nullptr;
	
	UPROPERTY(EditDefaultsOnly, Category = "Config | Mesh")
	TObjectPtr<UStaticMeshComponent> GateMeshComp = nullptr;
	
	bool bEnabled = true;
	
	EGateSide EnterGateSide = EGateSide::None;
	EGateSide ExitGateSide = EGateSide::None;
	
	UFUNCTION()
	void GateMeshOnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void GateMeshOnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};