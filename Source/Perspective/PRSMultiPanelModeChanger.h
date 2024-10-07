// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PRSMultiPanelModeChanger.generated.h"

class APRSCharacter;
class UBoxComponent;

UCLASS()
class PERSPECTIVE_API APRSMultiPanelModeChanger : public AActor
{
	GENERATED_BODY()
	
public:	
	APRSMultiPanelModeChanger();

	virtual void Tick(float DeltaTime) override;
	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	enum class EDirection : uint8
	{
		None,
		Front,
		Back,
		Right,
		Left,
		Top,
		Bottom,
	};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound")
	USoundCue* PerspectiveModeChangedSoundCue;
	
	virtual void BeginPlay() override;

	UFUNCTION()
	void CenterBoxCompOnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void CenterBoxCompOnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);
	UFUNCTION()
	void BoxCompOnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

private:
	const float PanelLength = 50.f;
	bool bIsInsideBox = false;
	bool bIsTouchingInsideBox = false;
	EDirection EnterDirection;
	EDirection ExitDirection;

	TMap<UBoxComponent*, EDirection> BoxComponentToDirectionMapping;
	TMap<EDirection, UBoxComponent*> DirectionToBoxComponentMapping;
	
	TWeakObjectPtr<UBoxComponent> CenterBoxComp;
	TWeakObjectPtr<UBoxComponent> FrontBoxComp;
	TWeakObjectPtr<UBoxComponent> BackBoxComp;
	TWeakObjectPtr<UBoxComponent> RightBoxComp;
	TWeakObjectPtr<UBoxComponent> LeftBoxComp;
	TWeakObjectPtr<UBoxComponent> TopBoxComp;
	TWeakObjectPtr<UBoxComponent> BottomBoxComp;

	void InternalBoxComponentOnComponentEndOverlap(const UBoxComponent* OverlappedBoxComponent, APRSCharacter* PRSCharacter);
};
