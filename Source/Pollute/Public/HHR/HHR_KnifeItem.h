// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HHR/HHR_WeaponItem.h"
#include "HHR_KnifeItem.generated.h"

/**
 * 
 */
UCLASS()
class POLLUTE_API AHHR_KnifeItem : public AHHR_WeaponItem
{
	GENERATED_BODY()

public:
	virtual void Attack() override;
	
};
