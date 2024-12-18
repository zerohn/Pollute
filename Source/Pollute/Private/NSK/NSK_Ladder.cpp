#include "NSK/NSK_Ladder.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"

ANSK_Ladder::ANSK_Ladder()
{
    // 부모 클래스의 RootComponent(ItemSphereComp)를 그대로 사용
    RootComponent = ItemSphereComp;

    // 추가 콜리전 컴포넌트 설정
    CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComponent"));
    RootComponent = CollisionComponent;

    CollisionComponent->SetBoxExtent(FVector(50.f, 50.f, 200.f)); // 크기 설정
    CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics); // 충돌 활성화
    CollisionComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
    CollisionComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block); // 플레이어와 충돌
    CollisionComponent->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block); // 고정 월드와 충돌

    // 콜리전 컴포넌트를 RootComponent에 연결
    CollisionComponent->SetupAttachment(RootComponent);

    // 사다리 상단 위치 컴포넌트 추가
    TopPosition = CreateDefaultSubobject<USceneComponent>(TEXT("TopPosition"));
    TopPosition->SetupAttachment(RootComponent);

    // 위치 조정 : 사다리 높이에 맞게
    TopPosition->SetRelativeLocation(FVector(0.f, 0.f, 1000.f));
}