// Fill out your copyright notice in the Description page of Project Settings.


#include "HHR/HHR_Item.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Pawn.h"

// Sets default values
AHHR_Item::AHHR_Item()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Components 생성
	ItemSphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("ItemSphere"));
	ItemSphereComp->InitSphereRadius(50);
	RootComponent = ItemSphereComp;

	ItemMehsComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMehs"));
	ItemMehsComp->SetupAttachment(ItemSphereComp);
	

}

// Called when the game starts or when spawned
void AHHR_Item::BeginPlay()
{
	Super::BeginPlay();

	
}

// Called every frame
void AHHR_Item::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHHR_Item::SetItemData(const FItemData& data)
{
	if(ItemData.ItemID == -1)
	{
		// Item이 할당 안되어 있으면 할당
		// StaticMesh도 할당
		ItemData = data;
		ItemMehsComp->SetStaticMesh(ItemData.ItemMesh);
	}
}

void AHHR_Item::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Pawn(임시)이면 Equip UI 띄우기
	// 일단 임시 Log
	if(Cast<APawn>(OtherActor))
	{
		P_LOG(PolluteLog, Log, TEXT("Equip UI 띄우기"));
	}
	
}

void AHHR_Item::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// Pawn이면 Grap UI 띄우기
	if(Cast<APawn>(OtherActor))
	{
		P_LOG(PolluteLog, Log, TEXT("Equip UI 숨기기"));
	}
}

