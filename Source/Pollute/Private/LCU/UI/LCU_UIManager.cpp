// Fill out your copyright notice in the Description page of Project Settings.


#include "LCU/UI/LCU_UIManager.h"

#include "Blueprint/UserWidget.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "LCU/Player/LCU_PlayerController.h"
#include "LCU/UI/LCU_CurseWidget.h"

ALCU_UIManager* ALCU_UIManager::Instance = nullptr;


// Sets default values
ALCU_UIManager::ALCU_UIManager()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
}

ALCU_UIManager* ALCU_UIManager::GetInstance(UWorld* world, TSubclassOf<ALCU_UIManager> UIManagerFactory)
{
    if(!Instance && UIManagerFactory)
    {
        Instance = world->SpawnActor<ALCU_UIManager>(UIManagerFactory);
    }
    return Instance;
}

ALCU_UIManager* ALCU_UIManager::GetInstance(UWorld* world)
{
    if(!Instance && world)
    {
        ALCU_PlayerController* pc= Cast<ALCU_PlayerController>(world->GetFirstPlayerController());
        if(pc)
        {
            GetInstance(world, pc->UIManagerFactory);
        }
    }
    return Instance;
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

void ALCU_UIManager::ClientRPC_ShowCurseWidget_Implementation(bool bShow)
{
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

    if(Instance && Instance != this)
    {
        Destroy();
    }

    //Instance = GetInstance(GetWorld());

    // Owner 설정하기
    AActor* player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
    Owner = player;

    // 사람 전용 저주 위젯 추가하가
    if(LCU_CurseWidgetFactory)
    {
        LCU_CurseWidget = CreateWidget<ULCU_CurseWidget>(GetWorld()->GetFirstPlayerController(),LCU_CurseWidgetFactory);
        if(LCU_CurseWidget)
        {
            LCU_CurseWidget->AddToViewport();
            LCU_CurseWidget->SetVisibility(ESlateVisibility::Hidden);
        }
    }
}

void ALCU_UIManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Instance = nullptr;
    
    Super::EndPlay(EndPlayReason);
}

// Called every frame
void ALCU_UIManager::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

