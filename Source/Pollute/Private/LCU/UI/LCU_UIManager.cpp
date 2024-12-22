// Fill out your copyright notice in the Description page of Project Settings.


#include "LCU/UI/LCU_UIManager.h"

#include "Blueprint/UserWidget.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "LCU/Player/LCU_PlayerController.h"
#include "LCU/UI/LCU_CurseWidget.h"
#include "HHR/UI/HHR_PlayerHUD.h"

// Sets default values
ALCU_UIManager::ALCU_UIManager()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    bReplicates = true;
}

void ALCU_UIManager::ShowCurseWidget(bool bShow)
{
    // 저주에 걸렸을 때 저주 위젯을 켜주자
    
    if(bShow)
    {
        if(LCU_CurseWidget)
        {
            LCU_CurseWidget->SetVisibility(ESlateVisibility::Visible);
        }
    }
    else
    {
        if(LCU_CurseWidget)
        {
            LCU_CurseWidget->SetVisibility(ESlateVisibility::Hidden);
        }
    }
}

// Called when the game starts or when spawned
void ALCU_UIManager::BeginPlay()
{
    Super::BeginPlay();

}

void ALCU_UIManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);
}

// Called every frame
void ALCU_UIManager::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ALCU_UIManager::Init()
{
    AActor* ow = GetOwner();
    // 오너 설정
    LCU_Pc = Cast<ALCU_PlayerController>(ow);
    if(!LCU_Pc)
    {
        P_LOG(PolluteLog, Log, TEXT("notowner"));
    }
    else
    {
        P_LOG(PolluteLog, Log, TEXT("123123213213"));
    }

    if(!LCU_Pc) return;
    
    // 사람 전용 저주 위젯 추가하가
    if(LCU_CurseWidgetFactory)
    {
        LCU_CurseWidget = CreateWidget<ULCU_CurseWidget>(LCU_Pc,LCU_CurseWidgetFactory);
        //LCU_CurseWidget = CreateWidget<ULCU_CurseWidget>(GetWorld()->GetFirstPlayerController(),LCU_CurseWidgetFactory);
        if(LCU_CurseWidget)
        {
            LCU_CurseWidget->AddToViewport();
            LCU_CurseWidget->SetVisibility(ESlateVisibility::Hidden);
        }
    }

    // 사람 HUD 추가하기
    if(PlayerHUDFactory)
    {
        PlayerHUD = CreateWidget<UHHR_PlayerHUD>(LCU_Pc,PlayerHUDFactory);
        //PlayerHUD = CreateWidget<UHHR_PlayerHUD>(GetWorld()->GetFirstPlayerController(),PlayerHUDFactory);
        if(PlayerHUD)
        {
            PlayerHUD->AddToViewport();
            PlayerHUD->SetItemDialogVisibility(false);
        }
    }
}

