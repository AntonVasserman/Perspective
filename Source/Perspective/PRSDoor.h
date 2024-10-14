// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PRSDoor.generated.h"

class APRSInteractableButton;

UCLASS()
class PERSPECTIVE_API APRSDoor : public AActor
{
	GENERATED_BODY()
	
public:	
	APRSDoor();

	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	APRSInteractableButton* InteractableButton = nullptr;

	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void Open();

private:
	bool bIsOpen = false;
	// TODO: Since this Door is currenlty only for prototyping, transfer this open logic into Blueprints using a Timeline
	float OpenDuration = 1.f;
	float PassedOpenDuration = 0.f;
	FVector OriginalScale3D = FVector(1.0f, 1.0f, 1.0f);
};
