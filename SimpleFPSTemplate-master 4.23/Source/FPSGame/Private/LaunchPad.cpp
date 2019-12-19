// Fill out your copyright notice in the Description page of Project Settings.

#include "LaunchPad.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

// Sets default values
ALaunchPad::ALaunchPad()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TriggerZone = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerZone"));
	SetRootComponent(TriggerZone);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BottomMesh"));
	Mesh->SetupAttachment(TriggerZone);

	ArrowMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArrowMesh"));
	ArrowMesh->SetupAttachment(TriggerZone);

	LanchStrength = 1500;
	LanchPitchAngle = 35.f;
}

// Called when the game starts or when spawned
void ALaunchPad::BeginPlay()
{
	Super::BeginPlay();

	TriggerZone->OnComponentBeginOverlap.AddDynamic(this, &ALaunchPad::OverlapLaunchPad);
}

// Called every frame
void ALaunchPad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALaunchPad::OverlapLaunchPad(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	FRotator LaunchDirection = GetActorRotation();
	LaunchDirection.Pitch += LanchPitchAngle;
	FVector LaunchVelocity = LaunchDirection.Vector() * LanchStrength;

	ACharacter* OtherCharacter = Cast<ACharacter>(OtherActor);

	if (OtherCharacter)
	{
		OtherCharacter->LaunchCharacter(LaunchVelocity, true, true);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ActiveLanchPadEffect, GetActorLocation());
	}
	else if (OtherComp, OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulse(LaunchVelocity, NAME_None, true);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ActiveLanchPadEffect, GetActorLocation());
	}
}