// Copyright Anton Vasserman, All Rights Reserved.

#include "PRSSoundStatics.h"

#include "Kismet/GameplayStatics.h"

void UPRSSoundStatics::PlaySoundAtLocation(const UObject* WorldContextObject, USoundBase* Sound, const FVector& Location, const FRotator& Rotation,
	const float VolumeMultiplier, const float PitchMultiplier)
{
	const float RandomizedVolumeMultiplier = FMath::RandRange(0.9f, 1.f) * VolumeMultiplier;
	const float RandomizedPitchMultiplier = FMath::RandRange(0.9f, 1.1f) * PitchMultiplier;
	UGameplayStatics::PlaySoundAtLocation(WorldContextObject, Sound, Location, Rotation, RandomizedVolumeMultiplier, RandomizedPitchMultiplier);
}
