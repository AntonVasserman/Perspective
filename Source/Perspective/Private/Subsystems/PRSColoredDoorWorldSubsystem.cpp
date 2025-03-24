// Copyright Anton Vasserman, All Rights Reserved.


#include "Subsystems/PRSColoredDoorWorldSubsystem.h"

void UPRSColoredDoorWorldSubsystem::SetBagColor(EDoorColor NewBagColor)
{
	CurrentBagColor = NewBagColor;
	OnBagColorChanged.Broadcast(CurrentBagColor);
}

void UPRSColoredDoorWorldSubsystem::AddKey(EDoorColor KeyColor)
{
	const uint32 CurrentCount = KeyColorCountMap[KeyColor];
	KeyColorCountMap[KeyColor] = CurrentCount + 1;
	OnKeyCountChanged.Broadcast();
}

bool UPRSColoredDoorWorldSubsystem::UseKey(EDoorColor KeyColor)
{
	if (!HasKey(KeyColor))
	{
		return false;
	}

	const uint32 CurrentCount = KeyColorCountMap[KeyColor];
	KeyColorCountMap[KeyColor] = CurrentCount - 1;
	OnKeyCountChanged.Broadcast();
	return true;
}

bool UPRSColoredDoorWorldSubsystem::HasKey(EDoorColor KeyColor)
{
	return KeyColorCountMap[KeyColor] > 0;
}

int32 UPRSColoredDoorWorldSubsystem::GetKeyCount(EDoorColor KeyColor)
{
	return KeyColorCountMap[KeyColor];
}

//~ UWorldSubsystem Begin

void UPRSColoredDoorWorldSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);

	KeyColorCountMap.Add(EDoorColor::EDC_Blue, 0);
	KeyColorCountMap.Add(EDoorColor::EDC_Green, 0);
	KeyColorCountMap.Add(EDoorColor::EDC_Red, 0);
	KeyColorCountMap.Add(EDoorColor::EDC_Yellow, 0);
}

//~ UWorldSubsystem End