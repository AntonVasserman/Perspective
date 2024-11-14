// Copyright Anton Vasserman, All Rights Reserved.


#include "PRSGate.h"

#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Perspective/Characters/PRSCharacter.h"
#include "Perspective/Doors/PRSDoor.h"
#include "Perspective/Interactables/PRSInteractableButton.h"
#include "Perspective/Subsystems/PRSModeWorldSubsystem.h"
#include "Sound/SoundCue.h"

APRSGate::APRSGate()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxComp = CreateDefaultSubobject<UBoxComponent>("Box");
	SetRootComponent(BoxComp);
	BoxComp->SetBoxExtent(FVector(PanelLength, PanelLength, PanelLength));

	CubeEffectComp = CreateDefaultSubobject<UStaticMeshComponent>("Cube Effect");
	CubeEffectComp->AttachToComponent(BoxComp, FAttachmentTransformRules::KeepRelativeTransform);
	CubeEffectComp->SetStaticMesh(UPRSStatics::GetCubeStaticMesh());
	CubeEffectComp->SetMaterial(0, UPRSStatics::GetPanelMaterial());
	CubeEffectComp->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
}

void APRSGate::BeginPlay()
{
	Super::BeginPlay();

	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &APRSGate::BoxComponentOnComponentBeginOverlap);
	BoxComp->OnComponentEndOverlap.AddDynamic(this, &APRSGate::BoxComponentOnComponentEndOverlap);

	for (APRSInteractableButton* Button : InteractableButtons)
	{
		if (Button == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("Gate: %s, has a null button set"), *this->GetName());
			continue;
		}
		
		Button->OnButtonPressed.AddDynamic(this, &APRSGate::OnButtonPressed);
	}
}

void APRSGate::BoxComponentOnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bEnabled)
	{
		return;
	}
	
	if (!IsValid(Cast<APRSCharacter>(OtherActor)))
	{
		return;
	}

	EnterVector = SweepResult.ImpactNormal * -1;

	UE_LOG(LogTemp, Warning, TEXT("Entered from direction: %s"), *EnterVector.ToString());
}

void APRSGate::BoxComponentOnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!bEnabled)
	{
		return;
	}
	
	APRSCharacter* PRSCharacter = Cast<APRSCharacter>(OtherActor);
	if (!IsValid(PRSCharacter))
	{
		return;
	}

	// TODO (Bug #12): Currently this logic will only work on non-rotated boxes.
	// BoxComp
	const FVector RelativeLocation = PRSCharacter->GetActorLocation() - BoxComp->GetComponentLocation();
	const FVector NormalizedRelativeLocation = RelativeLocation / BoxComp->GetScaledBoxExtent();

	if (FMath::Abs(NormalizedRelativeLocation.X) > FMath::Abs(NormalizedRelativeLocation.Y) &&
		FMath::Abs(NormalizedRelativeLocation.X) > FMath::Abs(NormalizedRelativeLocation.Z)) // Front or Back
	{
		ExitVector = NormalizedRelativeLocation.X > 0 ? FVector(1.f, 0.f, 0.f) : FVector(-1.f, 0.f, 0.f);
	}
	else if (FMath::Abs(NormalizedRelativeLocation.Y) > FMath::Abs(NormalizedRelativeLocation.X) &&
		FMath::Abs(NormalizedRelativeLocation.Y) > FMath::Abs(NormalizedRelativeLocation.Z)) // Right or Left
	{
		ExitVector = NormalizedRelativeLocation.Y > 0 ? NormalizedRelativeLocation : FVector(0.f, -1.f, 0.f);
	}
	else // Top or Bottom
	{
		ExitVector = NormalizedRelativeLocation.Z > 0 ? FVector(0.f, 0.f, 1.f) : FVector(0.f, 0.f, -1.f);
	}

	UE_LOG(LogTemp, Warning, TEXT("Exited from direction: %s"), *ExitVector.ToString());
	
	// Exit from the same direction entered
	if (EnterVector == ExitVector)
	{
		return;
	}

	if (ExitVector.Z != 0.f && EnterVector.Z == 0.f)
	{
		PRSCharacter->SetForwardVectorOverride(EnterVector * -1);
		GetWorld()->GetSubsystem<UPRSModeWorldSubsystem>()->Switch();
		UGameplayStatics::PlaySound2D(this, PerspectiveModeChangedSoundCue);
	}
	else if (ExitVector.Z == 0.f)
	{
		PRSCharacter->SetForwardVectorOverride(ExitVector);
		GetWorld()->GetSubsystem<UPRSModeWorldSubsystem>()->Switch();
		UGameplayStatics::PlaySound2D(this, PerspectiveModeChangedSoundCue);
	}

	// Cleanup vectors
	EnterVector = FVector::ZeroVector;
	ExitVector = FVector::ZeroVector;
}

void APRSGate::OnButtonPressed()
{
	bEnabled = !bEnabled;
	CubeEffectComp->SetVisibility(bEnabled);
}
