// Copyright Epic Games, Inc. All Rights Reserved.

#include "PerspectiveGameMode.h"
#include "PerspectiveCharacter.h"
#include "UObject/ConstructorHelpers.h"

APerspectiveGameMode::APerspectiveGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
