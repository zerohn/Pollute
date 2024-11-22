// Fill out your copyright notice in the Description page of Project Settings.


#include "LCU/Player/LCU_Human.h"


// Sets default values
ALCU_Human::ALCU_Human()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ALCU_Human::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALCU_Human::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ALCU_Human::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

