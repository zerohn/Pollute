#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/StaticMesh.h"
#include "Components/StaticMeshComponent.h"

#include "NSK_ItemSpawnPoint.generated.h"

UCLASS()
class POLLUTE_API ANSK_ItemSpawnPoint : public AActor
{
	GENERATED_BODY()
	
public:
	ANSK_ItemSpawnPoint();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemSpawnPoint")
	FName SpawnName; // ID�� ���� ���� ���� ����

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ItemSpawnPoint")
	bool bSpawnPointIsUsed = false; // ���� ����Ʈ�� ���Ǿ����� ����

	UFUNCTION(BlueprintCallable, Category = "ItemSpawnPoint") // �������� ���ü��� �����ؾ� �ϴ� ���
	void HideSpawnPointMesh(); // ���� ���� �� ���� ����Ʈ �޽ø� �����ϴ� �Լ�

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ItemSpawnPoint")
	UStaticMeshComponent* SpawnPointMesh; // ����ƽ �޽� ������Ʈ
};