// Copyright Anton Vasserman, All Rights Reserved.

#include "Subsystems/PRSModeWorldSubsystem.h"

#include "Subsystems/PerspectiveModeChangedArgs.h"

void UPRSModeWorldSubsystem::Switch(const FRotator& NewControlRotation)
{
	Mode = Mode == EPerspectiveMode::TwoDimensional ?
		EPerspectiveMode::ThreeDimensional :
		EPerspectiveMode::TwoDimensional;
	OnPerspectiveModeChanged.Broadcast(FPerspectiveModeChangedArgs(Mode, NewControlRotation));
}
