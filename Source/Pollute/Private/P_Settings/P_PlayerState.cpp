// Fill out your copyright notice in the Description page of Project Settings.


#include "P_Settings/P_PlayerState.h"

#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "KYH/KYH_CommonUserLobby.h"
#include "P_Settings/P_GameInstance.h"
#include "P_Settings/P_LobbyGameState.h"

AP_PlayerState::AP_PlayerState(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	
}

void AP_PlayerState::BeginPlay()
{
	Super::BeginPlay();
    
}

//
// void AP_PlayerState::ServerRPC_SendChat_Implementation(const FString& Message)
// {
//     FString Chat = GetPlayerName() + " : " + Message;
//     MulticastRPC_Chat(Chat);
// }
//
// void AP_PlayerState::MulticastRPC_Chat_Implementation(const FString& Message)
// {
//     LobbyWidget->AddChat(Message);
// }
//
// void AP_PlayerState::ServerRPC_UpdateLobbyUI()
// {
//     if (!HasAuthority()) return;
//     
//     TArray<FName> PlayerNames;
//     for (APlayerState* PS : GetWorld()->GetGameState()->PlayerArray)
//     {
//         PlayerNames.Add(FName(PS->GetPlayerName()));
//     }
//     
//     for (APlayerState* PS : GetWorld()->GetGameState()->PlayerArray)
//     {   
//         Cast<AP_PlayerState>(PS)->MakeUI(PlayerNames);        
//     }
// }
//
// void AP_PlayerState::MakeUI_Implementation(const TArray<FName>& PlayerNames)
// {
//     P_LOG(PolluteLog, Warning, TEXT("11111111"))
//
//     if (LobbyWidget ==nullptr)
//     {
//         LobbyWidget = CreateWidget<UKYH_CommonUserLobby>(GetWorld(), LobbyWidgetClass);
//         LobbyWidget->AddToViewport();
//     }
//     LobbyWidget->InitLobbyUI(FText::FromName(GetGameInstance<UP_GameInstance>()->GetCurrentSessionName()), PlayerNames);
// }
