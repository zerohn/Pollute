#include "NSK/NSK_UseParachute.h"

ANSK_UseParachute::ANSK_UseParachute()
{
    UseParachute = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("UseParachute"));
    UseParachute->SetupAttachment(RootComponent);
    UseParachute->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}