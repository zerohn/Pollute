// Fill out your copyright notice in the Description page of Project Settings.


#include "KYH/KYH_PlayerSlot.h"

#include "CommonTextBlock.h"
#include "Components/Image.h"
#include "KYH/KYH_PolluteButtonBase.h"
#include "LCU/Player/LCU_PlayerCharacter.h"


void UKYH_PlayerSlot::NativeConstruct()
{
    Super::NativeConstruct();

    Btn_Left->OnClicked().AddUObject(this, &UKYH_PlayerSlot::ChangeCharacterLeft);
    Btn_Right->OnClicked().AddUObject(this, &UKYH_PlayerSlot::ChangeCharacterRight);
}

void UKYH_PlayerSlot::Init(FName InPlayerName, EPlayerType InPlayerType)
{
    CurrentPlayerType = InPlayerType;
    PlayerName->SetText(FText::FromName(InPlayerName));

    FSlateBrush Brush;
    Brush.SetResourceObject(PlayerThumbImage[(int32)InPlayerType]);
    
    Brush.SetImageSize(FDeprecateSlateVector2D(FVector2f(172, 128)));
    Player_Thumbnail->SetBrush(Brush);
}

void UKYH_PlayerSlot::ChangeCharacterLeft_Implementation()
{
    CurrentPlayerType = (EPlayerType)((int32)CurrentPlayerType - 1);
    if ((int32)CurrentPlayerType < 0)
    {
        CurrentPlayerType = (EPlayerType)((int32)EPlayerType::Count - 1);
    }
    UpdatePlayerType(CurrentPlayerType);
}

void UKYH_PlayerSlot::ChangeCharacterRight_Implementation()
{
    CurrentPlayerType = (EPlayerType)((int32)CurrentPlayerType + 1);
    if ((int32)CurrentPlayerType >= (int32)EPlayerType::Count)
    {
        CurrentPlayerType = (EPlayerType)0;
    }
    UpdatePlayerType(CurrentPlayerType);
}

void UKYH_PlayerSlot::UpdatePlayerType_Implementation(EPlayerType InPlayerType)
{
    FSlateBrush Brush;
    Brush.SetResourceObject(PlayerThumbImage[(int32)InPlayerType]);
    Brush.SetImageSize(FDeprecateSlateVector2D(FVector2f(172, 128)));
    Player_Thumbnail->SetBrush(Brush);
}
