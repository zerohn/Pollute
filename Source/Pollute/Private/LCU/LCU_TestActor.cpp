// Fill out your copyright notice in the Description page of Project Settings.


#include "LCU/LCU_TestActor.h"

#include "Engine/Engine.h"


// Sets default values
ALCU_TestActor::ALCU_TestActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ALCU_TestActor::BeginPlay()
{
	Super::BeginPlay();
	
	P_SCREEN(5.f, FColor::Black, TEXT("ddd"));

	P_LOG(PolluteLog, Log, TEXT("asdasd"));

}

// Called every frame
void ALCU_TestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

