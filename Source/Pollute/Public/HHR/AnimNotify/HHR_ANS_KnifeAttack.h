// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "HHR_ANS_KnifeAttack.generated.h"

/**
 * 
 */
UCLASS()
class POLLUTE_API UHHR_ANS_KnifeAttack : public UAnimNotifyState
{
	GENERATED_BODY()

private:
    // Animation을 play 하는 character
    /*class ALCU_PlayerCharacter* Player;
    // Player가 가지고 있는 아이템
    class AHHR_KnifeItem* Knife;*/
    

protected:
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime) override;

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;

    virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
