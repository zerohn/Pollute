// Fill out your copyright notice in the Description page of Project Settings.


#include "KYH/KYH_PlayerSlot.h"

#include "CommonTextBlock.h"
#include "Components/Image.h"
#include "LCU/Player/LCU_PlayerCharacter.h"


void UKYH_PlayerSlot::Init(FName InPlayerName, EPlayerType InPlayerType)
{
    PlayerName->SetText(FText::FromName(InPlayerName));

    FSlateBrush Brush;
    switch (InPlayerType)
    {
    case EPlayerType::Eric:
        {
            Brush.SetResourceObject(PlayerThumbImage[0]);
            break;
        }
    case EPlayerType::Manuel:
        {
            Brush.SetResourceObject(PlayerThumbImage[1]);
            break;
        }
    case EPlayerType::Sophia:
        {
            Brush.SetResourceObject(PlayerThumbImage[2]);
            break;
        }
    case EPlayerType::Carla:
        {
            Brush.SetResourceObject(PlayerThumbImage[3]);
            break;
        }
    case EPlayerType::Nathan:
        {
            Brush.SetResourceObject(PlayerThumbImage[4]);
            break;
        }
    case EPlayerType::Claudia:
        {
            Brush.SetResourceObject(PlayerThumbImage[5]);
            break;
        }
    default:
        break;
    }
    Brush.SetImageSize(FVector2d(256,192));
    Player_Thumbnail->SetBrush(Brush);
}
