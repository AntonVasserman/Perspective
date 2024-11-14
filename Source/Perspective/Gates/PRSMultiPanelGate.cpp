// Copyright Anton Vasserman, All Rights Reserved.

#include "PRSMultiPanelGate.h"

#include "Engine/CollisionProfile.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Perspective/Characters/PRSCharacter.h"
#include "Perspective/Core/Utility/PRSStatics.h"
#include "Perspective/Subsystems/PRSModeWorldSubsystem.h"
#include "Sound/SoundCue.h"

APRSMultiPanelGate::APRSMultiPanelGate()
{
	PrimaryActorTick.bCanEverTick = true;

	// Setup Box Components and their respective Static Mesh Cube "Effect" Component
	CenterBoxComp = CreateDefaultSubobject<UBoxComponent>("Center Box");
	CenterBoxComp->SetBoxExtent(FVector(PanelLength, PanelLength, PanelLength));
	SetRootComponent(CenterBoxComp);

	FrontBoxComp = CreateDefaultSubobject<UBoxComponent>("Front Box");
	FrontBoxComp->SetBoxExtent(FVector(PanelLength, PanelLength, PanelLength));
	FrontBoxComp->SetRelativeLocation(FVector(PanelLength + 1.f, 0.f, 0.f));
	FrontBoxComp->SetRelativeRotation(FRotator::ZeroRotator);
	FrontBoxComp->SetRelativeScale3D(FVector(1.f / PanelLength, 1.f, 1.f));
	FrontBoxComp->AttachToComponent(CenterBoxComp, FAttachmentTransformRules::KeepRelativeTransform);
	FrontCubeEffectComp = CreateDefaultSubobject<UStaticMeshComponent>("Front Cube Effect");
	FrontCubeEffectComp->AttachToComponent(FrontBoxComp, FAttachmentTransformRules::KeepRelativeTransform);
	FrontCubeEffectComp->SetStaticMesh(UPRSStatics::GetCubeStaticMesh());
	FrontCubeEffectComp->SetMaterial(0, UPRSStatics::GetPanelGreenMaterial());
	FrontCubeEffectComp->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);

	BackBoxComp = CreateDefaultSubobject<UBoxComponent>("Back Box");
	BackBoxComp->SetBoxExtent(FVector(PanelLength, PanelLength, PanelLength));
	BackBoxComp->SetRelativeLocation(FVector(-PanelLength - 1.f, 0.f, 0.f));
	BackBoxComp->SetRelativeRotation(FRotator(0.f, 180.f, 0.f));
	BackBoxComp->SetRelativeScale3D(FVector(1.f / PanelLength, 1.f, 1.f));
	BackBoxComp->AttachToComponent(CenterBoxComp, FAttachmentTransformRules::KeepRelativeTransform);
	BackCubeEffectComp = CreateDefaultSubobject<UStaticMeshComponent>("Back Cube Effect");
	BackCubeEffectComp->AttachToComponent(BackBoxComp, FAttachmentTransformRules::KeepRelativeTransform);
	BackCubeEffectComp->SetStaticMesh(UPRSStatics::GetCubeStaticMesh());
	BackCubeEffectComp->SetMaterial(0, UPRSStatics::GetPanelGreenMaterial());
	BackCubeEffectComp->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);

	RightBoxComp = CreateDefaultSubobject<UBoxComponent>("Right Box");
	RightBoxComp->SetBoxExtent(FVector(PanelLength, PanelLength, PanelLength));
	RightBoxComp->SetRelativeLocation(FVector(0.f, PanelLength + 1.f, 0.f));
	RightBoxComp->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));
	RightBoxComp->SetRelativeScale3D(FVector(1.f / PanelLength, 1.f, 1.f));
	RightBoxComp->AttachToComponent(CenterBoxComp, FAttachmentTransformRules::KeepRelativeTransform);
	RightCubeEffectComp = CreateDefaultSubobject<UStaticMeshComponent>("Right Cube Effect");
	RightCubeEffectComp->AttachToComponent(RightBoxComp, FAttachmentTransformRules::KeepRelativeTransform);
	RightCubeEffectComp->SetStaticMesh(UPRSStatics::GetCubeStaticMesh());
	RightCubeEffectComp->SetMaterial(0, UPRSStatics::GetPanelGreenMaterial());
	RightCubeEffectComp->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);

	LeftBoxComp = CreateDefaultSubobject<UBoxComponent>("Left Box");
	LeftBoxComp->SetBoxExtent(FVector(PanelLength, PanelLength, PanelLength));
	LeftBoxComp->SetRelativeLocation(FVector(0.f, -PanelLength - 1.f, 0.f));
	LeftBoxComp->SetRelativeRotation(FRotator(0.f, 270.f, 0.f));
	LeftBoxComp->SetRelativeScale3D(FVector(1.f / PanelLength, 1.f, 1.f));
	LeftBoxComp->AttachToComponent(CenterBoxComp, FAttachmentTransformRules::KeepRelativeTransform);
	LeftCubeEffectComp = CreateDefaultSubobject<UStaticMeshComponent>("Left Cube Effect");
	LeftCubeEffectComp->AttachToComponent(LeftBoxComp, FAttachmentTransformRules::KeepRelativeTransform);
	LeftCubeEffectComp->SetStaticMesh(UPRSStatics::GetCubeStaticMesh());
	LeftCubeEffectComp->SetMaterial(0, UPRSStatics::GetPanelGreenMaterial());
	LeftCubeEffectComp->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);

	// Setup Box Components Mappings
	BoxComponentToDirectionMapping =
	{
		{FrontBoxComp, EGateDirection::Front},
		{BackBoxComp, EGateDirection::Back},
		{RightBoxComp, EGateDirection::Right},
		{LeftBoxComp, EGateDirection::Left},
	};
	DirectionToBoxComponentMapping =
	{
		{EGateDirection::Front, FrontBoxComp},
		{EGateDirection::Back, BackBoxComp},
		{EGateDirection::Right, RightBoxComp},
		{EGateDirection::Left, LeftBoxComp},
	};
}

