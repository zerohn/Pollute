// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HHR_Altar.generated.h"

class AHHR_Item;

UCLASS()
class POLLUTE_API AHHR_Altar : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHHR_Altar();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
    virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


// #### Variables ####
// *Components*
protected:
    UPROPERTY(EditDefaultsOnly, Category = "Components")
    class UStaticMeshComponent* AltarStaticMeshComp;
    UPROPERTY(EditDefaultsOnly, Category = "Components")
    class USphereComponent* SphereCollisionComp;


private:
    // Item들이 배치될 pos들
    UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess), Category = "ItemsLocation")
    TArray<FVector> ItemAttachPos;
    
    // 배치되어 있는 Item ins들
    TArray<AHHR_Item*> AttachedItems;
 
// *아이템 배치에 사용할 변수들*
private:
    int32 MaxItemCnt;
    // 현재 Item cnt는 동기화 되어야 하는 변수
    UPROPERTY()
    int32 CurrentItemCnt;

    

// #### Functions ####
// *RPC 함수*
public:
    // multicast 함수
    UFUNCTION(NetMulticast, Reliable)
    void NetMulticast_AttachToAltar(AHHR_Item* Item);

    UFUNCTION(NetMulticast, Reliable)
    void NetMulticast_Update(int32 idx);
    
    
// *충돌 처리 함수*
protected:
    UFUNCTION()
    void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                    bool bFromSweep, const FHitResult & SweepResult);
    UFUNCTION()
    void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

// *델리게이트에 바인딩 될 함수
protected:
    // Player가 G키 누르면 호출
    UFUNCTION()
    void OnAttachItem(AHHR_Item* Item);
    // Player가 Detatch 하면 델리게이트 실행
    UFUNCTION()
    void OnDetatchITem(AHHR_Item* Item);


// *걍 내부 함수*
private:
    int32 FindItemIdx(AHHR_Item* Item);

    int32 FindAttachIdx();

};



