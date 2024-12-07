// Copyright Anton Vasserman, All Rights Reserved.

#include "PRSRotatingDoor.h"

#include "Sound/SoundCue.h"

void APRSRotatingDoor::Operate()
{
	switch (CurrentState)
	{
	case ERotatingDoorState::Static:
		CurrentState = ERotatingDoorState::Rotating;
		UPRSSoundStatics::PlaySoundAtLocation(GetWorld(), OpenCloseSound, GetActorLocation(), GetActorRotation(), 1.f,
			OpenCloseSound->Duration / DoorOpenDuration);
		break;
	case ERotatingDoorState::Rotating:
		break;
	default:
		break;
	}

	OnRotationStateChanged.Broadcast(CurrentState);
}

void APRSRotatingDoor::BeginPlay()
{
	Super::BeginPlay();

	YawRotationDelta = bCounterClockwise ? -90.f : 90.f;
	OriginalRotation = GetActorRotation();
	NewRotation = bSupportFullRotation ?
		OriginalRotation :
		FRotator(OriginalRotation.Pitch, OriginalRotation.Yaw + YawRotationDelta, OriginalRotation.Roll);
}
