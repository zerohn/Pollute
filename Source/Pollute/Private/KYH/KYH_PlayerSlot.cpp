// Fill out your copyright notice in the Description page of Project Settings.


#include "KYH/KYH_PlayerSlot.h"

#include "CommonTextBlock.h"
#include "Components/Image.h"


void UKYH_PlayerSlot::Init(FName InPlayerName, UImage* InPlayerThumbnail)
{
    PlayerName->SetText(FText::FromName(InPlayerName));
    //Player_Thumbnail = InPlayerThumbnail;
}
