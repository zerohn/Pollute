#include "NSK/NSK_AltarHintPoint.h"
#include "Components/StaticMeshComponent.h"

ANSK_AltarHintPoint::ANSK_AltarHintPoint()
{
	// 기본 메시 블프 생성
	HintPointMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HintPointMesh"));
	RootComponent = HintPointMesh;

	// 메시 기본 스케일 변경
	FVector DefaultScale(0.5f, 0.5f, 0.5f);
	HintPointMesh->SetRelativeScale3D(DefaultScale);
}

void ANSK_AltarHintPoint::BeginPlay()
{
	Super::BeginPlay();
}

void ANSK_AltarHintPoint::HideHintPointMesh()
{
	if (HintPointMesh)
	{
		HintPointMesh->DestroyComponent();
	}
}