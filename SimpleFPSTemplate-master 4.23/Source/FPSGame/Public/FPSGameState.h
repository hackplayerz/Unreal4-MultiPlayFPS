// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "FPSGameState.generated.h"

/**
 *
 */
UCLASS()
class FPSGAME_API AFPSGameState : public AGameStateBase
{
	GENERATED_BODY()
public:

	/**
	 * NetMulticast : 서버에서 콜, 클라이언트로 결과전송
	 * Reliable : 서버가 혼잡해도 반드시 실행, unreliable은 서버가 복잡하면 패스.
	 */
	UFUNCTION(NetMulticast, Reliable)
		void MultiCastOnMissionComplete(APawn* InstigatorPawn, bool bIsMissionSuccess);
};
