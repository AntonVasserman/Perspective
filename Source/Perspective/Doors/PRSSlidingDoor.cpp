// Copyright Anton Vasserman, All Rights Reserved.

#include "PRSSlidingDoor.h"

void APRSSlidingDoor::Operate()
{
	switch (CurrentState)
	{
	case ESlidingDoorState::Closed:
		CurrentState = ESlidingDoorState::Opening;
		break;
	case ESlidingDoorState::Closing:
		break;
	case ESlidingDoorState::Open:
		CurrentState = ESlidingDoorState::Closing;
	case ESlidingDoorState::Opening:
		break;
	default:
		break;
	}

	OnDoorStateChanged.Broadcast(CurrentState);
}
