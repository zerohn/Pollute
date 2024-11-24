#include "NSK/NSK_SpawnManager.h"
#include "EngineUtils.h"

ANSK_SpawnManager::ANSK_SpawnManager()
{
    // ������ ���̺� �ε�
    static ConstructorHelpers::FObjectFinder<UDataTable> ItemDataTable(TEXT("DataTable'/Game/NSK/DT/DT_SpawnItemData.DT_SpawnItemData'"));
    if (ItemDataTable.Succeeded())
    {
        SpawnItemDataTable = ItemDataTable.Object;
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

    // ���� ������ ����
    SpawnRandomItems();
}

// �������� �����ϰ� �����ϴ� �Լ�
void ANSK_SpawnManager::SpawnRandomItems()
{
    // ������ ���̺��� ��� �� ��������
    // ContextString: ������ ���̺��� �����͸� ������ �� ���Ǵ� ����� ���ڿ�
    const FString ContextString(TEXT("SpawnDataTableContext"));
    TArray<FSpawnItemData*> AllRows; // ������ ���̺��� ��� ���� ������ �迭
    SpawnItemDataTable->GetAllRows(ContextString, AllRows); // ������ ���̺��� ��� ���� AllRows �迭�� ä��

    // ���� ����Ʈ �� 8���� �������� ����
    TArray<ANSK_ItemSpawnPoint*> RandomSpawnPoints = AllSpawnPoints;
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
                SpawnPoint->bIsUsed = true; // ���� ����Ʈ�� ����

                // ������ �������� �޽ø� ����
                UStaticMeshComponent* SpawnMesh = NewObject<UStaticMeshComponent>(SpawnPoint); // �޽� ������Ʈ ����
                SpawnMesh->SetStaticMesh(RandomRow->ItemMesh); // �޽ø� ������ ���̺��� ������ �޽÷� ����
                SpawnMesh->AttachToComponent(SpawnPoint->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform); // �޽ø� ���� ����Ʈ�� ����
                SpawnMesh->RegisterComponent(); // �޽� ������Ʈ ������ ���

                // ���� ����Ʈ �޽� �����
                SpawnPoint->HideSpawnPointMesh();

                // ������ �������� �̸� �α� ���
                UE_LOG(LogTemp, Warning, TEXT("Spawned Item: %s at SpawnPoint: %s"), *RandomRow->ItemName.ToString(), *SpawnPoint->GetName());
            }
        }
    }

    // ������ ���� ���� ����Ʈ ����
    for (ANSK_ItemSpawnPoint* SpawnPoint : AllSpawnPoints)
    {
        if (!SpawnPoint->bIsUsed) // �������� ���� ����Ʈ�� ���
        {
            SpawnPoint->Destroy(); // ���� ����Ʈ ����
        }
    }
}