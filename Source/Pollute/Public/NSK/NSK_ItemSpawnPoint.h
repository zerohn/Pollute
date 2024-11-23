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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnPoint")
	FName SpawnName; // ID�� ���� ���� ���� ����

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SpawnPoint")
	bool bIsUsed = false; // ���� ����Ʈ�� ���Ǿ����� ����

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SpawnPoint")
	UStaticMeshComponent* SpawnPointMesh; // ����ƽ �޽� ������Ʈ
};