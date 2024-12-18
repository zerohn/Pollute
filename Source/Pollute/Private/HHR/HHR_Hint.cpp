// Fill out your copyright notice in the Description page of Project Settings.


#include "HHR/HHR_Hint.h"

#include "Components/StaticMeshComponent.h"
#include "Net/UnrealNetwork.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
AHHR_Hint::AHHR_Hint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    FrameMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Frame Staticmesh"));
    SetRootComponent(FrameMeshComp);

    PictureMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Picture Staticmesh"));
    PictureMeshComp->SetupAttachment(FrameMeshComp);

    // 힌트 생성도 replicated 되어야 함
    bReplicates = true;

}

// Called when the game starts or when spawned
void AHHR_Hint::BeginPlay()
{
	Super::BeginPlay();
	
}

void AHHR_Hint::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AHHR_Hint, bInvisiblePicture);
}


// Called every frame
void AHHR_Hint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AHHR_Hint::OnRep_InvisiblePicture()
{
    InvisiblePicture();
}

void AHHR_Hint::InvisiblePicture()
{
    if(PictureMeshComp)
    {
        PictureMeshComp->SetVisibility(false);
    }
}

