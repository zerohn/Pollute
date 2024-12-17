#include "NSK/NSK_LadderInstallPoint.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Actor.h"
#include "LCU/Player/LCU_PlayerCharacter.h"
#include <Kismet/GameplayStatics.h>
#include "NSK/NSK_Ladder.h"

ANSK_LadderInstallPoint::ANSK_LadderInstallPoint()
{
	PrimaryActorTick.bCanEverTick = false;

    // 콜리전 컴포넌트
    InstallCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("InstallCollision"));
    RootComponent = InstallCollision;

    InstallCollision->SetBoxExtent(FVector(50.f, 50.f, 50.f));
    InstallCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    InstallCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
    InstallCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

    // 설치 지점 표시용 Mesh
    InstallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("InstallMesh"));
    InstallMesh->SetupAttachment(RootComponent);
    InstallMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ANSK_LadderInstallPoint::BeginPlay()
{
	Super::BeginPlay();
    InstallCollision->OnComponentBeginOverlap.AddDynamic(this, &ANSK_LadderInstallPoint::OnOverlapBegin);
    InstallCollision->OnComponentEndOverlap.AddDynamic(this, &ANSK_LadderInstallPoint::OnOverlapEnd);
}

void ANSK_LadderInstallPoint::OnOverlapBegin(
    UPrimitiveComponent* OverlappedComp,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult)
{
    if (ALCU_PlayerCharacter* Player = Cast<ALCU_PlayerCharacter>(OtherActor))
    {
        P_LOG(PolluteLog, Warning, TEXT("여기에 사다리를 설치할 수 있습니다"));

        bPlayerIsNear = true;

        // 설치 가능 상태 - 초록색 머티리얼로 변경
        bCanInstallLadder = true;

        if (InstallMesh && GreenMaterial)
        {
             InstallMesh->SetMaterial(0, GreenMaterial);
        }
    }
}

void ANSK_LadderInstallPoint::OnOverlapEnd(
    UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex)
{
    if (ALCU_PlayerCharacter* Player = Cast<ALCU_PlayerCharacter>(OtherActor))
    {
        P_LOG(PolluteLog, Warning, TEXT("설치 지점을 벗어났습니다"));

        bPlayerIsNear = false;

        // 설치 불가능 상태 - 빨간색 머티리얼로 변경
        bCanInstallLadder = false;

        if (InstallMesh && RedMaterial)
        {
            InstallMesh->SetMaterial(0, RedMaterial);
        }
    }
}

// 사다리 설치 함수
void ANSK_LadderInstallPoint::InstallLadder(ALCU_PlayerCharacter* Player)
{
    if (bPlayerIsNear)
    {
        // FActorSpawnParameters 정의 및 설정
        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = Player; // 플레이어를 소유자로 설정 (옵션)
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn; // 충돌 설정 (액터가 겹치지 않도록)

        // 사다리 액터를 설치 지점에 스폰
        FVector SpawnLocation = InstallMesh->GetComponentLocation(); // 설치 지점 위치
        FRotator SpawnRotation = InstallMesh->GetComponentRotation(); // 설치 지점의 회전

        // 실제로 액터를 스폰
        AActor* SpawnedActor = GetWorld()->SpawnActor<ANSK_Ladder>(ANSK_Ladder::StaticClass(), SpawnLocation, SpawnRotation, SpawnParams);

        if (SpawnedActor)
        {
            P_LOG(PolluteLog, Warning, TEXT("사다리 생성 성공!"));
        }
    }
}

// 키로 상호작용 시 사다리 설치
void ANSK_LadderInstallPoint::SetupInteraction()
{
    // 플레이어가 설치 지점에 있을 때 상호작용 확인 (예: 플레이어가 Overlap 상태일 때)
    if (bPlayerIsNear)
    {
        ALCU_PlayerCharacter* Player = Cast<ALCU_PlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));  // 플레이어 가져오기
        if (Player && Player->IsInputKeyDown(EKeys::LeftMouseButton))  // 마우스 왼쪽 클릭
        {
            InstallLadder(Player); // 사다리 설치 함수 호출
        }
    }
}
