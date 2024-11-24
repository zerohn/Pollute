#include "NSK/NSK_Altar.h"
#include "Components/StaticMeshComponent.h"

ANSK_Altar::ANSK_Altar()
{
	PrimaryActorTick.bCanEverTick = true;

	// 제단 메시 초기화
	AltarMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AltarMesh"));
	RootComponent = AltarMesh;

	// 메시 기본 스케일 변경
	FVector DefaultScale(1.0f, 1.0f, 1.0f);
	AltarMesh->SetRelativeScale3D(DefaultScale);
}

void ANSK_Altar::BeginPlay()
{
	Super::BeginPlay();
}

void ANSK_Altar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}