// Copyright Anton Vasserman, All Rights Reserved.

#include "Operatables/Platforms/PRSSwitchablePlatform.h"

#include "AVCollisionProfileStatics.h"
#include "Core/Utility/PRSStatics.h"
#include "Engine/StaticMesh.h"
#include "Sound/SoundCue.h"

APRSSwitchablePlatform::APRSSwitchablePlatform()
{
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComp");
}

void APRSSwitchablePlatform::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	UpdatePlatform();
}

void APRSSwitchablePlatform::Operate_Implementation()
{
	SetOperatability(false);
	bEnabled = !bEnabled;
	UPRSSoundStatics::PlaySoundAtLocation(GetWorld(), SwitchSound, GetActorLocation(), GetActorRotation());
	UpdatePlatform();
}

void APRSSwitchablePlatform::BeginPlay()
{
	Super::BeginPlay();

	checkf(SwitchSound, TEXT("Config | Sound: SwitchSound is not set"));
}

void APRSSwitchablePlatform::UpdatePlatform()
{
	StaticMeshComp->SetCollisionProfileName(
		bEnabled ? UAVCollisionProfileStatics::BlockAll_ProfileName : UAVCollisionProfileStatics::NoCollision_ProfileName, true);
	StaticMeshComp->SetMaterial(0, bEnabled ? PlatformEnabledMaterial : PlatformDisabledMaterial);
}
