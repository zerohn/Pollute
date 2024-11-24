#include "NSK/NSK_SpawnManager.h"
#include "EngineUtils.h"

ANSK_SpawnManager::ANSK_SpawnManager()
{
    // 데이터 테이블 로드
    static ConstructorHelpers::FObjectFinder<UDataTable> ItemDataTable(TEXT("DataTable'/Game/NSK/DT/DT_SpawnItemData.DT_SpawnItemData'"));
    if (ItemDataTable.Succeeded())
    {
        SpawnItemDataTable = ItemDataTable.Object;
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

    // 랜덤 아이템 스폰
    SpawnRandomItems();
}

// 아이템을 랜덤하게 생성하는 함수
void ANSK_SpawnManager::SpawnRandomItems()
{
    // 데이터 테이블에서 모든 행 가져오기
    // ContextString: 데이터 테이블에서 데이터를 가져올 때 사용되는 디버깅 문자열
    const FString ContextString(TEXT("SpawnDataTableContext"));
    TArray<FSpawnItemData*> AllRows; // 데이터 테이블의 모든 행을 저장할 배열
    SpawnItemDataTable->GetAllRows(ContextString, AllRows); // 데이터 테이블의 모든 행을 AllRows 배열에 채움

    // 스폰 포인트 중 8개를 랜덤으로 선택
    TArray<ANSK_ItemSpawnPoint*> RandomSpawnPoints = AllSpawnPoints;
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
                SpawnPoint->bIsUsed = true; // 스폰 포인트가 사용됨

                // 스폰된 아이템의 메시를 생성
                UStaticMeshComponent* SpawnMesh = NewObject<UStaticMeshComponent>(SpawnPoint); // 메시 컴포넌트 생성
                SpawnMesh->SetStaticMesh(RandomRow->ItemMesh); // 메시를 데이터 테이블에서 설정된 메시로 지정
                SpawnMesh->AttachToComponent(SpawnPoint->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform); // 메시를 스폰 포인트에 부착
                SpawnMesh->RegisterComponent(); // 메시 컴포넌트 엔진에 등록

                // 스폰 포인트 메시 숨기기
                SpawnPoint->HideSpawnPointMesh();

                // 스폰된 아이템의 이름 로그 출력
                UE_LOG(LogTemp, Warning, TEXT("Spawned Item: %s at SpawnPoint: %s"), *RandomRow->ItemName.ToString(), *SpawnPoint->GetName());
            }
        }
    }

    // 사용되지 않은 스폰 포인트 제거
    for (ANSK_ItemSpawnPoint* SpawnPoint : AllSpawnPoints)
    {
        if (!SpawnPoint->bIsUsed) // 스폰되지 않은 포인트인 경우
        {
            SpawnPoint->Destroy(); // 스폰 포인트 제거
        }
    }
}