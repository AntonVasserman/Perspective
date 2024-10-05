// Fill out your copyright notice in the Description page of Project Settings.


#include "PRSBoxModeChanger.h"

#include "PRSCharacter.h"
#include "PRSModeWorldSubsystem.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

APRSBoxModeChanger::APRSBoxModeChanger()
{
	PrimaryActorTick.bCanEverTick = true;

	// Setup Box Components
	CenterBoxComp = CreateDefaultSubobject<UBoxComponent>("Center Box Component");
	CenterBoxComp->SetBoxExtent(FVector(PanelLength, PanelLength, PanelLength));
	SetRootComponent(CenterBoxComp.Get());

	FrontBoxComp = CreateDefaultSubobject<UBoxComponent>("Front Box Component");
	FrontBoxComp->SetBoxExtent(FVector(1.f, PanelLength, PanelLength));
	FrontBoxComp->SetRelativeLocation(FVector(PanelLength + 1.f, 0.f, 0.f));
	FrontBoxComp->SetRelativeRotation(FRotator::ZeroRotator);
	FrontBoxComp->AttachToComponent(CenterBoxComp.Get(), FAttachmentTransformRules::KeepRelativeTransform);

	BackBoxComp = CreateDefaultSubobject<UBoxComponent>("Back Box Component");
	BackBoxComp->SetBoxExtent(FVector(1.f, PanelLength, PanelLength));
	BackBoxComp->SetRelativeLocation(FVector(-PanelLength - 1.f, 0.f, 0.f));
	BackBoxComp->SetRelativeRotation(FRotator(0.f, 180.f, 0.f));
	BackBoxComp->AttachToComponent(CenterBoxComp.Get(), FAttachmentTransformRules::KeepRelativeTransform);

	RightBoxComp = CreateDefaultSubobject<UBoxComponent>("Right Box Component");
	RightBoxComp->SetBoxExtent(FVector(1.f, PanelLength, PanelLength));
	RightBoxComp->SetRelativeLocation(FVector(0.f, PanelLength + 1.f, 0.f));
	RightBoxComp->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));
	RightBoxComp->AttachToComponent(CenterBoxComp.Get(), FAttachmentTransformRules::KeepRelativeTransform);

	LeftBoxComp = CreateDefaultSubobject<UBoxComponent>("Left Box Component");
	LeftBoxComp->SetBoxExtent(FVector(1.f, PanelLength, PanelLength));
	LeftBoxComp->SetRelativeLocation(FVector(0.f, -PanelLength - 1.f, 0.f));
	LeftBoxComp->SetRelativeRotation(FRotator(0.f, 270.f, 0.f));
	LeftBoxComp->AttachToComponent(CenterBoxComp.Get(), FAttachmentTransformRules::KeepRelativeTransform);

	TopBoxComp = CreateDefaultSubobject<UBoxComponent>("Top Box Component");
	TopBoxComp->SetBoxExtent(FVector(1.f, PanelLength, PanelLength));
	TopBoxComp->SetRelativeLocation(FVector(0.f, 0.f, PanelLength + 1.f));
	TopBoxComp->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));
	TopBoxComp->AttachToComponent(CenterBoxComp.Get(), FAttachmentTransformRules::KeepRelativeTransform);

	BottomBoxComp = CreateDefaultSubobject<UBoxComponent>("Bottom Box Component");
	BottomBoxComp->SetBoxExtent(FVector(1.f, PanelLength, PanelLength));
	BottomBoxComp->SetRelativeLocation(FVector(0.f, 0.f, -PanelLength - 1.f));
	BottomBoxComp->SetRelativeRotation(FRotator(-90.f, 0.f, 0.f));
	BottomBoxComp->AttachToComponent(CenterBoxComp.Get(), FAttachmentTransformRules::KeepRelativeTransform);

	// Setup Box Components Mappings
	BoxComponentToDirectionMapping =
	{
		{FrontBoxComp.Get(), EDirection::Front},
		{BackBoxComp.Get(), EDirection::Back},
		{RightBoxComp.Get(), EDirection::Right},
		{LeftBoxComp.Get(), EDirection::Left},
		{TopBoxComp.Get(), EDirection::Top},
		{BottomBoxComp.Get(), EDirection::Bottom},
	};
	DirectionToBoxComponentMapping =
	{
		{EDirection::Front, FrontBoxComp.Get()},
		{EDirection::Back, BackBoxComp.Get()},
		{EDirection::Right, RightBoxComp.Get()},
		{EDirection::Left, LeftBoxComp.Get()},
		{EDirection::Top, TopBoxComp.Get()},
		{EDirection::Bottom, BottomBoxComp.Get()},
	};
}

