// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HHR_ItemData.h"
#include "HHR_ItemSpawnManager.generated.h"



// Item data load, Item spawn, hint spawn
UCLASS()
class POLLUTE_API AHHR_ItemSpawnManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHHR_ItemSpawnManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

// TODO : UIManager로 옮기고 수정 
public:
    UPROPERTY()
    class UHHR_PlayerHUD* TestPlayerHUDIns;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI")
    TSubclassOf<class UHHR_PlayerHUD> PlayerHUDClass;


protected:
    // 생성할 아이템들 클래스
    UPROPERTY(EditDefaultsOnly, Category="Items")
    TSubclassOf<class AHHR_Item> ItemBaseClass;
    UPROPERTY(EditDefaultsOnly, Category="Items")
    TSubclassOf<class AHHR_KnifeItem> KnifeClass;
    UPROPERTY(EditDefaultsOnly, Category="Items")
    TSubclassOf<class AHHR_Gun> GunClass;

private:
    // Data table의 데이터 저장
    // 아이템 데이터 저장
    // Weapon, Escape, Combine 따로 저장 
    UPROPERTY(VisibleDefaultsOnly, Category="Items")
    TMap<int32, FItemData> CombineItemDataMap;
    UPROPERTY(VisibleDefaultsOnly, Category="Items")
    TMap<int32, FItemData> WeaponItemDataMap;
    UPROPERTY(VisibleDefaultsOnly, Category="Items")
    TMap<int32, FItemData> EscapeItemDataMap;

    // Spawn point 저장
    UPROPERTY(VisibleDefaultsOnly, Category="Items")
    TArray<class AHHR_ItemSpawnPoint*> SpawnPoints;
    // 랜던 Spawn point index
    //TArray<int32> RandomSpawnPointIdx;

private: 
    // Setting 변수들
    UPROPERTY(EditDefaultsOnly, Category="Settings", meta=(EditCondition="true"))
    int32 MaxAltarItem = 4;
    UPROPERTY(EditDefaultsOnly, Category="Settings", meta=(EditCondition="true"))
    int32 MaxCombineItem = 8;
    UPROPERTY(EditDefaultsOnly, Category="Settings", meta=(EditCondition="true"))
    int32 MaxWeaponItem = 2;
    UPROPERTY(EditDefaultsOnly, Category="Settings", meta=(EditCondition="true"))
    int32 MaxEscapeItem = 2;
    UPROPERTY(EditDefaultsOnly, Category="Settings", meta=(EditCondition="true"))
    int32 MaxSpawnItem = 15;
    

// ###### Functions #######
public:
    // 데이터 관련 함수들
    // Data load
    void LoadItemData(UDataTable* ItemDataTable);
    void LoadItemData();
    // Id를 통한 데이터 검색 (안전 검색, 걍 검색)
    FItemData& GetItemDataByID(int32 ItemID);
    // key 값의 아이템을 넣어줌

private:
    // Spawn Item Random 생성
    void SpawnRandomItem();

    // 레벨에 배치된 spawnpoint 찾아와서 저장
    void FindSpawnPoints();

    // shuffle로 인덱스 랜덤 뽑기
    void ShuffleIdx(TArray<int32> &OutRandomIdx);
    


    



    

};
