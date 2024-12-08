// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "Perspective/Core/Utility/PRSInputStatics.h"

#include "PRSPlayerController.generated.h"

class UInputAction;
class UInputMappingContext;

UCLASS()
class PERSPECTIVE_API APRSPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	virtual void Tick(float DeltaSeconds) override;
	
protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void SetupInputComponent() override;

	void RequestInteractionAction();
	void RequestLookAction(const FInputActionValue& InputActionValue);
	void RequestMoveAction(const FInputActionValue& InputActionValue);
	void RequestSprintAction(const FInputActionValue& InputActionValue);
	void RequestSprintActionCompleted(const FInputActionValue& InputActionValue);

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta=(AllowPrivateAccess="true"))
	UInputMappingContext* DefaultMappingContext = UPRSInputStatics::GetDefaultInputMappingContext();
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta=(AllowPrivateAccess="true"))
	UInputAction* InteractAction = UPRSInputStatics::GetInteractInputAction();
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta=(AllowPrivateAccess="true"))
	UInputAction* LookAction = UPRSInputStatics::GetLookInputAction();
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta=(AllowPrivateAccess="true"))
	UInputAction* MoveAction = UPRSInputStatics::GetMoveInputAction();
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta=(AllowPrivateAccess="true"))
	UInputAction* SprintAction = UPRSInputStatics::GetSprintInputAction();

	UPROPERTY(EditAnywhere, Category = "Config | Look")
	float BaseLookUpRate = 60.f;

	UPROPERTY(EditAnywhere, Category = "Config | Look")
	float BaseLookRightRate = 60.f;

	UPROPERTY(EditAnywhere, Category = "Config | Look")
	float LookMultiplierForMoveControls = 2.f;

	UPROPERTY()
	class UPRSModeWorldSubsystem* PerspectiveModeWorldSubsystem;
	
	UPROPERTY()
	class APRSCharacter* PossessedCharacter;

	FVector2D PreviousMovementInput;
	bool bAwaitingPerspectiveChangeHandling = false;
	bool bMovementInputChanged = false;
	float PreviousPitchRotation = 0.f;

	UFUNCTION()
	void OnPerspectiveModeChanged(const struct FPerspectiveModeChangedArgs& NewPerspectiveArgs);
};
