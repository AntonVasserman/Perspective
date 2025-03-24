// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Operatables//PRSOperatableActor.h"
#include "GameFramework/Actor.h"
#include "PRSGate.generated.h"

class UBoxComponent;

UCLASS()
class PERSPECTIVE_API APRSGate : public APRSOperatableActor
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

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config | Sound")
	USoundBase* PerspectiveModeChangedSound = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config | Sound")
	USoundBase* EnableDisableSound = nullptr;

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

	//~ APRSOperatableActor Begin
	// TODO: Do I need both PostInit and BeginPlay here???
public:
	virtual void PostInitializeComponents() override;
	virtual void Operate_Implementation() override;
protected:
	virtual void BeginPlay() override;
	//~ APRSOperatableActor End
};
