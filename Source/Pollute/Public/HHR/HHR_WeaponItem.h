// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HHR/HHR_Item.h"
#include "HHR_WeaponItem.generated.h"

/**
 * 
 */
UCLASS()
class POLLUTE_API AHHR_WeaponItem : public AHHR_Item
{
	GENERATED_BODY()

// 공격 함수
public:
	virtual void Attack();
	
};
