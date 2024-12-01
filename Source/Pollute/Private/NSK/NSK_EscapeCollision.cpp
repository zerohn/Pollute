#include "NSK/NSK_EscapeCollision.h"
#include "GameFramework/Character.h"

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
    if (Cast<ACharacter>(OtherActor))
    {
        P_LOG(PolluteLog, Warning, TEXT("Escape triggered! Executing sequence..."));
        // TODO: 시퀀스 실행 및 탈출 처리
    }
}

