// Fill out your copyright notice in the Description page of Project Settings.


#include "PRSBoxModeChanger.h"

#include "Components/BoxComponent.h"

APRSBoxModeChanger::APRSBoxModeChanger()
{
	PrimaryActorTick.bCanEverTick = true;

	// Setup Box Components
	CenterBoxComp = CreateDefaultSubobject<UBoxComponent>("Center Box Component");
	CenterBoxComp->SetBoxExtent(FVector(PanelLength - 1.f, PanelLength - 1.f, PanelLength - 1.f));
	SetRootComponent(CenterBoxComp.Get());

	FrontBoxComp = CreateDefaultSubobject<UBoxComponent>("Front Box Component");
	FrontBoxComp->SetBoxExtent(FVector(1.f, PanelLength - 1.f, PanelLength - 1.f));
	FrontBoxComp->SetRelativeLocation(FVector(PanelLength, 0.f, 0.f));
	FrontBoxComp->SetRelativeRotation(FRotator::ZeroRotator);
	FrontBoxComp->AttachToComponent(CenterBoxComp.Get(), FAttachmentTransformRules::KeepRelativeTransform);

	BackBoxComp = CreateDefaultSubobject<UBoxComponent>("Back Box Component");
	BackBoxComp->SetBoxExtent(FVector(1.f, PanelLength - 1.f, PanelLength - 1.f));
	BackBoxComp->SetRelativeLocation(FVector(-PanelLength, 0.f, 0.f));
	BackBoxComp->SetRelativeRotation(FRotator(0.f, 180.f, 0.f));
	BackBoxComp->AttachToComponent(CenterBoxComp.Get(), FAttachmentTransformRules::KeepRelativeTransform);

	RightBoxComp = CreateDefaultSubobject<UBoxComponent>("Right Box Component");
	RightBoxComp->SetBoxExtent(FVector(1.f, PanelLength - 1.f, PanelLength - 1.f));
	RightBoxComp->SetRelativeLocation(FVector(0.f, PanelLength, 0.f));
	RightBoxComp->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));
	RightBoxComp->AttachToComponent(CenterBoxComp.Get(), FAttachmentTransformRules::KeepRelativeTransform);

	LeftBoxComp = CreateDefaultSubobject<UBoxComponent>("Left Box Component");
	LeftBoxComp->SetBoxExtent(FVector(1.f, PanelLength - 1.f, PanelLength - 1.f));
	LeftBoxComp->SetRelativeLocation(FVector(0.f, -PanelLength, 0.f));
	LeftBoxComp->SetRelativeRotation(FRotator(0.f, 270.f, 0.f));
	LeftBoxComp->AttachToComponent(CenterBoxComp.Get(), FAttachmentTransformRules::KeepRelativeTransform);

	TopBoxComp = CreateDefaultSubobject<UBoxComponent>("Top Box Component");
	TopBoxComp->SetBoxExtent(FVector(1.f, PanelLength - 1.f, PanelLength - 1.f));
	TopBoxComp->SetRelativeLocation(FVector(0.f, 0.f, PanelLength));
	TopBoxComp->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));
	TopBoxComp->AttachToComponent(CenterBoxComp.Get(), FAttachmentTransformRules::KeepRelativeTransform);

	BottomBoxComp = CreateDefaultSubobject<UBoxComponent>("Bottom Box Component");
	BottomBoxComp->SetBoxExtent(FVector(1.f, PanelLength - 1.f, PanelLength - 1.f));
	BottomBoxComp->SetRelativeLocation(FVector(0.f, 0.f, -PanelLength));
	BottomBoxComp->SetRelativeRotation(FRotator(-90.f, 0.f, 0.f));
	BottomBoxComp->AttachToComponent(CenterBoxComp.Get(), FAttachmentTransformRules::KeepRelativeTransform);
}

void APRSBoxModeChanger::BeginPlay()
{
	Super::BeginPlay();

	// TODO PRS: remove
	CenterBoxComp->SetHiddenInGame(false);
	FrontBoxComp->SetHiddenInGame(false);
	BackBoxComp->SetHiddenInGame(false);
	RightBoxComp->SetHiddenInGame(false);
	LeftBoxComp->SetHiddenInGame(false);
	TopBoxComp->SetHiddenInGame(false);
	BottomBoxComp->SetHiddenInGame(false);
	//
}

void APRSBoxModeChanger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APRSBoxModeChanger::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	// Adjust all Boxes Scales
	const float XScale = Transform.GetScale3D().X;
	const float YScale = Transform.GetScale3D().Y;
	const float ZScale = Transform.GetScale3D().Z;

	RightBoxComp->SetRelativeScale3D(FVector(1.f, XScale / YScale, 1.f));
	LeftBoxComp->SetRelativeScale3D(FVector(1.f, XScale / YScale, 1.f));
	TopBoxComp->SetRelativeScale3D(FVector(1.f, 1.f, XScale / ZScale));
	BottomBoxComp->SetRelativeScale3D(FVector(1.f, 1.f, XScale / ZScale));
}

