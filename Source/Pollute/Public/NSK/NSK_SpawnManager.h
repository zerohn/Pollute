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

	void AssignAltarItems(); // ���ܿ� ���Ǵ� 4���� ��� ������ ����

private:
	UDataTable* SpawnItemDataTable; // ������ ������ ���̺�

	TArray<ANSK_ItemSpawnPoint*> AllSpawnPoints; // ���� �� ��� ���� ����Ʈ���� ����

	TArray<FSpawnItemData*> SelectedAltarItems; // ���� ���������� ���õ� �������� ������ �迭
};
