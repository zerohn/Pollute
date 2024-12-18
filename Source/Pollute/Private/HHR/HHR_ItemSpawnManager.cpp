// Fill out your copyright notice in the Description page of Project Settings.


#include "HHR/HHR_ItemSpawnManager.h"
#include "EngineUtils.h"    // TActorIterator

// uimanager로 옮기면 제거 
#include "Blueprint/UserWidget.h"
#include "Components/StaticMeshComponent.h"
#include "HHR/HHR_Gun.h"
#include "HHR/HHR_Hint.h"
#include "HHR/HHR_ItemSpawnPoint.h"
#include "HHR/HHR_Knife.h"
#include "Kismet/GameplayStatics.h"
#include "LCU/Player/LCU_PlayerCharacter.h"
#include "HHR/UI/HHR_PlayerHUD.h"

// Sets default values
AHHR_ItemSpawnManager::AHHR_ItemSpawnManager()
{
    // Item Data Table Load
    LoadItemData();   

}

// Called when the game starts or when spawned
void AHHR_ItemSpawnManager::BeginPlay()
{
	Super::BeginPlay();


    // Combine Item만 임시 생성
    //TestPlayerHUDIns = CreateWidget<UHHR_PlayerHUD>(GetWorld()->GetFirstPlayerController(), PlayerHUDClass);
    //TestPlayerHUDIns->AddToViewport();
    //// character에서 임시로 hud 생성
    //ALCU_PlayerCharacter* player = Cast<ALCU_PlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
    //player->PlayerHUD = TestPlayerHUDIns;


    if(!HasAuthority())
    {
        return;
    }
        
    // 아이템 spawn point 찾아오기
    FindSpawnPoints();
    // 게임 시작시 아이템 랜덤 생성, 아이템 힌트 생성
    SpawnRandomItem();
    // 힌트 생성
    SpawnHint();
	
}




