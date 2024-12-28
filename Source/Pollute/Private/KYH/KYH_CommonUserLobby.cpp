// Fill out your copyright notice in the Description page of Project Settings.


#include "KYH/KYH_CommonUserLobby.h"

#include "CommonHierarchicalScrollBox.h"
#include "CommonTextBlock.h"
#include "TimerManager.h"
#include "Components/EditableText.h"
#include "Components/Image.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Engine/Engine.h"
#include "GameFramework/GameState.h"
#include "GameFramework/Pawn.h"
#include "Kismet/GameplayStatics.h"
#include "KYH/KYH_CommonUserChat.h"
#include "KYH/KYH_LobbyController.h"
#include "KYH/KYH_PolluteButtonBase.h"
#include "KYH/KYH_PlayerSlot.h"
#include "LCU/Player/LCU_PlayerCharacter.h"
#include "P_Settings/P_GameInstance.h"
#include "P_Settings/P_PlayerState.h"

void UKYH_CommonUserLobby::NativeConstruct()
{
    Super::NativeConstruct();

    Btn_Start->OnClicked().AddUObject(this, &UKYH_CommonUserLobby::StartGame);
    
    Edit_ChatBox->OnTextCommitted.AddDynamic(this, &UKYH_CommonUserLobby::OnEditableTextCommittedEvent);
    
    // FTimerHandle AddSlotHandle;
    // GetWorld()->GetTimerManager().SetTimer(AddSlotHandle, this, &UKYH_CommonUserLobby::Init, 0.1f, false);
}

void UKYH_CommonUserLobby::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    //DOREPLIFETIME(UKYH_CommonUserLobby, Text_SessionName);
}

void UKYH_CommonUserLobby::StartGame()
{
    //if (GameState->PlayerArray.Num() < 6) return;
    P_LOG(PolluteLog, Warning, TEXT("Press Start"))
    if (!GetWorld()->GetFirstPlayerController()->HasAuthority()) return;
    UP_GameInstance* GI = Cast<UP_GameInstance>(GetWorld()->GetGameInstance());
    TArray<APlayerState*> PStateArray = GetWorld()->GetGameState()->PlayerArray;
    GI->PlayerTypes.Empty();
    for (APlayerState* PState : PStateArray)
    {
        GI->PlayerTypes.Add(Cast<AKYH_LobbyController>(PState->GetPlayerController())->GetCurrentPlayerType());
    }
    GetWorld()->ServerTravel(GI->GetMainGameLevelURL() + "?Listen", true);
}

void UKYH_CommonUserLobby::InitLobbyUI(const FText& SessionName)
{
    Text_SessionName->SetText(SessionName);
    VerticalBox->ClearChildren();
    PlayerSlots.Empty();
}

void UKYH_CommonUserLobby::AddPlayerSlotUI(const FName PlayerName, const EPlayerType PlayerType)
{
    UKYH_PlayerSlot* PlayerSlot = CreateWidget<UKYH_PlayerSlot>(GetWorld(), PlayerSlotClass);
    PlayerSlot->Init(PlayerName, PlayerType);
    
    //P_LOG(PolluteLog, Warning, TEXT("Get Name: %s Param Name: %s"), *GetWorld()->GetFirstPlayerController()->GetPlayerState<AP_PlayerState>()->GetPlayerName(), *PlayerName.ToString());
    
    VerticalBox->AddChild(PlayerSlot);
    
    if (GetWorld()->GetFirstPlayerController()->GetPlayerState<AP_PlayerState>()->GetPlayerName() == PlayerName.ToString())
    {
        PlayerSlot->SetButtonVisibility(true);
    }
    
    UVerticalBoxSlot* CurrentSlot = Cast<UVerticalBoxSlot>(PlayerSlot->Slot);
    CurrentSlot->SetPadding(FMargin(0, 0, 0, 15));
    
    PlayerSlots.Add(PlayerSlot);
}

void UKYH_CommonUserLobby::OnEditableTextCommittedEvent(const FText& Text, ETextCommit::Type CommitMethod)
{
    if (CommitMethod == ETextCommit::OnEnter)
    {
        // 서버에 Chat Item 추가 함수 호출
        AP_PlayerState* PS = Cast<AP_PlayerState>(GetWorld()->GetFirstPlayerController()->GetPlayerState<AP_PlayerState>());
        Cast<AKYH_LobbyController>(PS->GetPlayerController())->ServerRPC_SendChat(Text.ToString());
        
        // 채팅 입력 내용 초기화
        Edit_ChatBox->SetText(FText());
    }
    else if (CommitMethod == ETextCommit::OnCleared)
    {
        Edit_ChatBox->SetFocus();
    }
}

void UKYH_CommonUserLobby::AddChat(const FString& Chat)
{
    UKYH_CommonUserChat* ChatItem = CreateWidget<UKYH_CommonUserChat>(GetWorld(), ChatItemClass);
    ChatItem->SetChatItem(Chat);
    Scroll_ChatList->AddChild(ChatItem);

    if (Scroll_ChatList->GetScrollOffset() == Scroll_ChatList->GetScrollOffsetOfEnd())
    {
        FTimerHandle ChatScrollHandle;
        GetWorld()->GetTimerManager().SetTimer(ChatScrollHandle, [this]()
        {
            Scroll_ChatList->ScrollToEnd();
        }, 0.01f, false);
    }
}
