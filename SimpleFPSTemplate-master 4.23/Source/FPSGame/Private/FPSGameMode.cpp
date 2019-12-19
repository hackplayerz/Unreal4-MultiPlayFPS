// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "FPSGameMode.h"
#include "FPSHUD.h"
#include "FPSCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "FPSGameState.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"

AFPSGameMode::AFPSGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Blueprints/BP_Player"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AFPSHUD::StaticClass();

	GameStateClass = AFPSGameState::StaticClass();
}

void AFPSGameMode::CompleteMission(APawn* InstigatorPawn, bool bMissionSuccess)
{
	if (InstigatorPawn)
	{
		if (ViewPointClass)
		{
			TArray<AActor*>ReturnActors;
			UGameplayStatics::GetAllActorsOfClass(this, ViewPointClass, ReturnActors);

			if (ReturnActors.Num() > 0)
			{
				AActor* NewViewTarget = ReturnActors[0];

				for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
				{
					APlayerController* PlayerController = Iterator->Get();
					if (PlayerController)
					{
						PlayerController->SetViewTargetWithBlend(NewViewTarget, .5f, EViewTargetBlendFunction::VTBlend_Cubic);
					}
				}
			}
		}

		else
		{
			UE_LOG(LogTemp, Warning, TEXT("NO View point actor."));
		}
	}

	AFPSGameState* GameState = GetGameState<AFPSGameState>();
	if (GameState)
	{
		GameState->MultiCastOnMissionComplete(InstigatorPawn, bMissionSuccess);
	}

	OnMissionCompleted(InstigatorPawn, bMissionSuccess);
}