// Fill out your copyright notice in the Description page of Project Settings.


#include "P_Settings/P_LobbyGameState.h"

#include "Blueprint/UserWidget.h"
#include "Engine/Engine.h"
#include "KYH/KYH_CommonUserLobby.h"


void AP_LobbyGameState::BeginPlay()
{
    Super::BeginPlay();

    LobbyWidget = CreateWidget<UKYH_CommonUserLobby>(GetWorld(), LobbyWidgetClass);
    LobbyWidget->AddToViewport();
    LobbyWidget->ServerRPC_SetPlayerSlotUI(this);

    P_SCREEN(3, FColor::Orange, TEXT("CreateLobbyWidget : %s"), *GetNameSafe(LobbyWidget));

    
    GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeUIOnly());
    GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
}
