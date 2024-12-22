#include "NSK/NSK_Ladder.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"

ANSK_Ladder::ANSK_Ladder()
{
    // 사다리 상단 위치 컴포넌트 추가
    TopPosition = CreateDefaultSubobject<USceneComponent>(TEXT("TopPosition"));
    TopPosition->SetupAttachment(RootComponent);

    // 위치 조정 : 사다리 높이에 맞게
    TopPosition->SetRelativeLocation(FVector(0.f, 0.f, 1000.f));


    // 콜리전 컴포넌트를 생성하는 코드
    Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("LadderCollision"));
    Collision->SetupAttachment(RootComponent);

    // 콜리전이 제대로 설정되었는지 확인
    if (Collision)
    {
        Collision->SetCollisionEnabled(ECollisionEnabled::NoCollision); // 초기에는 충돌 비활성화
        Collision->SetCollisionObjectType(ECC_WorldDynamic);
        Collision->SetCollisionResponseToAllChannels(ECR_Ignore);
        Collision->SetBoxExtent(FVector(25.f, 25.f, 500.f));  // 콜리전 크기 설정
    }
}

void ANSK_Ladder::EnableCollisionAfterInstall()
{
    if (Collision)
    {
        Collision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics); // 충돌 활성화
        Collision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block); // 플레이어와 충돌 가능
        Collision->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block); // 월드와 충돌
    }
}