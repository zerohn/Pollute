// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "KYH_Main_UI.generated.h"

/**
 * 
 */
UCLASS()
class POLLUTE_API UKYH_Main_UI : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* MainUISwitcher;

	UFUNCTION(BlueprintCallable)
	bool ChangeUI(FName WidgetName);
	
protected:

	
};
