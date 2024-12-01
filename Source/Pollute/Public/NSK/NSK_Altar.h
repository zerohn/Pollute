#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <HHR/HHR_ItemData.h>
#include "NSK_DoorController.h"
#include <Components/BoxComponent.h>

#include "NSK_Altar.generated.h"

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

    // **슬롯 위치들**
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Altar")
    TArray<USceneComponent*> SlotLocations;

    // **상호작용 트리거**
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Altar")
    UBoxComponent* InteractionBox;

    // **플레이어 상호작용 처리**
    UFUNCTION(BlueprintCallable, Category = "Altar")
    void HandlePlayerInteraction(class ANSK_TESTPlayerCharacter* PlayerCharacter);

    // **슬롯에 아이템 추가**
    UFUNCTION(BlueprintCallable, Category = "Altar")
    void AddItemToSlot(FItemData Item);

    // **G키 상호작용**
    UFUNCTION()
    void OnInteract();

    // **Overlap 시작 이벤트 처리**
    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    // **Overlap 종료 이벤트 처리**
    UFUNCTION()
    void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
    // **슬롯 확인 함수 (정답 여부 판단)**
    void CheckSlots();

    // **슬롯 위치에 아이템 배치**
    void PlaceItemInSlot(const FItemData& Item, int32 SlotIndex);

    // 슬롯 아이템 제거
    void RemoveItemFromSlot();

    // **성공 메시지 출력**
    void ShowSuccessMessage();

    // **실패 메시지 출력**
    void ShowFailureMessage();
};
