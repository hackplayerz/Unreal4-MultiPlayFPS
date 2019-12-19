// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSBlackHole.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
AFPSBlackHole::AFPSBlackHole()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	InnerSphere = CreateDefaultSubobject<USphereComponent>(TEXT("InnerSphere"));
	InnerSphere->SetupAttachment(Mesh);
	InnerSphere->SetSphereRadius(100);

	OuterSphere = CreateDefaultSubobject<USphereComponent>(TEXT("OuterSphere"));
	OuterSphere->SetupAttachment(Mesh);
	OuterSphere->SetSphereRadius(3000);
}

// Called when the game starts or when spawned
void AFPSBlackHole::BeginPlay()
{
	Super::BeginPlay();

	InnerSphere->OnComponentBeginOverlap.AddDynamic(this, &AFPSBlackHole::OverlapInnerSphere);
}

// Called every frame
void AFPSBlackHole::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TArray<UPrimitiveComponent*> OverlappingComponents;
	OuterSphere->GetOverlappingComponents(OverlappingComponents);

	for (int32 i = 0; i < OverlappingComponents.Num(); i++)
	{
		UPrimitiveComponent* Primitive = OverlappingComponents[i];
		if (Primitive && Primitive->IsSimulatingPhysics())
		{
			const float SphereRadious = OuterSphere->GetScaledSphereRadius();
			const float ForceStrength = -2000;

			Primitive->AddRadialForce(GetActorLocation(), SphereRadious, ForceStrength, ERadialImpulseFalloff::RIF_Constant, true);
		}
	}
}

void AFPSBlackHole::OverlapInnerSphere(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		OtherActor->Destroy();
	}
}