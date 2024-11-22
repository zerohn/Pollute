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
	void SpawnRandomItems(); // ������ �������� �����ϰ� �����ϴ� �Լ�

private:
	UDataTable* FSpawnItemData; // ������ ������ ���̺�

	TArray<ANSK_ItemSpawnPoint*> AllSpawnPoints; // ���� �� ��� ���� ����Ʈ���� ����
};
