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


protected:
	// 아이템 데이터 저장
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Items")
	TMap<int32, FItemData> ItemDataMap;

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


// 임시 Player HUD
// TODO : 수정
public:
	UPROPERTY()
	class UHHR_TestPlayerHUD* TestPlayerHUDIns;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI")
	TSubclassOf<class UHHR_TestPlayerHUD> PlayerHUDClass;

	
};
