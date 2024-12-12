#include "NSK/NSK_SpawnPortPoint.h"
#include "Kismet/GameplayStatics.h"

ANSK_SpawnPortPoint::ANSK_SpawnPortPoint()
{
    SpawnPortMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SpawnPortMesh"));
    RootComponent = SpawnPortMesh;

}

void ANSK_SpawnPortPoint::BeginPlay()
{
    Super::BeginPlay();

    // 시작 시 실행 테스트
    if (HasAuthority())
    {
        RandomSpawnPortPoint(this);
        Multicast_SpawnPortSelected_Implementation(this);
    }
}

void ANSK_SpawnPortPoint::RandomSpawnPortPoint(UObject* WorldContextObject)
{
    if (!WorldContextObject || !WorldContextObject->GetWorld())
    {
        return;
    }

    // 서버에서만 실행 체크
    if (!HasAuthority())
    {
        return;
    }

    TArray<AActor*> AllSpawnPoints;
    UGameplayStatics::GetAllActorsOfClass(this, ANSK_SpawnPortPoint::StaticClass(), AllSpawnPoints);

    if (AllSpawnPoints.Num() > 0)
    {
        // 랜덤으로 1개 선택
        int32 RandomIndex = FMath::RandRange(0, AllSpawnPoints.Num() - 1);
        ANSK_SpawnPortPoint* SelectedSpawnPoint = Cast<ANSK_SpawnPortPoint>(AllSpawnPoints[RandomIndex]);

        for (AActor* Actor : AllSpawnPoints)
        {
            ANSK_SpawnPortPoint* SpawnPoint = Cast<ANSK_SpawnPortPoint>(Actor);
            if (SpawnPoint && SpawnPoint != SelectedSpawnPoint)
            {
                SpawnPoint->HideSpawnPortMesh();
                SpawnPoint->Destroy();
            }
        }

        if (SelectedSpawnPoint)
        {
            SelectedSpawnPoint->bSpawnPortIsUsed = true;

            // 서버에서 Multicast로 호출하여 클라에게 동기화
            Multicast_SpawnPortSelected(SelectedSpawnPoint);
        }
    }
}

void ANSK_SpawnPortPoint::HideSpawnPortMesh()
{
    if (SpawnPortMesh)
    {
        SpawnPortMesh->DestroyComponent();
    }
}

void ANSK_SpawnPortPoint::Multicast_SpawnPortSelected_Implementation(ANSK_SpawnPortPoint* SelectedSpawnPoint)
{
    if (SelectedSpawnPoint)
    {
        // 클라에서 선택된 포트의 표시를 업데이트
        SelectedSpawnPoint->bSpawnPortIsUsed = true;

        // 메시 표시
        if (SelectedSpawnPoint->SpawnPortMesh)
        {
            SelectedSpawnPoint->SpawnPortMesh->SetVisibility(true);
        }
    }
}
