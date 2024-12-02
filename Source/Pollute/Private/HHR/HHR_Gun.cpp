// Fill out your copyright notice in the Description page of Project Settings.


#include "HHR/HHR_Gun.h"

#include "Components/StaticMeshComponent.h"
#include "Engine/Engine.h"

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
        // TODO : 몬스터 생성후 ApplyDamage() 로 수정
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, "Hit");
    }

    if (isHit)
    {
        DrawDebugLine(GetWorld(), start, end, FColor::Red, true, 0.25);
    }
    else
    {
        DrawDebugLine(GetWorld(), start, end, FColor::Blue, true, 0.25);
    }

    bIsUsed = true;
    DestoryDelay();
    
}
