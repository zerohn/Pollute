#include "NSK/NSK_Altar.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "LCU/Player/LCU_PlayerCharacter.h"
#include "Engine//World.h"
#include <HHR/HHR_Item.h>

// 슬롯 초기화 및 배열 추가
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
        // 슬롯 이름 생성
        FString SlotName = FString::Printf(TEXT("Slot%d"), i + 1);

        // 슬롯 생성 및 루트에 부착
        UStaticMeshComponent* Slot = CreateDefaultSubobject<UStaticMeshComponent>(*SlotName);
        Slot->SetupAttachment(RootComponent);

        // 슬롯 초기 위치 설정
        FVector SlotLocation;
        switch (i)
        {
        case 0:
            SlotLocation = FVector(-50, 0, 50);
            break;
        case 1:
            SlotLocation = FVector(-25, 0, 50);
            break;
        case 2:
            SlotLocation = FVector(25, 0, 50);
            break;
        case 3:
            SlotLocation = FVector(50, 0, 50);
            break;
        }
        Slot->SetRelativeLocation(SlotLocation);

        // 슬롯을 배열에 추가
        SlotLocations.Add(Slot);

        // 슬롯 아이템 배열 초기화
        SlotItems.Init(FItemData(), SlotLocations.Num());

        // 로그 출력
        P_LOG(PolluteLog, Warning, TEXT("Slot %d initialized at: %s"), i + 1, *SlotLocation.ToString());
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
    if (ALCU_PlayerCharacter* Player = Cast<ALCU_PlayerCharacter>(OtherActor))
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
    if (ALCU_PlayerCharacter* Player = Cast<ALCU_PlayerCharacter>(OtherActor))
    {
        if (bIsPlayerNearby) // 중복 방지
        {
            bIsPlayerNearby = false;
            Player->SetNearbyAltar(nullptr);
            P_LOG(PolluteLog, Warning, TEXT("NearbyAltar 해제 완료"));
        }
    }
}

