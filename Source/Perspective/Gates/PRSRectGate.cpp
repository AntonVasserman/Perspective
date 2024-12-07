// Copyright Anton Vasserman, All Rights Reserved.

#include "PRSRectGate.h"

#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Perspective/Characters/PRSCharacter.h"
#include "Perspective/Subsystems/PRSModeWorldSubsystem.h"
#include "Sound/SoundCue.h"

APRSRectGate::APRSRectGate()
{
	CenterRectComp = CreateDefaultSubobject<UBoxComponent>("Center Rectangle");
	CenterRectComp->SetBoxExtent(FVector(PanelLength, PanelLength, PanelLength));
	SetRootComponent(CenterRectComp);
	
	Panels.Add(CreateDefaultSubobject<UPRSPanel>("Front Panel"));
	Panels[0]->SetRelativeLocation(FVector(PanelLength + 1.f, 0.f, 0.f));
	Panels[0]->SetRelativeRotation(FRotator::ZeroRotator);
	
	Panels.Add(CreateDefaultSubobject<UPRSPanel>("Back Panel"));
	Panels[1]->SetRelativeLocation(FVector(-PanelLength - 1.f, 0.f, 0.f));
	Panels[1]->SetRelativeRotation(FRotator(0.f, 180.f, 0.f));
	
	Panels.Add(CreateDefaultSubobject<UPRSPanel>("Right Panel"));
	Panels[2]->SetRelativeLocation(FVector(0.f, PanelLength + 1.f, 0.f));
	Panels[2]->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));
	
	Panels.Add(CreateDefaultSubobject<UPRSPanel>("Left Panel"));
	Panels[3]->SetRelativeLocation(FVector(0.f, -PanelLength - 1.f, 0.f));
	Panels[3]->SetRelativeRotation(FRotator(0.f, 270.f, 0.f));

	for (int i = 0; i < Panels.Num(); i++)
	{
		Panels[i]->SetRelativeScale3D(FVector(1.f / PanelLength, 1.f, 1.f));
		Panels[i]->AttachToComponent(CenterRectComp, FAttachmentTransformRules::KeepRelativeTransform);
	}
}

void APRSRectGate::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	const float XScale = Transform.GetScale3D().X;
	const float YScale = Transform.GetScale3D().Y;

	if (IsValid(Panels[2])) // Right Panel
	{
		Panels[2]->SetRelativeScale3D(FVector(1.f / PanelLength, XScale / YScale, 1.f));
	}
	if (IsValid(Panels[3])) // Left Panel
	{
		Panels[3]->SetRelativeScale3D(FVector(1.f / PanelLength, XScale / YScale, 1.f));
	}
}

void APRSRectGate::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	Panels[0]->SetEnabled(bFrontPanelEnabled);
	Panels[1]->SetEnabled(bBackPanelEnabled);
	Panels[2]->SetEnabled(bRightPanelEnabled);
	Panels[3]->SetEnabled(bLeftPanelEnabled);
}

void APRSRectGate::Operate()
{
	bEnabled = !bEnabled;
	Panels[0]->SetEnabled(bEnabled && bFrontPanelEnabled);
	Panels[1]->SetEnabled(bEnabled && bBackPanelEnabled);
	Panels[2]->SetEnabled(bEnabled && bRightPanelEnabled);
	Panels[3]->SetEnabled(bEnabled && bLeftPanelEnabled);
}

void APRSRectGate::BeginPlay()
{
	Super::BeginPlay();

	CenterRectComp->OnComponentBeginOverlap.AddDynamic(this, &APRSRectGate::CenterRectCompOnComponentBeginOverlap);
	CenterRectComp->OnComponentEndOverlap.AddDynamic(this, &APRSRectGate::CenterRectCompOnComponentEndOverlap);

	for (int i = 0; i < Panels.Num(); i++)
	{
		Panels[i]->OnPlayerEndOverlap.AddDynamic(this, &APRSRectGate::APRSRectGate::PanelOnPlayerEndOverlap);
	}
}

void APRSRectGate::CenterRectCompOnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsValid(Cast<APRSCharacter>(OtherActor)))
	{
		return;
	}
	
	bIsTouchingInsideRectangle = true;
}

void APRSRectGate::CenterRectCompOnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!IsValid(Cast<APRSCharacter>(OtherActor)))
	{
		return;
	}

	bIsTouchingInsideRectangle = false;
}

// TODO (PRS-59): Refactor this to maybe use State Enum instead of many bools
void APRSRectGate::PanelOnPlayerEndOverlap(UPRSPanel* OverlappedPanel, APRSCharacter* PlayerCharacter)
{
	// Entered the Rectangle
	if (bIsTouchingInsideRectangle)
	{
		// Entered (allegedly) from the outside.
		// There is an edge case here where we might be able to enter the rectangle without touching any side,
		// this should be handled by level design for now.
		// Else we entered from inside, meaning we were already inside, touched a panel, and went back, do nothing in that case
		if (EnterPanel == nullptr)
		{
			EnterPanel = OverlappedPanel;
			EnterPanel->SetPending();

			if (bCloseOnPassthrough && ExitPanel != nullptr)
			{
				ExitPanel->SetOpen(); 
			}
		}

		return;
	}

	// Exit the Rectangle
	if (EnterPanel == nullptr) // Edge case where we never actually entered the rectangle, but touched and left back the same panel
	{
		return;
	}

	if (EnterPanel == OverlappedPanel) // Exit the Rectangle from the same Panel
	{
		EnterPanel->SetOpen();
		EnterPanel = nullptr;

		if (bCloseOnPassthrough && ExitPanel != nullptr)
		{
			ExitPanel->SetClosed(); 
		}
	}
	else // Exit the Rectangle from a different Panel
	{
		EnterPanel->SetOpen();
		EnterPanel = nullptr;
		if (bCloseOnPassthrough)
		{
			// Here we first open the last Exit panel and then close the newly exit panel
			if (ExitPanel != nullptr) // Exit panel might be null if this is the first time we enter the rectangle
			{
				ExitPanel->SetOpen();
			}
			ExitPanel = OverlappedPanel;
			ExitPanel->SetClosed();
		}

		PlayerCharacter->SetForwardVectorOverride(OverlappedPanel->GetForwardVector());
		GetWorld()->GetSubsystem<UPRSModeWorldSubsystem>()->Switch();
		UGameplayStatics::PlaySound2D(this, PerspectiveModeChangedSoundCue);
	}
}
