#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "NSK_LadderInstallPoint.generated.h"

UCLASS()
class POLLUTE_API ANSK_LadderInstallPoint : public AActor
{
	GENERATED_BODY()
	
public:	
    ANSK_LadderInstallPoint();

protected:
	virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, Category = "Component")
    class UBoxComponent* InstallCollision;

    UPROPERTY(VisibleAnywhere, Category = "Component")
    class UStaticMeshComponent* InstallMesh;

    UPROPERTY(EditDefaultsOnly, Category = "Materials")
    UMaterialInterface* GreenMaterial;

    UPROPERTY(EditDefaultsOnly, Category = "Materials")
    UMaterialInterface* RedMaterial;

public:	

    UFUNCTION()
    void OnOverlapBegin(
        UPrimitiveComponent* OverlappedComp,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult& SweepResult);

    UFUNCTION()
    void OnOverlapEnd(
        UPrimitiveComponent* OverlappedComponent,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex);

    // 설치 가능 여부를 외부에서 가져갈 수 있게 설정
    bool IsInstallable() const { return bCanInstallLadder; }

    // 설치 지점 방향을 설정하는 변수 추가
    UPROPERTY(EditDefaultsOnly, Category = "Ladder")
    FRotator LadderInstallRotation = FRotator(0.f, 0.f, 0.f);

    // 사다리 설치 함수
    void InstallLadder(ALCU_PlayerCharacter* Player);

    // 키로 상호작용 시 사다리 설치
    void SetupInteraction();

    // 플레이어가 설치 지점 근처에 있는지 확인하는 변수
    UPROPERTY(BlueprintReadWrite, Category = "Ladder")
    bool bPlayerIsNear = false;

private:

    // 설치 가능 여부
    bool bCanInstallLadder;
};
