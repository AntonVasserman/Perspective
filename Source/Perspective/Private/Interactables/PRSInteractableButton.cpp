// Copyright Anton Vasserman, All Rights Reserved.

#include "Interactables/PRSInteractableButton.h"

#include "Operatables/PRSOperatableActor.h"
#include "Kismet/GameplayStatics.h"

static TAutoConsoleVariable CVarDisplayOperatableToInteractableLineTrace(
	TEXT("PRS.OpeartionComponent.Debug.OperatableToInteractableLineTrace"),
	false,
	TEXT("Display Operatable to Interactable Line Trace"),
	ECVF_Default);

APRSInteractableButton::APRSInteractableButton()
{
	PrimaryActorTick.bCanEverTick = true;
}

void APRSInteractableButton::TryEnableInteraction()
{
	for (const APRSOperatableActor* OperatableActor : OperatableActors)
	{
		if (!OperatableActor->IsOperatable())
		{
			return;
		}

		SetInteractable(true);
	}
}

void APRSInteractableButton::OnOperationStateChanged(const bool bOperatable)
{
	if (!bOperatable)
	{
		SetInteractable(false);
		return;
	}
	
	for (const APRSOperatableActor* OperatableActor : OperatableActors)
	{
		if (!OperatableActor->IsOperatable())
		{
			return;
		}

		if (bRepressable)
		{
			ResetButton();
		}
	}
}

//~ APRSInteractableActor Begin

// TODO: Consider using BeginPlay instead...
void APRSInteractableButton::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	checkf(ButtonClickSound, TEXT("Config | Sound: ButtonClickSound is not set"))
	
	for (APRSOperatableActor* OperatableActor : OperatableActors)
	{
		if (OperatableActor != nullptr)
		{
			OnInteractionSucceeded.AddDynamic(OperatableActor, &APRSOperatableActor::Operate);
			OperatableActor->OnOperationStateChanged.AddDynamic(this, &APRSInteractableButton::OnOperationStateChanged);
		}
	}
}

void APRSInteractableButton::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (CVarDisplayOperatableToInteractableLineTrace->GetBool())
	{
		for (const APRSOperatableActor* OperatableActor : OperatableActors)
		{
			DrawDebugLine(GetWorld(), GetOwner()->GetActorLocation(), OperatableActor->GetActorLocation(), FColor::Blue,
				false, 0.1f);
		}
	}
}

void APRSInteractableButton::Interact_Implementation()
{
	UGameplayStatics::PlaySoundAtLocation(this, ButtonClickSound, GetActorLocation(), GetActorRotation());
	SetInteractable(false);
}

//~ APRSInteractableActor End