void APRSMultiPanelGate::BeginPlay()
{
	Super::BeginPlay();

	CenterBoxComp->OnComponentBeginOverlap.AddDynamic(this, &APRSMultiPanelGate::CenterBoxCompOnComponentBeginOverlap);
	CenterBoxComp->OnComponentEndOverlap.AddDynamic(this, &APRSMultiPanelGate::CenterBoxCompOnComponentEndOverlap);
	FrontBoxComp->OnComponentEndOverlap.AddDynamic(this, &APRSMultiPanelGate::BoxCompOnComponentEndOverlap);
	BackBoxComp->OnComponentEndOverlap.AddDynamic(this, &APRSMultiPanelGate::BoxCompOnComponentEndOverlap);
	RightBoxComp->OnComponentEndOverlap.AddDynamic(this, &APRSMultiPanelGate::BoxCompOnComponentEndOverlap);
	LeftBoxComp->OnComponentEndOverlap.AddDynamic(this, &APRSMultiPanelGate::BoxCompOnComponentEndOverlap);
}

void APRSMultiPanelGate::CenterBoxCompOnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsValid(Cast<APRSCharacter>(OtherActor)))
	{
		return;
	}
	
	bIsTouchingInsideBox = true;
}

void APRSMultiPanelGate::CenterBoxCompOnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!IsValid(Cast<APRSCharacter>(OtherActor)))
	{
		return;
	}

	bIsTouchingInsideBox = false;
}

void APRSMultiPanelGate::BoxCompOnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
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

void APRSMultiPanelGate::InternalBoxComponentOnComponentEndOverlap(const UBoxComponent* OverlappedBoxComponent, APRSCharacter* PRSCharacter)
{
	// Get Box Direction
	const EGateDirection Direction = *BoxComponentToDirectionMapping.Find(OverlappedBoxComponent);
	UStaticMeshComponent* OverlappedCubeEffectComponent = Cast<UStaticMeshComponent>(OverlappedBoxComponent->GetChildComponent(0));

	// Entered box
	if (!bIsInsideBox && bIsTouchingInsideBox)
	{
		bIsInsideBox = true;
		EnterDirection = Direction;
		UE_LOG(LogTemp, Warning, TEXT("Entered from: %hhd"), Direction);
		OverlappedCubeEffectComponent->SetMaterial(0, UPRSStatics::GetPanelYellowMaterial());
		if (FrontCubeEffectComp != OverlappedCubeEffectComponent)
		{
			FrontCubeEffectComp->SetMaterial(0, UPRSStatics::GetPanelGreenMaterial());
		}
		if (BackCubeEffectComp != OverlappedCubeEffectComponent)
		{
			BackCubeEffectComp->SetMaterial(0, UPRSStatics::GetPanelGreenMaterial());
		}
		if (RightCubeEffectComp != OverlappedCubeEffectComponent)
		{
			RightCubeEffectComp->SetMaterial(0, UPRSStatics::GetPanelGreenMaterial());
		}
		if (LeftCubeEffectComp != OverlappedCubeEffectComponent)
		{
			LeftCubeEffectComp->SetMaterial(0, UPRSStatics::GetPanelGreenMaterial());
		}
	}
	else if (bIsInsideBox && !bIsTouchingInsideBox) // Exited box
	{
		bIsInsideBox = false;
		ExitDirection = Direction;
		UE_LOG(LogTemp, Warning, TEXT("Exited from: %hhd"), Direction);

		const UBoxComponent* EnterBoxComponent = Cast<UBoxComponent>(*DirectionToBoxComponentMapping.Find(EnterDirection));
		UStaticMeshComponent* EnterCubeEffectComponent = Cast<UStaticMeshComponent>(EnterBoxComponent->GetChildComponent(0));
		EnterCubeEffectComponent->SetMaterial(0, UPRSStatics::GetPanelGreenMaterial());
		
		// Exit from the same direction entered
		if (EnterDirection == ExitDirection)
		{
			return;
		}

		OverlappedCubeEffectComponent->SetMaterial(0, UPRSStatics::GetPanelRedMaterial());
		OverlappedCubeEffectComponent->SetCollisionProfileName(UCollisionProfile::BlockAll_ProfileName);
		
		PRSCharacter->SetForwardVectorOverride(OverlappedBoxComponent->GetForwardVector());
		GetWorld()->GetSubsystem<UPRSModeWorldSubsystem>()->Switch();
		UGameplayStatics::PlaySound2D(this, PerspectiveModeChangedSoundCue);
	}
}

void APRSMultiPanelGate::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	// Adjust all Boxes Scales
	const float XScale = Transform.GetScale3D().X;
	const float YScale = Transform.GetScale3D().Y;

	if (IsValid(RightBoxComp))
	{
		RightBoxComp->SetRelativeScale3D(FVector(1.f / PanelLength, XScale / YScale, 1.f));
	}
	if (IsValid(LeftBoxComp))
	{
		LeftBoxComp->SetRelativeScale3D(FVector(1.f / PanelLength, XScale / YScale, 1.f));
	}
}

