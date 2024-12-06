// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "KYH_PlayerSlot.generated.h"

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

    void Init(FName InPlayerName, EPlayerType InPlayerType);
    
protected:
    
    UPROPERTY(meta = (BindWidget))
    UCommonTextBlock* PlayerName;
    UPROPERTY(meta = (BindWidget))
    UImage* Player_Thumbnail;

    UPROPERTY(EditDefaultsOnly, Category = "PlayerSlot")
    TArray<UTexture*> PlayerThumbImage;
private:
    
};
