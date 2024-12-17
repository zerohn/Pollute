#include "NSK/NSK_LadderPlacementComponent.h"
#include "NSK/NSK_BuildActor.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Components/StaticMeshComponent.h"

UNSK_LadderPlacementComponent::UNSK_LadderPlacementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
    RotationSpeed = 10.0f;
    bCanPlaceLadder = false;
    SpawnedLadderActor = nullptr;
}

void UNSK_LadderPlacementComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UNSK_LadderPlacementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // 설치 가능 여부 체크
    CheckPlacement();
}

// 설치 가능 여부 체크 함수
void UNSK_LadderPlacementComponent::CheckPlacement()
{
    // 라인 트레이스로 설치 가능 여부를 확인 : 캐릭터 전방
    FHitResult HitResult;
    FVector Start = GetOwner()->GetActorLocation();
    FVector ForwardVector = GetOwner()->GetActorForwardVector();
    FVector End = ((ForwardVector * 200.f) + Start);

    // 라인 트레이스 수행
    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(GetOwner());

    bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, CollisionParams);

    // 디버그 라인 그리기 (확인용)
    FColor LineColor = bHit ? FColor::Green : FColor::Red;
    DrawDebugLine(GetWorld(), Start, End, LineColor, false, 1.0f, 0, 2.0f);

    // 설치 가능 여부 판단
    if (bHit & HitResult.bBlockingHit)
    {
        bCanPlaceLadder = true;
        P_LOG(PolluteLog, Warning, TEXT("설치 가능 : %s"), *HitResult.ImpactPoint.ToString());
    }
    else
    {
        bCanPlaceLadder = false;
        P_LOG(PolluteLog, Warning, TEXT("설치 불가능"));
    }
}

// 사다리를 설치하는 함수
void UNSK_LadderPlacementComponent::TryPlaceLadder()
{
    if (bCanPlaceLadder && LadderActorClass)
    {
        // 사다리 액터 스폰
        FVector SpawnLocation = GetOwner()->GetActorLocation() + GetOwner()->GetActorForwardVector() * 200.f;
        FRotator SpawnRotation = GetOwner()->GetActorRotation();

        SpawnedLadderActor = GetWorld()->SpawnActor<ANSK_BuildActor>(LadderActorClass, SpawnLocation, SpawnRotation);

        // 설치 후 상태 업데이트
        if (SpawnedLadderActor)
        {
            SpawnedLadderActor->CheckSpawn();
        }
    }
}

// 마우스 휠 입력 받기
void UNSK_LadderPlacementComponent::OnMouseWheelInput(float Value)
{
    if (SpawnedLadderActor)
    {
        // 사다리 회전
        FRotator CurrentRotation = SpawnedLadderActor->GetActorRotation();

        // Wheel Up (양수)과 Wheel Down (음수)에 따라 회전 방향 조정
        if (Value > 0) // 휠 위로
        {
            CurrentRotation.Yaw += RotationSpeed; // 시계 방향
        }
        else if (Value < 0) // 휠 아래로
        {
            CurrentRotation.Yaw -= RotationSpeed; // 반시계 방향
        }

        SpawnedLadderActor->SetActorRotation(CurrentRotation);
    }
}