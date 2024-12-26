// Fill out your copyright notice in the Description page of Project Settings.


#include "LCU/Animation/AN_MonsterDie.h"

#include "Components/SkeletalMeshComponent.h"
#include "LCU/Player/LCU_MonsterCharacter.h"

void UAN_MonsterDie::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    AActor* Own = MeshComp->GetOwner();
    if (!Own)
        return;

    ALCU_MonsterCharacter* MyCharacter = Cast<ALCU_MonsterCharacter>(Own);
    if (!MyCharacter)
        return;

    MyCharacter->DieAndChandChar();
}
