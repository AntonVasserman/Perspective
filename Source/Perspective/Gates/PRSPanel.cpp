// Copyright Anton Vasserman, All Rights Reserved.

#include "PRSPanel.h"

#include "Perspective/Characters/PRSCharacter.h"
#include "Perspective/Core/Utility/PRSStatics.h"

UPRSPanel::UPRSPanel()
{
	PrimaryComponentTick.bCanEverTick = true;
	
	UStaticMeshComponent::SetStaticMesh(UPRSStatics::GetCubeStaticMesh());
	UMeshComponent::SetMaterial(0, UPRSStatics::GetPanelGreenMaterial());
	UStaticMeshComponent::SetCollisionProfileName(UPRSStatics::GetCollisionProfileNameOverlapAllDynamic());
}

void UPRSPanel::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	SetEnabled(bPanelEnabled);
}

void UPRSPanel::SetOpen()
{
	CurrentState = EState::Open;
	UMeshComponent::SetMaterial(0, UPRSStatics::GetPanelGreenMaterial());
	UStaticMeshComponent::SetCollisionProfileName(UPRSStatics::GetCollisionProfileNameOverlapAllDynamic());
}

void UPRSPanel::SetPending()
{
	CurrentState = EState::Pending;
	UMeshComponent::SetMaterial(0, UPRSStatics::GetPanelYellowMaterial());
	UStaticMeshComponent::SetCollisionProfileName(UPRSStatics::GetCollisionProfileNameOverlapAllDynamic());
}

void UPRSPanel::SetClosed()
{
	CurrentState = EState::Closed;
	UMeshComponent::SetMaterial(0, UPRSStatics::GetPanelRedMaterial());
	UStaticMeshComponent::SetCollisionProfileName(UCollisionProfile::BlockAll_ProfileName);
}

void UPRSPanel::SetEnabled(bool bEnabled)
{
	bPanelEnabled = bEnabled;

	SetVisibility(bPanelEnabled);
	UStaticMeshComponent::SetCollisionProfileName(bPanelEnabled ? UPRSStatics::GetCollisionProfileNameOverlapAllDynamic() : UCollisionProfile::NoCollision_ProfileName);
}

void UPRSPanel::BeginPlay()
{
	Super::BeginPlay();

	OnComponentBeginOverlap.AddDynamic(this, &UPRSPanel::OnBeginOverlap);
	OnComponentEndOverlap.AddDynamic(this, &UPRSPanel::OnEndOverlap);
}

void UPRSPanel::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	APRSCharacter* PlayerCharacter = Cast<APRSCharacter>(OtherActor);

	if (!IsValid(PlayerCharacter))
	{
		return;
	}
	
	if (UPRSPanel* OverlappedPanel = Cast<UPRSPanel>(OverlappedComponent);
		IsValid(OverlappedPanel))
	{
		OnPlayerBeginOverlap.Broadcast(OverlappedPanel, PlayerCharacter);
	}
}

void UPRSPanel::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	APRSCharacter* PlayerCharacter = Cast<APRSCharacter>(OtherActor);

	if (!IsValid(PlayerCharacter))
	{
		return;
	}
	
	if (UPRSPanel* OverlappedPanel = Cast<UPRSPanel>(OverlappedComponent);
		IsValid(OverlappedPanel))
	{
		OnPlayerEndOverlap.Broadcast(OverlappedPanel, PlayerCharacter);
	}
}
