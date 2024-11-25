// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "KYH_Session_UI.generated.h"

/**
 * 
 */
UCLASS()
class POLLUTE_API UKYH_Session_UI : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "UI")
	void AddItemToGrid();
protected:
	
private:
	
};
