// Fill out your copyright notice in the Description page of Project Settings.


#include "P_Settings/P_GameManager.h"


// Sets default values
AP_GameManager::AP_GameManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AP_GameManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AP_GameManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AP_GameManager::SelectPlayer()
{
	//GetAllCharacters();
//
	//// 플레이어 배열이 비어 있는지 확인
	//if (AllPlayers.IsEmpty()) return;
	//
	//// Curse 유효성 검사
	//if (!Curse || !Curse->IsValidLowLevel()) return;
//
	//// 랜덤 인덱스 선택
	//int32 RandomIndex = UKismetMathLibrary::RandomIntegerInRange(0, AllPlayers.Num() - 1);
	//// 선택된 플레이어
	//AActor* SelectedPlayer = AllPlayers[RandomIndex];
	//
	//if (SelectedPlayer)
	//{
	//	// 저주 시작
	//	Curse->GetInstance(GetWorld())->StartCurseTimer(SelectedPlayer);
	//}
	//GetWorld()->GetTimerManager().ClearTimer(CurseSelectedTimer);
}

