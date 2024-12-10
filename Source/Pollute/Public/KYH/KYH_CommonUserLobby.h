// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "KYH_CommonUserLobby.generated.h"

enum class EPlayerType : uint8;
class UKYH_CommonUserChat;
class UEditableText;
class UCommonHierarchicalScrollBox;
class AP_GameState;
class UKYH_PlayerSlot;
class UVerticalBox;
class UP_GameInstance;
class UCommonTextBlock;
class UKYH_PolluteButtonBase;

/**
 * 
 */
UCLASS()
class POLLUTE_API UKYH_CommonUserLobby : public UCommonUserWidget
{
	GENERATED_BODY()

public:

    virtual void NativeConstruct() override;
    virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
    // 세션 로비 UI
    UPROPERTY(meta = (BindWidget))
    UCommonTextBlock* Text_SessionName;
    UPROPERTY(meta = (BindWidget))
    UKYH_PolluteButtonBase* Btn_Start;
    UFUNCTION()
    void StartGame();
    UPROPERTY(meta = (BindWidget))
    UVerticalBox* VerticalBox;
    UFUNCTION()
    void InitLobbyUI(const FText& SessionName, const TArray<FName>& PlayerNames);
    UFUNCTION()
    void AddPlayerSlotUI(const TArray<FName>& PlayerNames);
    UPROPERTY(EditDefaultsOnly, Category = "Session")
    TSubclassOf<UKYH_PlayerSlot> PlayerSlotClass;
    UPROPERTY()
    TArray<UKYH_PlayerSlot*> PlayerSlots;

    // 채팅창
    UPROPERTY(meta = (BindWidget))
    UCommonHierarchicalScrollBox* Scroll_ChatList;
    UPROPERTY(meta = (BindWidget))
    UEditableText* Edit_ChatBox;
    UPROPERTY(EditDefaultsOnly, Category = "Session")
    TSubclassOf<UKYH_CommonUserChat> ChatItemClass;

    
    UFUNCTION()
    void OnEditableTextCommittedEvent(const FText& Text, ETextCommit::Type CommitMethod);
    UFUNCTION()
    void AddChat(const FString& Chat);
    
};
