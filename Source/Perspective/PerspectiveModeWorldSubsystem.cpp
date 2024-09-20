// Fill out your copyright notice in the Description page of Project Settings.

#include "PerspectiveModeWorldSubsystem.h"

EPerspectiveMode UPerspectiveModeWorldSubsystem::GetMode() const
{
	return Mode;
}

void UPerspectiveModeWorldSubsystem::Switch()
{
	Mode = Mode == EPerspectiveMode::TwoDimensional ?
		EPerspectiveMode::ThreeDimensional :
		EPerspectiveMode::TwoDimensional;
	OnPerspectiveModeChanged.Broadcast(Mode);
}
