// Fill out your copyright notice in the Description page of Project Settings.


#include "HHR/HHR_KnifeItem.h"

#include "Components/StaticMeshComponent.h"
#include "Engine/DamageEvents.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "LCU/Player/LCU_MonsterCharacter.h"

AHHR_KnifeItem::AHHR_KnifeItem()
{
    WeaponType = EWeaponType::Knife;
}

void AHHR_KnifeItem::Attack()
{
	// Notify를 통해서 호출됨
	// 매 틱마다 호출됨
	
	// LineTrace를 소켓의 맨 앞부분 소켓과 뒷부분 소켓의 위치에 Line그려서 체크
	// GameTracechannel2
	FVector start = ItemMehsComp->GetSocketLocation("BladeStart");
	FVector end = ItemMehsComp->GetSocketLocation("BladeEnd");

	FCollisionQueryParams params;
	params.AddIgnoredActor(this);

	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_GameTraceChannel2);

	FHitResult hitResult;

	//bool isHit = GetWorld()->LineTraceSingleByChannel(hitResult, start, end, ECollisionChannel::ECC_GameTraceChannel2, params);
	bool isHit = GetWorld()->LineTraceSingleByObjectType(
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

	// blade의 괴적 적용을 위한 LineTrace
    // 이전 EndPoint를 기억한 후에 LineTrace
    FVector startBlade = PrevEndPos;
    FVector endBlade = ItemMehsComp->GetSocketLocation("BladeEnd");

    FCollisionQueryParams paramsB;
    paramsB.AddIgnoredActor(this);

    FCollisionObjectQueryParams ObjectQueryParamsB;
    ObjectQueryParamsB.AddObjectTypesToQuery(ECollisionChannel::ECC_GameTraceChannel2);

    FHitResult hitResultB;

    //bool isHit = GetWorld()->LineTraceSingleByChannel(hitResult, start, end, ECollisionChannel::ECC_GameTraceChannel2, params);
    bool isHitBlade = GetWorld()->LineTraceSingleByObjectType(
        hitResultB,
        startBlade,
        endBlade,
        ObjectQueryParamsB,
        paramsB
    );

    
    if (isHitBlade)
    {
        // 충돌시 Damage 적용
        if (ALCU_MonsterCharacter* mon = Cast<ALCU_MonsterCharacter>(hitResult.GetActor()))
        {
            hitResult.GetActor()->TakeDamage(1.f, FDamageEvent(), GetInstigatorController(), this);
        }
    }

    
	
	// DrawLineDebug
	/*if (isHit)
	{
		DrawDebugLine(GetWorld(), start, end, FColor::Red, true, 0.25);
	}
	else
	{
		DrawDebugLine(GetWorld(), start, end, FColor::Blue, true, 0.25);
	}

    // DrawLineDebug
    if (isHitBlade)
    {
        DrawDebugLine(GetWorld(), startBlade, endBlade, FColor::Red, true, 0.25);
    }
    else
    {
        DrawDebugLine(GetWorld(), startBlade, endBlade, FColor::Blue, true, 0.25);
    }*/

    // prevEndPos 업뎃
    PrevEndPos = endBlade;
    bIsUsed = true;
	
}

void AHHR_KnifeItem::SetPrevPos()
{
    PrevEndPos =ItemMehsComp->GetSocketLocation("BladeEnd");
}
