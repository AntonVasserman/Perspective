// Copyright Anton Vasserman, All Rights Reserved.

#include "Interactables/PRSInteractableActor.h"

#include "Characters/PRSCharacter.h"
#include "Components/BoxComponent.h"

APRSInteractableActor::APRSInteractableActor()
{
	PrimaryActorTick.bCanEverTick = true;

	DefaultRootSceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("Default Root"));
	DefaultRootSceneComp->SetupAttachment(GetRootComponent());
	BoxCollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	BoxCollisionComp->SetupAttachment(DefaultRootSceneComp);
}

void APRSInteractableActor::Interact()
{
	if (IsInteractable())
	{
		Interact_Implementation();
		OnInteractionSucceeded.Broadcast();
	}
}

void APRSInteractableActor::Highlight()
{
	bHighlighted = true;
	Highlight_BP();
}

void APRSInteractableActor::UnHighlight()
{
	bHighlighted = false;
	UnHighlight_BP();
}

void APRSInteractableActor::SetInteractable(const bool bInInteractable)
{
	bInteractable = bInInteractable;
}

void APRSInteractableActor::OnBoxCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (APRSCharacter* Character = Cast<APRSCharacter>(OtherActor))
	{
		Character->SetInteractionTarget(this);
	}
}

void APRSInteractableActor::OnBoxCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	if (APRSCharacter* Character = Cast<APRSCharacter>(OtherActor))
	{
		Character->UnsetInteractionTarget(this);
	}
}

//~ AActor Begin

void APRSInteractableActor::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	BoxCollisionComp->OnComponentBeginOverlap.AddDynamic(this, &APRSInteractableActor::OnBoxCollisionBeginOverlap);
	BoxCollisionComp->OnComponentEndOverlap.AddDynamic(this, &APRSInteractableActor::OnBoxCollisionEndOverlap);
}

//~ AActor End