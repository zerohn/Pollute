// Fill out your copyright notice in the Description page of Project Settings.


#include "HHR/HHR_Hint.h"

#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
AHHR_Hint::AHHR_Hint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    FrameMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Frame Staticmesh"));
    SetRootComponent(FrameMeshComp);
    

}

// Called when the game starts or when spawned
void AHHR_Hint::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHHR_Hint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

