#include "NSK/NSK_EscapeCollision.h"
#include "GameFramework/Character.h"
#include <LCU/Player/LCU_PlayerController.h>
#include <LCU/Player/LCU_PlayerCharacter.h>

ANSK_EscapeCollision::ANSK_EscapeCollision()
{
    EscapeTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("EscapeTrigger"));
    RootComponent = EscapeTrigger;

    EscapeTrigger->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    EscapeTrigger->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
    EscapeTrigger->SetCollisionResponseToAllChannels(ECR_Overlap);

    EscapeTrigger->OnComponentBeginOverlap.AddDynamic(this, &ANSK_EscapeCollision::OnOverlapBegin);
}

void ANSK_EscapeCollision::BeginPlay()
{
	Super::BeginPlay();
}

void ANSK_EscapeCollision::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    // 캐릭터인지 확인
    ACharacter* OverlappingCharacter = Cast<ALCU_PlayerCharacter>(OtherActor);
    if (OverlappingCharacter)
    {
        P_LOG(PolluteLog, Warning, TEXT("Escape triggered by: %s"), *OverlappingCharacter->GetName());

        // TODO: 캐릭터를 탈출 처리 상태로 설정
        if (ALCU_PlayerController* PlayerController = Cast<ALCU_PlayerController>(OverlappingCharacter->GetController()))
        {
            P_LOG(PolluteLog, Warning, TEXT("PlayerController found: %s"), *PlayerController->GetName());

            if (PlayerController->IsLocalController())
            {
                PlayerController->ServerRPC_ChangeToSpector();
            }

            // 모두 스펙터 모드 시 -> 게임 로비로 이동
        }
    }
}
