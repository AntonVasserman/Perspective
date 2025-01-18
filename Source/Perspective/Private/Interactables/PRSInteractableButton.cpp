// Copyright Anton Vasserman, All Rights Reserved.

#include "Interactables/PRSInteractableButton.h"

#include "Operatables/PRSOperatableActor.h"
#include "Core/Utility/PRSSoundStatics.h"
#include "Sound/SoundCue.h"

static TAutoConsoleVariable CVarDisplayOperatableToInteractableLineTrace(
	TEXT("PRS.OpeartionComponent.Debug.OperatableToInteractableLineTrace"),
	false,
	TEXT("Display Operatable to Interactable Line Trace"),
	ECVF_Default);

APRSInteractableButton::APRSInteractableButton()
{
	PrimaryActorTick.bCanEverTick = true;
}

void APRSInteractableButton::Highlight()
{
	Super::Highlight();

	Highlight_Implementation();
}

void APRSInteractableButton::UnHighlight()
{
	Super::UnHighlight();

	UnHighlight_Implementation();
}

void APRSInteractableButton::PostInitializeComponents()
{
	Super::PostInitializeComponents();

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
	UPRSSoundStatics::PlaySoundAtLocation(GetWorld(), ButtonClickSound, GetActorLocation(), GetActorRotation());
	SetInteractable(false);
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
