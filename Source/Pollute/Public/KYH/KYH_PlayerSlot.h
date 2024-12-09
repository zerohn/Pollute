// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "KYH_PlayerSlot.generated.h"

class UKYH_PolluteButtonBase;
enum class EPlayerType : uint8;
class UCommonTextBlock;
class UImage;

/**
 * 
 */
UCLASS()
class POLLUTE_API UKYH_PlayerSlot : public UCommonUserWidget
{
	GENERATED_BODY()

public:

    virtual void NativeConstruct() override;
    void Init(FName InPlayerName, EPlayerType InPlayerType);
    
protected:
    
    UPROPERTY(meta = (BindWidget))
    UCommonTextBlock* PlayerName;
    UPROPERTY(meta = (BindWidget))
    UImage* Player_Thumbnail;
    UPROPERTY(meta = (BindWidget))
    UKYH_PolluteButtonBase* Btn_Left;
    UPROPERTY(meta = (BindWidget))
    UKYH_PolluteButtonBase* Btn_Right;

    UPROPERTY(EditDefaultsOnly, Category = "PlayerSlot")
    TArray<UTexture*> PlayerThumbImage;
    
    EPlayerType CurrentPlayerType;
    
public:

    UFUNCTION(Server, Reliable)
    void ChangeCharacterLeft();
    UFUNCTION(Server, Reliable)
    void ChangeCharacterRight();
    UFUNCTION(NetMulticast, Reliable)
    void UpdatePlayerType(EPlayerType InPlayerType);
    
};
