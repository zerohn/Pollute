// Fill out your copyright notice in the Description page of Project Settings.


#include "KYH/KYH_Main_UI.h"

#include "Components/WidgetSwitcher.h"

bool UKYH_Main_UI::ChangeUI(FName WidgetName)
{
	for (UWidget* Widget : MainUISwitcher->GetAllChildren())
	{
		if (Widget->GetFName() == WidgetName)
		{
			MainUISwitcher->SetActiveWidget(Widget);
			return true;
		}
	}
	return false;
}
