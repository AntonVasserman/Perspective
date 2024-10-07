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

protected:
	UPROPERTY(EditAnywhere)
	UBoxComponent* BoxComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound")
	USoundCue* PerspectiveModeChangedSoundCue;

	virtual void BeginPlay() override;
	
	UFUNCTION()
	void BoxComponentOnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void BoxComponentOnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

private:
	FVector EnterVector = FVector::ZeroVector;
	FVector ExitVector = FVector::ZeroVector;
};
