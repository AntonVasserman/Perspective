// Copyright Anton Vasserman, All Rights Reserved.

#include "PRSSoundStatics.h"

#include "Kismet/GameplayStatics.h"

void UPRSSoundStatics::PlaySoundAtLocation(const UObject* WorldContextObject, USoundBase* Sound, FVector Location, FRotator Rotation)
{
	const float VolumeMultiplier = FMath::RandRange(0.9f, 1.f);
	const float PitchMultiplier = FMath::RandRange(0.9f, 1.1f);
	UGameplayStatics::PlaySoundAtLocation(WorldContextObject, Sound, Location, Rotation, VolumeMultiplier, PitchMultiplier);
}
