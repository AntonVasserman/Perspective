// Copyright Anton Vasserman, All Rights Reserved.

#include "PRSSwitchablePlatform.h"

#include "Perspective/Core/Utility/PRSStatics.h"
#include "Perspective/Interactables/PRSInteractableButton.h"

APRSSwitchablePlatform::APRSSwitchablePlatform()
{
	GetStaticMeshComponent()->SetStaticMesh(UPRSStatics::GetCubeStaticMesh());
}

void APRSSwitchablePlatform::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	UpdatePlatform();
}

void APRSSwitchablePlatform::BeginPlay()
{
	Super::BeginPlay();

	for (APRSInteractableButton* Button : InteractableButtons)
	{
		if (Button == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("Platform: %s, has a null button set"), *this->GetName());
			continue;
		}

		Button->OnButtonPressed.AddDynamic(this, &APRSSwitchablePlatform::OnButtonPressed);
	}
}

void APRSSwitchablePlatform::OnButtonPressed()
{
	bEnabled = !bEnabled;
	UpdatePlatform();
}

void APRSSwitchablePlatform::UpdatePlatform()
{
	GetStaticMeshComponent()->SetCollisionProfileName(
		bEnabled ? UPRSStatics::GetCollisionProfileNameBlockAll() : UPRSStatics::GetCollisionProfileNameNoCollision(),
		true);
	GetStaticMeshComponent()->SetMaterial(0, bEnabled ? UPRSStatics::GetPlatformEnabledMaterial() : UPRSStatics::GetPlatformDisabledMaterial());
}
