// Fill out your copyright notice in the Description page of Project Settings.


#include "HHR/AnimNotify/HHR_AN_GunFire.h"

#include "Components/SkeletalMeshComponent.h"
#include "HHR/HHR_Gun.h"
#include "LCU/Player/LCU_PlayerCharacter.h"

void UHHR_AN_GunFire::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    Super::Notify(MeshComp, Animation);

    //
    ALCU_PlayerCharacter* player = Cast<ALCU_PlayerCharacter>(MeshComp->GetOwner());
    if(player)
    {
        AHHR_Gun* gun = Cast<AHHR_Gun>(player->GetItem());
        if(gun)
        {
            gun->Attack();
        }
    }
}
