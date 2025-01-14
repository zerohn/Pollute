// Fill osut your copyright notice in the Description page of Project Settings.


#include "HHR/HHR_ItemManager.h"

#include "TimerManager.h"
#include "Engine/World.h"
#include "HHR/HHR_Item.h"
#include "Blueprint/UserWidget.h"
#include "HHR/HHR_Gun.h"
#include "HHR/HHR_Knife.h"
#include "HHR/UI/HHR_PlayerHUD.h"
#include "Kismet/GameplayStatics.h"
#include "LCU/Player/LCU_PlayerCharacter.h"
#include "Net/UnrealNetwork.h"



AHHR_ItemManager::AHHR_ItemManager()
{
	LoadItemData();
}

void AHHR_ItemManager::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    //DOREPLIFETIME(AHHR_ItemManager, GeneratedItems);
    //DOREPLIFETIME(AHHR_ItemManager, ItemsDataIdx);
}



void AHHR_ItemManager::BeginPlay()
{
    Super::BeginPlay();

    //if(!HasAuthority()) return;
    //ServerRPC_GenerateItem();
    FVector pivot = GetActorLocation();
    pivot.Z += 50.f;
    float loc = 150;
    if(HasAuthority())
    {
        for(const TPair<int32, FItemData>& Pair : ItemDataMap)
        {
            AHHR_Item* item = nullptr;
            // Combine Item만 생성
            if(HasAuthority())
            {
                if(Pair.Value.ItemType == EItemType::CombineItem)
                {
                    // 생성만 적용 되는 듯
                    pivot.Y += loc;
                    item = GetWorld()->SpawnActor<AHHR_Item>(ItemClass, pivot, GetActorRotation());
                    item->SetItemData(Pair.Value);
                    NetMulticast_SetData(item, Pair.Key);
                    
                    /*GeneratedItems.Add(item);
                    ItemsDataIdx.Add(Pair.Key);*/
                }
                else if(Pair.Value.ItemType == EItemType::WeaponItem)
                {
                    if(Pair.Value.ItemName.ToString() == FString("Knife"))
                    {
                        pivot.Y += loc;
                        item = GetWorld()->SpawnActor<AHHR_Knife>(KnifeItemClass, pivot, GetActorRotation());
                        item->SetItemData(Pair.Value);
                        NetMulticast_SetData(item, Pair.Key);
                        /*GeneratedItems.Add(item);
                        ItemsDataIdx.Add(Pair.Key);*/
                    }
                    else
                    {
                        pivot.Y += loc;
                        item = GetWorld()->SpawnActor<AHHR_Gun>(GunItemClass, pivot, GetActorRotation());
                        item->SetItemData(Pair.Value);
                        NetMulticast_SetData(item, Pair.Key);
                        /*GeneratedItems.Add(item);
                        ItemsDataIdx.Add(Pair.Key);*/
                    }

                }
            }

            //loc += 150;
            //NetMulticast_SetData(item, Pair.Value);
        }
    }


    // Array에 있는 객체들 data 넣어줌
    /*FTimerHandle TimerHandle;
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([&]()
    {
        for(int32 i = 0; i < GeneratedItems.Num(); i++)
        {
            GeneratedItems[i]->SetItemData(ItemDataMap[ItemsDataIdx[i]]);
            GeneratedItems[i]->PlayerHUD = TestPlayerHUDIns;
        }
    }), 0.5f, false);*/



    
    /*int i = 0;
    for(const TPair<int32, FItemData>& Pair : ItemDataMap)
    {
        if(i >= ItemActors.Num()) break;
        
        AHHR_Item* item = Cast<AHHR_Item>(ItemActors[i++]);
        if(item)
        {
            if(item->GetName().Contains(Pair.Value.ItemName.ToString()))
            {
                // Item Data Setting
                item->SetItemData(Pair.Value);
                item->PlayerHUD = TestPlayerHUDIns;
                break;
            }
            else
            {
                item->SetItemData(Pair.Value);
                item->PlayerHUD = TestPlayerHUDIns;
                break;
            }

        }

    }*/


	
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
	UDataTable* ItemDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Script/Engine.DataTable'/Game/HHR/Item/Data/ItemDataTable.ItemDataTable'"));
	
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



void AHHR_ItemManager::NetMulticast_SetData_Implementation(class AHHR_Item* Item, int32 idx)
{
    Item->ItemManager = this;
    Item->DataIdx = idx;
    
}
