#include "NSK/NSK_SpawnPortPoint.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include <LCU/Player/LCU_PlayerCharacter.h>
#include <LCU/Player/LCU_PlayerController.h>
#include "LevelSequence.h"
#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"
#include "UObject/ConstructorHelpers.h"

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
}

void ANSK_SpawnPortPoint::BeginPlay()
{
    Super::BeginPlay();

    // 시작 시 실행 테스트
    if (HasAuthority())
    {
        P_LOG(PolluteLog, Warning, TEXT("포트 삭제 실행 파악"));
        RandomSpawnPortPoint(this);
    }
}

void ANSK_SpawnPortPoint::RandomSpawnPortPoint(UObject* WorldContextObject)
{
    if (!WorldContextObject || !WorldContextObject->GetWorld())
    {
        return;
    }

    // 서버에서만 실행
    if (!HasAuthority())
    {
        return;
    }

    TArray<AActor*> AllSpawnPoints;
    UGameplayStatics::GetAllActorsOfClass(this, ANSK_SpawnPortPoint::StaticClass(), AllSpawnPoints);

    if (AllSpawnPoints.Num() > 0)
    {
        int32 RandomIndex = FMath::RandRange(0, AllSpawnPoints.Num() - 1);
        ANSK_SpawnPortPoint* SelectedSpawnPoint = Cast<ANSK_SpawnPortPoint>(AllSpawnPoints[RandomIndex]);

        for (AActor* Actor : AllSpawnPoints)
        {
            ANSK_SpawnPortPoint* SpawnPoint = Cast<ANSK_SpawnPortPoint>(Actor);
            if (SpawnPoint && SpawnPoint != SelectedSpawnPoint && !SpawnPoint->bIsPendingDestroy)
            {
                SpawnPoint->bIsPendingDestroy = true; // 삭제 예정 상태 설정
                SpawnPoint->Multicast_DestroyPort();
            }
        }

        if (SelectedSpawnPoint)
        {
            SelectedSpawnPoint->bSpawnPortIsUsed = true;
            Multicast_SpawnPortSelected(SelectedSpawnPoint);
        }
    }
}

void ANSK_SpawnPortPoint::Multicast_DestroyPort_Implementation()
{
    Destroy(); // 서버와 클라이언트 모두에서 삭제
}

void ANSK_SpawnPortPoint::HideSpawnPortMesh()
{
    if (!SpawnPortMesh || bIsPendingDestroy)
    {
        return;
    }
        SpawnPortMesh->DestroyComponent();
        bIsPendingDestroy = true; // 상태 플래그 설정
}

void ANSK_SpawnPortPoint::Multicast_SpawnPortSelected_Implementation(ANSK_SpawnPortPoint* SelectedSpawnPoint)
{
    if (SelectedSpawnPoint && !SelectedSpawnPoint->bIsPendingDestroy)
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
    if (bHasOverlapped || bIsPendingDestroy)
    {
        P_LOG(PolluteLog, Warning, TEXT("이미 오버랩 되었거나 파괴 대기 중"));
        return;
    }

    // 캐릭터 캐스팅
    ALCU_PlayerCharacter* OverlapCharacter = Cast<ALCU_PlayerCharacter>(OtherActor);
    if (!OverlapCharacter)
    {
        P_LOG(PolluteLog, Warning, TEXT("캐릭터 캐스팅 실패"));
        return;
    }

    // 서버에서 처리
    if (HasAuthority())
    {
        bHasOverlapped = true;
        bIsPendingDestroy = true;

        Multicast_DestroyPort();
    }

    if (ALCU_PlayerController* PlayerController = Cast<ALCU_PlayerController>(GetWorld()->GetFirstPlayerController()))
    {
        if (PlayerController->IsLocalController())
        {
            // 시퀀스 파일을 로드 (경로 설정)
            ULevelSequence* Sequence = LoadObject<ULevelSequence>(nullptr, TEXT("LevelSequence'/Game/NSK/Sequence/Seq_EscapePort.Seq_EscapePort'"));
            
            if (Sequence)
            {
                // 시퀀스를 재생할 Level Sequence Actor를 생성
                FActorSpawnParameters SpawnParams;
                SpawnParams.Owner = this;
                ALevelSequenceActor* SequenceActor = GetWorld()->SpawnActor<ALevelSequenceActor>(ALevelSequenceActor::StaticClass(), SpawnParams);

                if (SequenceActor)
                {
                    // 시퀀스를 Actor에 설정
                    SequenceActor->SetSequence(Sequence);

                    // Level Sequence Player 생성
                    FMovieSceneSequencePlaybackSettings PlaybackSettings;
                    ULevelSequencePlayer* SequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), Sequence, PlaybackSettings, SequenceActor);

                    if (SequencePlayer)
                    {
                        // 시퀀스 재생 시작
                        SequencePlayer->Play();

                        // 시퀀스 재생이 시작된 후 스펙터 모드로 전환
                        PlayerController->ServerRPC_ChangeToSpector();
                    }
                    else
                    {
                        P_LOG(PolluteLog, Warning, TEXT("시퀀스를 LevelSequenceActor에서 추출 실패"));
                    }
                }
                else
                {
                    P_LOG(PolluteLog, Warning, TEXT("SequenceActor 생성 실패"));
                }
            }
            else
            {
                P_LOG(PolluteLog, Warning, TEXT("시퀀스를 찾을 수 없습니다."));
            }
        }
    }
}