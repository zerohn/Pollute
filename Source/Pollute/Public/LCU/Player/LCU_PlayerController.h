// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LCU/LCU_Properties/LCU_Property.h"
#include "LCU_PlayerController.generated.h"

/**
 * 
 */
UCLASS()
class POLLUTE_API ALCU_PlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	ALCU_PlayerController();




public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player State")
	EPlayerMode PlayerMode = EPlayerMode::Human;
};
