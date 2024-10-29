// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Perspective/Core/Utility/PerspectiveStatics.h"

#include "PRSBoxModeChanger.generated.h"

UCLASS()
class PERSPECTIVE_API APRSBoxModeChanger : public AActor
{
	GENERATED_BODY()
	
public:	
	APRSBoxModeChanger();

protected:
	UPROPERTY(EditAnywhere)
	class UBoxComponent* BoxComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sound")
	USoundCue* PerspectiveModeChangedSoundCue = UPerspectiveStatics::GetModeChangedSoundCue();

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
