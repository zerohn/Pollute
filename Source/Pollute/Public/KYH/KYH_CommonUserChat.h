// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "KYH_CommonUserChat.generated.h"

class UCommonTextBlock;
/**
 * 
 */
UCLASS()
class POLLUTE_API UKYH_CommonUserChat : public UCommonUserWidget
{
	GENERATED_BODY()

public:
    
    UPROPERTY(meta = (BindWidget))
    UCommonTextBlock* Text_Chat;

    UFUNCTION()
    void SetChatItem(const FString Chat);
};
