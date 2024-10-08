// Fill out your copyright notice in the Description page of Project Settings.


#include "PRSDoor.h"

#include "PRSInteractableButton.h"

APRSDoor::APRSDoor()
{
	PrimaryActorTick.bCanEverTick = true;

}

void APRSDoor::BeginPlay()
{
	Super::BeginPlay();

	if (InteractableButton != nullptr)
	{
		InteractableButton->OnButtonPressed.AddDynamic(this, &APRSDoor::Open);
	}

	OriginalScale3D = GetActorScale3D();
}

void APRSDoor::Open()
{
	UE_LOG(LogTemp, Warning, TEXT("APRSDoor::Open"));
	bIsOpen = true;
}

void APRSDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (bIsOpen && GetActorScale().Y > 0.f)
	{
		PassedOpenDuration += DeltaTime;
		PassedOpenDuration = FMath::Clamp(PassedOpenDuration, 0.f, OpenDuration);
		const float NewActorYScale = FMath::Lerp(OriginalScale3D.Y, 0.f, PassedOpenDuration / OpenDuration);
		SetActorScale3D(FVector(OriginalScale3D.X, NewActorYScale, OriginalScale3D.Z));
	}
}

