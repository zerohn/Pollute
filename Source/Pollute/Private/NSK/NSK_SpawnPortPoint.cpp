#include "NSK/NSK_SpawnPortPoint.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include <LCU/Player/LCU_PlayerCharacter.h>
#include <LCU/Player/LCU_PlayerController.h>

ANSK_SpawnPortPoint::ANSK_SpawnPortPoint()
{
    SpawnPortMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SpawnPortMesh"));
    RootComponent = SpawnPortMesh;

    // 콜리전 박스 초기화
    OverlapBox = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapBox"));
    OverlapBox->SetupAttachment(RootComponent);
    OverlapBox->SetBoxExtent(FVector(50.f, 50.f, 50.f));
    OverlapBox->SetCollisionProfileName(TEXT("Trigger"));

    // 오버랩 이벤트 바인딩
    OverlapBox->OnComponentBeginOverlap.AddDynamic(this, &ANSK_SpawnPortPoint::OnOverlapBegin);

    // 이 객체를 네트워크에서 복제하도록 설정
    // 로컬 테스트에서 정상 작동해도 멀티 환경에서 필요할 수 있음
    bReplicates = true;

    bHasOverlapped = false;
}

void ANSK_SpawnPortPoint::BeginPlay()
{
    Super::BeginPlay();

    // 시작 시 실행 테스트
    /*if (HasAuthority())
    {
        RandomSpawnPortPoint(this);
        Multicast_SpawnPortSelected_Implementation(this);
    }*/
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

void ANSK_SpawnPortPoint::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (bHasOverlapped)
    {
        return;
    }

    if (!OtherActor && OtherActor == this)
    {
        return;
    }

    // OtherActor가 플레이어 캐릭터인지 확인
    ACharacter* OverlapCharacter = Cast<ALCU_PlayerCharacter>(OtherActor);
    if (OverlapCharacter)
    {
        P_LOG(PolluteLog, Warning, TEXT(" 탈출자 : %s"), *OverlapCharacter->GetName());

        // 서버에서만 처리
        if (HasAuthority())
        {
            bHasOverlapped = true;

            P_LOG(PolluteLog, Warning, TEXT("탈출!!"));
        }

        if (ALCU_PlayerController* PlayerController = Cast<ALCU_PlayerController>(OverlapCharacter->GetController()))
        {
            P_LOG(PolluteLog, Warning, TEXT("플레이어 컨트롤러 : %s"), *PlayerController->GetName());

            if (PlayerController->IsLocalController())
            {
                PlayerController->ServerRPC_ChangeToSpector();
            }

            // 모두 스펙터 모드 시 -> 게임 로비로 이동
        }
    }
}
