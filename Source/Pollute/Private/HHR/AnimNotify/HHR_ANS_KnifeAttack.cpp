// Fill out your copyright notice in the Description page of Project Settings.


#include "HHR/AnimNotify/HHR_ANS_KnifeAttack.h"

#include "Components/SkeletalMeshComponent.h"
#include "HHR/HHR_KnifeItem.h"
#include "HHR/Test/HHR_TestPlayerCharacter.h"

void UHHR_ANS_KnifeAttack::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                      float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);
	
	// Character가 가지고 있는 아이템 가져오기
	// knife Attack 호출
	AHHR_TestPlayerCharacter* player = Cast<AHHR_TestPlayerCharacter>(MeshComp->GetOwner());
	if(player)
	{
		AHHR_KnifeItem* knife = Cast<AHHR_KnifeItem>(player->GetItem());
		if(knife)
		{
			knife->Attack();
		}
	}
	
}
