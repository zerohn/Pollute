// Fill out your copyright notice in the Description page of Project Settings.


#include "LCU/Animation/LCU_MonsterAnim.h"

#include "LCU/Player/LCU_MonsterCharacter.h"

void ULCU_MonsterAnim::NativeUpdateAnimation(float DeltaTime)
{
    Super::NativeUpdateAnimation(DeltaTime);

    if (LCU_monster)
    {
        // 캐릭터의 속도를 가져옴
        FVector Velocity = LCU_monster->GetVelocity();
        
        // XY 평면의 속도 크기를 계산
        FVector Velocity2D = FVector(Velocity.X, Velocity.Y, 0.0f);
        GroundSpeed = Velocity2D.Size();
        
        // 움직임 상태 업데이트 (GroundSpeed가 특정 값 이상일 때만 True)
        bIsMove = GroundSpeed > 3.0f; // 임계값은 3.0f로 설정


        // Transform을 이용하여 월드 속도를 로컬 좌표로 변환
        FVector LocalVelocity = LCU_monster->GetActorTransform().InverseTransformVector(Velocity);

        P_Horizontal = LocalVelocity.Y;
        P_Vertical = LocalVelocity.X;
    }
    else
    {
        P_Horizontal = 0.f;
        P_Vertical = 0.f;
        Direction = 0.f;
        GroundSpeed = 0.0f; // 캐릭터가 없을 때 기본값
        bIsMove = false;
    }
}

void ULCU_MonsterAnim::NativeInitializeAnimation()
{
    Super::NativeInitializeAnimation();

    APawn* pawn = TryGetPawnOwner();
    if(!pawn) return;
    ALCU_MonsterCharacter* MonsterCharacter = Cast<ALCU_MonsterCharacter>(pawn);
    if(!MonsterCharacter) return;
    LCU_monster = MonsterCharacter;
}