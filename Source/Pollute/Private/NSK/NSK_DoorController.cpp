#include "NSK/NSK_DoorController.h"

ANSK_DoorController::ANSK_DoorController()
{

	PrimaryActorTick.bCanEverTick = true;

    // 도어 메쉬 초기화 부분이 없음
}

void ANSK_DoorController::BeginPlay()
{
	Super::BeginPlay();

    // 초기 상태: 닫힘
    CloseDoor();
	
}

// 도어 매쉬가 2가지가 존재함 2개를 열고 닫고 해야함
// 러프로 자연스럽게 문을 열고 닫게 해야함
// 제단 재료가 True/False 시 시퀀서를 각각 만들어서 재생하게 할꺼야

void ANSK_DoorController::OpenDoor()
{
    for (AStaticMeshActor* Door : DoorMeshes)
    {
        if (Door)
        {
            Door->SetActorRotation(FRotator(0.f, 90.f, 0.f)); // 임시로 문을 회전
        }
    }
    bIsDoorOpen = true;
}

void ANSK_DoorController::CloseDoor()
{
    for (AStaticMeshActor* Door : DoorMeshes)
    {
        if (Door)
        {
            Door->SetActorRotation(FRotator(0.f, 0.f, 0.f)); // 원래 위치로 닫음
        }
    }
    bIsDoorOpen = false;
}

