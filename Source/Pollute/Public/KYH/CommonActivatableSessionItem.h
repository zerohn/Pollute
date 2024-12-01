// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "CommonActivatableSessionItem.generated.h"

/**
 * 
 */
UCLASS()
class POLLUTE_API UCommonActivatableSessionItem : public UCommonActivatableWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	class UKYH_PolluteButtonBase* Btn_Join;
	UPROPERTY(meta = (BindWidget))
	class UCommonTextBlock* Session_Name;
	UPROPERTY(meta = (BindWidget))
	class UCommonTextBlock* Session_Creator;
	
	UFUNCTION()
	void Join();

	void SetInfo(int32 Idx, FString SessionName, FString SessionCreator);
	
protected:
	
	int32 SelectIdx;
};
