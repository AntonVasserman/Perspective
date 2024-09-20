// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "PerspectivePlayerController.generated.h"

class UInputAction;
class UInputMappingContext;
/**
 * 
 */
UCLASS()
class PERSPECTIVE_API APerspectivePlayerController : public APlayerController
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, Category= Look)
	float BaseLookUpRate = 90.0f;

	UPROPERTY(EditAnywhere, Category= Look)
	float BaseLookRightRate = 90.0f;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	
	void RequestMoveAction(const FInputActionValue& InputActionValue);
	void RequestLookAction(const FInputActionValue& InputActionValue);

private:
	bool bEnableYInput = true;
	bool bIsPerspectiveChanged = true;
	float PreviousControllerPitchRotation = 0.0f;

	UFUNCTION()
	void OnPerspectiveModeChanged(EPerspectiveMode NewPerspectiveMode);
};
