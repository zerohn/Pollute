// Fill out your copyright notice in the Description page of Project Settings.


#include "Pollute/LCU/Public/TestLCU.h"

#include "Engine/Engine.h"


// Sets default values
ATestLCU::ATestLCU()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ATestLCU::BeginPlay()
{
	Super::BeginPlay();
	if(GEngine)

	P_LOG(PolluteLog, Log, TEXT("3333"));
	P_SCREEN(10.f, FColor::Blue, TEXT("TEST"));
}

// Called every frame
void ATestLCU::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

