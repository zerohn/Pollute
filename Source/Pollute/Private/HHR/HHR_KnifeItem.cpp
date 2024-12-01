// Fill out your copyright notice in the Description page of Project Settings.


#include "HHR/HHR_KnifeItem.h"

#include "Components/StaticMeshComponent.h"
#include "Engine/Engine.h"
#include "Engine/World.h"

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

	FHitResult hitResult;

	bool isHit = GetWorld()->LineTraceSingleByChannel(hitResult, start, end, ECollisionChannel::ECC_GameTraceChannel2, params);

	if (isHit)
	{
		// 충돌시 Damage 적용
		// TODO : 몬스터 생성후 ApplyDamage() 로 수정
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, "Hit");
	}

	// blade의 괴적 적용을 위한 LineTrace
	
	// DrawLineDebug
	if (isHit)
	{
		DrawDebugLine(GetWorld(), start, end, FColor::Red, true, 0.25);
	}
	else
	{
		DrawDebugLine(GetWorld(), start, end, FColor::Blue, true, 0.25);
	}
	
	
}
