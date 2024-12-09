// Fill out your copyright notice in the Description page of Project Settings.


#include "LCU/Animation/AN_MonsterHit.h"

#include "Components/SkeletalMeshComponent.h"
#include "LCU/Player/LCU_MonsterCharacter.h"

void UAN_MonsterHit::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    //Super::Notify(MeshComp, Animation);

    
}

void UAN_MonsterHit::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
    const FAnimNotifyEventReference& EventReference)
{
    //Super::Notify(MeshComp, Animation, EventReference);

    ALCU_MonsterCharacter* MonChar = Cast<ALCU_MonsterCharacter>(MeshComp->GetOwner());
    if (MonChar)
    {
        MonChar->OnNotifyAttack();
    }
}
