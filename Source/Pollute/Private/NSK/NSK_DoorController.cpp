#include "NSK/NSK_DoorController.h"
#include "NSk/NSK_DoorActor.h"
#include "EngineUtils.h"

ANSK_DoorController::ANSK_DoorController()
{

	PrimaryActorTick.bCanEverTick = false;
}

void ANSK_DoorController::BeginPlay()
{
	Super::BeginPlay();

    // 레벨에 배치된 두 문 액터를 찾기
    for (TActorIterator<ANSK_DoorActor> It(GetWorld()); It; ++It)
    {
        ANSK_DoorActor* FoundDoor = *It;

        if (FoundDoor->IsA(LeftDoorClass) && !LeftDoorInstance)
        {
            LeftDoorInstance = FoundDoor;
        }
        else if (FoundDoor->IsA(RightDoorClass) && !RightDoorInstance)
        {
            RightDoorInstance = FoundDoor;
        }
    }

    if (!LeftDoorInstance || !RightDoorInstance)
    {
        UE_LOG(LogTemp, Error, TEXT("DoorController: Doors not found!"));
    }
}

void ANSK_DoorController::OpenDoor()
{
    if (!LeftDoorInstance || !RightDoorInstance)
    {
        UE_LOG(LogTemp, Error, TEXT("문 액터가 할당되지 않았습니다! Left: %s, Right: %s"),
            LeftDoorInstance ? *LeftDoorInstance->GetName() : TEXT("None"),
            RightDoorInstance ? *RightDoorInstance->GetName() : TEXT("None"));
        return;
    }

    if (LeftDoorInstance) LeftDoorInstance->OpenDoor();
    if (RightDoorInstance) RightDoorInstance->OpenDoor();
}