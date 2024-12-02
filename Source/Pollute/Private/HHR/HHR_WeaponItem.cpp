// Fill out your copyright notice in the Description page of Project Settings.


#include "HHR/HHR_WeaponItem.h"

#include "TimerManager.h"
#include "Engine/World.h"
#include "LCU/Player/LCU_PlayerCharacter.h"
#include "Slate/SGameLayerManager.h"

void AHHR_WeaponItem::Attack()
{
}

void AHHR_WeaponItem::DestoryDelay()
{
    // 몇 초 후에 사라짐
    FTimerHandle timerHandler;
    GetWorld()->GetTimerManager().SetTimer(timerHandler, FTimerDelegate::CreateLambda([&]()
    {
        // Owner의 FinalItem null 설정
        // 오브젝트 삭제
        ALCU_PlayerCharacter* player = Cast<ALCU_PlayerCharacter>(GetOwner());
        if(player)
        {
            player->InitItem();
        }
        Destroy();
        
    }), 5.f, false );
    
}
