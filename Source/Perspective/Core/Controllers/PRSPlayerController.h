// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"

#include "PRSPlayerController.generated.h"

class APRSCharacter;
class UInputAction;
class UInputMappingContext;

UCLASS()
class PERSPECTIVE_API APRSPlayerController : public APlayerController
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, DisplayName="Move X Action", meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveXAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, DisplayName="Move Y Action", meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveYAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InteractionAction;

	UPROPERTY(EditAnywhere, Category=Movement)
	float BackwardMovementMultiplier = 0.5f;
	
	UPROPERTY(EditAnywhere, Category=Look)
	float BaseLookUpRate = 45.0f;

	UPROPERTY(EditAnywhere, Category= Look)
	float BaseLookRightRate = 45.0f;

	UPROPERTY(EditAnywhere, Category= Look)
	float LookMultiplierForMoveControls = 1.5f;
	
protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void SetupInputComponent() override;
	virtual void Tick(float DeltaTime) override;

	void RequestMoveXAction(const FInputActionValue& InputActionValue);
	void RequestMoveYAction(const FInputActionValue& InputActionValue);
	void RequestMoveActionCompleted();
	void RequestLookAction(const FInputActionValue& InputActionValue);
	void RequestInteractionAction();

private:
	bool bEnableYInput = true;
	bool bIsPerspectiveChangedRequiresHandling = false;
	bool bMovingByX = false;
	bool bMovingByY = false;
	float PreviousControllerPitchRotation = 0.0f;
	TWeakObjectPtr<APRSCharacter> PossessedCharacter;

	UFUNCTION()
	void OnPerspectiveModeChanged(EPerspectiveMode NewPerspectiveMode);
	void HandlePerspectiveChanged();
};
