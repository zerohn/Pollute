// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HHR_TestPlayerHUD.generated.h"

/**
 * 
 */
UCLASS()
class POLLUTE_API UHHR_TestPlayerHUD : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

// 캐릭터의 Hand Item의 Item Data로 이미지 할당 
// 위젯
public:
	// Panel
	UPROPERTY(meta = (BindWidget))
	class UCanvasPanel* CanvasPanel;

	// Border
	UPROPERTY(meta = (BindWidget))
	class UBorder* BorderPanel;

	// Image
	UPROPERTY(meta = (BindWidget))
	class UImage* ItemImage;

	// Item Dialog widget
	UPROPERTY(meta = (BindWidget))
	//UPROPERTY()
	class UHHR_ItemDialog* ItemDialog;

// Functions
public:
	// Item Dialog visibility 설정
	void SetItemDialogVisibility(bool Visible);

	// ItemDialog wbp Text 세팅
	void SetItemDialogText(FText Text);

    // item Image 변경
    void ChangeItemImage(class UTexture2D* Texture);
    void ChangeItemImageNull();
	
};
