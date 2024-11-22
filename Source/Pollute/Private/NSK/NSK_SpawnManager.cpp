#include "NSK/NSK_SpawnManager.h"
#include "Engine/World.h"

ANSK_SpawnManager::ANSK_SpawnManager()
{

}

void ANSK_SpawnManager::BeginPlay()
{
	Super::BeginPlay();
}

// 아이템을 랜덤하게 생성하는 함수
void ANSK_SpawnManager::SpawnRandomItems()
{
	// 모든 스폰 포인트를 가져오기
	GetAllActorsOfClass(GetWorld(), ANSK_ItemSpawnPoint::StaticClass(), AllSpawnPoints);

	TArray<ANSK_ItemSpawnPoint*> SelectedPoints;

	// 랜덤으로 8개의 포인트 선택
	while (SelectedPoints.Num() < 8)
	{
		int32 RandomIndex = FMath::RandRange(0, AllSpawnPoints.Num() - 1);
		ANSK_ItemSpawnPoint* SpawnPoint = AllSpawnPoints[RandomIndex];

		if (!SpawnPoint->bIsUsed) // 이미 사용된 포인트는 제외
		{
			SpawnPoint->bIsUsed = true;
			SelectedPoints.Add(SpawnPoint);
		}
	}

	// 선택된 포인트에서 아이템을 스폰
	for (ANSK_ItemSpawnPoint* Point : SelectedPoints)
	{
		// 랜덤 아이템 RowName 가져오기
		FName RandomRowName = FName(*FString::Printf(TEXT("Item_%d"), FMath::RandRange(1, 8)));

		if (FSpawnItemData)
		{
			FItemData* ItemData = FSpawnItemData->FindRow<FItemData>(RandomRowName, TEXT("SpawnRandomItems"));

			if (ItemData)
			{
				// 아이템 스폰
				GetWorld()->SpawnActor<AActor>(ItemData->ItemClass, SpawnPoint->GetActorLocation(), FRotator::ZeroRotator);
			}
		}
	}
}