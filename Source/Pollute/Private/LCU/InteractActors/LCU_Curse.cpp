// Fill out your copyright notice in the Description page of Project Settings.


#include "LCU/InteractActors/LCU_Curse.h"

#include "Engine/Engine.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "LCU/Player/LCU_PlayerCharacter.h"
#include "LCU/Player/LCU_PlayerController.h"
#include "Net/UnrealNetwork.h"
#include "P_Settings/P_GameState.h"

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

	if(!HasAuthority()) return;
	
	// 저주의 카운트 다운을 시작
	if(bStartCurseTime && OwnerCharacter)
	{
		CurrentCurseTime -= DeltaTime;
		
		// 시간이 0이 되면 변신
		if(CurrentCurseTime <= 0.f)
		{
			CurrentCurseTime = EndCurseTime;
			bStartCurseTime = false;
			
			ALCU_PlayerController* P_pc =  Cast<ALCU_PlayerController>(OwnerCharacter->GetController());
			if(P_pc)
			{
				P_pc->ServerRPC_ChangeToMonster();
			}
			
			AP_GameState* P_GS =  Cast<AP_GameState>(UGameplayStatics::GetGameState(GetWorld()));
			
			// 다시 저주를 시작합니다.
			if(!P_GS) return;
			P_GS->SelectCursePlayer();
		}
	}
}

void ALCU_Curse::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALCU_Curse, OwnerCharacter);
}

void ALCU_Curse::InitCurseTime()
{
	CurrentCurseTime = EndCurseTime;
}

void ALCU_Curse::SetCharacter(ALCU_PlayerCharacter* character)
{
	OwnerCharacter = character;
	Owner = character;
}

void ALCU_Curse::StartCurseTimer(ALCU_PlayerCharacter* player)
{	
	if(!player)	return;
	// 오너 및 현재 소유 중인 플레이어를 캐싱
	Owner = player;

	OwnerCharacter = player;
	OwnerCharacter->SetHasCurse(true);
		
	// 저주 시작
	bStartCurseTime = true;
}
