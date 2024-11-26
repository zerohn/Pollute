#include "NSK/NSK_SpawnManager.h"
#include "EngineUtils.h"

ANSK_SpawnManager::ANSK_SpawnManager()
{
    // 아이템 데이터 테이블 로드
    static ConstructorHelpers::FObjectFinder<UDataTable> ItemDataTable(TEXT("DataTable'/Game/HHR/Item/ItemDataTable.ItemDataTable'"));
    if (ItemDataTable.Succeeded())
    {
        SpawnItemDataTable = ItemDataTable.Object;
    }
    // 힌트 데이터 테이블 로드
    static ConstructorHelpers::FObjectFinder<UDataTable> AltarHintDataTable(TEXT("DataTable'/Game/NSK/DT/DT_NSK_AltarHintData.DT_NSK_AltarHintData'"));
    if (AltarHintDataTable.Succeeded())
    {
        SpawnHintDataTable = AltarHintDataTable.Object;
    }
}

void ANSK_SpawnManager::BeginPlay() // 게임이 시작된 후 호출 -> 스폰 액터 검색은 게임 시작 후 : 시작 전에 월드에 액터가 완전히 준비되지 않을 수 있음
{
	Super::BeginPlay();

    // 맵에 있는 모든 ItemSpawnPoint 찾기
    // 템플릿 타입으로 지정한 클래스(예: MyActorClass)에 해당하는 액터들만 필터링하여 순회합니다.
    for (TActorIterator<ANSK_ItemSpawnPoint> It(GetWorld()); It; ++It)
    {
        AllSpawnPoints.Add(*It);
    }

    // 맵에 있는 모든 AltarHintPoint 찾기
    for (TActorIterator<ANSK_AltarHintPoint> It(GetWorld()); It; ++It)
    {
        AllHintPoints.Add(*It);
    }

    // 1. 랜덤 아이템 스폰
    SpawnRandomItems();

    // 2. 제단 아이템 선택
    AssignAltarItems(); // 여기 터짐

    // 선택 함수 실행 후 결과로 힌트 스폰 (실행 순서 유의)
    
    // 3. 힌트 스폰
    SpawnAltarHint(); //여기 터짐
}

