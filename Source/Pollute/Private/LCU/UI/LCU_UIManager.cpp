// Fill out your copyright notice in the Description page of Project Settings.


#include "LCU/UI/LCU_UIManager.h"

#include "Blueprint/UserWidget.h"
#include "Components/Border.h"
#include "Components/Overlay.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Engine/Engine.h"
#include "GameFramework/Character.h"
#include "LCU/Player/LCU_PlayerController.h"
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
        if(PlayerHUD)
        {
            PlayerHUD->CurseOverlay->SetVisibility(ESlateVisibility::Visible);
            PlayerHUD->CarryCurseCool->SetPercent(1.0f);
        }
    }
    else
    {
        if(PlayerHUD)
        {
            PlayerHUD->CurseOverlay->SetVisibility(ESlateVisibility::Hidden);
            PlayerHUD->CarryCurseCool->SetPercent(0.0f);
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

    if(!LCU_Pc) return;
    
    // 사람 HUD 추가하기
    if(PlayerHUDFactory)
    {
        PlayerHUD = CreateWidget<UHHR_PlayerHUD>(LCU_Pc,PlayerHUDFactory);
        if(PlayerHUD)
        {
            PlayerHUD->AddToViewport();
            PlayerHUD->SetItemDialogVisibility(false);
        }
    }
}

