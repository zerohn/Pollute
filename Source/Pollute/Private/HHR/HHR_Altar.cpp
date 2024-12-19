// Fill out your copyright notice in the Description page of Project Settings.


#include "HHR/HHR_Altar.h"
#include "HHR/HHR_Item.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Debugging/SlateDebugging.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "LCU/Player/LCU_PlayerCharacter.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
AHHR_Altar::AHHR_Altar()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    // * Components 생성
    SphereCollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
    SetRootComponent(SphereCollisionComp);
    SphereCollisionComp->InitSphereRadius(115.f);
    // collision 세팅
    // collision 함수 바인딩
    SphereCollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AHHR_Altar::OnComponentBeginOverlap);
    SphereCollisionComp->OnComponentEndOverlap.AddDynamic(this, &AHHR_Altar::OnComponentEndOverlap);

    // mesh 설정
    AltarStaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AltarStaticMesh"));
    AltarStaticMeshComp->SetupAttachment(SphereCollisionComp);
    AltarStaticMeshComp->AddRelativeLocation(FVector(24.f, -94.f, 28.f));
    static ConstructorHelpers::FObjectFinder<UStaticMesh> altarMeshAsset(TEXT("/Script/Engine.StaticMesh'/Game/HHR/Item/ItemMesh/SM_Altar.SM_Altar'"));
    if(altarMeshAsset.Succeeded())
    {
        AltarStaticMeshComp->SetStaticMesh(altarMeshAsset.Object);
    }

    

}

// Called when the game starts or when spawned
void AHHR_Altar::BeginPlay()
{
	Super::BeginPlay();

    // PlayerCharacter에 바인딩
    ALCU_PlayerCharacter* player =  Cast<ALCU_PlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
    if(player)
    {
        player->OnAttachItemOnAltar.BindDynamic(this, &AHHR_Altar::OnAttachItem);
    }

    // 변수 세팅
    MaxItemCnt = ItemAttachPos.Num();

}

// Called every frame
void AHHR_Altar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AHHR_Altar::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                         UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    // TODO : 종속성..
    // Player의 bNearByAltar 변수 변경
    // TODO : Altar에 아이템이 다 차 있으면 굳이 안 변경해줘도 될듯 
    ALCU_PlayerCharacter* player = Cast<ALCU_PlayerCharacter>(OtherActor);
    if(player)
    {
        player->SetNearByAltar(true);
    }
}

void AHHR_Altar::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    // Player의 bNearByAltar 변수 변경
    ALCU_PlayerCharacter* player = Cast<ALCU_PlayerCharacter>(OtherActor);
    if(player)
    {
        player->SetNearByAltar(false);
    }
}

void AHHR_Altar::OnAttachItem(AHHR_Item* Item)
{
    // Item Attach
    // TODO : Item 부착 동기화 필요

    if(CurrentItemCnt < MaxItemCnt)
    {
    

        
        // TODO : Attach 해주면 그 Item의 collision 어떻게 해줄지 정해줘야 함 
    }
    else
    {
        // max일때 NearByAltar를 변경 안해주면 호출될 일 없긴 함 
        return;
    }
}



