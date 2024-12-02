// Fill out your copyright notice in the Description page of Project Settings.


#include "KYH/KYH_LobbyController.h"

#include "KYH/KYH_CommonUserLobby.h"
#include "Blueprint/UserWidget.h"

void AKYH_LobbyController::BeginPlay()
{
    Super::BeginPlay();

    CreateWidget<UKYH_CommonUserLobby>(LobbyWidget);
    LobbyWidget->SetOwningPlayer(this);
    LobbyWidget->AddToViewport();
    LobbyWidget->Init();
    
    SetInputMode(FInputModeUIOnly());
}
