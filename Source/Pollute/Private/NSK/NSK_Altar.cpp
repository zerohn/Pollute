#include "NSK/NSK_Altar.h"
#include <NSK/NSK_TESTPlayerCharacter.h>
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"

ANSK_Altar::ANSK_Altar()
{
    // 제단 메시 초기화
    AltarMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AltarMesh"));
    RootComponent = AltarMesh;

    // 상호작용 트리거 설정
    InteractionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionBox"));
    InteractionBox->SetupAttachment(RootComponent);
    InteractionBox->SetBoxExtent(FVector(100.0f, 100.0f, 50.0f));

    // 슬롯 위치 초기화
    for (int32 i = 0; i < 4; i++)
    {
        FString SlotName = FString::Printf(TEXT("Slot%d"), i + 1);
        USceneComponent* Slot = CreateDefaultSubobject<USceneComponent>(*SlotName);
        Slot->SetupAttachment(RootComponent);
        SlotLocations.Add(Slot);
    }
}

void ANSK_Altar::BeginPlay()
{
    Super::BeginPlay();

    // 슬롯 위치 설정
    if (SlotLocations.Num() >= 4)
    {
        SlotLocations[0]->SetRelativeLocation(FVector(50, 50, 50));
        SlotLocations[1]->SetRelativeLocation(FVector(-50, 50, 50));
        SlotLocations[2]->SetRelativeLocation(FVector(50, -50, 50));
        SlotLocations[3]->SetRelativeLocation(FVector(-50, -50, 50));
    }

    // 상호작용 트리거에 오버랩 이벤트 바인딩
    InteractionBox->OnComponentBeginOverlap.AddDynamic(this, &ANSK_Altar::OnOverlapBegin);
    InteractionBox->OnComponentEndOverlap.AddDynamic(this, &ANSK_Altar::OnOverlapEnd);
}

void ANSK_Altar::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    ANSK_TESTPlayerCharacter* PlayerCharacter = Cast<ANSK_TESTPlayerCharacter>(OtherActor);
    if (PlayerCharacter)
    {
        PlayerCharacter->SetNearbyAltar(this); // 제단 참조 설정
    }
}

void ANSK_Altar::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    ANSK_TESTPlayerCharacter* PlayerCharacter = Cast<ANSK_TESTPlayerCharacter>(OtherActor);
    if (PlayerCharacter)
    {
        PlayerCharacter->SetNearbyAltar(nullptr); // 제단 참조 해제
    }
}

void ANSK_Altar::OnInteract()
{
    ANSK_TESTPlayerCharacter* PlayerCharacter = Cast<ANSK_TESTPlayerCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
    if (PlayerCharacter)
    {
        HandlePlayerInteraction(PlayerCharacter);
    }
}

void ANSK_Altar::HandlePlayerInteraction(ANSK_TESTPlayerCharacter* PlayerCharacter)
{
    if (!PlayerCharacter || !PlayerCharacter->bHasItem)
    {
        return; // 플레이어가 아이템이 없으면 무시
    }

    FItemData HeldItem = PlayerCharacter->GetHeldItem(); // 플레이어의 아이템 가져오기
    AddItemToSlot(HeldItem); // 제단에 아이템 추가
    PlayerCharacter->ResetHeldItem(); // 플레이어 아이템 초기화
}

void ANSK_Altar::AddItemToSlot(FItemData Item)
{
    if (Slots.Num() < 4)
    {
        Slots.Add(Item);
        PlaceItemInSlot(Item, Slots.Num() - 1); // 슬롯 위치에 배치

        P_LOG(PolluteLog, Warning, TEXT("재단에 아이템 추가: %s"), *Item.ItemName.ToString());
    }

    if (Slots.Num() == 4)
    {
        CheckSlots(); // 슬롯이 모두 차면 확인
    }
}

void ANSK_Altar::CheckSlots()
{
    if (Slots == CorrectItems) // 모든 재료가 정확히 맞았는지 확인
    {
        ShowSuccessMessage();

        P_LOG(PolluteLog, Warning, TEXT("저주가 약해지면서 정문이 열립니다!"));
        if (DoorController)
        {
            DoorController->OpenDoor(); // 정문 열기
        }

        // TODO: 플레이어가 지나갈 때 탈출 처리 (로비 or 관전자 모드)
    }
    else
    {
        ShowFailureMessage();

        P_LOG(PolluteLog, Warning, TEXT("저주가 강력해지면서 죽음이 가까워 집니다."));

        // TODO: 저주 패널티 구현
    }
}

void ANSK_Altar::PlaceItemInSlot(const FItemData& Item, int32 SlotIndex)
{
    if (SlotLocations.IsValidIndex(SlotIndex))
    {
        // 슬롯 위치에 아이템 메시 배치
        UStaticMeshComponent* ItemMesh = NewObject<UStaticMeshComponent>(this);
        ItemMesh->SetStaticMesh(Item.ItemMesh); // 아이템 메시 설정
        ItemMesh->SetupAttachment(SlotLocations[SlotIndex]);
        ItemMesh->SetRelativeLocation(FVector::ZeroVector);
        ItemMesh->RegisterComponent();
    }
}

void ANSK_Altar::RemoveItemFromSlot()
{
    if (Slots.Num() > 0)
    {
        // 마지막 아이템을 빼는 로직
        Slots.RemoveAt(Slots.Num() - 1);
        P_LOG(PolluteLog, Warning, TEXT("재단에서 아이템 제거"));
    }
}

void ANSK_Altar::ShowSuccessMessage()
{
    // 성공 UI 메시지 표시 (BlueprintAssignable 이벤트 호출 가능)
    P_LOG(PolluteLog, Warning, TEXT("성공 메시지를 UI로 띄웁니다."));
}

void ANSK_Altar::ShowFailureMessage()
{
    // 실패 UI 메시지 표시 (BlueprintAssignable 이벤트 호출 가능)
    P_LOG(PolluteLog, Warning, TEXT("실패 메시지를 UI로 띄웁니다."));
}
