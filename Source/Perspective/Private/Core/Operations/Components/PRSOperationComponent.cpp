// Copyright Anton Vasserman, All Rights Reserved.

#include "Core/Operations/Components/PRSOperationComponent.h"

#include "Core/Operations/Interfaces/PRSOperatableInterface.h"
#include "Interactables/PRSInteractableButton.h"

static TAutoConsoleVariable CVarDisplayOperatableToInteractableLineTrace(
	TEXT("PRS.OpeartionComponent.Debug.OperatableToInteractableLineTrace"),
	false,
	TEXT("Display Operatable to Interactable Line Trace"),
	ECVF_Default);

UPRSOperationComponent::UPRSOperationComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UPRSOperationComponent::BeginPlay()
{
	Super::BeginPlay();

	if (Cast<IPRSOperatableInterface>(GetOwner()))
	{
		for (APRSInteractableButton* Button : InteractableButtons)
		{
			if (Button == nullptr)
			{
				UE_LOG(LogTemp, Error, TEXT("Door: %s, has a null button set"), *this->GetName());
				continue;
			}

			Button->OnInteractionEnded.AddDynamic(this, &UPRSOperationComponent::OnInteractionEnded);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Non-Operatable Actor: %s, has an Operation Component"), *this->GetName());
	}
}

void UPRSOperationComponent::OnInteractionEnded(const bool bInteractionSucceeded)
{
	if (bInteractionSucceeded)
	{
		ExecuteOperation();
	}
}

void UPRSOperationComponent::ExecuteOperation()
{
	if (IPRSOperatableInterface* Operatable = Cast<IPRSOperatableInterface>(GetOwner()))
	{
		Operatable->Operate();
	}
}

void UPRSOperationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (CVarDisplayOperatableToInteractableLineTrace->GetBool())
	{
		for (const APRSInteractableButton* Button : InteractableButtons)
		{
			DrawDebugLine(GetWorld(), GetOwner()->GetActorLocation(), Button->GetActorLocation(), FColor::Blue,
				false, 0.1f);
		}
	}
}

