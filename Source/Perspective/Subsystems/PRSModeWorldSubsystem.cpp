// Copyright Anton Vasserman, All Rights Reserved.

#include "PRSModeWorldSubsystem.h"

EPerspectiveMode UPRSModeWorldSubsystem::GetMode() const
{
	return Mode;
}

void UPRSModeWorldSubsystem::Switch()
{
	Mode = Mode == EPerspectiveMode::TwoDimensional ?
		EPerspectiveMode::ThreeDimensional :
		EPerspectiveMode::TwoDimensional;
	OnPerspectiveModeChanged.Broadcast(Mode);
}
