// Fill out your copyright notice in the Description page of Project Settings.


#include "KYH/KYH_CommonUserLobby.h"

#include "CommonTextBlock.h"
#include "Components/Image.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "KYH/KYH_PolluteButtonBase.h"
#include "KYH/KYH_PlayerSlot.h"
#include "Net/UnrealNetwork.h"
#include "P_Settings/P_GameInstance.h"
#include "P_Settings/P_GameState.h"

void UKYH_CommonUserLobby::NativeConstruct()
{
    Super::NativeConstruct();

    Btn_Start->OnClicked().AddUObject(this, &UKYH_CommonUserLobby::StartGame);
}

void UKYH_CommonUserLobby::Init()
{
    AP_GameState* GameState = Cast<AP_GameState>(GetWorld()->GetGameState());
    
    if (GetWorld()->GetFirstPlayerController()->HasAuthority())
    {
        Btn_Start->SetVisibility(ESlateVisibility::Visible);
    }
    else
    {
        Btn_Start->SetVisibility(ESlateVisibility::Hidden);
    }
    ServerRPC_SetPlayerSlotUI(GameState);
}

void UKYH_CommonUserLobby::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(UKYH_CommonUserLobby, Text_SessionName);
}

void UKYH_CommonUserLobby::StartGame()
{
    //if (GameState->PlayerArray.Num() < 6) return;

    UP_GameInstance* GI = Cast<UP_GameInstance>(GetWorld()->GetGameInstance());
    GetWorld()->ServerTravel(GI->GetMainGameLevelURL(), true);
}

void UKYH_CommonUserLobby::ServerRPC_SetPlayerSlotUI_Implementation(AP_GameState* GameState)
{
    if (!GameState) return;
    Text_SessionName->SetText(FText::FromName(Cast<UP_GameInstance>(GameState->GetGameInstance())->GetCurrentSessionName()));
    ClientRPC_AddPlayerSlotUI(GameState);
}

void UKYH_CommonUserLobby::ClientRPC_AddPlayerSlotUI_Implementation(AP_GameState* GameState)
{
    VerticalBox->ClearChildren();
    
    for (int i = 0; i < GameState->PlayerArray.Num(); i++)
    {
        UKYH_PlayerSlot* PlayerSlot = CreateWidget<UKYH_PlayerSlot>(GetWorld(), PlayerSlotClass);
        PlayerSlot->Init(FName(GameState->PlayerArray[i]->GetPlayerName()), nullptr);
        VerticalBox->AddChild(PlayerSlot);
        UVerticalBoxSlot* CurrentSlot = Cast<UVerticalBoxSlot>(PlayerSlot->Slot);
        CurrentSlot->SetPadding(FMargin(0, 0, 0, 15));
    }
}