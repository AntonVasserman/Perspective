// Copyright Anton Vasserman, All Rights Reserved.

#include "PRSOperationComponent.h"
#include "Perspective/Core/Operations/Interfaces/PRSOperatableInterface.h"
#include "Perspective/Interactables/PRSInteractableButton.h"

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
		
			Button->OnButtonPressed.AddDynamic(this, &UPRSOperationComponent::ExecuteOperation);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Non-Operatable Actor: %s, has an Operation Component"), *this->GetName());
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

