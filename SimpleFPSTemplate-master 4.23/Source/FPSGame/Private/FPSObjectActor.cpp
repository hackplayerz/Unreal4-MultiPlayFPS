// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSObjectActor.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "FPSCharacter.h"

// Sets default values
AFPSObjectActor::AFPSObjectActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(Mesh);
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Sphere->SetCollisionResponseToChannel(ECC_MAX, ECR_Overlap);
}

// Called when the game starts or when spawned
void AFPSObjectActor::BeginPlay()
{
	Super::BeginPlay();

	PlayEffect();
}

void AFPSObjectActor::PlayEffect()
{
	UGameplayStatics::SpawnEmitterAtLocation(this, PickupFX, GetActorLocation());
}

// Called every frame
void AFPSObjectActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFPSObjectActor::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	PlayEffect();

	/** 현재 액터가 서버일경우 */
	if (Role == ROLE_Authority)
	{
		UE_LOG(LogTemp, Warning, TEXT("Server Catch"));
		AFPSCharacter* Player = Cast<AFPSCharacter>(OtherActor);
		if (Player)
		{
			Player->bIsCarryingObject = true;

			Destroy();
		}
	}
	Destroy();
}