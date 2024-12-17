#include "NSK/NSK_BuildActor.h"
#include "HHR/HHR_Item.h"
#include "Components/StaticMeshComponent.h"

ANSK_BuildActor::ANSK_BuildActor()
{
    PrimaryActorTick.bCanEverTick = true;

    bCanSpawn = false;
}

void ANSK_BuildActor::BeginPlay()
{
	Super::BeginPlay();
}

void ANSK_BuildActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    CheckSpawn();
}

void ANSK_BuildActor::CheckSpawn()
{
    if (!ItemMehsComp) return;

    // 설치 가능 여부 확인
    TArray<AActor*> OverlappingActors;
    ItemMehsComp->GetOverlappingActors(OverlappingActors);

    if (OverlappingActors.Num() > 0)
    {
        // 겹치는 액터가 있으면 설치 불가능 + 빨간색 머티리얼
        bCanSpawn = false;
        ItemMehsComp->SetMaterial(0, RedMaterial);
    }
    else
    {
        // 설치 가능 -> 초록색 머티리얼
        bCanSpawn = true;
        ItemMehsComp->SetMaterial(0, GreenMaterial);
    }
}