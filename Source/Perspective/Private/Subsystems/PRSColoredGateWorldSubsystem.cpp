// Copyright Anton Vasserman, All Rights Reserved.


#include "Subsystems/PRSColoredGateWorldSubsystem.h"

void UPRSColoredGateWorldSubsystem::SetColor(EGateColor NewGateColor)
{
	CurrentGateColor = NewGateColor;
	OnGateColorChanged.Broadcast(CurrentGateColor);
}
