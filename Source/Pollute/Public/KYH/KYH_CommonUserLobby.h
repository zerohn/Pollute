// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "KYH_CommonUserLobby.generated.h"

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
    
    UPROPERTY(meta = (BindWidget))
    UCommonTextBlock* Text_SessionName;
    UPROPERTY(meta = (BindWidget))
    UKYH_PolluteButtonBase* Btn_Start;
    void StartGame();
    UPROPERTY(ReplicatedUsing = OnRep_SetPlayerSlot, meta = (BindWidget))
    UVerticalBox* VerticalBox;
    UFUNCTION()
    void OnRep_SetPlayerSlot();
    UPROPERTY(EditDefaultsOnly, Category = "Session")
    TSubclassOf<UKYH_PlayerSlot> PlayerSlotClass;
    
};
