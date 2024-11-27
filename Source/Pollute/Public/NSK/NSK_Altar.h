#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <HHR/HHR_ItemData.h>
#include "NSK_DoorController.h"

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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Altar") // 블프에서 제단 메시를 설정
	UStaticMeshComponent* AltarMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Altar") // 제단의 이름
	FName AltarName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Altar")
    TArray<FItemData> Slots; // 슬롯에 놓인 재료 데이터

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Altar")
    TArray<FItemData> CorrectItems; // 정답 데이터 (정문을 열기 위해 필요한 아이템)

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Altar")
    ANSK_DoorController* DoorController; // 정문 제어 클래스

    void AddItemToSlot(FItemData Item);

private:
    void CheckSlots(); // 슬롯 확인 함수
};
