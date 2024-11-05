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

	GetStaticMeshComponent()->SetCollisionProfileName(
		bEnabled ? UPRSStatics::GetCollisionProfileNameBlockAll() : UPRSStatics::GetCollisionProfileNameNoCollision(),
		true);
	// TODO (PRS-16): Add proper materials
	GetStaticMeshComponent()->SetMaterial(0, bEnabled ? UPRSStatics::GetPanelGreenMaterial() : UPRSStatics::GetPanelRedMaterial());
}

void APRSSwitchablePlatform::BeginPlay()
{
	Super::BeginPlay();
	
	if (InteractableButton != nullptr)
	{
		InteractableButton->OnButtonPressed.AddDynamic(this, &APRSSwitchablePlatform::OnButtonPressed);
	}
}

void APRSSwitchablePlatform::OnButtonPressed()
{
	bEnabled = !bEnabled;

	GetStaticMeshComponent()->SetCollisionProfileName(
		bEnabled ? UPRSStatics::GetCollisionProfileNameBlockAll() : UPRSStatics::GetCollisionProfileNameNoCollision(),
		true);
	// TODO (PRS-16): Add proper materials
	GetStaticMeshComponent()->SetMaterial(0, bEnabled ? UPRSStatics::GetPanelGreenMaterial() : UPRSStatics::GetPanelRedMaterial());
}
