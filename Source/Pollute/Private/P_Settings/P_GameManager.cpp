// Fill out your copyright notice in the Description page of Project Settings.


#include "P_Settings/P_GameManager.h"

#include "TimerManager.h"
#include "Engine/World.h"
#include "Kismet/KismetMathLibrary.h"
#include "LCU/InteractActors/LCU_Curse.h"


// Sets default values
AP_GameManager::AP_GameManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AP_GameManager::BeginPlay()
{
	Super::BeginPlay();

	
}

// Called every frame
void AP_GameManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
