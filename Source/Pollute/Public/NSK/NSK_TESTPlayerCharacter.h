#pragma once

#include "CoreMinimal.h"
#include "LCU/Player/LCU_PlayerCharacter.h"
#include "HHR/HHR_ItemData.h"
#include "NSK_Altar.h"
#include "NSK_TESTPlayerCharacter.generated.h"

UCLASS()
class POLLUTE_API ANSK_TESTPlayerCharacter : public ALCU_PlayerCharacter
{
	GENERATED_BODY()
	
public:
    ANSK_TESTPlayerCharacter();

protected:
    virtual void BeginPlay() override;

public:
    // 플레이어 상호작용 로직 (예: G키로 제단과 상호작용)
    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    UInputAction* IA_G;

    // G키 상호작용 처리 함수
    void OnInteract();

    // 아이템 초기화
    void ResetHeldItem();

    // 들고 있는 아이템 반환
    FItemData GetHeldItem() const;

    // 근처 제단 설정 함수
    void SetNearbyAltar(ANSK_Altar* Altar);

private:
    // 현재 들고 있는 아이템 데이터
    FItemData HeldItem;

    // 근처의 제단 참조
    ANSK_Altar* NearbyAltar;
};
