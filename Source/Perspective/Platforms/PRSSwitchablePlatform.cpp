// Copyright Anton Vasserman, All Rights Reserved.

#include "PRSSwitchablePlatform.h"

#include "AVCollisionProfileStatics.h"
#include "Perspective/Core/Utility/PRSStatics.h"
#include "Sound/SoundCue.h"

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
	UPRSSoundStatics::PlaySoundAtLocation(GetWorld(), SwitchSound, GetActorLocation(), GetActorRotation());
	UpdatePlatform();
}

void APRSSwitchablePlatform::UpdatePlatform()
{
	GetStaticMeshComponent()->SetCollisionProfileName(
		bEnabled ? UAVCollisionProfileStatics::BlockAll_ProfileName : UAVCollisionProfileStatics::NoCollision_ProfileName, true);
	GetStaticMeshComponent()->SetMaterial(0, bEnabled ? UPRSStatics::GetPlatformEnabledMaterial() : UPRSStatics::GetPlatformDisabledMaterial());
}
