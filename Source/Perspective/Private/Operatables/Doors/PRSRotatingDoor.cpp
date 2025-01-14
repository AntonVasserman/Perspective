// Copyright Anton Vasserman, All Rights Reserved.

#include "Operatables/Doors/PRSRotatingDoor.h"

#include "Sound/SoundCue.h"

void APRSRotatingDoor::Operate_Implementation()
{
	SetOperatability(false);
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

	RotationDelta = FRotator(0.f, bCounterClockwise ? -90.f : 90.f, 0.f);
	OriginalRotation = GetActorRotation();
	NextRotation = OriginalRotation + RotationDelta;
}
