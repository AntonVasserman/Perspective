#pragma once

#include "CoreMinimal.h"

// TODO: Consider changing this into a single PRSConstants class, that has multiple sections/namespaces???
// TODO: Or consider moving those constants to their related class, e.g. PRSCharacter...
class PERSPECTIVE_API FPRSCharacterAnimationConstants
{
public:
	FPRSCharacterAnimationConstants() = delete;

	inline static const FName InteractedNotifyName = FName(TEXT("Interacted"));
	inline static const FName InteractionTargetWarpTargetName = FName(TEXT("InteractionTarget"));
};