// 아이템을 랜덤하게 생성하는 함수
void ANSK_SpawnManager::SpawnRandomItems()
{
    TArray<FItemData*> CombineItems = FilterCombineItems(SpawnItemDataTable);

    // 랜덤으로 8개 아이템 선택
    SpawnedItems.Empty(); // 멤버 변수로 저장
    for (int32 i = 0; i < 8 && CombineItems.Num() > 0; ++i)
    {
        int32 RandomIndex = FMath::RandRange(0, CombineItems.Num() - 1);
        FItemData* RandomItem = CombineItems[RandomIndex];
        SpawnedItems.Add(RandomItem); // 스폰된 아이템 저장
        CombineItems.RemoveAt(RandomIndex);
    }

    // 랜덤 스폰 포인트 배열 준비
    TArray<ANSK_ItemSpawnPoint*> RandomSpawnPoints;
    for (ANSK_ItemSpawnPoint* SpawnPoint : AllSpawnPoints)
    {
        if (SpawnPoint)
        {
            RandomSpawnPoints.Add(SpawnPoint);
        }
    }

    // 랜덤하게 8개의 스폰 포인트 선택
    RandomSpawnPoints.Sort([](const ANSK_ItemSpawnPoint& A, const ANSK_ItemSpawnPoint& B)
        {
            return FMath::RandBool();
        });

    RandomSpawnPoints.SetNum(8);

    // 아이템과 포인트 매칭 후 스폰
    for (int32 i = 0; i < SpawnedItems.Num(); ++i)
    {
        FItemData* SelectedItem = SpawnedItems[i];
        ANSK_ItemSpawnPoint* SpawnPoint = RandomSpawnPoints[i];

        if (SelectedItem && SpawnPoint)
        {
            SpawnPoint->bSpawnPointIsUsed = true;

            UStaticMeshComponent* SpawnMesh = NewObject<UStaticMeshComponent>(SpawnPoint);
            if (SpawnMesh && SelectedItem->ItemMesh)
            {
                SpawnMesh->SetStaticMesh(SelectedItem->ItemMesh);
                SpawnMesh->AttachToComponent(SpawnPoint->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
                SpawnMesh->RegisterComponent();
            }

            SpawnPoint->HideSpawnPointMesh();

            P_LOG(PolluteLog, Warning, TEXT("Spawned Item: %s at SpawnPoint: %s"), *SelectedItem->ItemName.ToString(), *SpawnPoint->GetName());
        }
    }

    for (ANSK_ItemSpawnPoint* SpawnPoint : AllSpawnPoints)
    {
        if (SpawnPoint && !SpawnPoint->bSpawnPointIsUsed)
        {
            SpawnPoint->Destroy();
        }
    }
}

void ANSK_SpawnManager::AssignAltarItems()
{
    SelectedAltarItems.Empty();

    // 스폰된 아이템에서 4개 선택
    int32 ItemCount = FMath::Min(SpawnedItems.Num(), 4);
    TArray<FItemData*> TempSpawnedItems = SpawnedItems; // 복사본 생성

    for (int32 i = 0; i < ItemCount; ++i)
    {
        int32 RandomIndex = FMath::RandRange(0, TempSpawnedItems.Num() - 1);
        FItemData* RandomItem = TempSpawnedItems[RandomIndex];
        SelectedAltarItems.Add(RandomItem); // 제단 아이템에 추가
        TempSpawnedItems.RemoveAt(RandomIndex); // 중복 방지
    }

    // 로그 출력
    P_LOG(PolluteLog, Warning, TEXT("Altar Items Assigned :"));
    for (const FItemData* AltarItem : SelectedAltarItems)
    {
        if (AltarItem)
        {
            P_LOG(PolluteLog, Warning, TEXT(" - %s"), *AltarItem->ItemName.ToString());
        }
    }
}

void ANSK_SpawnManager::SpawnAltarHint()
{
    const FString ContextString(TEXT("HintDataTableContext"));
    TArray<FAltarHintData*> AllHintRows;

    // 힌트 데이터 테이블에서 모든 행 가져오기
    if (!SpawnHintDataTable)
    {
        P_LOG(PolluteLog, Error, TEXT("SpawnHintDataTable is null"));
        return;
    }
    SpawnHintDataTable->GetAllRows(ContextString, AllHintRows);

    // 제단 아이템 이름과 매칭되는 힌트 데이터 필터링
    TArray<FAltarHintData*> MatchingHints;
    for (const FItemData* AltarItem : SelectedAltarItems)
    {
        if (!AltarItem)
            continue;

        // HintName과 ItemName이 일치하는 데이터 찾기
        FAltarHintData** MatchingHint = AllHintRows.FindByPredicate([&](FAltarHintData* HintData)
            {
                return HintData->HintName.ToString() == AltarItem->ItemName.ToString();
            });

        if (MatchingHint)
        {
            MatchingHints.Add(*MatchingHint);
        }
    }

    // 힌트 포인트를 랜덤화
    TArray<ANSK_AltarHintPoint*> RandomizedHintPoints = AllHintPoints;
    RandomizedHintPoints.Sort([](const ANSK_AltarHintPoint& A, const ANSK_AltarHintPoint& B)
        {
            return FMath::RandBool();
        });

    // 첫 4개의 힌트 포인트에 매칭된 힌트 할당
    for (int32 i = 0; i < 4 && i < MatchingHints.Num() && i < RandomizedHintPoints.Num(); i++)
    {
        ANSK_AltarHintPoint* HintPoint = RandomizedHintPoints[i];
        FAltarHintData* HintData = MatchingHints[i];

        if (HintPoint && HintData)
        {
            // 힌트 활성화 및 메시 설정
            HintPoint->bHintPointIsUsed = true;
            if (HintPoint->HintPointMesh && HintData->HintMesh)
            {
                HintPoint->HintPointMesh->SetStaticMesh(HintData->HintMesh);
            }
            HintPoint->HintName = HintData->HintName;

            // 로그 출력
            P_LOG(PolluteLog, Warning, TEXT("Hint Spawned: %s at HintPoint %s"), *HintData->HintName.ToString(), *HintPoint->GetName());
        }
    }

    // 선택되지 않은 힌트 포인트 삭제
    for (int32 i = 4; i < RandomizedHintPoints.Num(); i++)
    {
        if (RandomizedHintPoints[i])
        {
            RandomizedHintPoints[i]->Destroy(); // 힌트 포인트 삭제
        }
    }
}

TArray<FItemData*> ANSK_SpawnManager::FilterCombineItems(UDataTable* DataTable)
{
    const FString ContextString(TEXT("ItemDataTableContext"));
    TArray<FItemData*> AllItemRows;
    TArray<FItemData*> CombineItems;

    if (!DataTable) return CombineItems;

    DataTable->GetAllRows(ContextString, AllItemRows);
    for (FItemData* Item : AllItemRows)
    {
        if (Item && Item->ItemType == EItemType::CombineItem)
        {
            CombineItems.Add(Item);
        }
    }

    return CombineItems;
}