// Copyright Anton Vasserman, All Rights Reserved.

#include "Doors/PRSSlidingDoor.h"

#include "Core/Utility/PRSSoundStatics.h"
#include "Sound/SoundCue.h"

void APRSSlidingDoor::Operate_Implementation()
{
	SetOperatability(false);
	switch (CurrentState)
	{
	case ESlidingDoorState::Closed:
		CurrentState = ESlidingDoorState::Opening;
		UPRSSoundStatics::PlaySoundAtLocation(GetWorld(), OpenCloseSound, GetActorLocation(), GetActorRotation(), 1.f,
			OpenCloseSound->Duration / DoorOpenDuration);
		break;
	case ESlidingDoorState::Closing:
		break;
	case ESlidingDoorState::Open:
		CurrentState = ESlidingDoorState::Closing;
		UPRSSoundStatics::PlaySoundAtLocation(GetWorld(), OpenCloseSound, GetActorLocation(), GetActorRotation(), 1.f,
			OpenCloseSound->Duration / DoorOpenDuration);
		break;
	case ESlidingDoorState::Opening:
		break;
	default:
		break;
	}

	OnDoorStateChanged.Broadcast(CurrentState);
}

void APRSSlidingDoor::BeginPlay()
{
	Super::BeginPlay();

	if (bOpenOnGameStart && !bCloseOnOpened)
	{
		CurrentState = ESlidingDoorState::Opening;
		OnDoorStateChanged.Broadcast(CurrentState);
	}
}
