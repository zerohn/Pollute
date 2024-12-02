// Fill out your copyright notice in the Description page of Project Settings.


#include "KYH/CommonActivatableSessionList.h"
#include "P_Settings/P_GameInstance.h"
#include "CommonHierarchicalScrollBox.h"
#include "CommonButtonBase.h"
#include "KYH/CommonActivatableSessionItem.h"
#include "KYH/KYH_PolluteButtonBase.h"

void UCommonActivatableSessionList::NativeConstruct()
{
    Super::NativeConstruct();

    GI = Cast<UP_GameInstance>(GetGameInstance());

    GI->OnAddSessionDelegates.BindUObject(this, &UCommonActivatableSessionList::OnAddSession);
    GI->OnFindCompleteDelegates.BindUObject(this, &UCommonActivatableSessionList::OnFindComplete);

}

void UCommonActivatableSessionList::NativeOnActivated()
{
    Super::NativeOnActivated();
    
    FindSessions();
}

void UCommonActivatableSessionList::FindSessions()
{
    SessionScrollBox->ClearChildren();
    Btn_Find->SetIsEnabled(false);
    GI->FindSessions();
}

void UCommonActivatableSessionList::OnAddSession(int32 Idx, FString SessionName, FString SessionCreator)
{
    UCommonActivatableSessionItem* SessionItem = CreateWidget<UCommonActivatableSessionItem>(GetWorld(), SessionItemClass);
    SessionScrollBox->AddChild(Cast<UWidget>(SessionItem));
    SessionItem->SetInfo(Idx, SessionName, SessionCreator);
}

void UCommonActivatableSessionList::OnFindComplete(bool bIsComplete)
{
    Btn_Find->SetIsEnabled(bIsComplete);
}
