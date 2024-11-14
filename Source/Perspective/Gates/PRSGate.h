// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Perspective/Core/Utility/PRSStatics.h"

#include "PRSGate.generated.h"

UCLASS()
class PERSPECTIVE_API APRSGate : public AActor
{
	GENERATED_BODY()
	
public:	
	APRSGate();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config")
	bool bEnabled = true;
	
	UPROPERTY(EditAnywhere)
	class UBoxComponent* BoxComp;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* CubeEffectComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config | Interaction")
	class APRSInteractableButton* InteractableButton = nullptr;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sound")
	USoundCue* PerspectiveModeChangedSoundCue = UPRSStatics::GetModeChangedSoundCue();

	virtual void BeginPlay() override;
	
	UFUNCTION()
	void BoxComponentOnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void BoxComponentOnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

private:
	const float PanelLength = 50.f;
	FVector EnterVector = FVector::ZeroVector;
	FVector ExitVector = FVector::ZeroVector;

	UFUNCTION()
	void OnButtonPressed();
};
