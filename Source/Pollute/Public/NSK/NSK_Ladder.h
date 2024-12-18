#pragma once

#include "CoreMinimal.h"
#include "HHR/HHR_Item.h"
#include "NSK_Ladder.generated.h"

UCLASS()
class POLLUTE_API ANSK_Ladder : public AHHR_Item
{
	GENERATED_BODY()
	
public:
    ANSK_Ladder();

    // 사다리 상단 위치
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Ladder")
    USceneComponent* TopPosition;

    // 사다리 클래스 설치 상태 확인
    UPROPERTY(BlueprintReadOnly, Category = "Ladder")
    bool bIsInstalled = false;

    // 충돌 컴포넌트
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ladder")
    UBoxComponent* CollisionComponent;
};
