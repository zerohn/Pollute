// Fill out your copyright notice in the Description page of Project Settings.


#include "HHR/HHR_ItemManager.h"

#include "Engine/World.h"
#include "HHR/HHR_Item.h"
#include "Blueprint/UserWidget.h"
#include "HHR/HHR_TestPlayerHUD.h"

AHHR_ItemManager::AHHR_ItemManager()
{
	LoadItemData();
}

void AHHR_ItemManager::BeginPlay()
{
	Super::BeginPlay();

	float loc = 0;
	// Combine Item만 임시 생성
	for(const TPair<int32, FItemData>& Pair : ItemDataMap)
	{
		// Combine Item만 생성
		if(Pair.Value.ItemType == EItemType::CombineItem)
		{
			AHHR_Item* Item = GetWorld()->SpawnActor<AHHR_Item>(AHHR_Item::StaticClass(), FVector(0, loc, 50), GetActorRotation());
			loc += 150;

			if(Item)
			{
				// Item Data Setting
				Item->SetItemData(Pair.Value);
				// TODO : 수정
				TestPlayerHUDIns = CreateWidget<UHHR_TestPlayerHUD>(GetWorld()->GetFirstPlayerController(), PlayerHUDClass);
				
				TestPlayerHUDIns->AddToViewport();
				
				Item->TestPlayerHUD = TestPlayerHUDIns;
			}
		}
	}

	
}

void AHHR_ItemManager::LoadItemData(UDataTable* ItemDataTable)
{
	if(ItemDataTable)
	{
		FString ContextString;
		TArray<FItemData*> AllItems;
		ItemDataTable->GetAllRows(ContextString, AllItems);

		for(FItemData* ItemData : AllItems)
		{
			ItemDataMap.Add(ItemData->ItemID, *ItemData);
		}
	}
}

void AHHR_ItemManager::LoadItemData()
{
	// 직접 로드해서 가져오기
	UDataTable* ItemDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Script/Engine.DataTable'/Game/HHR/Item/ItemDataTable.ItemDataTable'"));
	
	if(!ItemDataTable)
	{
		P_LOG(PolluteLog, Log, TEXT("ItemTable 로드 실패"));
		return;
	}

	// Data Table 경로 
	FString ContextString;

	TArray<FItemData*> AllItems;
	ItemDataTable->GetAllRows(ContextString, AllItems);
	
	for(FItemData* ItemData : AllItems)
	{
		ItemDataMap.Add(ItemData->ItemID, *ItemData);
	}

}

FItemData& AHHR_ItemManager::GetItemDataByID(int32 ItemID)
{
	// id로 검색
	if(ItemDataMap.Contains(ItemID))
	{
		return ItemDataMap[ItemID];
	}
	else return ItemDataMap[0];
}
