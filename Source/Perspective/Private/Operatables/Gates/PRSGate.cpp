// Copyright Anton Vasserman, All Rights Reserved.


#include "Operatables/Gates/PRSGate.h"

#include "AVCollisionProfileStatics.h"
#include "Characters/PRSCharacter.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Subsystems/PRSModeWorldSubsystem.h"

APRSGate::APRSGate()
{
	RootComp = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(RootComp);
	
	BackBoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Back Box"));
	BackBoxComp->SetupAttachment(GetRootComponent());
	BackBoxComp->SetBoxExtent(FVector(50.f, 100.f, 100.f));
	BackBoxComp->SetRelativeLocation(FVector(-55.f, 0.f, 100.f));
	BackBoxComp->bEditableWhenInherited = false;
	
	FrontBoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Front Box"));
	FrontBoxComp->SetupAttachment(GetRootComponent());
	FrontBoxComp->SetBoxExtent(FVector(50.f, 100.f, 100.f));
	FrontBoxComp->SetRelativeLocation(FVector(55.f, 0.f, 100.f));
	FrontBoxComp->bEditableWhenInherited = false;
	
	GateMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Gate Static Mesh"));
	GateMeshComp->SetupAttachment(GetRootComponent());
	GateMeshComp->SetCollisionProfileName(UAVCollisionProfileStatics::OverlapAllDynamic_ProfileName);
}

void APRSGate::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	GateMeshComp->OnComponentBeginOverlap.AddDynamic(this, &APRSGate::GateMeshOnBeginOverlap);
	GateMeshComp->OnComponentEndOverlap.AddDynamic(this, &APRSGate::GateMeshOnEndOverlap);
}

void APRSGate::Operate_Implementation()
{
	SetOperatability(false);
	bEnabled = !bEnabled;
	GateMeshComp->SetHiddenInGame(!bEnabled);
	UPRSSoundStatics::PlaySoundAtLocation(GetWorld(), EnableDisableSound, GetActorLocation(), GetActorRotation());
	SetOperatability(true);
}

void APRSGate::GateMeshOnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                      int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bEnabled || !Cast<APRSCharacter>(OtherActor))
	{
		return;
	}
	
	if (BackBoxComp->IsOverlappingActor(OtherActor))
	{
		EnterGateSide = EGateSide::Back;
	}
	else if (FrontBoxComp->IsOverlappingActor(OtherActor))
	{
		EnterGateSide = EGateSide::Front;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("APRSGate::GateMeshOnEndOverlap: Undesired case, Character started overlapping while not being in boxes"));
	}
}

void APRSGate::GateMeshOnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!bEnabled || !Cast<APRSCharacter>(OtherActor))
	{
		return;
	}
	
	if (BackBoxComp->IsOverlappingActor(OtherActor))
	{
		ExitGateSide = EGateSide::Back;
	}
	else if (FrontBoxComp->IsOverlappingActor(OtherActor))
	{
		ExitGateSide = EGateSide::Front;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("APRSGate::GateMeshOnEndOverlap: Undesired case, Character stopped overlapping while not being in boxes"));
	}

	if (EnterGateSide == ExitGateSide)
	{
		// Exit from same side that entered, in that case no need to change perspective mode
		return;
	}

	FRotator NewRotation = FRotator(GateMeshComp->GetComponentRotation());

	NewRotation = ExitGateSide == EGateSide::Front ? NewRotation : NewRotation + FRotator(0.f, 180.f, 0.f);
	
	GetWorld()->GetSubsystem<UPRSModeWorldSubsystem>()->Switch(NewRotation);
	UGameplayStatics::PlaySound2D(this, PerspectiveModeChangedSoundCue);

	EnterGateSide = EGateSide::None;
	ExitGateSide = EGateSide::None;
}
