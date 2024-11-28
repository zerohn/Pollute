// Fill out your copyright notice in the Description page of Project Settings.


#include "LCU/Player/LCU_PlayerController.h"

ALCU_PlayerController::ALCU_PlayerController()
{
}

void ALCU_PlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void ALCU_PlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void ALCU_PlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
