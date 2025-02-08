// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/PlayerController.h"

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
	virtual void OnUnPossess() override;
	virtual void SetupInputComponent() override;

	void RequestInteractionAction();
	void RequestLookAction(const FInputActionValue& InputActionValue);
	void RequestMoveAction(const FInputActionValue& InputActionValue);
	void RequestSprintAction(const FInputActionValue& InputActionValue);

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config | Input", meta=(AllowPrivateAccess="true"))
	UInputMappingContext* DefaultMappingContext = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config | Input", meta=(AllowPrivateAccess="true"))
	UInputAction* InteractAction = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config | Input", meta=(AllowPrivateAccess="true"))
	UInputAction* LookAction = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config | Input", meta=(AllowPrivateAccess="true"))
	UInputAction* MoveAction = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config | Input", meta=(AllowPrivateAccess="true"))
	UInputAction* SprintAction = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config | Input", meta=(AllowPrivateAccess="true"))
	UForceFeedbackEffect* InteractForceFeedbackEffect = nullptr;
	
	UPROPERTY(EditAnywhere, Category = "Config | Look")
	float BaseLookUpRate = 60.f;

	UPROPERTY(EditAnywhere, Category = "Config | Look")
	float BaseLookRightRate = 60.f;

	UPROPERTY(EditAnywhere, Category = "Config | Look")
	float LookMultiplierForMoveControls = 2.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config | Perspective", meta=(AllowPrivateAccess="true"))
	UForceFeedbackEffect* PerspectiveChangedForceFeedbackEffect = nullptr;
	
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

	UFUNCTION()
	void OnPlayerCharacterInteracted();
};
