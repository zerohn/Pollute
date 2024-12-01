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

protected:
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime) override;

	
};
