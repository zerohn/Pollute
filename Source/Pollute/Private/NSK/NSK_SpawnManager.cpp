#include "NSK/NSK_SpawnManager.h"
#include "EngineUtils.h"

ANSK_SpawnManager::ANSK_SpawnManager()
{
    // ������ ������ ���̺� �ε�
    static ConstructorHelpers::FObjectFinder<UDataTable> ItemDataTable(TEXT("DataTable'/Game/NSK/DT/DT_NSK_SpawnItemData.DT_NSK_SpawnItemData'"));
    if (ItemDataTable.Succeeded())
    {
        SpawnItemDataTable = ItemDataTable.Object;
    }
    // ��Ʈ ������ ���̺� �ε�
    static ConstructorHelpers::FObjectFinder<UDataTable> AltarHintDataTable(TEXT("DataTable'/Game/NSK/DT/DT_NSK_AltarHintData.DT_NSK_AltarHintData'"));
    if (AltarHintDataTable.Succeeded())
    {
        SpawnHintDataTable = AltarHintDataTable.Object;
    }
}

void ANSK_SpawnManager::BeginPlay() // ������ ���۵� �� ȣ�� -> ���� ���� �˻��� ���� ���� �� : ���� ���� ���忡 ���Ͱ� ������ �غ���� ���� �� ����
{
	Super::BeginPlay();

    // �ʿ� �ִ� ��� ItemSpawnPoint ã��
    // ���ø� Ÿ������ ������ Ŭ����(��: MyActorClass)�� �ش��ϴ� ���͵鸸 ���͸��Ͽ� ��ȸ�մϴ�.
    for (TActorIterator<ANSK_ItemSpawnPoint> It(GetWorld()); It; ++It)
    {
        AllSpawnPoints.Add(*It);
    }

    // �ʿ� �ִ� ��� AltarHintPoint ã��
    for (TActorIterator<ANSK_AltarHintPoint> It(GetWorld()); It; ++It)
    {
        AllHintPoints.Add(*It);
    }

    // 1. ���� ������ ����
    SpawnRandomItems();

    // 2. ���� ������ ����
    AssignAltarItems(); // ���� ����

    // ���� �Լ� ���� �� ����� ��Ʈ ���� (���� ���� ����)
    
    // 3. ��Ʈ ����
    SpawnAltarHint(); //���� ����
}

// �������� �����ϰ� �����ϴ� �Լ�
void ANSK_SpawnManager::SpawnRandomItems()
{
    // ������ ���̺��� ��� �� ��������
    // ContextString: ������ ���̺��� �����͸� ������ �� ���Ǵ� ����� ���ڿ�
    const FString ContextString(TEXT("SpawnDataTableContext"));
    TArray<FSpawnItemData*> AllRows; // ������ ���̺��� ��� ���� ������ �迭
    SpawnItemDataTable->GetAllRows(ContextString, AllRows); // ������ ���̺��� ��� ���� AllRows �迭�� ä��

    // AllSpawnPoints �迭�� ������� ������ ���� ���� 8���� �����ϴ� ����
    TArray<ANSK_ItemSpawnPoint*> RandomSpawnPoints = AllSpawnPoints; // ���� ����
    RandomSpawnPoints.Sort([](const ANSK_ItemSpawnPoint& A, const ANSK_ItemSpawnPoint& B)
        {
            return FMath::RandBool();
        });

    RandomSpawnPoints.SetNum(8);

    // ���� �������� �����Ͽ� ����
    for (ANSK_ItemSpawnPoint* SpawnPoint : RandomSpawnPoints)
    {
        if (AllRows.Num() > 0)
        {
            // ���� �� ����
            FSpawnItemData* RandomRow = AllRows[FMath::RandRange(0, AllRows.Num() - 1)];

            if (RandomRow && SpawnPoint) // ���� �����Ϳ� ���� ����Ʈ�� ��ȿ�� ���
            {
                // ������ ����
                SpawnPoint->bSpawnPointIsUsed = true; // ���� ����Ʈ�� ����

                // ������ �������� �޽ø� ����
                UStaticMeshComponent* SpawnMesh = NewObject<UStaticMeshComponent>(SpawnPoint); // �޽� ������Ʈ ����
                SpawnMesh->SetStaticMesh(RandomRow->ItemMesh); // �޽ø� ������ ���̺��� ������ �޽÷� ����
                SpawnMesh->AttachToComponent(SpawnPoint->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform); // �޽ø� ���� ����Ʈ�� ����
                SpawnMesh->RegisterComponent(); // �޽� ������Ʈ ������ ���

                // ���� ����Ʈ �޽� �����
                SpawnPoint->HideSpawnPointMesh();

                // ������ �������� �̸� �α� ���
                //UE_LOG(LogTemp, Warning, TEXT("Spawned Item: %s at SpawnPoint: %s"), *RandomRow->ItemName.ToString(), *SpawnPoint->GetName());
                P_LOG(PolluteLog, Warning, TEXT("Spawned Item: %s at SpawnPoint: %s"), *RandomRow->ItemName.ToString(), *SpawnPoint->GetName()); // ������Ʈ �α� ������
            }
        }
    }

    // ������ ���� ���� ����Ʈ ����
    for (ANSK_ItemSpawnPoint* SpawnPoint : AllSpawnPoints)
    {
        if (!SpawnPoint->bSpawnPointIsUsed) // �������� ���� ����Ʈ�� ���
        {
            SpawnPoint->Destroy(); // ���� ����Ʈ ����
        }
    }
}

