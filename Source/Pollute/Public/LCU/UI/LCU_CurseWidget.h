// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LCU_CurseWidget.generated.h"

/**
 * 
 */
UCLASS()
class POLLUTE_API ULCU_CurseWidget : public UUserWidget
{
    GENERATED_BODY()
    UPROPERTY(meta=(BindWidget))
    class UTextBlock* Text;
    

    
};
