// Fill out your copyright notice in the Description page of Project Settings.


#include "HHR/HHR_ItemDialog.h"

#include "Components/TextBlock.h"

void UHHR_ItemDialog::SetText(FText ItemName)
{

	if(TextDialog)
	{
		FString text = ItemName.ToString() + TEXT(" 줍기");
		TextDialog->SetText(FText::FromString(text));
	}
}
