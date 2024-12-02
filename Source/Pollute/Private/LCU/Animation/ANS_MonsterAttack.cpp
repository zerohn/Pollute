// Fill out your copyright notice in the Description page of Project Settings.


#include "LCU/Animation/ANS_MonsterAttack.h"

#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "LCU/Player/LCU_MonsterCharacter.h"

void UANS_MonsterAttack::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                     float TotalDuration)
{
    Super::NotifyBegin(MeshComp, Animation, TotalDuration);

    MonChar = Cast<ALCU_MonsterCharacter>(MeshComp->GetOwner());
}

void UANS_MonsterAttack::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                    float FrameDeltaTime)
{
    Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);
    
    if (!MonChar) return;
    if(MonChar->IsLocallyControlled())
    {
        MonChar->ServerRPC_OnNotifyAttack();
    }
}

void UANS_MonsterAttack::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
    const FAnimNotifyEventReference& EventReference)
{
    Super::NotifyEnd(MeshComp, Animation, EventReference);
    if(!MonChar) return;
    MonChar->bCanAttack = true;
}