void AHHR_ItemSpawnManager::LoadItemData(UDataTable* ItemDataTable)
{
    // 매개변수로 Table 받아서 저장 
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

void AHHR_ItemSpawnManager::LoadItemData()
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

FItemData& AHHR_ItemSpawnManager::GetItemDataByID(int32 ItemID)
{
    // id로 검색
    if(ItemDataMap.Contains(ItemID))
    {
        return ItemDataMap[ItemID];
    }
    else return ItemDataMap[0];
}

void AHHR_ItemSpawnManager::SpawnRandomItem()
{

    // 1. spawnpoint 들 중에 n 개( 8 + 4 + 3 : 15 개 선택 / spawnpoint : 25개)
    
    // 랜덤 위치 뽑아줌 
    TArray<int32> RandomSpawnPointIdx;
    // SpawnPoint에서 스폰되는 아이템 만큼 랜덤 indx 뽑아줌
    // TODO : 나중에 상수에서 MaxSpawnItem 으로 수정 
    ShuffleIdx(RandomSpawnPointIdx, SpawnPoints.Num(),14 );

    int32 spawnPointIdx = 0;
    int32 altarCnt = 0;
    // 아이템 생성 
    for(const TPair<int32, FItemData> &itemPair : ItemDataMap)
    {
        if(spawnPointIdx >= SpawnPoints.Num()) break;
        
        AHHR_Item* item = nullptr;
        if(itemPair.Value.ItemType == EItemType::CombineItem)
        {
            // 조합 아이템 생성
            item = GetWorld()->SpawnActor<AHHR_Item>(ItemBaseClass, SpawnPoints[RandomSpawnPointIdx[spawnPointIdx]]->GetActorLocation(), SpawnPoints[RandomSpawnPointIdx[spawnPointIdx]]->GetActorRotation());
            ++spawnPointIdx;
            ItemArray.Add(item);
            
            item->SetItemData(itemPair.Value);
            NetMuulticast_SetData(item, itemPair.Key);
        }
        else if(itemPair.Value.ItemType == EItemType::WeaponItem)
        {
            // 무기 아이템 생성 (칼 2개 / 총 2개)
            if(itemPair.Value.ItemName.ToString().Contains("Knife"))
            {
                for(int32 i = 0; i < MaxKinfe; ++i)
                {
                    item = GetWorld()->SpawnActor<AHHR_Knife>(KnifeClass, SpawnPoints[RandomSpawnPointIdx[spawnPointIdx]]->GetActorLocation(), SpawnPoints[RandomSpawnPointIdx[spawnPointIdx]]->GetActorRotation());
                    ++spawnPointIdx;
                    ItemArray.Add(item);
                    item->SetItemData(itemPair.Value);
                    NetMuulticast_SetData(item, itemPair.Key);
                }
            }
            else if(itemPair.Value.ItemName.ToString().Contains("Gun"))
            {
                for(int32 i = 0; i < MaxGun; ++i)
                {
                    item = GetWorld()->SpawnActor<AHHR_Gun>(GunClass, SpawnPoints[RandomSpawnPointIdx[spawnPointIdx]]->GetActorLocation(), SpawnPoints[RandomSpawnPointIdx[spawnPointIdx]]->GetActorRotation());
                    ++spawnPointIdx;
                    ItemArray.Add(item);
                    item->SetItemData(itemPair.Value);
                    NetMuulticast_SetData(item, itemPair.Key);
                }
            }
            
        }
        else if(itemPair.Value.ItemType == EItemType::EscapeItem)
        {
            // 탈출 아이템 생성 (사다리 : 1개 / 낙하산 : 2개)
            // TODO : 탈출 아이템 클래스로 생성, 개수 중복해서 생성 
            item = GetWorld()->SpawnActor<AHHR_Item>(ItemBaseClass, SpawnPoints[RandomSpawnPointIdx[spawnPointIdx]]->GetActorLocation(), SpawnPoints[RandomSpawnPointIdx[spawnPointIdx]]->GetActorRotation());
            ++spawnPointIdx;
            ItemArray.Add(item);
            item->SetItemData(itemPair.Value);
            NetMuulticast_SetData(item, itemPair.Key);
        }
    }
    
}

void AHHR_ItemSpawnManager::FindSpawnPoints()
{
    for(TActorIterator<AHHR_ItemSpawnPoint> it(GetWorld()); it; ++it)
    {
        SpawnPoints.Add(*it);
    }
}

void AHHR_ItemSpawnManager::SpawnHint()
{
    // 2. 제단 아이템 선택
    // 제단 세팅 동기화 -> Replicated 변수 이용
    // ! 조합 아이템만 제단 아이템으로 선택해줘야 함

    // 1) 조합 아이템만 뽑기
    TArray<AHHR_Item*> CombineItems;
    for(AHHR_Item* item: ItemArray)
    {
        if(item->ItemData.ItemType == EItemType::CombineItem)
        {
            CombineItems.Add(item);
        }
    }
    // 2) 랜덤 인덱스들 설정 
    TArray<int32> AltarItemIdx;
    ShuffleIdx(AltarItemIdx, CombineItems.Num(), MaxAltarItem );
    for(int32 idx : AltarItemIdx)
    {
        CombineItems[idx]->SetIsAltarItem(true);
        //P_LOG(PolluteLog, Warning, TEXT("%s"), *(ItemArray[idx]->ItemData.ItemName.ToString()));
    }

    // 3) 힌트 생성 
    for(AHHR_Item* cItem : CombineItems)
    {
        // key 값에 대응하는 hint 생성
        AHHR_Hint* hint = GetWorld()->SpawnActor<AHHR_Hint>(Hints[cItem->ItemData.ItemID], cItem->GetActorLocation(), cItem->GetActorRotation());
        hint->SetActorLocation(hint->GetHintSpawnTransform()->GetLocation());
        hint->SetActorRotation(hint->GetHintSpawnTransform()->GetRotation());

        // 제단 아이템 아니면 
        if(!cItem->GetIsAltarItem())
        {
            if(hint->GetIsHideHint())
            {
                // hint가 숨겨줘야 하면 삭제
                hint->Destroy();
            }
            else
            {
                // mesh 꺼주는 거 동기화 필요 -> 이것도 OnRep 함수 사용해서 나중에 동기화 처리
                hint->InvisiblePicture();
                hint->SetInvisiblePicture(true);
            }
        }
    }
    
    
}

void AHHR_ItemSpawnManager::ShuffleIdx(TArray<int32> &OutRandomIdx, int32 MaxNum, int32 RandomNum)
{
    // MaxNum 만큼 넣어줌 
    for(int32 i = 0; i < MaxNum; ++i)
    {
        OutRandomIdx.Add(i);
    }

    // Shuffle
    for(int32 i = 0; i < OutRandomIdx.Num(); ++i)
    {
        int32 SwapIdx = FMath::RandRange(0, OutRandomIdx.Num() - 1);
        OutRandomIdx.Swap(i, SwapIdx);
    }

    // shuffle로 섞인 것들 중에 RandomNum 만큼 빼줌
    if(OutRandomIdx.Num() >= RandomNum)
    {
        OutRandomIdx.SetNum(RandomNum);
    }
    
}

void AHHR_ItemSpawnManager::NetMulticast_InvisiblePicture_Implementation(class AHHR_Hint* hint)
{
    hint->InvisiblePicture();
}



void AHHR_ItemSpawnManager::NetMuulticast_SetData_Implementation(class AHHR_Item* Item, int32 idx)
{
    //Item->SetItemData(ItemDataMap[idx]);
    
    Item->ItemSpawnManager = this;
    Item->DataIdx = idx;
    
}

