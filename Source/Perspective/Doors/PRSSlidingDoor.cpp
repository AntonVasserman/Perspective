// Copyright Anton Vasserman, All Rights Reserved.

#include "PRSSlidingDoor.h"

#include "Perspective/Core/Utility/PRSSoundStatics.h"
#include "Sound/SoundCue.h"

void APRSSlidingDoor::Operate()
{
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
	case ESlidingDoorState::Opening:
		break;
	default:
		break;
	}

	OnDoorStateChanged.Broadcast(CurrentState);
}