void ANSK_SpawnManager::AssignAltarItems()
{
    const FString ContextString(TEXT("SpawnDataTableContext"));
    TArray<FSpawnItemData*> AllRows;
    SpawnItemDataTable->GetAllRows(ContextString, AllRows);

    // 8���� ������ ������ �߿��� �����ϰ� 4���� ���� ������ ����
    TArray<FSpawnItemData*> RandomizedItems = AllRows; // ���� ����
    RandomizedItems.Sort([](const FSpawnItemData& A, const FSpawnItemData& B)
        {
            return FMath::RandBool();
        });

    SelectedAltarItems = TArray<FSpawnItemData*>(RandomizedItems.GetData(), 4);

    // ���õ� ���� ������ �α� ���
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
    // ��Ʈ ������ ���̺��� ��� �� ��������
    SpawnHintDataTable->GetAllRows(ContextString, AllHintRows);

    // ���� ������ �̸��� ��Ī�Ǵ� ��Ʈ ������ ���͸�
    TArray<FAltarHintData*> MatchingHints;
    for (FSpawnItemData* AltarItem : SelectedAltarItems)
    {
        if (!AltarItem)
            continue;

        // HintName�� ItemName�� ��ġ�ϴ� ������ ã��
        FAltarHintData** MatchingHint = AllHintRows.FindByPredicate([&](FAltarHintData* HintData)
            {
                return HintData->HintName == AltarItem->ItemName;
            });

        if (MatchingHint)
        {
            MatchingHints.Add(*MatchingHint);
        }
    }

    // ��Ʈ ����Ʈ ����ȭ �� �Ҵ�
    TArray<ANSK_AltarHintPoint*> RandomizedHintPoints = AllHintPoints;
    RandomizedHintPoints.Sort([](const ANSK_AltarHintPoint& A, const ANSK_AltarHintPoint& B)
        {
            return FMath::RandBool();
        });

    // ù 4���� ��Ʈ ����Ʈ�� ��Ī�� ��Ʈ ����
    for (int32 i = 0; i < 4 && i < MatchingHints.Num() && i < RandomizedHintPoints.Num(); i++)
    {
        ANSK_AltarHintPoint* HintPoint = RandomizedHintPoints[i];
        FAltarHintData* HintData = MatchingHints[i];

        if (HintPoint && HintData)
        {
            // ��Ʈ Ȱ��ȭ
            HintPoint->bHintPointIsUsed = true;

            // ��Ʈ �޽� ����
            if (HintPoint->HintPointMesh)
            {
                HintPoint->HintPointMesh->SetStaticMesh(HintData->HintMesh); // �޽� ����
            }
            HintPoint->HintName = HintData->HintName;

            // �α� ���
            P_LOG(PolluteLog, Warning, TEXT("Hint Spawned: %s at HintPoint %s"), *HintData->HintName.ToString(), *HintPoint->GetName());
        }
    }

    // ������ ��Ʈ ����Ʈ �޽� �����
    for (int32 i = 4; i < RandomizedHintPoints.Num(); i++)
    {
        if (RandomizedHintPoints[i])
        {
            RandomizedHintPoints[i]->HideHintPointMesh();
        }
    }
}