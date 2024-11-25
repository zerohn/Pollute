#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "NSK/NSK_ItemDataStruct.h"
#include "NSK/NSK_ItemSpawnPoint.h"

#include "NSK/NSK_AltarHintPoint.h"
#include "NSK/NSK_AltarHintDataStruct.h"

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
	UDataTable* SpawnItemDataTable; // 아이템 데이터 테이블

	TArray<ANSK_ItemSpawnPoint*> AllSpawnPoints; // 게임 내 모든 스폰 포인트들을 저장

	TArray<FSpawnItemData*> SelectedAltarItems; // 제단 아이템으로 선택된 아이템의 포인터 배열

	// 힌트
	UDataTable* SpawnHintDataTable; // 힌트 데이터 테이블

	TArray<ANSK_AltarHintPoint*> AllHintPoints; // 맵에 배치된 모든 힌트 포인트 저장

	TArray<FAltarHintData*> AltarHintData;  // 데이터 테이블에서 가져온 힌트 데이터
};
