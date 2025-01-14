// Copyright Anton Vasserman, All Rights Reserved.

#include "Core/PRSOperatableActor.h"

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
