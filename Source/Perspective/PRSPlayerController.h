// Fill out your copyright notice in the Description page of Project Settings.

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InteractionAction;
	
	UPROPERTY(EditAnywhere, Category= Look)
	float BaseLookUpRate = 90.0f;

	UPROPERTY(EditAnywhere, Category= Look)
	float BaseLookRightRate = 90.0f;

protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* aPawn) override;
	virtual void SetupInputComponent() override;
	virtual void Tick(float DeltaTime) override;

	void RequestMoveAction(const FInputActionValue& InputActionValue);
	void RequestMoveActionCompleted();
	void RequestLookAction(const FInputActionValue& InputActionValue);
	void RequestInteractionAction();

private:
	bool bEnableYInput = true;
	bool bIsPerspectiveChangedRequiresHandling = false;
	float PreviousControllerPitchRotation = 0.0f;
	TWeakObjectPtr<APRSCharacter> PossessedCharacter;

	UFUNCTION()
	void OnPerspectiveModeChanged(EPerspectiveMode NewPerspectiveMode);
};
