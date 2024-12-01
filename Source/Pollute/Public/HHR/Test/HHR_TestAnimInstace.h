// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Pollute/Public/LCU/LCU_Properties/LCU_Property.h"
#include "HHR_TestAnimInstace.generated.h"

/**
 * 
 */
UCLASS()
class POLLUTE_API UHHR_TestAnimInstace : public UAnimInstance
{
	GENERATED_BODY()
public:
	virtual void NativeUpdateAnimation(float DeltaTime) override;

	virtual void NativeInitializeAnimation() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	class AHHR_TestPlayerCharacter* HHR_player;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float GroundSpeed;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	EGender Gender;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	bool bIsMove;
	
	
};
