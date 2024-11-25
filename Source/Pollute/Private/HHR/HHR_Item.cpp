// Fill out your copyright notice in the Description page of Project Settings.


#include "HHR/HHR_Item.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Character.h"
#include "HHR/HHR_TestPlayerHUD.h"

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


	// Overlap 델리게이트 바인딩
	ItemSphereComp->OnComponentBeginOverlap.AddDynamic(this, &AHHR_Item::OnComponentBeginOverlap);
	ItemSphereComp->OnComponentEndOverlap.AddDynamic(this, &AHHR_Item::OnComponentEndOverlap);
	

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
	
	if(Cast<ACharacter>(OtherActor))
	{
		// Character이면 UI 띄우기
		// 임시로 직접 가져와서 띄우기
		P_LOG(PolluteLog, Warning, TEXT("UI 띄우기"));
		if(TestPlayerHUD)
		{
			// TODO : 수정
			P_LOG(PolluteLog, Warning, TEXT("Player HUD 있음"));
			UHHR_TestPlayerHUD* PlayerHUD = Cast<UHHR_TestPlayerHUD>(TestPlayerHUD);
			if(PlayerHUD)
			{
				PlayerHUD->SetItemDialogText(ItemData.ItemUIName);
				PlayerHUD->SetItemDialogVisibility(true);
			}
		}
	}
	
}

void AHHR_Item::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(Cast<ACharacter>(OtherActor))
	{
		P_LOG(PolluteLog, Warning, TEXT("UI 안띄우기"));
		if(TestPlayerHUD)
		{
			UHHR_TestPlayerHUD* PlayerHUD = Cast<UHHR_TestPlayerHUD>(TestPlayerHUD);
			if(PlayerHUD)
			{
				PlayerHUD->SetItemDialogVisibility(false);
			}
		}
	}
}

