#include "NSK/NSK_AltarHintPoint.h"
#include "Components/StaticMeshComponent.h"

ANSK_AltarHintPoint::ANSK_AltarHintPoint()
{
	// �⺻ �޽� ���� ����
	HintPointMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HintPointMesh"));
	RootComponent = HintPointMesh;

	// �޽� �⺻ ������ ����
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