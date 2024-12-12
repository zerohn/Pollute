// Fill out your copyright notice in the Description page of Project Settings.


#include "HHR/HHR_ItemSpawnPoint.h"


// Sets default values
AHHR_ItemSpawnPoint::AHHR_ItemSpawnPoint()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AHHR_ItemSpawnPoint::BeginPlay()
{
    Super::BeginPlay();
    
}

// Called every frame
void AHHR_ItemSpawnPoint::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

