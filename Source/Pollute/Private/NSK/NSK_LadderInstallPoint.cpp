#include "NSK/NSK_LadderInstallPoint.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "LCU/Player/LCU_PlayerCharacter.h"
#include <Kismet/GameplayStatics.h>
#include "Engine/World.h"
#include "NSK/NSK_Ladder.h"
#include <Net/UnrealNetwork.h>

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

    bReplicates = true; // 이 액터가 복제를 지원하도록 설정
}

void ANSK_LadderInstallPoint::BeginPlay()
{
	Super::BeginPlay();
    InstallCollision->OnComponentBeginOverlap.AddDynamic(this, &ANSK_LadderInstallPoint::OnOverlapBegin);
    InstallCollision->OnComponentEndOverlap.AddDynamic(this, &ANSK_LadderInstallPoint::OnOverlapEnd);
    UWorld* world = GetWorld();
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
    if (bPlayerIsNear && !bLadderInstalled) // 설치되지 않은 상태에서 설치 가능
    {
        UClass* BP_LadderClass = LoadObject<UClass>(nullptr, TEXT("Blueprint'/Game/NSK/BP/BP_NSK_Ladder.BP_NSK_Ladder_C'"));

        if (BP_LadderClass)
        {
            // FActorSpawnParameters 정의 및 설정
            FActorSpawnParameters SpawnParams;
            SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn; // 충돌 설정 (액터가 겹치지 않도록)

            // 사다리 액터를 설치 지점에 스폰
            FVector SpawnLocation = InstallMesh->GetComponentLocation(); // 설치 지점 위치
            FRotator SpawnRotation = InstallMesh->GetComponentRotation(); // 설치 지점의 회전

            // 실제로 액터를 스폰
            AActor* SpawnedActor = GetWorld()->SpawnActor<ANSK_Ladder>(BP_LadderClass, SpawnLocation, SpawnRotation, SpawnParams);

            // 가져오는게 잘못됨 메쉬가 빠짐 -> 원인 : C++ 자체를 가지고 오고 있어서 Mesh가 빠졌는데 BP를 가져와서 생성해줘야함!!

            if (SpawnedActor)
            {
                P_LOG(PolluteLog, Warning, TEXT("사다리 생성 성공!"));

                /*if (Player)
                {
                    Player->InstallAndDeleteItem();
                }*/

                ANSK_Ladder* Ladder = Cast<ANSK_Ladder>(SpawnedActor);
                if (Ladder)
                {
                    Ladder->bIsInstalled = true; // 설치 상태
                    Ladder->EnableCollisionAfterInstall(); // 설치된 사다리의 충돌 활성화
                    //P_LOG(PolluteLog, Warning, TEXT("사다리 설치 성공"));
                }

                // 설치 후 인스톨 포인트 삭제
                //Destroy();

                // 설치 후 더 이상 설치 지점 사용 불가능
                //bPlayerIsNear = false;

                bLadderInstalled = true;
                MulticastInstallLadder(Player);
            }
            else
            {
                P_LOG(PolluteLog, Warning, TEXT("사다리 생성 실패!"));
            }
        }
    }
}

// 키로 상호작용 시 사다리 설치
void ANSK_LadderInstallPoint::SetupInteraction()
{
    if (bPlayerIsNear)
    {
        ALCU_PlayerCharacter* Player = Cast<ALCU_PlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
        if (Player)
        {
            InstallLadder(Player);
        }
        else
        {
            P_LOG(PolluteLog, Warning, TEXT("플레이어가 존재하지 않는다"));
        }
    }
    else
    {
        P_LOG(PolluteLog, Warning, TEXT("플레이어가 인스톨 포인트 근처에 없습니다"));
    }
}

void ANSK_LadderInstallPoint::MulticastInstallLadder_Implementation(ALCU_PlayerCharacter* Player)
{
    if (Player)
    {
        Player->InstallAndDeleteItem(); // 플레이어의 아이템 제거
        Destroy(); // 설치 지점 제거
        P_LOG(PolluteLog, Warning, TEXT("사다리 설치 상태가 모든 클라이언트에 동기화되었습니다."));
    }
}

void ANSK_LadderInstallPoint::OnRep_LadderInstalled()
{
    if (bLadderInstalled)
    {
        P_LOG(PolluteLog, Warning, TEXT("사다리 설치 상태가 클라이언트에서 업데이트되었습니다."));

        // 추가 클라 동작 로직 ex) UI 업데이트
    }
    else
    {
        P_LOG(PolluteLog, Warning, TEXT("사다리가 제거되었습니다! 클라이언트에서 동기화됨."));
    }
}

void ANSK_LadderInstallPoint::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    // bLadderInstalled를 복제
    DOREPLIFETIME(ANSK_LadderInstallPoint, bLadderInstalled);
}