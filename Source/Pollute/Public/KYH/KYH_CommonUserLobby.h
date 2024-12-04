// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "KYH_CommonUserLobby.generated.h"

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
    void Init();
    virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
    
    UPROPERTY(Replicated, meta = (BindWidget))
    UCommonTextBlock* Text_SessionName;
    UPROPERTY(meta = (BindWidget))
    UKYH_PolluteButtonBase* Btn_Start;
    UFUNCTION()
    void StartGame();
    UPROPERTY(meta = (BindWidget))
    UVerticalBox* VerticalBox;
    UFUNCTION(Server, Reliable)
    void ServerRPC_SetPlayerSlotUI(AP_GameState* GameState);
    UFUNCTION(NetMulticast, Reliable)
    void ClientRPC_AddPlayerSlotUI(AP_GameState* GameState);
    UPROPERTY(EditDefaultsOnly, Category = "Session")
    TSubclassOf<UKYH_PlayerSlot> PlayerSlotClass;
    
};
