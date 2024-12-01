// Fill out your copyright notice in the Description page of Project Settings.


#include "KYH/KYH_LobbyController.h"

#include "CommonUserWidget.h"
#include "Blueprint/UserWidget.h"

void AKYH_LobbyController::BeginPlay()
{
    Super::BeginPlay();

    CreateWidget<UCommonUserWidget>(LobbyWidget);

    LobbyWidget->AddToViewport();
    
    SetInputMode(FInputModeUIOnly());
}
