// Fill out your copyright notice in the Description page of Project Settings.


#include "LCU/Player/LCU_PlayerController.h"

#include "Engine/World.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/SpectatorPawn.h"

ALCU_PlayerController::ALCU_PlayerController()
{
}

void ALCU_PlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void ALCU_PlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void ALCU_PlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALCU_PlayerController::ServerRPC_ChangeToSpector_Implementation()
{
	// 현재 Possess 하고 있는 폰 가져오기
	APawn* player = GetPawn();

	UnPossess();

	// Spector Pawn 생성
	AGameModeBase* gm = GetWorld()->GetAuthGameMode();
	ASpectatorPawn* spector = GetWorld()->SpawnActor<ASpectatorPawn>(gm->SpectatorClass,player->GetActorTransform());

	// 생성된 Spector 로 Possess
	Possess(spector);

	player->Destroy();

}

void ALCU_PlayerController::ServerRPC_ChangeToMonster_Implementation()
{
	// TODO
	// 몬스터로 변하기

	
}

