// Fill out your copyright notice in the Description page of Project Settings.


#include "HHR/UI/HHR_PlayerHUD.h"

#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Engine/Texture2D.h"
#include "HHR/UI/HHR_ItemDialog.h"
#include "Kismet/KismetMathLibrary.h"


void UHHR_PlayerHUD::NativeConstruct()
{
	Super::NativeConstruct();

	ItemDialog->SetVisibility(ESlateVisibility::Hidden);
    CurseOverlay->SetVisibility(ESlateVisibility::Hidden);
}

void UHHR_PlayerHUD::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

}

void UHHR_PlayerHUD::SetItemDialogVisibility(bool Visible)
{
    if(!CanShowDialog) return;
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

void UHHR_PlayerHUD::SetCarryCurseCool(float Value)
{
    float percent = UKismetMathLibrary::NormalizeToRange(Value, 0, MaxCoolDownTime);
    CarryCurseCool->SetPercent(percent);
}

void UHHR_PlayerHUD::SetStaminaBarPercent(float value)
{
    float percent = UKismetMathLibrary::NormalizeToRange(value, 0, MaxStemina);
    SteminaBar->SetPercent(percent);
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
