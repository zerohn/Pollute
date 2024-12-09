// Fill out your copyright notice in the Description page of Project Settings.


#include "P_Settings/P_PlayerState.h"

#include "Kismet/GameplayStatics.h"
#include "KYH/KYH_CommonUserLobby.h"
#include "P_Settings/P_LobbyGameState.h"

AP_PlayerState::AP_PlayerState(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	
}

void AP_PlayerState::BeginPlay()
{
	Super::BeginPlay();
	
}

void AP_PlayerState::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
}

void AP_PlayerState::ServerRPC_SendChat_Implementation(const FString& Message)
{
    FString Chat = GetPlayerName() + " : " + Message;
    MulticastRPC_Chat(Chat);
}

void AP_PlayerState::MulticastRPC_Chat_Implementation(const FString& Message)
{
    AP_LobbyGameState* LobbyGameState = GetWorld()->GetGameState<AP_LobbyGameState>();
    LobbyGameState->LobbyWidget->AddChat(Message);
}
