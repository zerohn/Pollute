#include "NSK/NSK_Altar.h"
#include "Components/StaticMeshComponent.h"

ANSK_Altar::ANSK_Altar()
{
	PrimaryActorTick.bCanEverTick = true;

	// ���� �޽� �ʱ�ȭ
	AltarMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AltarMesh"));
	RootComponent = AltarMesh;

	// �޽� �⺻ ������ ����
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