// Fill out your copyright notice in the Description page of Project Settings.


#include "LCU/InteractActors/LCU_Curse.h"

#include "Engine/Engine.h"
#include "Engine/World.h"
#include "LCU/Player/LCU_PlayerCharacter.h"

ALCU_Curse* ALCU_Curse::Instance = nullptr;

ALCU_Curse* ALCU_Curse::GetInstance(UWorld* world)
{
	if(!Instance)
	{
		Instance = world->SpawnActor<ALCU_Curse>();
	}
	return Instance;
}

// Sets default values
ALCU_Curse::ALCU_Curse()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ALCU_Curse::BeginPlay()
{
	Super::BeginPlay();
	
	// 객체를 하나만 사용 할 수 있도록 미연에 방지한다.
	if (Instance && Instance != this)
	{
		Instance = nullptr; // 포인터 초기화
		Destroy();
	}
	else
	{
		Instance = this;
	}
}

void ALCU_Curse::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Instance = nullptr;
	
	Super::EndPlay(EndPlayReason);
}

// Called every frame
void ALCU_Curse::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 저주의 카운트 다운을 시작
	if(bStartCurseTime)
	{
		CurrentCurseTime -= DeltaTime;
		
		// 시간이 0이 되면 변신
		if(CurrentCurseTime <= 0.f)
		{
			CurrentCurseTime = EndCurseTime;
			bStartCurseTime = false;
			//TODO
			P_SCREEN(5.f, FColor::Cyan, TEXT("ChangeMonster"));
			P_LOG(PolluteLog, Log, TEXT("ChangeMonster"));
		}
	}
}

void ALCU_Curse::StartCurseTimer(AActor* player)
{
	if(!player)	return;
	// 오너 및 현재 소유 중인 플레이어를 캐싱
	Owner = player;

	OwnerCharacter = Cast<ALCU_PlayerCharacter>(player);
	OwnerCharacter->SetHasCurse(true);
	
	//P_SCREEN(5.f, FColor::Green, TEXT("SelectedPlayer"));
	P_LOG(PolluteLog, Log, TEXT("SelectedPlayer"));
	
	// 저주 시작
	bStartCurseTime = true;
}
