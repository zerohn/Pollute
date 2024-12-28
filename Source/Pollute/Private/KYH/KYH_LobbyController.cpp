// Fill out your copyright notice in the Description page of Project Settings.


#include "KYH/KYH_LobbyController.h"

#include "TimerManager.h"
#include "Blueprint/UserWidget.h"
#include "Engine/World.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"
#include "KYH/KYH_CommonUserLobby.h"
#include "KYH/KYH_PolluteButtonBase.h"
#include "P_Settings/P_GameInstance.h"
#include "P_Settings/P_PlayerState.h"
#include "P_Settings/PlayData.h"


void AKYH_LobbyController::BeginPlay()
{
    Super::BeginPlay();
    
    CurrentPlayerType = EPlayerType::Eric;
    
    if (IsLocalController())
    {
        SetInputMode(FInputModeUIOnly());
        SetShowMouseCursor(true);
        LobbyWidget = CreateWidget<UKYH_CommonUserLobby>(GetWorld(), LobbyWidgetClass);
        LobbyWidget->AddToViewport();
        if (HasAuthority()) LobbyWidget->Btn_Start->SetVisibility(ESlateVisibility::Visible);
        FTimerHandle DelayHandle;
        GetWorldTimerManager().SetTimer(DelayHandle, this, &AKYH_LobbyController::InitLobbyWidget, 0.2f, false);
    }
}

void AKYH_LobbyController::ServerRPC_SetPlayerType_Implementation(EPlayerType InPlayerType)
{
    CurrentPlayerType = InPlayerType;
    InitLobbyWidget();
}

void AKYH_LobbyController::InitLobbyWidget_Implementation()
{
    TArray<APlayerState*> PStateArray = GetWorld()->GetGameState()->PlayerArray;
    TArray<FPlayerLobbyInfo> PlayerInfoArray;
    for (APlayerState* PState : PStateArray)
    {
        PlayerInfoArray.Add(FPlayerLobbyInfo(PState->GetPlayerId(), Cast<AKYH_LobbyController>(PState->GetPlayerController())->CurrentPlayerType));
    }
    for (APlayerState* PState : PStateArray)
    {
        Cast<AKYH_LobbyController>(PState->GetPlayerController())->UpdatePlayerSlot(PlayerInfoArray);
    }
}

void AKYH_LobbyController::UpdatePlayerSlot_Implementation(const TArray<FPlayerLobbyInfo>& PlayerInfoArray)
{
    LobbyWidget->InitLobbyUI(FText::FromName(GetGameInstance<UP_GameInstance>()->GetCurrentSessionName()));
    
    for (FPlayerLobbyInfo PlayerInfo : PlayerInfoArray)
    {
        for (APlayerState* PState : GetWorld()->GetGameState()->PlayerArray)
        {
            if (PState->GetPlayerId() == PlayerInfo.PlayerId)
            {
                LobbyWidget->AddPlayerSlotUI(FName(PState->GetPlayerName()), PlayerInfo.PlayerType);
                break;
            }
        }
    }
}

void AKYH_LobbyController::ServerRPC_SendChat_Implementation(const FString& Message)
{
    FString Chat = GetPlayerState<AP_PlayerState>()->GetPlayerName() + TEXT(" : ") + Message;
    for (APlayerState* PS : GetWorld()->GetGameState()->PlayerArray)
    {
        Cast<AKYH_LobbyController>(PS->GetPlayerController())->ClientRPC_UpdateChat(Chat);
    }
}

void AKYH_LobbyController::ClientRPC_UpdateChat_Implementation(const FString& Message)
{
    LobbyWidget->AddChat(Message);
}
