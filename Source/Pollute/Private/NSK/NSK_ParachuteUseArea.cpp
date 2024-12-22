#include "NSK/NSK_ParachuteUseArea.h"
#include "LCU/Player/LCU_PlayerCharacter.h"

ANSK_ParachuteUseArea::ANSK_ParachuteUseArea()
{
    // 기본 콜리전 설정
    ParachuteUseArea = CreateDefaultSubobject<UBoxComponent>(TEXT("ParachuteUseArea"));
    RootComponent = ParachuteUseArea;

    // 범위 크기 설정
    ParachuteUseArea->SetBoxExtent(FVector(700.f, 50.f, 100.f));
    ParachuteUseArea->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    ParachuteUseArea->SetCollisionResponseToAllChannels(ECR_Ignore);
    ParachuteUseArea->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap); // 플레이어와 충돌 시 Overlap 발생
}

void ANSK_ParachuteUseArea::BeginPlay()
{
    Super::BeginPlay();

    ParachuteUseArea->OnComponentBeginOverlap.AddDynamic(this, &ANSK_ParachuteUseArea::BeginOverlap);
    ParachuteUseArea->OnComponentEndOverlap.AddDynamic(this, &ANSK_ParachuteUseArea::EndOverlap);
}

// 플레이어가 낙하산 범위에 들어갔을 때
void ANSK_ParachuteUseArea::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    ALCU_PlayerCharacter* Player = Cast<ALCU_PlayerCharacter>(OtherActor);
    if (Player)
    {
        // 낙하산을 사용할 수 있도록 허용
        Player->CanUseParachute(true);  // 이 함수에서 낙하산 사용 가능 여부를 설정
    }
}

// 범위에서 나가면 낙하산 사용 불가 처리
void ANSK_ParachuteUseArea::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    ALCU_PlayerCharacter* Player = Cast<ALCU_PlayerCharacter>(OtherActor);
    if (Player)
    {
        Player->CanUseParachute(false); // 범위 밖이면 낙하산 사용 불가
    }
}

