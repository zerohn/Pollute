// Fill out your copyright notice in the Description page of Project Settings.


#include "HHR/Escape/HHR_Gate.h"

#include "EngineUtils.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/StaticMesh.h"
#include "HHR/Escape/HHR_Altar.h"
// Timeline
#include "Components/TimelineComponent.h"

// Sets default values
AHHR_Gate::AHHR_Gate()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    // Component 생성 & 설정
    // mesh 할당
    FenceMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Fence Static Mesh Comp"));
    SetRootComponent(FenceMeshComp);
    static ConstructorHelpers::FObjectFinder<UStaticMesh> fenceMesh(TEXT("/Script/Engine.StaticMesh'/Game/Horror_Mansion/Models/Environment/SM_Fence_Gate.SM_Fence_Gate'"));
    if(fenceMesh.Succeeded())
    {
        FenceMeshComp->SetStaticMesh(fenceMesh.Object);
    }
    //
    RightDoorMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Right Door Mesh Comp"));
    RightDoorMeshComp->SetupAttachment(FenceMeshComp);
    static ConstructorHelpers::FObjectFinder<UStaticMesh> rDoorMesh(TEXT("/Script/Engine.StaticMesh'/Game/Horror_Mansion/Models/Environment/SM_Gate_Door_02.SM_Gate_Door_02'"));
    if(rDoorMesh.Succeeded())
    {
        RightDoorMeshComp->SetStaticMesh(rDoorMesh.Object);
    }
    RightDoorMeshComp->SetRelativeLocation(FVector(115.f, 0, 0));
    //
    LeftDoorMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Left Door Mesh Comp"));
    LeftDoorMeshComp->SetupAttachment(FenceMeshComp);
    static ConstructorHelpers::FObjectFinder<UStaticMesh> lDoorMesh(TEXT("/Script/Engine.StaticMesh'/Game/Horror_Mansion/Models/Environment/SM_Gate_Door_01.SM_Gate_Door_01'"));
    if(lDoorMesh.Succeeded())
    {
        LeftDoorMeshComp->SetStaticMesh(lDoorMesh.Object);
    }
    LeftDoorMeshComp->SetRelativeLocation(FVector(-115.f, 0, 0));
    //
    BoxColliderComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider Comp"));
    BoxColliderComp->SetupAttachment(FenceMeshComp);
    BoxColliderComp->SetRelativeLocation(FVector(0, 80.f, 100.f));
    BoxColliderComp->SetBoxExtent(FVector(200.f, 32.f, 100.f));

    // 복제 허용
    bReplicates = true;

}

// Called when the game starts or when spawned
void AHHR_Gate::BeginPlay()
{
	Super::BeginPlay();

    // 델리게이트 바인딩(서버만 바인딩 해주면 됨)
    // 레벨에 있는 Altar 클래스를 찾아서 바인딩 해줌 
    if(HasAuthority())
    {
        for(TActorIterator<AHHR_Altar> It(GetWorld()); It; ++It)
        {
            AHHR_Altar* altar = *It;
            if(altar)
            {
                P_LOG(PolluteLog, Warning, TEXT("OpenDoor Binding"));
                altar->OnOpenDoor.BindDynamic(this, &AHHR_Gate::OpenDoor);
            }
        }
    }

    FOnTimelineFloat TImelineCallback;
    FOnTimelineEvent TimelineFinishedCallback;
    FTimeline OpenDoorTimeline;
    // 델리게이트 설정 
    OpenDoorTimeline.SetTimelineFinishedFunc(TimelineFinishedCallback);
    
	
}

// Called every frame
void AHHR_Gate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Altar를 통해서 델리게이트로 호출 (server에서 호출됨)
void AHHR_Gate::OpenDoor()
{
    P_LOG(PolluteLog, Warning, TEXT("Open Door Delegate"));
    NetMulticast_Open();
}

void AHHR_Gate::NetMulticast_Open_Implementation()
{
    P_LOG(PolluteLog, Warning, TEXT("NetMulticast Open"));
    // TODO : 문 열어주는 로직 수행
    
    
}

