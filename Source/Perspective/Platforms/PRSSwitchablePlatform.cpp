// Copyright Anton Vasserman, All Rights Reserved.

#include "PRSSwitchablePlatform.h"
#include "Perspective/Core/Utility/PRSStatics.h"

APRSSwitchablePlatform::APRSSwitchablePlatform()
{
	GetStaticMeshComponent()->SetStaticMesh(UPRSStatics::GetCubeStaticMesh());
}

void APRSSwitchablePlatform::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	UpdatePlatform();
}

void APRSSwitchablePlatform::Operate()
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
