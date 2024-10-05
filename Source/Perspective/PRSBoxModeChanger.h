// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PRSBoxModeChanger.generated.h"

class UBoxComponent;

UCLASS()
class PERSPECTIVE_API APRSBoxModeChanger : public AActor
{
	GENERATED_BODY()
	
public:	
	APRSBoxModeChanger();

	virtual void Tick(float DeltaTime) override;
	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	virtual void BeginPlay() override;

private:
	const float PanelLength = 100.f;
	TWeakObjectPtr<UBoxComponent> CenterBoxComp;
	TWeakObjectPtr<UBoxComponent> FrontBoxComp;
	TWeakObjectPtr<UBoxComponent> BackBoxComp;
	TWeakObjectPtr<UBoxComponent> RightBoxComp;
	TWeakObjectPtr<UBoxComponent> LeftBoxComp;
	TWeakObjectPtr<UBoxComponent> TopBoxComp;
	TWeakObjectPtr<UBoxComponent> BottomBoxComp;
};
