// Copyright Anton Vasserman, All Rights Reserved.

#include "Subsystems/PRSModeWorldSubsystem.h"

#include "Core/Utility/PRSStatics.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Subsystems/PerspectiveModeChangedArgs.h"

void UPRSModeWorldSubsystem::Switch(const FRotator& NewControlRotation, const bool bOverridePlayerCharacterX, const float PlayerCharacterXOverride,
		const bool bOverridePlayerCharacterY, const float PlayerCharacterYOverride)
{
	Mode = Mode == EPerspectiveMode::TwoDimensional ?
		EPerspectiveMode::ThreeDimensional :
		EPerspectiveMode::TwoDimensional;
	UKismetMaterialLibrary::SetScalarParameterValue(
		GetWorld(),
		UPRSStatics::GetMaskingMaterialParameterCollection(),
		UPRSStatics::GetMaskingMaterialParameterCollectionMaskParameterName(),
		Mode == EPerspectiveMode::TwoDimensional ? 1.f : 0.f);
	OnPerspectiveModeChanged.Broadcast(FPerspectiveModeChangedArgs(Mode, NewControlRotation, bOverridePlayerCharacterX, PlayerCharacterXOverride,
		bOverridePlayerCharacterY, PlayerCharacterYOverride));
}

//~ UWorldSubsystem Begin
//~ UWorldSubsystem End
