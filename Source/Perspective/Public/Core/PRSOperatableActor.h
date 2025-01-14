// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PRSOperatableActor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnOperationStateChanged, const bool, bOperatable);

UCLASS(Abstract)
class PERSPECTIVE_API APRSOperatableActor : public AActor
{
	GENERATED_BODY()

public:
	FOnOperationStateChanged OnOperationStateChanged;
	
	UFUNCTION()
	void Operate();

	FORCEINLINE bool IsOperatable() const { return bOperatable; }
	
protected:
	UFUNCTION(BlueprintCallable)
	void SetOperatability(const bool bInOperatable);
	
	UFUNCTION()
	virtual void Operate_Implementation() { }
	
private:
	bool bOperatable = true;
};
