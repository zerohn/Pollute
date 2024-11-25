// Fill out your copyright notice in the Description page of Project Settings.


#include "LCU/Animation/LCU_HumanAnim.h"

#include "LCU/Player/LCU_PlayerCharacter.h"


void ULCU_HumanAnim::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	APawn* pawn = TryGetPawnOwner();
	if(!pawn) return;
	ALCU_PlayerCharacter* playerCharacter = Cast<ALCU_PlayerCharacter>(pawn);
	if(!playerCharacter) return;
	LCU_player = playerCharacter;

	Gender = LCU_player->GetGender();
}

void ULCU_HumanAnim::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);
	
	if (LCU_player)
	{
		// 캐릭터의 속도를 가져옴
		FVector Velocity = LCU_player->GetVelocity();
        
		// XY 평면의 속도 크기를 계산
		FVector Velocity2D = FVector(Velocity.X, Velocity.Y, 0.0f);
		GroundSpeed = Velocity2D.Size();
		
		// 움직임 상태 업데이트 (GroundSpeed가 특정 값 이상일 때만 True)
		bIsMove = GroundSpeed > 3.0f; // 임계값은 3.0f로 설정
	}
	else
	{
		GroundSpeed = 0.0f; // 캐릭터가 없을 때 기본값
		bIsMove = false;
	}
}
