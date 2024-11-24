#include "NSK/NSK_ItemSpawnPoint.h"

ANSK_ItemSpawnPoint::ANSK_ItemSpawnPoint()
{

	// ���� ����Ʈ �⺻ �޽� ���� �� �θ�� ���
	// ����Ʈ ���������Ʈ �̸� : SpawnPointMesh
	// �������� ����
	SpawnPointMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SpawnPointMesh"));
	RootComponent = SpawnPointMesh;

	// �޽� �⺻ ������ ����
	FVector DefaultScale(0.5f, 0.5f, 0.5f);
	SpawnPointMesh->SetRelativeScale3D(DefaultScale);
}

void ANSK_ItemSpawnPoint::BeginPlay()
{
	Super::BeginPlay();
}

void ANSK_ItemSpawnPoint::HideSpawnPointMesh()
{
	if (SpawnPointMesh)
	{
		SpawnPointMesh->DestroyComponent();
	}
}
