// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSAIGuard.h"
#include "Perception/PawnSensingComponent.h"
#include "DrawDebugHelpers.h"
#include "TimerManager.h"
#include "FPSGameMode.h"
#include "Engine/World.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Engine.h"
#include "UnrealNetwork.h"

// Sets default values
AFPSAIGuard::AFPSAIGuard()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PawnScensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnScencingComponent"));

	GuardState = EAIState::Idle;
	bPatrol = true;
}

// Called when the game starts or when spawned
void AFPSAIGuard::BeginPlay()
{
	Super::BeginPlay();
	PawnScensing->OnSeePawn.AddDynamic(this, &AFPSAIGuard::OnPawnSeen);
	PawnScensing->OnHearNoise.AddDynamic(this, &AFPSAIGuard::OnNoiseHeard);

	OriginalRotation = GetActorRotation();

	if (bPatrol)
	{
		MoveToNextPatrolPoint();
	}
}

void AFPSAIGuard::MoveToNextPatrolPoint()
{
	if (CurrentPatrolPoint == nullptr || CurrentPatrolPoint == SecondPatrolPoint)
	{
		CurrentPatrolPoint = FirstPatrolPoint;
	}
	else if (CurrentPatrolPoint == FirstPatrolPoint)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Change Direction"));
		CurrentPatrolPoint = SecondPatrolPoint;
	}

	UAIBlueprintHelperLibrary::SimpleMoveToActor(GetController(), CurrentPatrolPoint);
}

void AFPSAIGuard::OnRep_GuardState()
{
	OnStateChange(GuardState);
}

void AFPSAIGuard::SetGuardState(EAIState NewState)
{
	if (GuardState == NewState)
		return;
	GuardState = NewState;
	OnRep_GuardState();
}

// Called every frame
void AFPSAIGuard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentPatrolPoint)
	{
		FVector Delta = GetActorLocation() - CurrentPatrolPoint->GetActorLocation();
		float DistanceToGoal = Delta.Size();
		if (DistanceToGoal < 50)
		{
			MoveToNextPatrolPoint();
		}
	}
}

void AFPSAIGuard::OnPawnSeen(APawn* SeenPawn)
{
	if (!SeenPawn)
	{
		return;
	}

	DrawDebugSphere(GetWorld(), SeenPawn->GetActorLocation(), 32.f, 12, FColor::Yellow, false, 10.f);

	AFPSGameMode* GameMode = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());
	if (GameMode)
	{
		GameMode->CompleteMission(SeenPawn, false);
	}

	SetGuardState(EAIState::Alerted);

	AController* Controller = GetController();
	if (Controller)
	{
		Controller->StopMovement();
	}
}

void AFPSAIGuard::OnNoiseHeard(APawn* NoiseInstigator, const FVector& Location, float Volume)
{
	if (GuardState == EAIState::Alerted)
		return;

	if (NoiseInstigator)
	{
		DrawDebugSphere(GetWorld(), Location, 32.f, 12, FColor::Green, false, 10.f);
	}

	FVector Direction = Location - GetActorLocation();

	Direction.Normalize();

	FRotator  NewLookAt = FRotationMatrix::MakeFromX(Direction).Rotator();
	NewLookAt.Pitch = 0.f;
	NewLookAt.Roll = 0.f;

	SetActorRotation(NewLookAt);

	GetWorldTimerManager().ClearTimer(TimerHandle_ResetOrientation);

	GetWorldTimerManager().SetTimer(TimerHandle_ResetOrientation, this, &AFPSAIGuard::ResetOrientation, 3.f, false);

	SetGuardState(EAIState::Suspicious);

	AController* Controller = GetController();
	if (Controller)
	{
		Controller->StopMovement();
	}
}

void AFPSAIGuard::ResetOrientation()
{
	if (GuardState == EAIState::Alerted)
		return;

	SetActorRotation(OriginalRotation);

	SetGuardState(EAIState::Idle);

	if (bPatrol)
	{
		MoveToNextPatrolPoint();
	}
}

void AFPSAIGuard::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFPSAIGuard, GuardState);
}