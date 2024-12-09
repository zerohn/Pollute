// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HHR_ItemData.h"
#include "GameFramework/Actor.h"
#include "HHR_ItemManager.generated.h"

/**
 * 
 */
UCLASS()
class POLLUTE_API AHHR_ItemManager : public AActor
{
	GENERATED_BODY()

public:
	AHHR_ItemManager();

protected:
	virtual void BeginPlay() override;
    virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

private:
    /*UPROPERTY(Replicated)
    class AHHR_Item* GeneratedItem;*/
    /*UPROPERTY(Replicated)
    TArray<class AHHR_Item*> GeneratedItems;
    UPROPERTY(Replicated)
    TArray<int32> ItemsDataIdx;*/
    

public:
	// 아이템 데이터 저장
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Items")
	TMap<int32, FItemData> ItemDataMap;
protected:
	// 생성할 아이템 클래스
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Items")
	TSubclassOf<class AHHR_Item> ItemClass;
	// 무기
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Items")
	TSubclassOf<class AHHR_KnifeItem> KnifeItemClass;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Items")
    TSubclassOf<class AHHR_Gun> GunItemClass;
	

// Functions
public:
	// DataTable에서 Item 데이터 로드
	void LoadItemData(UDataTable* ItemDataTable);
	void LoadItemData();

	// Item ID로 검색 (안전 검색, 걍 검색)
	FItemData& GetItemDataByID(int32 ItemID);

    // item data 넣어주는 것을 multicast
    UFUNCTION(Server, Reliable)
    void ServerRPC_GenerateItem();

    UFUNCTION(NetMulticast, reliable)
    void NetMulticast_SetData(class AHHR_Item* Item, int32 idx);
    


// 임시 Player HUD
// TODO : 수정
public:
	UPROPERTY()
	class UHHR_TestPlayerHUD* TestPlayerHUDIns;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI")
	TSubclassOf<class UHHR_TestPlayerHUD> PlayerHUDClass;

	
};
