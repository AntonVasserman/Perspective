// Copyright Anton Vasserman, All Rights Reserved.

#include "Operatables//PRSOperatableActor.h"

void APRSOperatableActor::Operate()
{
	if (IsOperatable())
	{
		Operate_Implementation();
	}
}

void APRSOperatableActor::SetOperatability(const bool bInOperatable)
{
	bOperatable = bInOperatable;
	OnOperationStateChanged.Broadcast(bOperatable);
}
