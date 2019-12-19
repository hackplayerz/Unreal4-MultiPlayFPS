// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FPSObjectActor.generated.h"

UCLASS()
class FPSGAME_API AFPSObjectActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFPSObjectActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
		class UStaticMeshComponent* Mesh;
	UPROPERTY(VisibleAnywhere, Category = "Components")
		class USphereComponent* Sphere;
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
		UParticleSystem* PickupFX;

	void PlayEffect();
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
};
