// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSGameState.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "FPSPlayerController.h"

void AFPSGameState::MultiCastOnMissionComplete_Implementation(APawn* InstigatorPawn, bool bIsMissionSuccess)
{
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		AFPSPlayerController* PlayerController = Cast<AFPSPlayerController>(Iterator->Get());
		if (PlayerController && PlayerController->IsLocalController())
		{
			PlayerController->OnMissionComplete(InstigatorPawn, bIsMissionSuccess);

			APawn* Pawn = PlayerController->GetPawn(); ///< DisableInput.
			if (Pawn && Pawn->IsLocallyControlled())
			{
				Pawn->DisableInput(PlayerController);
			}
		}
	}
}