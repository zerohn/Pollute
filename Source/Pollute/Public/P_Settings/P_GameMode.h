// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "P_GameMode.generated.h"

UCLASS()
class POLLUTE_API AP_GameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	AP_GameMode();

public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	
};
