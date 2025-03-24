// Copyright Anton Vasserman, All Rights Reserved.

#include "Operatables/Doors/PRSSlidingDoor.h"

#include "Kismet/GameplayStatics.h"

//~ APRSDoor Begin

void APRSSlidingDoor::Operate_Implementation()
{
	SetOperatability(false);
	switch (CurrentState)
	{
	case ESlidingDoorState::Closed:
		CurrentState = ESlidingDoorState::Opening;
		UGameplayStatics::PlaySoundAtLocation(this, OpenCloseSound, GetActorLocation(), GetActorRotation(), 1.f,
			OpenCloseSound->Duration / DoorOpenDuration);
		break;
	case ESlidingDoorState::Closing:
		break;
	case ESlidingDoorState::Open:
		CurrentState = ESlidingDoorState::Closing;
		UGameplayStatics::PlaySoundAtLocation(this, OpenCloseSound, GetActorLocation(), GetActorRotation(), 1.f,
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

	checkf(OpenCloseSound, TEXT("Config | Sound: OpenCloseSound is not set"));
	
	if (bOpenOnGameStart && !bCloseOnOpened)
	{
		CurrentState = ESlidingDoorState::Opening;
		OnDoorStateChanged.Broadcast(CurrentState);
	}
}

//~ APRSDoor End
