#include "NSK/NSK_ItemSpawnPoint.h"

ANSK_ItemSpawnPoint::ANSK_ItemSpawnPoint()
{

	// 스폰 포인트 기본 메시 생성 및 부모로 등록
	// 디폴트 서브오브젝트 이름 : SpawnPointMesh
	// 블프에서 설정
	SpawnPointMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SpawnPointMesh"));
	RootComponent = SpawnPointMesh;

	// 메시 기본 스케일 변경
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
