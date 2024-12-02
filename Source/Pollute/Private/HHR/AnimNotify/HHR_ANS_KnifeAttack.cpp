// Fill out your copyright notice in the Description page of Project Settings.


#include "HHR/AnimNotify/HHR_ANS_KnifeAttack.h"

#include "Components/SkeletalMeshComponent.h"
#include "HHR/HHR_KnifeItem.h"
#include "LCU/Player/LCU_PlayerCharacter.h"

void UHHR_ANS_KnifeAttack::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
    float TotalDuration)
{

    // PrevEnd 저장
    ALCU_PlayerCharacter* player = Cast<ALCU_PlayerCharacter>(MeshComp->GetOwner());
    if(player)
    {
        AHHR_KnifeItem* knife = Cast<AHHR_KnifeItem>(player->GetItem());
        if(knife)
        {
            knife->SetPrevPos();
        }
    }
}

void UHHR_ANS_KnifeAttack::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                      float FrameDeltaTime)
{
	
	// Character가 가지고 있는 아이템 가져오기
	// knife Attack 호출

    ALCU_PlayerCharacter* player = Cast<ALCU_PlayerCharacter>(MeshComp->GetOwner());
    if(player)
    {
        AHHR_KnifeItem* knife = Cast<AHHR_KnifeItem>(player->GetItem());
        if(knife)
        {
            knife->Attack();
        }
    }
	
}


void UHHR_ANS_KnifeAttack::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{

    // Destory 호출
    ALCU_PlayerCharacter* player = Cast<ALCU_PlayerCharacter>(MeshComp->GetOwner());
    if(player)
    {
        AHHR_KnifeItem* knife = Cast<AHHR_KnifeItem>(player->GetItem());
        if(knife)
        {
            knife->DestoryDelay();
        }
        else
        {
            P_LOG(PolluteLog, Warning, TEXT("knife 없어 왜"));
        }
    }

}
