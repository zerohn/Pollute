// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Pollute/Public/LCU/LCU_Properties/LCU_Property.h"
#include "LCU_HumanAnim.generated.h"

/**
 * 
 */
UCLASS()
class POLLUTE_API ULCU_HumanAnim : public UAnimInstance
{
	GENERATED_BODY()
public:
	virtual void NativeUpdateAnimation(float DeltaTime) override;

	virtual void NativeInitializeAnimation() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	class ALCU_PlayerCharacter* LCU_player;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float GroundSpeed;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	EGender Gender;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	bool bIsMove;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
    bool bIsGunInHand;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
    bool bIsItemInHand;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
    TObjectPtr<class AHHR_Item> ItemInHand;
	
};
