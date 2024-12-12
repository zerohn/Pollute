// Fill out your copyright notice in the Description page of Project Settings.


#include "KYH/KYH_LobbyController.h"

#include "TimerManager.h"
#include "Blueprint/UserWidget.h"
#include "Engine/World.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"
#include "KYH/KYH_CommonUserLobby.h"
#include "P_Settings/P_GameInstance.h"
#include "P_Settings/P_PlayerState.h"
#include "P_Settings/PlayData.h"


void AKYH_LobbyController::BeginPlay()
{
    Super::BeginPlay();
    
    CurrentPlayerType = EPlayerType::Eric;
    
    SetInputMode(FInputModeUIOnly());
    SetShowMouseCursor(true);
    
    LobbyWidget = CreateWidget<UKYH_CommonUserLobby>(GetWorld(), LobbyWidgetClass);
    LobbyWidget->AddToViewport();
    
    FTimerHandle DelayHandle;
    GetWorldTimerManager().SetTimer(DelayHandle, this, &AKYH_LobbyController::InitLobbyWidget, 0.1f, false);
}

void AKYH_LobbyController::InitLobbyWidget_Implementation()
{
    TArray<APlayerState*> PStateArray = GetWorld()->GetGameState()->PlayerArray;
    TArray<TPair<int32, int32>> PlayerTypeArray;
    for (APlayerState* PState : PStateArray)
    {
        PlayerTypeArray.Add(TPair<int32, int8>(PState->GetPlayerId(), (int32)Cast<AKYH_LobbyController>(PState->GetPlayerController())->CurrentPlayerType));
    }
    for (APlayerState* PState : PStateArray)
    {
        Cast<AKYH_LobbyController>(PState->GetPlayerController())->UpdatePlayerSlot(PlayerTypeArray);
    }
}

void AKYH_LobbyController::UpdatePlayerSlot_Implementation(TArray<TPair<int32, int32>> PlayerTypeMap)
{
    LobbyWidget->InitLobbyUI(FText::FromName(GetGameInstance<UP_GameInstance>()->GetCurrentSessionName()));
    
    for (TPair<int32, int8> PlayerType : PlayerTypeMap)
    {
        for (APlayerState* PState : GetWorld()->GetGameState()->PlayerArray)
        {
            if (PState->GetPlayerId() == PlayerType.Key)
            {
                LobbyWidget->AddPlayerSlotUI(FName(PState->GetPlayerName()), (EPlayerType)PlayerType.Value);
                break;
            }
        }
    }
}

void AKYH_LobbyController::ServerRPC_SendChat_Implementation(const FString& Message)
{
    FString Chat = GetPlayerState<AP_PlayerState>()->GetPlayerName() + Message;
    NetMulticastRPC_UpdateChat(Chat);
}

void AKYH_LobbyController::NetMulticastRPC_UpdateChat_Implementation(const FString& Message)
{
    LobbyWidget->AddChat(Message);
}
