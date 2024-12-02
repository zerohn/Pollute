// Fill out your copyright notice in the Description page of Project Settings.


#include "KYH/KYH_CommonUserLobby.h"

#include "CommonTextBlock.h"
#include "Components/Image.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "KYH/KYH_PolluteButtonBase.h"
#include "KYH/KYH_PlayerSlot.h"
#include "Net/UnrealNetwork.h"
#include "P_Settings/P_GameInstance.h"

void UKYH_CommonUserLobby::NativeConstruct()
{
    Super::NativeConstruct();

    
    APlayerController* PC = GetWorld()->GetFirstPlayerController();
    
    Text_SessionName->SetText(FText::FromName(Cast<UP_GameInstance>(PC->GetGameInstance())->GetCurrentSessionName()));

    
    if (PC && PC->HasAuthority())
    {
        Btn_Start->SetVisibility(ESlateVisibility::Visible);
    }
    else
    {
        Btn_Start->SetVisibility(ESlateVisibility::Hidden);
    }

    if (!PC || !PC->HasAuthority()) return;
    //OnRep_SetPlayerSlot();
}

void UKYH_CommonUserLobby::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    
    DOREPLIFETIME(UKYH_CommonUserLobby, VerticalBox);
}

void UKYH_CommonUserLobby::StartGame()
{
    
}

void UKYH_CommonUserLobby::OnRep_SetPlayerSlot()
{
    VerticalBox->ClearChildren();
    
    for (int i = 0; i < UGameplayStatics::GetNumPlayerControllers(GetWorld()); i++)
    {
        UKYH_PlayerSlot* PlayerSlot = CreateWidget<UKYH_PlayerSlot>(GetWorld(), PlayerSlotClass);
        PlayerSlot->Init(FName(UGameplayStatics::GetPlayerState(GetWorld(), i)->GetPlayerName()), nullptr);
        VerticalBox->AddChild(PlayerSlot);
        UVerticalBoxSlot* CurrentSlot = Cast<UVerticalBoxSlot>(PlayerSlot->Slot);
        CurrentSlot->SetPadding(FMargin(0, 0, 0, 15));
    }
    
}
