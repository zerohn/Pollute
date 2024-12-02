// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "CommonActivatableSessionList.generated.h"

class UP_GameInstance;
class UCommonHierarchicalScrollBox;
class UCommonActivatableSessionItem;
class UKYH_PolluteButtonBase;

/**
 * 
 */
UCLASS()
class POLLUTE_API UCommonActivatableSessionList : public UCommonActivatableWidget
{
	GENERATED_BODY()

public:
    virtual void NativeConstruct() override;
    virtual void NativeOnActivated() override;

    UPROPERTY(meta = (BindWidget))
    UCommonHierarchicalScrollBox* SessionScrollBox;
    UPROPERTY(EditAnywhere)
    TSubclassOf<UUserWidget> SessionItemClass;
    UPROPERTY(meta = (BindWidget))
    UKYH_PolluteButtonBase* Btn_Find;

    UPROPERTY()
    UP_GameInstance* GI;

    UFUNCTION()
    void FindSessions();
    UFUNCTION()
    void OnAddSession(int32 Idx, FString SessionName, FString SessionCreator);
    UFUNCTION()
    void OnFindComplete(bool bIsComplete);
};
