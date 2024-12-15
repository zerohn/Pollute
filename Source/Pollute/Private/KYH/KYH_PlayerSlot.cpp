// Fill out your copyright notice in the Description page of Project Settings.


#include "KYH/KYH_PlayerSlot.h"

#include "CommonTextBlock.h"
#include "Components/Image.h"
#include "KYH/KYH_LobbyController.h"
#include "KYH/KYH_PolluteButtonBase.h"
#include "LCU/Player/LCU_PlayerCharacter.h"


void UKYH_PlayerSlot::NativeConstruct()
{
    Super::NativeConstruct();

    Btn_Left->OnClicked().AddUObject(this, &UKYH_PlayerSlot::ChangeCharacterLeft);
    Btn_Right->OnClicked().AddUObject(this, &UKYH_PlayerSlot::ChangeCharacterRight);
    //Btn_Left->SetVisibility(ESlateVisibility::Hidden);
    //Btn_Right->SetVisibility(ESlateVisibility::Hidden);
}

void UKYH_PlayerSlot::Init(FName InPlayerName, EPlayerType InPlayerType)
{
    CurrentPlayerType = InPlayerType;
    PlayerName->SetText(FText::FromName(InPlayerName));

    FSlateBrush Brush;
    Brush.SetResourceObject(PlayerThumbImage[(int32)InPlayerType]);
    
    Brush.SetImageSize(FDeprecateSlateVector2D(FVector2f(144, 106)));
    Player_Thumbnail->SetBrush(Brush);
}

void UKYH_PlayerSlot::SetButtonVisibility(bool bIsVisible)
{
    if (bIsVisible)
    {
        Btn_Left->SetVisibility(ESlateVisibility::Visible);
        Btn_Right->SetVisibility(ESlateVisibility::Visible);
    }
    else
    {
        Btn_Left->SetVisibility(ESlateVisibility::Hidden);
        Btn_Right->SetVisibility(ESlateVisibility::Hidden);
    }
}

void UKYH_PlayerSlot::ChangeCharacterLeft()
{
    if ((int32)CurrentPlayerType - 1 < 0)
    {
        CurrentPlayerType = (EPlayerType)((int32)EPlayerType::Count - 1);
    }
    else
    {
        CurrentPlayerType = (EPlayerType)((int32)CurrentPlayerType - 1);
    }
    UpdatePlayerType(CurrentPlayerType);
}

void UKYH_PlayerSlot::ChangeCharacterRight()
{
    if ((int32)CurrentPlayerType + 1 >= (int32)EPlayerType::Count)
    {
        CurrentPlayerType = (EPlayerType)0;
    }
    else
    {
        CurrentPlayerType = (EPlayerType)((int32)CurrentPlayerType + 1);
    }
    UpdatePlayerType(CurrentPlayerType);
}

void UKYH_PlayerSlot::UpdatePlayerType(EPlayerType InPlayerType)
{
    AKYH_LobbyController* LobbyController = Cast<AKYH_LobbyController>(GetWorld()->GetFirstPlayerController());
    LobbyController->ServerRPC_SetPlayerType(InPlayerType);
    // FSlateBrush Brush;
    // Brush.SetResourceObject(PlayerThumbImage[(int32)InPlayerType]);
    // Brush.SetImageSize(FDeprecateSlateVector2D(FVector2f(144, 106)));
    // Player_Thumbnail->SetBrush(Brush);
}
