#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "NSK/NSK_ItemSpawnPoint.h"

#include "NSK/NSK_AltarHintPoint.h"
#include "NSK/NSK_AltarHintDataStruct.h"
#include "HHR/HHR_ItemData.h"

#include "NSK_SpawnManager.generated.h"

UCLASS()
class POLLUTE_API ANSK_SpawnManager : public AActor
{
	GENERATED_BODY()
	
public:	
	ANSK_SpawnManager();

protected:
	virtual void BeginPlay() override;

public:
	void SpawnRandomItems(); // 스폰할 아이템을 랜덤하게 생성

	void AssignAltarItems(); // 제단에 사용되는 4개의 재료 아이템 선택

	void SpawnAltarHint(); // 힌트 생성


private:
	// 아이템
    UPROPERTY(EditAnywhere, Category = "Spawn Manager")
	UDataTable* SpawnItemDataTable; // 아이템 데이터 테이블

	TArray<ANSK_ItemSpawnPoint*> AllSpawnPoints; // 게임 내 모든 스폰 포인트들을 저장

	TArray<FItemData*> SelectedAltarItems; // 제단 아이템으로 선택된 아이템의 포인터 배열

    TArray<FItemData*> SpawnedItems; // 스폰된 아이템 저장

	// 힌트
	UDataTable* SpawnHintDataTable; // 힌트 데이터 테이블

	TArray<ANSK_AltarHintPoint*> AllHintPoints; // 맵에 배치된 모든 힌트 포인트 저장

    // 필터링
    TArray<FItemData*> FilterCombineItems(UDataTable* DataTable); // 필터링 함수
};
