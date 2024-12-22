#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NSK_BuildActor.h"

#include "NSK_LadderPlacementComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class POLLUTE_API UNSK_LadderPlacementComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UNSK_LadderPlacementComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    // 사다리 설치 가능 여부 확인 함수
	UFUNCTION(BlueprintCallable, Category="LadderPlacement")
    void CheckPlacement();

    // 설치를 위한 라인 트레이스 수행
    UFUNCTION(BlueprintCallable, Category="LadderPlacement")
    void TryPlaceLadder();

    // 설치할 사다리 액터
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="LadderPlacement")
    TSubclassOf<ANSK_BuildActor> LadderActorClass;

    // 설치된 사다리 액터
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="LadderPlacement")
    ANSK_BuildActor* SpawnedLadderActor;

    // 횔 회전용 변수
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LadderPlacement")
    float RotationSpeed;

    // 플레이어가 사다리를 설치할 수 있는지 여부
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="LadderPlacement")
    bool bCanPlaceLadder;

    // 마우스 휠 입력 받기
    void OnMouseWheelInput(float Value);
};
