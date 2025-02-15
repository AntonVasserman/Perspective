// Copyright Anton Vasserman, All Rights Reserved.

#include "Operatables/Gates/PRSPanel.h"

#include "AVCollisionProfileStatics.h"
#include "Characters/PRSCharacter.h"
#include "Core/Utility/PRSStatics.h"

UPRSPanel::UPRSPanel()
{
	PrimaryComponentTick.bCanEverTick = true;
	
	UStaticMeshComponent::SetStaticMesh(UPRSStatics::GetCubeStaticMesh());
	UMeshComponent::SetMaterial(0, PanelOpenMaterial);
	UStaticMeshComponent::SetCollisionProfileName(UAVCollisionProfileStatics::OverlapAllDynamic_ProfileName);
}

void UPRSPanel::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	SetEnabled(bPanelEnabled);
}

void UPRSPanel::SetOpen()
{
	CurrentState = EState::Open;
	UMeshComponent::SetMaterial(0, PanelOpenMaterial);
	UStaticMeshComponent::SetCollisionProfileName(UAVCollisionProfileStatics::OverlapAllDynamic_ProfileName);
}

void UPRSPanel::SetPending()
{
	CurrentState = EState::Pending;
	UMeshComponent::SetMaterial(0, PanelPendingMaterial);
	UStaticMeshComponent::SetCollisionProfileName(UAVCollisionProfileStatics::OverlapAllDynamic_ProfileName);
}

void UPRSPanel::SetClosed()
{
	CurrentState = EState::Closed;
	UMeshComponent::SetMaterial(0, PanelClosedMaterial);
	UStaticMeshComponent::SetCollisionProfileName(UAVCollisionProfileStatics::BlockAll_ProfileName);
}

void UPRSPanel::SetEnabled(bool bEnabled)
{
	bPanelEnabled = bEnabled;

	SetVisibility(bPanelEnabled);
	UStaticMeshComponent::SetCollisionProfileName(bPanelEnabled ? UAVCollisionProfileStatics::OverlapAllDynamic_ProfileName : UAVCollisionProfileStatics::NoCollision_ProfileName);
}

void UPRSPanel::BeginPlay()
{
	Super::BeginPlay();

	checkf(PanelOpenMaterial, TEXT("Config | Material: PanelOpenMaterial is not set"));
	checkf(PanelPendingMaterial, TEXT("Config | Material: PanelPendingMaterial is not set"));
	checkf(PanelClosedMaterial, TEXT("Config | Material: PanelClosedMaterial is not set"));
	
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
