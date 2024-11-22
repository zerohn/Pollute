// Fill out your copyright notice in the Description page of Project Settings.


#include "KYH/KYH_PolluteButtonBase.h"

#include "CommonActionWidget.h"


void UKYH_PolluteButtonBase::NativeConstruct()
{
	Super::NativeConstruct();
	UpdateButtonStyle();
	RefreshButtonText();
}

void UKYH_PolluteButtonBase::UpdateInputActionWidget()
{
	Super::UpdateInputActionWidget();
	UpdateButtonStyle();
	RefreshButtonText();
}

void UKYH_PolluteButtonBase::SetButtonText(const FText& InText)
{
	bOverride_ButtonText = InText.IsEmpty();
	ButtonText = InText;
	RefreshButtonText();
}

void UKYH_PolluteButtonBase::RefreshButtonText()
{
	if (bOverride_ButtonText || ButtonText.IsEmpty())
	{
		if (InputActionWidget)
		{
			const FText ActionDisplayText = InputActionWidget->GetDisplayText();
			if (!ActionDisplayText.IsEmpty())
			{
				UpdateButtonText(ActionDisplayText);
				return;
			}
		}
	}
}

void UKYH_PolluteButtonBase::OnInputMethodChanged(ECommonInputType CurrentInputType)
{
	Super::OnInputMethodChanged(CurrentInputType);

	UpdateButtonStyle();
}
