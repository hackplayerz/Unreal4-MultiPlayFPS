// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSExtractionZone.h"
#include "Components/BoxComponent.h"
#include "FPSCharacter.h"
#include "Components/DecalComponent.h"
#include  "FPSGameMode.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AFPSExtractionZone::AFPSExtractionZone()
{
	Overlap = CreateDefaultSubobject<UBoxComponent>(TEXT("Overlap"));
	Overlap->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Overlap->SetCollisionResponseToAllChannels(ECR_Overlap);
	Overlap->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	Overlap->SetBoxExtent(FVector(20.f));
	RootComponent = Overlap;

	Overlap->SetHiddenInGame(false);

	Overlap->OnComponentBeginOverlap.AddDynamic(this, &AFPSExtractionZone::HandleOverlap);

	DecalComponent = CreateDefaultSubobject<UDecalComponent>(TEXT("Decal"));
	DecalComponent->DecalSize = FVector(200, 200, 200);
	DecalComponent->SetupAttachment(Overlap);
}

// Called when the game starts or when spawned
void AFPSExtractionZone::BeginPlay()
{
	Super::BeginPlay();
}

void AFPSExtractionZone::HandleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AFPSCharacter* Main = Cast<AFPSCharacter>(OtherActor);
	if (!Main)
	{
		return;
	}

	if (Main->bIsCarryingObject)
	{
		AFPSGameMode* GameMode = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());
		if (GameMode)
		{
			GameMode->CompleteMission(Main, true);
		}
	}

	else
	{
		UGameplayStatics::PlaySound2D(this, ObjectiveMissingSound);
	}
}