void APRSBoxModeChanger::BeginPlay()
{
	Super::BeginPlay();

	CenterBoxComp->OnComponentBeginOverlap.AddDynamic(this, &APRSBoxModeChanger::CenterBoxCompOnComponentBeginOverlap);
	CenterBoxComp->OnComponentEndOverlap.AddDynamic(this, &APRSBoxModeChanger::CenterBoxCompOnComponentEndOverlap);
	FrontBoxComp->OnComponentEndOverlap.AddDynamic(this, &APRSBoxModeChanger::BoxCompOnComponentEndOverlap);
	BackBoxComp->OnComponentEndOverlap.AddDynamic(this, &APRSBoxModeChanger::BoxCompOnComponentEndOverlap);
	RightBoxComp->OnComponentEndOverlap.AddDynamic(this, &APRSBoxModeChanger::BoxCompOnComponentEndOverlap);
	LeftBoxComp->OnComponentEndOverlap.AddDynamic(this, &APRSBoxModeChanger::BoxCompOnComponentEndOverlap);
	TopBoxComp->OnComponentEndOverlap.AddDynamic(this, &APRSBoxModeChanger::BoxCompOnComponentEndOverlap);
	BottomBoxComp->OnComponentEndOverlap.AddDynamic(this, &APRSBoxModeChanger::BoxCompOnComponentEndOverlap);
	
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

void APRSBoxModeChanger::CenterBoxCompOnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsValid(Cast<APRSCharacter>(OtherActor)))
	{
		return;
	}
	
	bIsTouchingInsideBox = true;
}

void APRSBoxModeChanger::CenterBoxCompOnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!IsValid(Cast<APRSCharacter>(OtherActor)))
	{
		return;
	}

	bIsTouchingInsideBox = false;
}

void APRSBoxModeChanger::BoxCompOnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (APRSCharacter* PRSCharacter = Cast<APRSCharacter>(OtherActor);
		IsValid(PRSCharacter))
	{
		if (UBoxComponent* BoxComp = Cast<UBoxComponent>(OverlappedComponent);
			IsValid(BoxComp))
		{
			InternalBoxComponentOnComponentEndOverlap(BoxComp, PRSCharacter);
		}
	}
}

void APRSBoxModeChanger::InternalBoxComponentOnComponentEndOverlap(const UBoxComponent* OverlappedBoxComponent, APRSCharacter* PRSCharacter)
{
	// Get Box Direction
	const EDirection Direction = *BoxComponentToDirectionMapping.Find(OverlappedBoxComponent);
	
	// Entered box
	if (!bIsInsideBox && bIsTouchingInsideBox)
	{
		bIsInsideBox = true;
		EnterDirection = Direction;
		UE_LOG(LogTemp, Warning, TEXT("Entered from: %hhd"), Direction);
	}
	else if (bIsInsideBox && !bIsTouchingInsideBox) // Exited box
	{
		bIsInsideBox = false;
		ExitDirection = Direction;
		UE_LOG(LogTemp, Warning, TEXT("Exited from: %hhd"), Direction);

		// Exit from the same direction entered
		if (EnterDirection == ExitDirection)
		{
			return;
		}

		if (ExitDirection == EDirection:: Top || ExitDirection == EDirection:: Bottom)
		{
			if (EnterDirection != EDirection:: Top && EnterDirection != EDirection:: Bottom)
			{
				PRSCharacter->SetForwardVectorOverride((*DirectionToBoxComponentMapping.Find(EnterDirection))->GetForwardVector() * -1);
				GetWorld()->GetSubsystem<UPRSModeWorldSubsystem>()->Switch();
				UGameplayStatics::PlaySound2D(this, PerspectiveModeChangedSoundCue);
			}
		}
		else
		{
			PRSCharacter->SetForwardVectorOverride(OverlappedBoxComponent->GetForwardVector());
			GetWorld()->GetSubsystem<UPRSModeWorldSubsystem>()->Switch();
			UGameplayStatics::PlaySound2D(this, PerspectiveModeChangedSoundCue);
		}
	}
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

