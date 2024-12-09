// Fill out your copyright notice in the Description page of Project Settings.


#include "HHR/HHR_Gun.h"

#include "Components/StaticMeshComponent.h"
#include "Engine/DamageEvents.h"
#include "Engine/Engine.h"
#include "LCU/Player/LCU_MonsterCharacter.h"

AHHR_Gun::AHHR_Gun()
{
    // Weapon type 설정
    WeaponType = EWeaponType::TaserGun;
    
}

void AHHR_Gun::Attack()
{
    Super::Attack();


    // LineTrace로 총 쏘기
    FVector start = ItemMehsComp->GetSocketLocation("Fire");
    FVector end = start + GetActorRightVector() * Range;
    
    FCollisionQueryParams params;
    params.AddIgnoredActor(this);

    FCollisionObjectQueryParams ObjectQueryParams;
    ObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_GameTraceChannel2);

    FHitResult hitResult;

    //bool isHit = GetWorld()->LineTraceSingleByChannel(hitResult, start, end, ECollisionChannel::ECC_GameTraceChannel2, params);
    bool isHit =
        GetWorld()->LineTraceSingleByObjectType(
        hitResult,
        start,
        end,
        ObjectQueryParams,
        params
    );
    
    if (isHit)
    {
        // 충돌시 Damage 적용
        if (ALCU_MonsterCharacter* mon = Cast<ALCU_MonsterCharacter>(hitResult.GetActor()))
        {
            hitResult.GetActor()->TakeDamage(1.f, FDamageEvent(), GetInstigatorController(), this);
        }
    }

    /*if (isHit)
    {
        DrawDebugLine(GetWorld(), start, end, FColor::Red, true, 0.25);
    }
    else
    {
        DrawDebugLine(GetWorld(), start, end, FColor::Blue, true, 0.25);
    }*/

    bIsUsed = true;
    DestoryDelay();
    
}
