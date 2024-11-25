// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "KYH_PolluteButtonBase.generated.h"

/**
 * 
 */
UCLASS(Abstract, BlueprintType, Blueprintable)
class POLLUTE_API UKYH_PolluteButtonBase : public UCommonButtonBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetButtonText(const FText& InText);

protected:
	virtual void NativeConstruct() override;
	virtual void UpdateInputActionWidget() override;
	virtual void OnInputMethodChanged(ECommonInputType CurrentInputType) override;

	void RefreshButtonText();

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateButtonText(const FText& InText);
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateButtonStyle();
	
private:
	UPROPERTY(EditAnywhere, Category = "Button", meta = (InlineEditConditionToggle))
	uint8 bOverride_ButtonText = 1;

	UPROPERTY(EditAnywhere, Category = "Button", meta = (EditCondition = "bOverride_ButtonText"))
	FText ButtonText;
};
