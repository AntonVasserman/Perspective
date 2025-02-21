// Copyright Anton Vasserman, All Rights Reserved.


#include "Subsystems/PRSBagColorWorldSubsystem.h"

void UPRSBagColorWorldSubsystem::SetColor(EGateColor NewGateColor)
{
	CurrentGateColor = NewGateColor;
	OnGateColorChanged.Broadcast(CurrentGateColor);
}
