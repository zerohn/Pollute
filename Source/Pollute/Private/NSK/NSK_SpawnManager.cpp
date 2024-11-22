#include "NSK/NSK_SpawnManager.h"
#include "Engine/World.h"

ANSK_SpawnManager::ANSK_SpawnManager()
{

}

void ANSK_SpawnManager::BeginPlay()
{
	Super::BeginPlay();
}

// �������� �����ϰ� �����ϴ� �Լ�
void ANSK_SpawnManager::SpawnRandomItems()
{
	// ��� ���� ����Ʈ�� ��������
	GetAllActorsOfClass(GetWorld(), ANSK_ItemSpawnPoint::StaticClass(), AllSpawnPoints);

	TArray<ANSK_ItemSpawnPoint*> SelectedPoints;

	// �������� 8���� ����Ʈ ����
	while (SelectedPoints.Num() < 8)
	{
		int32 RandomIndex = FMath::RandRange(0, AllSpawnPoints.Num() - 1);
		ANSK_ItemSpawnPoint* SpawnPoint = AllSpawnPoints[RandomIndex];

		if (!SpawnPoint->bIsUsed) // �̹� ���� ����Ʈ�� ����
		{
			SpawnPoint->bIsUsed = true;
			SelectedPoints.Add(SpawnPoint);
		}
	}

	// ���õ� ����Ʈ���� �������� ����
	for (ANSK_ItemSpawnPoint* Point : SelectedPoints)
	{
		// ���� ������ RowName ��������
		FName RandomRowName = FName(*FString::Printf(TEXT("Item_%d"), FMath::RandRange(1, 8)));

		if (FSpawnItemData)
		{
			FItemData* ItemData = FSpawnItemData->FindRow<FItemData>(RandomRowName, TEXT("SpawnRandomItems"));

			if (ItemData)
			{
				// ������ ����
				GetWorld()->SpawnActor<AActor>(ItemData->ItemClass, SpawnPoint->GetActorLocation(), FRotator::ZeroRotator);
			}
		}
	}
}