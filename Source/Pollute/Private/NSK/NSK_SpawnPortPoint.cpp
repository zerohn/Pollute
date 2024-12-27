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

    P_LOG(PolluteLog, Warning, TEXT("탈출자: %s"), *OverlapCharacter->GetName());

    // 서버에서 처리
    if (HasAuthority())
    {
        bHasOverlapped = true;
        bIsPendingDestroy = true;

        P_LOG(PolluteLog, Warning, TEXT("서버에서 Multicast_DestroyPort 호출 전"));
        Multicast_DestroyPort();

        P_LOG(PolluteLog, Warning, TEXT("서버에서 Multicast_PlayEscapeSequence 호출 전"));
        Multicast_PlayEscapeSequence(OverlapCharacter);
    }
    else
    {
        // 클라이언트는 서버로 요청
        P_LOG(PolluteLog, Warning, TEXT("클라이언트에서 서버로 Escape 요청"));
        ServerHandleEscape(OverlapCharacter);
    }
}

void ANSK_SpawnPortPoint::ServerHandleEscape_Implementation(ALCU_PlayerCharacter* OverlapCharacter)
{
    if (!HasAuthority())
    {
        return;
    }

    if (!OverlapCharacter)
    {
        P_LOG(PolluteLog, Warning, TEXT("ServerHandleEscape 실패: OverlapCharacter가 null"));
        return;
    }

    bHasOverlapped = true;
    bIsPendingDestroy = true;

    P_LOG(PolluteLog, Warning, TEXT("서버에서 Multicast_DestroyPort 호출 전"));
    Multicast_DestroyPort();

    P_LOG(PolluteLog, Warning, TEXT("서버에서 Multicast_PlayEscapeSequence 호출 전"));
    Multicast_PlayEscapeSequence(OverlapCharacter);
}

void ANSK_SpawnPortPoint::Multicast_PlayEscapeSequence_Implementation(ALCU_PlayerCharacter* OverlapCharacter)
{
    if (!OverlapCharacter)
    {
        P_LOG(PolluteLog, Warning, TEXT("Multicast_PlayEscapeSequence 실패: OverlapCharacter가 null"));
        return;
    }

    P_LOG(PolluteLog, Warning, TEXT("Multicast_PlayEscapeSequence 실행 성공: OverlapCharacter = %s"), *OverlapCharacter->GetName());

    ALCU_PlayerController* PlayerController = Cast<ALCU_PlayerController>(OverlapCharacter->GetController());
    if (!PlayerController)
    {
        P_LOG(PolluteLog, Warning, TEXT("PlayerController 캐스팅 실패"));
        return;
    }

    P_LOG(PolluteLog, Warning, TEXT("PlayerController 캐스팅 성공: %s"), *PlayerController->GetName());

    OverlapCharacter->OnEscape();
}

//void ANSK_SpawnPortPoint::PlayEscapeSequence(ALCU_PlayerController* PlayerController)
//{
//    if (!PlayerController)
//    {
//        P_LOG(PolluteLog, Warning, TEXT("PlayerController is null"));
//        return;
//    }
//
//    P_LOG(PolluteLog, Warning, TEXT("PlayEscapeSequence 실행"));
//
//    ULevelSequence* Sequence = LoadObject<ULevelSequence>(nullptr, TEXT("LevelSequence'/Game/NSK/Sequence/Seq_EscapePort.Seq_EscapePort'"));
//    P_LOG(PolluteLog, Warning, TEXT("Sequence 로드 상태: %s"), Sequence ? TEXT("성공") : TEXT("실패"));
//
//    if (!Sequence)
//    {
//        P_LOG(PolluteLog, Warning, TEXT("시퀀스를 찾을 수 없습니다."));
//        return;
//    }
//
//    // SequenceActor 생성
//    FActorSpawnParameters SpawnParams;
//    SpawnParams.Owner = this;
//    ALevelSequenceActor* SequenceActor = GetWorld()->SpawnActor<ALevelSequenceActor>(ALevelSequenceActor::StaticClass(), SpawnParams);
//    P_LOG(PolluteLog, Warning, TEXT("SequenceActor 생성 상태: %s"), SequenceActor ? TEXT("성공") : TEXT("실패"));
//
//    if (!SequenceActor)
//    {
//        P_LOG(PolluteLog, Warning, TEXT("SequenceActor 생성 실패"));
//        return;
//    }
//
//    // Level Sequence Player 생성
//    SequenceActor->SetSequence(Sequence);
//    FMovieSceneSequencePlaybackSettings PlaybackSettings;
//    ULevelSequencePlayer* SequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), Sequence, PlaybackSettings, SequenceActor);
//    P_LOG(PolluteLog, Warning, TEXT("SequencePlayer 생성 상태: %s"), SequencePlayer ? TEXT("성공") : TEXT("실패"));
//
//    if (!SequencePlayer)
//    {
//        P_LOG(PolluteLog, Warning, TEXT("Level Sequence Player 생성 실패"));
//        return;
//    }
//
//    // 시퀀스 재생
//    SequencePlayer->Play();
//    P_LOG(PolluteLog, Warning, TEXT("시퀀스 실행 시작"));
//    
//    // 스펙터 모드로 전환
//    PlayerController->ServerRPC_ChangeToSpector();
//    P_LOG(PolluteLog, Warning, TEXT("PlayEscapeSequence 완료"));
//}