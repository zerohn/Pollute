#include "NSK/NSK_SpawnManager.h"
#include "EngineUtils.h"

ANSK_SpawnManager::ANSK_SpawnManager()
{
    // 아이템 데이터 테이블 로드
    static ConstructorHelpers::FObjectFinder<UDataTable> ItemDataTable(TEXT("DataTable'/Game/NSK/DT/DT_NSK_SpawnItemData.DT_NSK_SpawnItemData'"));
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
    // 데이터 테이블에서 모든 행 가져오기
    // ContextString: 데이터 테이블에서 데이터를 가져올 때 사용되는 디버깅 문자열
    const FString ContextString(TEXT("SpawnDataTableContext"));
    TArray<FSpawnItemData*> AllRows; // 데이터 테이블의 모든 행을 저장할 배열
    SpawnItemDataTable->GetAllRows(ContextString, AllRows); // 데이터 테이블의 모든 행을 AllRows 배열에 채움

    // AllSpawnPoints 배열을 기반으로 랜덤한 스폰 지점 8개를 선택하는 로직
    TArray<ANSK_ItemSpawnPoint*> RandomSpawnPoints = AllSpawnPoints; // 여기 터짐
    RandomSpawnPoints.Sort([](const ANSK_ItemSpawnPoint& A, const ANSK_ItemSpawnPoint& B)
        {
            return FMath::RandBool();
        });

    RandomSpawnPoints.SetNum(8);

    // 랜덤 아이템을 선택하여 스폰
    for (ANSK_ItemSpawnPoint* SpawnPoint : RandomSpawnPoints)
    {
        if (AllRows.Num() > 0)
        {
            // 랜덤 행 선택
            FSpawnItemData* RandomRow = AllRows[FMath::RandRange(0, AllRows.Num() - 1)];

            if (RandomRow && SpawnPoint) // 선택 데이터와 스폰 포인트가 유효한 경우
            {
                // 아이템 스폰
                SpawnPoint->bSpawnPointIsUsed = true; // 스폰 포인트가 사용됨

                // 스폰된 아이템의 메시를 생성
                UStaticMeshComponent* SpawnMesh = NewObject<UStaticMeshComponent>(SpawnPoint); // 메시 컴포넌트 생성
                SpawnMesh->SetStaticMesh(RandomRow->ItemMesh); // 메시를 데이터 테이블에서 설정된 메시로 지정
                SpawnMesh->AttachToComponent(SpawnPoint->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform); // 메시를 스폰 포인트에 부착
                SpawnMesh->RegisterComponent(); // 메시 컴포넌트 엔진에 등록

                // 스폰 포인트 메시 숨기기
                SpawnPoint->HideSpawnPointMesh();

                // 스폰된 아이템의 이름 로그 출력
                //UE_LOG(LogTemp, Warning, TEXT("Spawned Item: %s at SpawnPoint: %s"), *RandomRow->ItemName.ToString(), *SpawnPoint->GetName());
                P_LOG(PolluteLog, Warning, TEXT("Spawned Item: %s at SpawnPoint: %s"), *RandomRow->ItemName.ToString(), *SpawnPoint->GetName()); // 프로젝트 로그 컨벤션
            }
        }
    }

    // 사용되지 않은 스폰 포인트 제거
    for (ANSK_ItemSpawnPoint* SpawnPoint : AllSpawnPoints)
    {
        if (!SpawnPoint->bSpawnPointIsUsed) // 스폰되지 않은 포인트인 경우
        {
            SpawnPoint->Destroy(); // 스폰 포인트 제거
        }
    }
}

void ANSK_SpawnManager::AssignAltarItems()
{
    const FString ContextString(TEXT("SpawnDataTableContext"));
    TArray<FSpawnItemData*> AllRows;
    SpawnItemDataTable->GetAllRows(ContextString, AllRows);

    // 8개의 생성된 아이템 중에서 랜덤하게 4가지 제단 아이템 선택
    TArray<FSpawnItemData*> RandomizedItems = AllRows; // 여기 터짐
    RandomizedItems.Sort([](const FSpawnItemData& A, const FSpawnItemData& B)
        {
            return FMath::RandBool();
        });

    SelectedAltarItems = TArray<FSpawnItemData*>(RandomizedItems.GetData(), 4);

    // 선택된 제단 아이템 로그 출력
    P_LOG(PolluteLog, Warning, TEXT("Altar Items Assigned :"));
    for (const FSpawnItemData* AltarItem : SelectedAltarItems)
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
    SpawnHintDataTable->GetAllRows(ContextString, AllHintRows);

    // 제단 아이템 이름과 매칭되는 힌트 데이터 필터링
    TArray<FAltarHintData*> MatchingHints;
    for (FSpawnItemData* AltarItem : SelectedAltarItems)
    {
        if (!AltarItem)
            continue;

        // HintName과 ItemName이 일치하는 데이터 찾기
        FAltarHintData** MatchingHint = AllHintRows.FindByPredicate([&](FAltarHintData* HintData)
            {
                return HintData->HintName == AltarItem->ItemName;
            });

        if (MatchingHint)
        {
            MatchingHints.Add(*MatchingHint);
        }
    }

    // 힌트 포인트 랜덤화 및 할당
    TArray<ANSK_AltarHintPoint*> RandomizedHintPoints = AllHintPoints;
    RandomizedHintPoints.Sort([](const ANSK_AltarHintPoint& A, const ANSK_AltarHintPoint& B)
        {
            return FMath::RandBool();
        });

    // 첫 4개의 힌트 포인트에 매칭된 힌트 설정
    for (int32 i = 0; i < 4 && i < MatchingHints.Num() && i < RandomizedHintPoints.Num(); i++)
    {
        ANSK_AltarHintPoint* HintPoint = RandomizedHintPoints[i];
        FAltarHintData* HintData = MatchingHints[i];

        if (HintPoint && HintData)
        {
            // 힌트 활성화
            HintPoint->bHintPointIsUsed = true;

            // 힌트 메시 설정
            if (HintPoint->HintPointMesh)
            {
                HintPoint->HintPointMesh->SetStaticMesh(HintData->HintMesh); // 메시 설정
            }
            HintPoint->HintName = HintData->HintName;

            // 로그 출력
            P_LOG(PolluteLog, Warning, TEXT("Hint Spawned: %s at HintPoint %s"), *HintData->HintName.ToString(), *HintPoint->GetName());
        }
    }

    // 나머지 힌트 포인트 메시 숨기기
    for (int32 i = 4; i < RandomizedHintPoints.Num(); i++)
    {
        if (RandomizedHintPoints[i])
        {
            RandomizedHintPoints[i]->HideHintPointMesh();
        }
    }
}