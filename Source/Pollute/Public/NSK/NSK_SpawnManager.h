#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "NSK/NSK_ItemDataStruct.h"
#include "NSK/NSK_ItemSpawnPoint.h"

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
	void SpawnRandomItems(); // 스폰할 아이템을 랜덤하게 생성하는 함수

private:
	UDataTable* FSpawnItemData; // 아이템 데이터 테이블

	TArray<ANSK_ItemSpawnPoint*> AllSpawnPoints; // 게임 내 모든 스폰 포인트들을 저장
};
