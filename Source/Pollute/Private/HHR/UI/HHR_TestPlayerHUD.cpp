// Fill out your copyright notice in the Description page of Project Settings.


#include "HHR/UI/HHR_TestPlayerHUD.h"

#include "Components/Image.h"
#include "Engine/Texture2D.h"
#include "HHR/HHR_ItemData.h"
#include "HHR/UI/HHR_ItemDialog.h"


void UHHR_TestPlayerHUD::NativeConstruct()
{
	Super::NativeConstruct();

	//ItemDialog = Cast<UUserWidget>(GetWidgetFromName("ItemDialog_Test"));
	
	// Item Dialog 숨기기
	ItemDialog->SetVisibility(ESlateVisibility::Hidden);

}

void UHHR_TestPlayerHUD::SetItemDialogVisibility(bool Visible)
{

	if(!ItemDialog)
	{
		P_LOG(PolluteLog, Warning, TEXT("widget 바인드가 안됨"));
		return;
	}
	
	ItemDialog->SetVisibility(Visible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	
}

void UHHR_TestPlayerHUD::SetItemDialogText(FText Text)
{
	if(ItemDialog)
	{
		ItemDialog->SetText(Text);
	}
}

void UHHR_TestPlayerHUD::ChangeItemImageNull()
{
    FSlateBrush Brush;
    Brush.SetResourceObject(nullptr);
    ItemImage->SetBrush(Brush);

    FLinearColor CurrentColor = ItemImage->ColorAndOpacity;
    CurrentColor.A = 0.0f;
    
    ItemImage->SetColorAndOpacity(CurrentColor);
}

void UHHR_TestPlayerHUD::ChangeItemImage(class UTexture2D* Texture)
{
    // 아이템 있으면 img 데이터로 change + tint의 알파값 변경
    FSlateBrush Brush;
    Brush.SetResourceObject(Texture);
    ItemImage->SetBrush(Brush);

    FLinearColor CurrentColor = ItemImage->ColorAndOpacity;
    CurrentColor.A = 1.0f;
    
    ItemImage->SetColorAndOpacity(CurrentColor);
}
