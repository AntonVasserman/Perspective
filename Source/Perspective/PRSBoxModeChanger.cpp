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

	BoxComp = CreateDefaultSubobject<UBoxComponent>("Box Component");
	SetRootComponent(BoxComp);
}

void APRSBoxModeChanger::BeginPlay()
{
	Super::BeginPlay();

	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &APRSBoxModeChanger::BoxComponentOnComponentBeginOverlap);
	BoxComp->OnComponentEndOverlap.AddDynamic(this, &APRSBoxModeChanger::BoxComponentOnComponentEndOverlap);
}

void APRSBoxModeChanger::BoxComponentOnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsValid(Cast<APRSCharacter>(OtherActor)))
	{
		return;
	}

	EnterVector = SweepResult.ImpactNormal * -1;

	UE_LOG(LogTemp, Warning, TEXT("Entered from direction: %s"), *EnterVector.ToString());
}

void APRSBoxModeChanger::BoxComponentOnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	APRSCharacter* PRSCharacter = Cast<APRSCharacter>(OtherActor);
	if (!IsValid(PRSCharacter))
	{
		return;
	}

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
		ExitVector = NormalizedRelativeLocation.Y > 0 ? FVector(0.f, 1.f, 0.f) : FVector(0.f, -1.f, 0.f);
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
