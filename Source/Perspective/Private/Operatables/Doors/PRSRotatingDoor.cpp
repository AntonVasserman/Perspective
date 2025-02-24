// Copyright Anton Vasserman, All Rights Reserved.

#include "Operatables/Doors/PRSRotatingDoor.h"

#include "Kismet/GameplayStatics.h"

void APRSRotatingDoor::Operate_Implementation()
{
	SetOperatability(false);
	switch (CurrentState)
	{
	case ERotatingDoorState::Static:
		CurrentState = ERotatingDoorState::Rotating;
		UGameplayStatics::PlaySoundAtLocation(this, OpenCloseSound, GetActorLocation(), GetActorRotation(), 1.f,
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

	checkf(OpenCloseSound, TEXT("Config | Sound: OpenCloseSound is not set"));
	
	RotationDelta = FRotator(0.f, bCounterClockwise ? -90.f : 90.f, 0.f);
	OriginalRotation = GetActorRotation();
	NextRotation = OriginalRotation + RotationDelta;
}
