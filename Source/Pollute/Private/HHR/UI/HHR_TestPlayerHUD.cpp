// Fill out your copyright notice in the Description page of Project Settings.


#include "HHR/UI/HHR_TestPlayerHUD.h"

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
