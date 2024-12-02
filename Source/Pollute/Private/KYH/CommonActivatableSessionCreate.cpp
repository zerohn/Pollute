// Fill out your copyright notice in the Description page of Project Settings.


#include "KYH/CommonActivatableSessionCreate.h"

#include "Components/CircularThrobber.h"
#include "Components/EditableText.h"
#include "KYH/KYH_PolluteButtonBase.h"
#include "P_Settings/P_GameInstance.h"

void UCommonActivatableSessionCreate::NativeConstruct()
{
    Super::NativeConstruct();

    GI = Cast<UP_GameInstance>(GetGameInstance());
    
    GI->OnCreateCompleteDelegates.BindUObject(this, &UCommonActivatableSessionCreate::OnCreateSessionComplete);

    Btn_Create->OnClicked().AddUObject(this, &UCommonActivatableSessionCreate::OnCreateClicked);

    CircularThrobber_Loading->SetVisibility(ESlateVisibility::Hidden);
}

void UCommonActivatableSessionCreate::CreateSession()
{
    if (EditText_SessionName->GetText().IsEmpty()) return;

    FString SessionName = EditText_SessionName->GetText().ToString();
    int32 PlayerLimit = 6;
    GI->CreateOwnSession(FName(SessionName), PlayerLimit);
}
