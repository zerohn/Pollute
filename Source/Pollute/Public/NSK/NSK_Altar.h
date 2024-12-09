#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include <HHR/HHR_ItemData.h>
#include <HHR/HHR_Item.h>

#include "NSK_DoorController.h"
//#include <Components/BoxComponent.h>

#include "NSK_Altar.generated.h"

// 구체적인 멤버 함수 필요 없을 시 전방선언 ( 후에 헤더 정리 )
class UBoxComponent;
class UStaticMeshComponent;
class ALCU_PlayerCharacter;

UCLASS()
class POLLUTE_API ANSK_Altar : public AActor
{
    GENERATED_BODY()

public:
    ANSK_Altar();

protected:
    virtual void BeginPlay() override;

public:

    // **제단 메시**
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Altar")
    UStaticMeshComponent* AltarMesh;

    // **제단 이름**
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Altar")
    FName AltarName;

    // **제단 슬롯에 놓인 재료 데이터**
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Altar")
    TArray<FItemData> Slots;

    // **정답 데이터 (정문 열기 위해 필요한 재료)**
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Altar")
    TArray<FItemData> CorrectItems;

    // **정문 제어 클래스**
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Altar")
    ANSK_DoorController* DoorController;

    // **상호작용 트리거**
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Altar")
    UBoxComponent* InteractionBox;

    // **플레이어 상호작용 처리**
    UFUNCTION(BlueprintCallable, Category = "Altar")
    void HandlePlayerInteraction(class ALCU_PlayerCharacter* PlayerCharacter, int32 SlotIndex);



    // 슬롯에 아이템 추가
    UFUNCTION(BlueprintCallable, Category = "Altar")
    void AddItemToSlot(const FItemData& ItemData, int32 SlotID);

    // 슬롯 위치 배열
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Altar")
    TArray<UStaticMeshComponent*> SlotLocations;

    // 슬롯별 충돌 영역 추가
    UPROPERTY()
    TArray<UBoxComponent*> SlotCollisionBoxes;

    // 슬롯에 등록된 아이템 데이터 관리 배열
    UPROPERTY()
    TArray<FItemData> SlotItems;

    // 슬롯에 생성될 액터 아이템
    UPROPERTY()
    TArray<AHHR_Item*> SlotActorItems;

    // 플레이어 근접 상태
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Altar")
    bool bIsPlayerNearby;

    // G키 상호작용
    UFUNCTION()
    void OnInteract();

    // 슬롯별로 상태를 관리할 수 있도록 슬롯 ID 추적
    TMap<UBoxComponent*, int32> SlotCollisionMap;



    // 제단 Overlap 시작 이벤트 처리
    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    // 제단 Overlap 종료 이벤트 처리
    UFUNCTION()
    void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    // 슬롯 Overlap 시작
    UFUNCTION()
    void OnSlotBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    // 슬롯 Overlap 종료
    UFUNCTION()
    void OnSlotEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    // 플레이어가 가장 가까운 슬롯을 찾는 기능
    UFUNCTION()
    int32 FindClosestSlot(const FVector& Location);

    // 현재 상호작용 중인 슬롯
    int32 CurrentSlotIndex;

    // 슬롯 아이템 제거
    AHHR_Item* RemoveItemFromSlot(int32 SlotIndex);

private:
    // **슬롯 확인 함수 (정답 여부 판단)**
    void CheckSlots();

    // **슬롯 위치에 아이템 배치**
    void PlaceItemInSlot(const FItemData& ItemMesh, int32 SlotIndex);

    // **성공 메시지 출력**
    void ShowSuccessMessage();

    // **실패 메시지 출력**
    void ShowFailureMessage();
};
