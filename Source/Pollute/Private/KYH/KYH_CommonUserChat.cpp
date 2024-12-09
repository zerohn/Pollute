// Fill out your copyright notice in the Description page of Project Settings.


#include "KYH/KYH_CommonUserChat.h"

#include "CommonTextBlock.h"

void UKYH_CommonUserChat::SetChatItem(const FString Chat)
{
    Text_Chat->SetText(FText::FromString(Chat));
}
