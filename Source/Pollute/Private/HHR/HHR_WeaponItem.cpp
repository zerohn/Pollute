// Fill out your copyright notice in the Description page of Project Settings.


#include "HHR/HHR_WeaponItem.h"

#include "TimerManager.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "HHR/UI/HHR_TestPlayerHUD.h"
#include "Kismet/GameplayStatics.h"
#include "LCU/Player/LCU_PlayerCharacter.h"
#include "Slate/SGameLayerManager.h"


void AHHR_WeaponItem::Attack()
{
}

void AHHR_WeaponItem::DestoryDelay(ALCU_PlayerCharacter* player)
{
    // 몇 초 후에 사라짐
    /*
    FTimerHandle timerHandler;
    GetWorld()->GetTimerManager().SetTimer(timerHandler, FTimerDelegate::CreateLambda([&](ALCU_PlayerCharacter* player)
    {
        // Owner의 FinalItem null 설정
        // 오브젝트 삭제
        // item의 setowner가 Replicated 되어 있어서 이렇게 하면 안됨 
        //ALCU_PlayerCharacter* player = Cast<ALCU_PlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
        if(player)
        {
            // 임시 UI
            // Controller 가지고 있어야 삭제해야함
            if(player->PlayerHUD && player->IsLocallyControlled())
            {
                player->PlayerHUD->ChangeItemImageNull();
            }
            player->InitItem();
        }
        Destroy();
        
    }), 1.5f, false );
    */

    FTimerHandle timerHandler;
    
    FTimerDelegate timerDelegate;
    timerDelegate.BindUFunction(this, FName("DelegateDestroy"), player);
    
    GetWorld()->GetTimerManager().SetTimer(timerHandler, timerDelegate, 1.5f, false );
    
}

void AHHR_WeaponItem::DelegateDestroy(class ALCU_PlayerCharacter* player)
{
    // Owner의 FinalItem null 설정
    // 오브젝트 삭제
    // item의 setowner가 Replicated 되어 있어서 이렇게 하면 안됨 
    //ALCU_PlayerCharacter* player = Cast<ALCU_PlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
    if(player)
    {
        // 임시 UI
        // Controller 가지고 있어야 삭제해야함
        if(player->PlayerHUD && player->IsLocallyControlled())
        {
            player->PlayerHUD->ChangeItemImageNull();
        }
        player->InitItem();
    }
    Destroy();
}

void AHHR_WeaponItem::NetMulticast_UInull_Implementation()
{

}
