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
	FName SpawnName; // ID를 통해 개별 관리 가능

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ItemSpawnPoint")
	bool bSpawnPointIsUsed = false; // 스폰 포인트가 사용되었는지 여부

	UFUNCTION(BlueprintCallable, Category = "ItemSpawnPoint") // 블프에서 가시성을 관리해야 하는 경우
	void HideSpawnPointMesh(); // 게임 시작 시 스폰 포인트 메시를 제거하는 함수

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ItemSpawnPoint")
	UStaticMeshComponent* SpawnPointMesh; // 스태틱 메시 컴포넌트
};