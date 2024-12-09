// Fill out your copyright notice in the Description page of Project Settings.


#include "LCU/Player/LCU_PlayerController.h"

#include "Engine/World.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/SpectatorPawn.h"
#include "HHR/HHR_ItemManager.h"
#include "HHR/UI/HHR_TestPlayerHUD.h"
#include "Kismet/GameplayStatics.h"
#include "LCU/Player/LCU_MonsterCharacter.h"
#include "LCU/Player/LCU_PlayerCharacter.h"
#include "P_Settings/P_GameState.h"

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
    ClientRPC_ItemUIOff();
	UnPossess();

	// Spector Pawn 생성
	AGameModeBase* gm = GetWorld()->GetAuthGameMode();
	ASpectatorPawn* spector = GetWorld()->SpawnActor<ASpectatorPawn>(gm->SpectatorClass,player->GetActorTransform());

	// 생성된 Spector 로 Possess
	Possess(spector);

	player->Destroy();

     AP_GameState* GS =Cast<AP_GameState>(UGameplayStatics::GetGameState(GetWorld()));
    if (GS)
    {
        GS->GetAllCharacters();
    }
}

void ALCU_PlayerController::ChangeToMonster()
{
    if(!IsValid(MonChar)) return;
    // 현재 Possess 하고 있는 폰 가져오기
    APawn* player = GetPawn();
    ALCU_PlayerCharacter* PlayerCharacter = Cast<ALCU_PlayerCharacter>(player);
    if(PlayerCharacter)
    {
        PlayerCharacter->DropDown();
    }
    ClientRPC_ItemUIOff();
    UnPossess();

    // Spector Pawn 생성
    AGameModeBase* gm = GetWorld()->GetAuthGameMode();
    ALCU_MonsterCharacter* MonSter = GetWorld()->SpawnActor<ALCU_MonsterCharacter>(MonChar,player->GetActorTransform());

    // 생성된 Spector 로 Possess
    Possess(MonSter);

    player->Destroy();

    AP_GameState* GS =Cast<AP_GameState>(UGameplayStatics::GetGameState(GetWorld()));
    if (GS)
    {
        GS->GetAllCharacters();
    }
}

void ALCU_PlayerController::ClientRPC_ItemUIOff_Implementation()
{
    AActor* IM = UGameplayStatics::GetActorOfClass(GetWorld(), AHHR_ItemManager::StaticClass());
    if(IM)
    {
        AHHR_ItemManager* ItemManager = Cast<AHHR_ItemManager>(IM);
        if(ItemManager)
        {
            ItemManager->TestPlayerHUDIns->SetVisibility(ESlateVisibility::Hidden);
            //ItemManager->TestPlayerHUDIns->SetItemDialogVisibility(false);
            //ItemManager->TestPlayerHUDIns->SetItemDialogText(FText());
        }
    }
}
