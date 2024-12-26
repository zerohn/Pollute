// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "LCU_MonsterAnim.generated.h"

/**
 * 
 */
UCLASS()
class POLLUTE_API ULCU_MonsterAnim : public UAnimInstance
{
    GENERATED_BODY()
public:
    virtual void NativeUpdateAnimation(float DeltaTime) override;

    virtual void NativeInitializeAnimation() override;

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
    class ALCU_MonsterCharacter* LCU_monster;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float GroundSpeed;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
    float Direction;
        
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
    bool bIsMove;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
    float P_Vertical;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
    float P_Horizontal;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
    bool bDeath;
};
