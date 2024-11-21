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
	int32 SpawnPointID; // ID를 통해 개별 관리 가능

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsUsed; // 이미 사용된 포인트인지 여부

protected:
	// 스태틱 메시 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* SpawnPointMesh;

public:
	void ItemSpawnPoint();
};