// 상호작용 감지 함수
void ANSK_Altar::OnInteract()
{
    P_LOG(PolluteLog, Warning, TEXT("OnInteract 호출"));
    ALCU_PlayerCharacter* PlayerCharacter = Cast<ALCU_PlayerCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
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

// 플레이어가 제단에 아이템 추가 함수
void ANSK_Altar::HandlePlayerInteraction(ALCU_PlayerCharacter* PlayerCharacter)
{
    if (!PlayerCharacter || !PlayerCharacter->bHasItem)
    {
        P_LOG(PolluteLog, Warning, TEXT("플레이어가 아이템을 들고 있지 않음"));
        return; // 플레이어가 아이템이 없으면 무시
    }

    P_LOG(PolluteLog, Warning, TEXT("플레이어가 아이템을 들고 있음"));
    FItemData HeldItem = PlayerCharacter->HeldItem; // 플레이어의 아이템 가져오기
    AddItemToSlot(HeldItem); // 제단에 아이템 추가
    PlayerCharacter->PickUpDropDown(); // 플레이어 아이템 초기화

    // 플레이어의 아이템 상태 초기화
    PlayerCharacter->HeldItem = FItemData(); // 데이터 초기화
    PlayerCharacter->bHasItem = false;       // 아이템을 들고 있지 않은 상태로 전환
}

// 슬롯 아이템 등록 및 액터 배치
void ANSK_Altar::AddItemToSlot(const FItemData& ItemData)
{
    for (int32 i = 0; i < SlotLocations.Num(); i++)
    {
        // 빈 슬롯 찾기
        if (!SlotItems[i].ItemMesh)
        {
            // 슬롯에 아이템 등록
            SlotItems[i] = ItemData;

            // 슬롯에 아이템 배치
            PlaceItemInSlot(ItemData, i);

            P_LOG(PolluteLog, Warning, TEXT("슬롯 %d에 아이템 등록: %s"), i, *ItemData.ItemName.ToString());
            return;
        }
    }
        P_LOG(PolluteLog, Warning, TEXT("슬롯이 가득 찼습니다. 더 이상 아이템을 등록할 수 없습니다."));

    if (Slots.Num() == 4)
    {
        CheckSlots(); // 슬롯이 모두 차면 확인
    }
}

// 슬롯 체크 함수
void ANSK_Altar::CheckSlots()
{
    if (Slots == CorrectItems) // 모든 재료가 정확히 맞았는지 확인
    {
        ShowSuccessMessage();

        P_LOG(PolluteLog, Warning, TEXT("저주가 약해지면서 정문이 열립니다! 플레이어가 지나갈 때 탈출 처리 (로비 or 관전자 모드 선택 UI)"));
        if (DoorController)
        {
            DoorController->OpenDoor(); // 정문 열기
        }

        // TODO: 플레이어가 지나갈 때 탈출 처리 (로비 or 관전자 모드 선택 UI)
    }
    else
    {
        ShowFailureMessage();

        P_LOG(PolluteLog, Warning, TEXT("저주가 강력해지면서 죽음이 가까워 집니다. TODO: 저주 패널티 구현"));

        // TODO: 저주 패널티 구현
    }
}

// 슬롯 아이템 액터 생성 및 슬롯 배치
void ANSK_Altar::PlaceItemInSlot(const FItemData& SlotItem, int32 SlotIndex)
{
    if (!SlotLocations.IsValidIndex(SlotIndex))
    {
        P_LOG(PolluteLog, Error, TEXT("잘못된 슬롯 인덱스: %d"), SlotIndex);
        return;
    }

    UStaticMeshComponent* SlotLocation = SlotLocations[SlotIndex];
    if (!SlotLocation)
    {
        P_LOG(PolluteLog, Error, TEXT("슬롯 %d의 컴포넌트가 유효하지 않습니다."), SlotIndex);
        return;
    }

    // 슬롯에 이미 아이템이 있는지 확인
    if (SlotActorItems.IsValidIndex(SlotIndex) && SlotActorItems[SlotIndex])
    {
        P_LOG(PolluteLog, Warning, TEXT("슬롯 %d에 이미 아이템이 있습니다. 아이템을 배치할 수 없습니다."), SlotIndex);
        return;  // 아이템이 있으면 배치하지 않음
    }

    // 새로운 AHHR_Item 액터 생성
    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = this;

    AHHR_Item* NewItemActor = GetWorld()->SpawnActor<AHHR_Item>(AHHR_Item::StaticClass(), SlotLocation->GetComponentLocation(), SlotLocation->GetComponentRotation(), SpawnParams);

    if (NewItemActor)
    {
        // 아이템 데이터 설정
        NewItemActor->SetItemData(SlotItem);
        NewItemActor->SetActorLocation(SlotLocation->GetComponentLocation());
        NewItemActor->SetActorRotation(SlotLocation->GetComponentRotation());

        if (!SlotActorItems.IsValidIndex(SlotIndex))
        {
            SlotActorItems.SetNum(SlotIndex + 1);  // 인덱스 확장
        }
        SlotActorItems[SlotIndex] = NewItemActor;

        P_LOG(PolluteLog, Warning, TEXT("슬롯 %d에 아이템 액터 생성 및 등록: %s"), SlotIndex, *SlotItem.ItemName.ToString());
    }

    /*// 기존 메시를 제거하고 새 메시 컴포넌트 추가
    UStaticMeshComponent* ItemMeshComp = NewObject<UStaticMeshComponent>(this);
    if (SlotItem.ItemMesh)
    {
        ItemMeshComp->SetStaticMesh(SlotItem.ItemMesh);
        P_LOG(PolluteLog, Warning, TEXT("슬롯 %d에 아이템 메시 등록: %s"), SlotIndex, *SlotItem.ItemName.ToString());
    }
    else
    {
        P_LOG(PolluteLog, Error, TEXT("슬롯 %d에 아이템 메시가 없습니다."), SlotIndex);
        return;
    }

    // 메시 컴포넌트를 슬롯에 연결
    ItemMeshComp->SetupAttachment(SlotLocation);
    ItemMeshComp->SetRelativeLocation(FVector::ZeroVector); // 슬롯 중앙 배치를 위한
    ItemMeshComp->RegisterComponent();*/
}

// 슬롯 아이템 제거 함수
AHHR_Item* ANSK_Altar::RemoveItemFromSlot(int32 SlotIndex)
{
    if (!SlotLocations.IsValidIndex(SlotIndex))
    {
        P_LOG(PolluteLog, Error, TEXT("잘못된 슬롯 인덱스: %d"), SlotIndex);
        return nullptr;
    }

    // 슬롯에 아이템이 존재하는지 확인
    if (SlotActorItems.IsValidIndex(SlotIndex) && SlotActorItems[SlotIndex] != nullptr)
    {
        // 슬롯에서 액터 반환
        AHHR_Item* RetrievedItem = SlotActorItems[SlotIndex];

        // 아이템 액터를 월드에서 삭제
        if (RetrievedItem)
        {
            RetrievedItem->Destroy();  // 아이템 액터 삭제
        }

        // 슬롯에서 해당 아이템 데이터 초기화
        SlotItems[SlotIndex] = FItemData();  // 기본값으로 초기화
        SlotActorItems[SlotIndex] = nullptr;

        return RetrievedItem;

        P_LOG(PolluteLog, Warning, TEXT("슬롯 %d에서 아이템 제거: %s"), SlotIndex, *RetrievedItem->ItemData.ItemName.ToString());
    }
    else
    {
        P_LOG(PolluteLog, Warning, TEXT("슬롯 %d는 비어 있습니다."), SlotIndex);
    }

    return nullptr;
}

// 성공 UI 표시 (현재 UI X 로그로만)
void ANSK_Altar::ShowSuccessMessage()
{
    // 성공 UI 메시지 표시 (BlueprintAssignable 이벤트 호출 가능)
    P_LOG(PolluteLog, Warning, TEXT("성공 메시지를 UI로 띄웁니다."));
}

// 실패 UI 표시 (현재 UI X 로그로만)
void ANSK_Altar::ShowFailureMessage()
{
    // 실패 UI 메시지 표시 (BlueprintAssignable 이벤트 호출 가능)
    P_LOG(PolluteLog, Warning, TEXT("실패 메시지를 UI로 띄웁니다."));
}
