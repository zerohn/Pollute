// Fill out your copyright notice in the Description page of Project Settings.


#include "KYH/CommonActivatableSessionItem.h"
#include "KYH/KYH_PolluteButtonBase.h"
#include "CommonTextBlock.h"
#include "P_Settings/P_GameInstance.h"

void UCommonActivatableSessionItem::NativeConstruct()
{
	Super::NativeConstruct();

	Btn_Join->OnClicked().AddUObject(this, &UCommonActivatableSessionItem::Join);
}

void UCommonActivatableSessionItem::Join()
{
	UP_GameInstance* GI = Cast<UP_GameInstance>(GetGameInstance());
	GI->JoinSelectSession(SelectIdx);
}

void UCommonActivatableSessionItem::SetInfo(int32 Idx, FString SessionName, FString SessionCreator)
{
	SelectIdx = Idx;
	Session_Name->SetText(FText::FromString(SessionName));
	Session_Creator->SetText(FText::FromString(SessionCreator));
}
