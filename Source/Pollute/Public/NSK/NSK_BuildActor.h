#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HHR/HHR_Item.h"

#include "NSK_BuildActor.generated.h"

UCLASS()
class POLLUTE_API ANSK_BuildActor : public AHHR_Item
{
	GENERATED_BODY()
	
public:	
    ANSK_BuildActor();

protected:
	virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    // 설치될 사다리의 스태틱 메시 컴포넌트 : HHR_Item.h ItemMehsComp

    // 설치 가능한지 확인하는 변수
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ladder")
    bool bCanSpawn;

    // BP 머티리얼 설정
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ladder")
    UMaterialInterface* RedMaterial;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ladder")
    UMaterialInterface* GreenMaterial;

    // 설치 가능 여부를 체크하는 함수
    UFUNCTION(BlueprintCallable, Category = "Ladder")
    void CheckSpawn();
};
