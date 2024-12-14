// Fill out your copyright notice in the Description page of Project Settings.


#include "LCU/Animation/LCU_HumanAnim.h"

#include "HHR/HHR_Gun.h"
#include "HHR/HHR_KnifeItem.h"
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

	    // Transform을 이용하여 월드 속도를 로컬 좌표로 변환
	    FVector LocalVelocity = LCU_player->GetActorTransform().InverseTransformVector(Velocity);

	    P_Horizontal = LocalVelocity.Y;
	    P_Vertical = LocalVelocity.X;

	    // Gun 있는지 체크
	    AHHR_Item* item = Cast<AHHR_Item>(LCU_player->GetItem());
	    ItemInHand = item;
	    if(item)
	    {
	        AHHR_Gun* gun = Cast<AHHR_Gun>(item);
	        AHHR_KnifeItem* knife = Cast<AHHR_KnifeItem>(item);
	        if(gun)
	        {
	            bIsGunInHand = true;
	        }
	        else if(knife)
	        {
	            bIsGunInHand = false;
	            bIsItemInHand = false;
	        }
            else
            {
                // 일반 아이템
                bIsGunInHand = false;
                bIsItemInHand = true;
            }
	    }
        else
        {
            bIsItemInHand = false;
            bIsGunInHand = false;
        }

	}
	else
	{
	    P_Horizontal = 0.f;
	    P_Vertical = 0.f;
		GroundSpeed = 0.0f; // 캐릭터가 없을 때 기본값
		bIsMove = false;
	}
}
