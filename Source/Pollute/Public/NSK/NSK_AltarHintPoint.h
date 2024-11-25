#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "NSK_AltarHintPoint.generated.h"

UCLASS()
class POLLUTE_API ANSK_AltarHintPoint : public AActor
{
	GENERATED_BODY()
	
public:	
    ANSK_AltarHintPoint();

protected:
	virtual void BeginPlay() override;

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HintPoint")
    FName HintName; // 힌트 이름

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HintPoint")
	bool bHintPointIsUsed = false; // 스폰 포인트가 사용되었는지 여부

	UFUNCTION(BlueprintCallable, Category = "HintPoint") // 블프에서 가시성을 관리해야 하는 경우
	void HideHintPointMesh(); // 게임 시작 시 스폰 포인트 메시를 제거하는 함수

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HintPoint")
	UStaticMeshComponent* HintPointMesh; // 스태틱 메시 컴포넌트
};