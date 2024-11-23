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
	FName SpawnName; // ID를 통해 개별 관리 가능

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SpawnPoint")
	bool bIsUsed = false; // 스폰 포인트가 사용되었는지 여부

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SpawnPoint")
	UStaticMeshComponent* SpawnPointMesh; // 스태틱 메시 컴포넌트
};