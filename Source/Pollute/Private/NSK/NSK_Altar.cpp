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

        // 블루프린트 편집 방지
        Slot->SetMobility(EComponentMobility::Static);
        Slot->bEditableWhenInherited = false;

        SlotLocations.Add(Slot);
    }

    if (SlotLocations.Num() >= 4)
    {
        if (SlotLocations[0]->GetRelativeLocation().IsZero())
        {
            SlotLocations[0]->SetRelativeLocation(FVector(-50, 0, 50));
            SlotLocations[1]->SetRelativeLocation(FVector(-25, 0, 50));
            SlotLocations[2]->SetRelativeLocation(FVector(25, 0, 50));
            SlotLocations[3]->SetRelativeLocation(FVector(50, 0, 50));
        }
        else
        {
            P_LOG(PolluteLog, Warning, TEXT("슬롯 위치가 이미 블루프린트에서 설정됨"));
        }

        for (int32 i = 0; i < SlotLocations.Num(); i++)
        {
            FVector Location = SlotLocations[i]->GetRelativeLocation();
            P_LOG(PolluteLog, Warning, TEXT("Slot %d Location: %s"), i + 1, *Location.ToString());
        }
    }

    bIsPlayerNearby = false;
}

void ANSK_Altar::BeginPlay()
{
    Super::BeginPlay();

    // 상호작용 트리거에 오버랩 이벤트 바인딩
    InteractionBox->OnComponentBeginOverlap.AddDynamic(this, &ANSK_Altar::OnOverlapBegin);
    InteractionBox->OnComponentEndOverlap.AddDynamic(this, &ANSK_Altar::OnOverlapEnd);
}

void ANSK_Altar::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (ANSK_TESTPlayerCharacter* Player = Cast<ANSK_TESTPlayerCharacter>(OtherActor))
    {
        if (!bIsPlayerNearby) // 중복 방지
        {
            bIsPlayerNearby = true;
            Player->SetNearbyAltar(this);
            P_LOG(PolluteLog, Warning, TEXT("NearbyAltar 설정 완료: %s"), *GetName());
        }
    }
}

void ANSK_Altar::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (ANSK_TESTPlayerCharacter* Player = Cast<ANSK_TESTPlayerCharacter>(OtherActor))
    {
        if (bIsPlayerNearby) // 중복 방지
        {
            bIsPlayerNearby = false;
            Player->SetNearbyAltar(nullptr);
            P_LOG(PolluteLog, Warning, TEXT("NearbyAltar 해제 완료"));
        }
    }
}

void ANSK_Altar::OnInteract()
{
    P_LOG(PolluteLog, Warning, TEXT("OnInteract 호출"));
    ANSK_TESTPlayerCharacter* PlayerCharacter = Cast<ANSK_TESTPlayerCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
    if (PlayerCharacter)
    {
        P_LOG(PolluteLog, Warning, TEXT("플레이어 캐릭터 감지됨"));
        HandlePlayerInteraction(PlayerCharacter);
    }
    else
    {
        P_LOG(PolluteLog, Warning, TEXT("플레이어 캐릭터 없음"));
    }
}

void ANSK_Altar::HandlePlayerInteraction(ANSK_TESTPlayerCharacter* PlayerCharacter)
{
    if (!PlayerCharacter || !PlayerCharacter->bHasItem)
    {
        P_LOG(PolluteLog, Warning, TEXT("플레이어가 아이템을 들고 있지 않음"));
        return; // 플레이어가 아이템이 없으면 무시
    }

    P_LOG(PolluteLog, Warning, TEXT("플레이어가 아이템을 들고 있음"));
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
    // 함수 시작 부분 호출 확인
    P_LOG(PolluteLog, Warning, TEXT("PlaceItemInSlot 호출됨: 슬롯 인덱스: %d, 아이템 이름: %s, 메시 유효성: %s"),
        SlotIndex,
        *Item.ItemName.ToString(),
        Item.ItemMesh ? TEXT("유효함") : TEXT("nullptr"));

    // 슬롯 유효성 검사
    if (!SlotLocations.IsValidIndex(SlotIndex))
    {
        P_LOG(PolluteLog, Error, TEXT("잘못된 슬롯 인덱스: %d"), SlotIndex);
        return;
    }

    UStaticMeshComponent* ItemMesh = NewObject<UStaticMeshComponent>(this);
    if (!ItemMesh)
    {
        P_LOG(PolluteLog, Error, TEXT("UStaticMeshComponent 생성 실패"));
        return;
    }

    if (Item.ItemMesh)
    {
        P_LOG(PolluteLog, Warning, TEXT("슬롯 %d에 아이템 배치: %s"), SlotIndex, *Item.ItemName.ToString());
        ItemMesh->SetStaticMesh(Item.ItemMesh);
    }
    else
    {
        P_LOG(PolluteLog, Error, TEXT("슬롯 %d에 배치할 아이템의 메시가 비어 있음. 기본 메시를 사용하거나 오류 처리 필요."), SlotIndex);
        // ItemMesh->SetStaticMesh(DefaultMesh); // 기본 메시가 있다면 설정
    }

    if (!SlotLocations[SlotIndex])
    {
        P_LOG(PolluteLog, Error, TEXT("SlotLocations[%d]가 유효하지 않음"), SlotIndex);
        return;
    }

    ItemMesh->SetupAttachment(SlotLocations[SlotIndex]);
    ItemMesh->SetRelativeLocation(FVector::ZeroVector);
    ItemMesh->RegisterComponent();

    P_LOG(PolluteLog, Warning, TEXT("슬롯 %d에 아이템 메시 등록 완료"), SlotIndex);
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
