// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Perspective/Core/Utility/PRSStatics.h"

#include "PRSMultiPanelGate.generated.h"

class APRSCharacter;
class UBoxComponent;

UCLASS()
class PERSPECTIVE_API APRSMultiPanelGate : public AActor
{
	GENERATED_BODY()
	
public:	
	APRSMultiPanelGate();

	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	enum class EGateDirection : uint8
	{
		None,
		Front,
		Back,
		Right,
		Left,
	};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sound")
	USoundCue* PerspectiveModeChangedSoundCue = UPRSStatics::GetModeChangedSoundCue();
	
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
	EGateDirection EnterDirection;
	EGateDirection ExitDirection;

	TMap<UBoxComponent*, EGateDirection> BoxComponentToDirectionMapping;
	TMap<EGateDirection, UBoxComponent*> DirectionToBoxComponentMapping;

	UPROPERTY(EditAnywhere)
	UBoxComponent* CenterBoxComp;
	
	UPROPERTY(EditAnywhere)
	UBoxComponent* FrontBoxComp;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* FrontCubeEffectComp;
	
	UPROPERTY(EditAnywhere)
	UBoxComponent* BackBoxComp;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* BackCubeEffectComp;
	
	UPROPERTY(EditAnywhere)
	UBoxComponent* RightBoxComp;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* RightCubeEffectComp;
	
	UPROPERTY(EditAnywhere)
	UBoxComponent* LeftBoxComp;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* LeftCubeEffectComp;

	void InternalBoxComponentOnComponentEndOverlap(const UBoxComponent* OverlappedBoxComponent, APRSCharacter* PRSCharacter);
};
