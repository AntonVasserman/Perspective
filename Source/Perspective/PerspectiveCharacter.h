// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Enums/PerspectiveMode.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Logging/LogMacros.h"
#include "PerspectiveCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class APerspectiveCharacter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

public:
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	APerspectiveCharacter();

	FVector GetForwardVector() const;
	FVector GetRightVector() const;
	FORCEINLINE bool IsMoving() const { return GetCharacterMovement()->Velocity.X == 0.f && GetCharacterMovement()->Velocity.Y == 0.f; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector ForwardVectorOverride = FVector(1.0f, 0.0f, 0.0f);

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	bool bShouldUseForwardVectorOverride = false;
	bool bIsPerspectiveChanged = true;

	UFUNCTION()
	void OnPerspectiveModeChanged(EPerspectiveMode NewPerspectiveMode);
};

