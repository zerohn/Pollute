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
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SpawnPointID; // ID�� ���� ���� ���� ����

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsUsed; // �̹� ���� ����Ʈ���� ����

protected:
	// ����ƽ �޽� ������Ʈ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* SpawnPointMesh;

public:
	void ItemSpawnPoint();
};