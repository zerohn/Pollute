#include "NSK/NSK_Altar.h"
#include "Components/StaticMeshComponent.h"

ANSK_Altar::ANSK_Altar()
{
	// 제단 메시 초기화
	AltarMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AltarMesh"));
	RootComponent = AltarMesh;

	// 메시 기본 스케일 변경
	FVector DefaultScale(1.0f, 1.0f, 1.0f);
	AltarMesh->SetRelativeScale3D(DefaultScale);
}

void ANSK_Altar::BeginPlay()
{
	Super::BeginPlay();
}

// 스폰매니저의 제단 재료로 선택된 4가지를 가져와야 함
// 캐릭터가 G키로 슬롯에 상호작용해서 놓을 수 있게 해야함
// 제단 위 4군데 위치에 각각 놓아둘 수 있게 해야함

void ANSK_Altar::AddItemToSlot(FItemData Item)
{
    if (Slots.Num() < 4)
    {
        Slots.Add(Item);
        P_LOG(PolluteLog, Warning, TEXT("재료 아이템: %s"), *Item.ItemName.ToString());
    }

    if (Slots.Num() == 4)
    {
        CheckSlots();
    }
}

void ANSK_Altar::CheckSlots()
{
    if (Slots == CorrectItems) // 모든 재료가 정확히 맞았는지 확인
    {
        P_LOG(PolluteLog, Warning, TEXT("제료가 맞았다 문을 열어주마"));
        if (DoorController)
        {
            DoorController->OpenDoor();
        }
    }
    else
    {
        P_LOG(PolluteLog, Warning, TEXT("재료가 틀렸다 저주 패널티를 주마."));
        // TODO: 저주 시간 단축 처리
    }
}