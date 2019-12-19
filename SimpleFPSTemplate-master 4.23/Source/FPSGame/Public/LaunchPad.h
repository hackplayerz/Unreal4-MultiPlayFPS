// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LaunchPad.generated.h"

UCLASS()
class FPSGAME_API ALaunchPad : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALaunchPad();
	UPROPERTY(EditDefaultsOnly, Category = "Mesh")
		class UStaticMeshComponent* Mesh;
	UPROPERTY(EditDefaultsOnly, Category = "Mesh")
		class UStaticMeshComponent* ArrowMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Mesh")
		class UBoxComponent* TriggerZone;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
		void OverlapLaunchPad(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditInstanceOnly, Category = "LanchPad")
		float LanchStrength;
	UPROPERTY(EditInstanceOnly, Category = "LanchPad")
		float LanchPitchAngle;

	UPROPERTY(EditDefaultsOnly, Category = "LanchPad")
		UParticleSystem* ActiveLanchPadEffect;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
