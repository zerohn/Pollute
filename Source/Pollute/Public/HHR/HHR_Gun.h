// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HHR/HHR_WeaponItem.h"
#include "HHR_Gun.generated.h"

/**
 * 
 */
UCLASS()
class POLLUTE_API AHHR_Gun : public AHHR_WeaponItem
{
	GENERATED_BODY()

protected:
    UPROPERTY(EditAnywhere, Category = "Setting")
    float Range = 1000.f;


public:
    void Attack() override;
	
};
