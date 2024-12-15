// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "KYH_PlayerSlot.generated.h"

enum class EPlayerType : uint8;
class UKYH_PolluteButtonBase;
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

    UFUNCTION()
    void SetButtonVisibility(bool bIsVisible);
    UFUNCTION()
    void ChangeCharacterLeft();
    UFUNCTION()
    void ChangeCharacterRight();
    UFUNCTION()
    void UpdatePlayerType(EPlayerType InPlayerType);
    
};
