// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "Perspective/Core/Utility/PRSInputStatics.h"

#include "PRSPlayerController.generated.h"

class APRSCharacter;
class UInputAction;
class UInputMappingContext;

UCLASS()
class PERSPECTIVE_API APRSPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void SetupInputComponent() override;
	virtual void Tick(float DeltaTime) override;

	void RequestInteractionAction();
	void RequestLookAction(const FInputActionValue& InputActionValue);
	void RequestMoveXAction(const FInputActionValue& InputActionValue);
	void RequestMoveYAction(const FInputActionValue& InputActionValue);
	void RequestMoveActionCompleted();
	void RequestSprintAction(const FInputActionValue& InputActionValue);
	void RequestSprintActionCompleted(const FInputActionValue& InputActionValue);

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Input", meta=(AllowPrivateAccess="true"))
	UInputMappingContext* DefaultMappingContext = UPRSInputStatics::GetDefaultInputMappingContext();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Input", meta=(AllowPrivateAccess="true"))
	UInputAction* InteractAction = UPRSInputStatics::GetInteractInputAction();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Input", meta=(AllowPrivateAccess="true"))
	UInputAction* LookAction = UPRSInputStatics::GetLookInputAction();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Input", DisplayName="Move X Action", meta=(AllowPrivateAccess="true"))
	UInputAction* MoveXAction = UPRSInputStatics::GetMoveXInputAction();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Input", DisplayName="Move Y Action", meta=(AllowPrivateAccess="true"))
	UInputAction* MoveYAction = UPRSInputStatics::GetMoveYInputAction();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Input", DisplayName="Sprint", meta=(AllowPrivateAccess="true"))
	UInputAction* SprintAction = UPRSInputStatics::GetSprintInputAction();

	UPROPERTY(EditAnywhere, Category = "Config | Movement")
	float BackwardMovementMultiplier = 0.5f;

	UPROPERTY(EditAnywhere, Category = "Config | Look")
	float BaseLookUpRate = 60.f;

	UPROPERTY(EditAnywhere, Category = "Config | Look")
	float BaseLookRightRate = 60.f;

	UPROPERTY(EditAnywhere, Category = "Config | Look")
	float LookMultiplierForMoveControls = 2.f;
	
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
