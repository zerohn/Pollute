// Fill out your copyright notice in the Description page of Project Settings.


#include "NSK/NSK_DoorActor.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ANSK_DoorActor::ANSK_DoorActor()
{
    PrimaryActorTick.bCanEverTick = true;
    bIsLeftDoor = true;  // 기본값을 왼쪽 문으로 설정

    // 스태틱 메시 컴포넌트 초기화
    DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
    RootComponent = DoorMesh;
}

// Called when the game starts or when spawned
void ANSK_DoorActor::BeginPlay()
{
	Super::BeginPlay();

    // 문이 열릴 목표 위치 및 회전 설정
    SetTargetPositionAndRotation();
}

void ANSK_DoorActor::SetTargetPositionAndRotation()
{
    // 왼쪽 문인 경우: 문이 안에서 바깥으로 열리도록
    if (bIsLeftDoor)
    {
        // 왼쪽 문: 문이 -X 방향으로 열림 (피벗 기준으로 이동)
        TargetPosition = GetActorLocation();
        TargetRotation = FRotator(0.f, -90.f, 0.f);  // 90도 회전 (왼쪽으로 열림)
    }
    else
    {
        // 오른쪽 문: 문이 +X 방향으로 열리도록
        TargetPosition = GetActorLocation();
        TargetRotation = FRotator(0.f, 90.f, 0.f);  // 90도 회전 (오른쪽으로 열림)
    }
}

void ANSK_DoorActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bIsOpening)
    {
        // 문이 열리고 있을 때만 실행
        FVector CurrentPosition = GetActorLocation();
        FRotator CurrentRotation = GetActorRotation();

        if (CurrentPosition != TargetPosition)
        {
            // 선형 보간으로 문이 목표 위치로 이동
            FVector NewLocation = FMath::VInterpTo(CurrentPosition, TargetPosition, DeltaTime, 2.0f);
            SetActorLocation(NewLocation);
        }

        if (CurrentRotation != TargetRotation)
        {
            // 회전도 선형 보간으로 진행
            FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, 2.0f);
            SetActorRotation(NewRotation);
        }

        // 목표 위치와 회전에 도달했는지 확인
        if (CurrentPosition.Equals(TargetPosition, 1.0f) && CurrentRotation.Equals(TargetRotation, 1.0f))
        {
            bIsOpening = false; // 문 열림 완료
            UE_LOG(LogTemp, Warning, TEXT("문이 완전히 열렸습니다."));
        }
    }
}

void ANSK_DoorActor::OpenDoor()
{
    bIsOpening = true; // 문 열림 시작
    UE_LOG(LogTemp, Warning, TEXT("문이 열기 시작했습니다."));
}