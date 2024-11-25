// Fill out your copyright notice in the Description page of Project Settings.


#include "P_Settings/P_GameMode.h"

#include "TimerManager.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "LCU/InteractActors/LCU_Curse.h"
#include "LCU/Player/LCU_PlayerCharacter.h"

AP_GameMode::AP_GameMode()
{
}

void AP_GameMode::GetAllCharacters()
{
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ALCU_PlayerCharacter::StaticClass(), Actors);

	for(auto& actor : Actors)
	{
		if(ALCU_PlayerCharacter* PlayerCharacter = Cast<ALCU_PlayerCharacter>(actor))
		{
			AllPlayers.AddUnique(PlayerCharacter);
		}
	}
}

void AP_GameMode::SelectPlayer()
{
	// 플레이어 배열이 비어 있는지 확인
	if (AllPlayers.IsEmpty())
	{
		return;
	}
	
	// 랜덤 인덱스 선택
	int32 RandomIndex = UKismetMathLibrary::RandomIntegerInRange(0, AllPlayers.Num() - 1);

	// 선택된 플레이어
	AActor* SelectedPlayer = AllPlayers[RandomIndex];
	
	if (SelectedPlayer)
	{
		Curse->GetInstance(GetWorld())->StartCurseTimer(SelectedPlayer);
	}
	GetWorld()->GetTimerManager().ClearTimer(CurseSelectedTimer);
}

void AP_GameMode::BeginPlay()
{
	Super::BeginPlay();
	
	Curse = ALCU_Curse::GetInstance(GetWorld()); 

	GetAllCharacters();
	
	GetWorld()->GetTimerManager().SetTimer(CurseSelectedTimer, this, &ThisClass::SelectPlayer, 5.f,false);
}

void AP_GameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

TObjectPtr<ALCU_PlayerCharacter> AP_GameMode::FindPlayer(ALCU_PlayerCharacter* player)
{
	if(AllPlayers.IsEmpty()) return nullptr;

	for(auto& Player : AllPlayers)
	{
		if(Player == player) return Player;
	}
	return nullptr;
}
