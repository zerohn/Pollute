// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "CommonActivatableSessionCreate.generated.h"

class UCircularThrobber;
class UP_GameInstance;
class UEditableText;
class UKYH_PolluteButtonBase;

/**
 * 
 */
UCLASS()
class POLLUTE_API UCommonActivatableSessionCreate : public UCommonActivatableWidget
{
	GENERATED_BODY()

public:
    virtual void NativeConstruct() override;

    UPROPERTY()
    UP_GameInstance* GI;
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    UKYH_PolluteButtonBase* Btn_Create;
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    UEditableText* EditText_SessionName;
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    UCircularThrobber* CircularThrobber_Loading;
    UFUNCTION(BlueprintImplementableEvent, Category = "Session")
    void OnCreateClicked();
    UFUNCTION(BlueprintCallable, Category = "Session")
    void CreateSession();
    UFUNCTION(BlueprintImplementableEvent, Category = "Session")
    void OnCreateSessionComplete(bool bIsComplete);
};
