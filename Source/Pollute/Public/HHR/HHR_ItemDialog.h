// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HHR_ItemDialog.generated.h"

/**
 * 
 */
UCLASS()
class POLLUTE_API UHHR_ItemDialog : public UUserWidget
{
	GENERATED_BODY()


// widget
protected:
	// vertical box
	UPROPERTY(meta = (BindWidget))
	class UHorizontalBox* DialogPanel;

	// Boarder
	UPROPERTY(meta = (BindWidget))
	class UBorder* KeyBoarder;

	// Boarder Text
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* KeyName;

	// Text Dialog
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TextDialog;


// Functions
public:
	void SetText(FText ItemName);
	
	
};
