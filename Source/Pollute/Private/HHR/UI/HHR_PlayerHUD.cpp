// Fill out your copyright notice in the Description page of Project Settings.


#include "HHR/UI/HHR_PlayerHUD.h"

#include "Components/Image.h"
#include "Engine/Texture2D.h"
#include "HHR/HHR_ItemData.h"
#include "HHR/UI/HHR_ItemDialog.h"


void UHHR_PlayerHUD::NativeConstruct()
{
	Super::NativeConstruct();

	//ItemDialog = Cast<UUserWidget>(GetWidgetFromName("ItemDialog_Test"));
	
	// Item Dialog 숨기기
	ItemDialog->SetVisibility(ESlateVisibility::Hidden);

}

void UHHR_PlayerHUD::SetItemDialogVisibility(bool Visible)
{
	if(!ItemDialog)
	{
		P_LOG(PolluteLog, Warning, TEXT("widget 바인드가 안됨"));
		return;
	}
	
	ItemDialog->SetVisibility(Visible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	
}

void UHHR_PlayerHUD::SetItemDialogText(FText Text)
{
	if(ItemDialog)
	{
		ItemDialog->SetText(Text);
	}
}

void UHHR_PlayerHUD::ChangeItemImageNull()
{
    FSlateBrush Brush;
    Brush.SetResourceObject(nullptr);
    ItemImage->SetBrush(Brush);

    FLinearColor CurrentColor = ItemImage->ColorAndOpacity;
    CurrentColor.A = 0.0f;
    
    ItemImage->SetColorAndOpacity(CurrentColor);
}

void UHHR_PlayerHUD::ChangeItemImage(class UTexture2D* Texture)
{
    // 아이템 있으면 img 데이터로 change + tint의 알파값 변경
    FSlateBrush Brush;
    Brush.SetResourceObject(Texture);
    ItemImage->SetBrush(Brush);

    FLinearColor CurrentColor = ItemImage->ColorAndOpacity;
    CurrentColor.A = 1.0f;
    
    ItemImage->SetColorAndOpacity(CurrentColor);
}
