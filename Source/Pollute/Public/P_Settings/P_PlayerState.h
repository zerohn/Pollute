// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "P_PlayerState.generated.h"

/**
 * 
 */
UCLASS()
class POLLUTE_API AP_PlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	AP_PlayerState(const FObjectInitializer& ObjectInitializer);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player State")
	
	
};
