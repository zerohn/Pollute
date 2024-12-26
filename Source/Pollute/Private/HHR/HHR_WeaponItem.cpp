// Fill out your copyright notice in the Description page of Project Settings.


#include "HHR/HHR_WeaponItem.h"

#include "TimerManager.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "HHR/UI/HHR_PlayerHUD.h"
#include "Kismet/GameplayStatics.h"
#include "LCU/Player/LCU_PlayerCharacter.h"
#include "LCU/Player/LCU_PlayerController.h"
#include "LCU/UI/LCU_UIManager.h"
#include "Slate/SGameLayerManager.h"


void AHHR_WeaponItem::Attack()
{
}

void AHHR_WeaponItem::DestoryDelay(ALCU_PlayerCharacter* player)
{
    if(!HasAuthority()) return;
    
    FTimerHandle timerHandler;
    
    FTimerDelegate timerDelegate;
    timerDelegate.BindUFunction(this, FName("DelegateDestroy"), player);
    
    GetWorld()->GetTimerManager().SetTimer(timerHandler, timerDelegate, 1.5f, false );
    
}

void AHHR_WeaponItem::DelegateDestroy(ALCU_PlayerCharacter* player)
{
    Multicast_DelegateDestroy(player);
}

void AHHR_WeaponItem::Multicast_DelegateDestroy_Implementation(ALCU_PlayerCharacter* player)
{
    if(player)
    {
        player->InitItem();
    }

    ALCU_PlayerController* pc =Cast<ALCU_PlayerController>(GetWorld()->GetFirstPlayerController());
    if(pc && player->IsLocallyControlled())
    {
        pc->UIManager->PlayerHUD->ChangeItemImageNull();
    }
    Destroy();
}

void AHHR_WeaponItem::NetMulticast_UInull_Implementation()
{

}
