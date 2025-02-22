// Fill out your copyright notice in the Description page of Project Settings.


#include "P_Settings/P_GameState.h"

#include "TimerManager.h"
#include "Engine/World.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "LCU/InteractActors/LCU_Curse.h"
#include "LCU/Player/LCU_PlayerCharacter.h"
#include "Net/UnrealNetwork.h"
#include "P_Settings/P_GameInstance.h"

AP_GameState::AP_GameState()
{
}

void AP_GameState::SelectCursePlayer()
{
	GetWorld()->GetTimerManager().SetTimer(CurseSelectedTimer, this, &ThisClass::SelectPlayer, 5.f,false);
}

void AP_GameState::BeginPlay()
{
	Super::BeginPlay();
    
	Curse = ALCU_Curse::GetInstance(GetWorld()); 
	
	SelectCursePlayer();
    
    GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameOnly());
    GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(false);

    FTimerHandle SetMeshHandle;
    GetWorldTimerManager().SetTimer(SetMeshHandle, this, &AP_GameState::InitPlayerMesh, 10.f, false);
}

void AP_GameState::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AP_GameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AP_GameState, HumanPlayers);
}

TObjectPtr<ALCU_PlayerCharacter> AP_GameState::FindPlayer(ALCU_PlayerCharacter* player)
{
	if(HumanPlayers.IsEmpty()) return nullptr;

	for(auto& Player : HumanPlayers)
	{
		if(Player == player) return Player;
	}
	return nullptr;
}

void AP_GameState::GetAllCharacters()
{
	if(!HumanPlayers.IsEmpty())
	{
		HumanPlayers.Empty();
	}
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ALCU_PlayerCharacter::StaticClass(), Actors);

	for(auto& actor : Actors)
	{
		if(ALCU_PlayerCharacter* PlayerCharacter = Cast<ALCU_PlayerCharacter>(actor))
		{
			HumanPlayers.AddUnique(PlayerCharacter);
		}
	}

    if(HumanPlayers.IsEmpty() && HasAuthority())
    {
        // 게임 끝
        UP_GameInstance* GI = Cast<UP_GameInstance>(GetGameInstance());
        FString MapName = GI->GetLobbyLevelURL(); // 이동하려는 맵 이름
        if (!MapName.IsEmpty())
        {
            FString URL = MapName + "?Listen";
            GetWorld()->ServerTravel(URL, true);
        }
    }
}

void AP_GameState::RemoveHumanPlayer(ALCU_PlayerCharacter* humanPlayer)
{
	if(!HasAuthority()) return;
	if(HumanPlayers.IsEmpty()) return;
	if(!HumanPlayers.Contains(humanPlayer)) return;

	HumanPlayers.Remove(humanPlayer);
}

void AP_GameState::SelectPlayer()
{
	if(!HasAuthority()) return;
	
	GetAllCharacters();
	
	// 플레이어 배열이 비어 있는지 확인
	if (HumanPlayers.IsEmpty()) return;
	
	// Curse 유효성 검사
	if (!Curse || !Curse->IsValidLowLevel()) return;

	// 랜덤 인덱스 선택
	int32 RandomIndex = UKismetMathLibrary::RandomIntegerInRange(0, HumanPlayers.Num() - 1);
	// 선택된 플레이어
	ALCU_PlayerCharacter* SelectedPlayer = HumanPlayers[RandomIndex];

	GetWorld()->GetTimerManager().ClearTimer(CurseSelectedTimer);

	P_LOG(PolluteLog, Log, TEXT("Curse have %s"), *SelectedPlayer->GetName());

	// 선택된 플레이어로부터 저주를 시작합니다.
	StartCurse(SelectedPlayer);
}

// 저주를 다시 시작 ( 저주를 가진 사람이 죽거나 탈출 했을 때 호출 )
void AP_GameState::RestartCurse()
{
	ALCU_Curse::GetInstance(GetWorld())->InitCurseTime();
	SelectPlayer();
}

void AP_GameState::InitPlayerMesh()
{
    if (!HasAuthority()) return;
    
    for (int i = 0; i < PlayerArray.Num(); i++)
    {
        ALCU_PlayerCharacter* PlayerCharacter = Cast<ALCU_PlayerCharacter>(PlayerArray[i]->GetPawn());
        if (!PlayerCharacter || GetGameInstance<UP_GameInstance>()->PlayerTypes.Num() <= i) continue;
        PlayerCharacter->ServerRPC_SetPlayerType(GetGameInstance<UP_GameInstance>()->PlayerTypes[i]);
    }
}

//void AP_GameState::StartCurse_Implementation(ALCU_PlayerCharacter* selectedPlayer)
//{
//	ALCU_Curse::GetInstance(GetWorld())->StartCurseTimer(selectedPlayer);
//}
void AP_GameState::StartCurse(ALCU_PlayerCharacter* selectedPlayer)
{
	ALCU_Curse::GetInstance(GetWorld())->StartCurseTimer(selectedPlayer);
}