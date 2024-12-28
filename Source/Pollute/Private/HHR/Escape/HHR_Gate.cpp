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
#include "Engine/Engine.h"
#include "LCU/Player/LCU_PlayerCharacter.h"
#include "LCU/Player/LCU_PlayerController.h"

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
    LeftDoorMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Left Door Mesh Comp"));
    LeftDoorMeshComp->SetupAttachment(FenceMeshComp);
    static ConstructorHelpers::FObjectFinder<UStaticMesh> lDoorMesh(TEXT("/Script/Engine.StaticMesh'/Game/Horror_Mansion/Models/Environment/SM_Gate_Door_02.SM_Gate_Door_02'"));
    if(lDoorMesh.Succeeded())
    {
        LeftDoorMeshComp->SetStaticMesh(lDoorMesh.Object);
    }
    LeftDoorMeshComp->SetRelativeLocation(FVector(115.f, 0, 0));
    //
    RightDoorMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Right Door Mesh Comp"));
    RightDoorMeshComp->SetupAttachment(FenceMeshComp);
    static ConstructorHelpers::FObjectFinder<UStaticMesh> rDoorMesh(TEXT("/Script/Engine.StaticMesh'/Game/Horror_Mansion/Models/Environment/SM_Gate_Door_01.SM_Gate_Door_01'"));
    if(rDoorMesh.Succeeded())
    {
        RightDoorMeshComp->SetStaticMesh(rDoorMesh.Object);
    }
    RightDoorMeshComp->SetRelativeLocation(FVector(-115.f, 0, 0));
    //
    BoxColliderComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider Comp"));
    BoxColliderComp->SetupAttachment(FenceMeshComp);
    BoxColliderComp->SetRelativeLocation(FVector(0, 80.f, 100.f));
    BoxColliderComp->SetBoxExtent(FVector(200.f, 32.f, 100.f));
    // 충돌 함수 바인딩
    BoxColliderComp->OnComponentBeginOverlap.AddDynamic(this, &AHHR_Gate::OnComponentBeginOverlap);

    // Timeline Component 생성
    TimelineComp = CreateDefaultSubobject<UTimelineComponent>(TEXT("Timeline Comp"));
    // Curve Asset 찾아서 넣어주기
    static ConstructorHelpers::FObjectFinder<UCurveFloat> curveFloat(TEXT("/Script/Engine.CurveFloat'/Game/HHR/Miscellaneous/Cur_OpenDoor.Cur_OpenDoor'"));
    if(curveFloat.Succeeded())
    {
        OpenDoorCurve = curveFloat.Object;
    }

    
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


    // Timeline Setting
    //FTimeline OpenDoorTimeline;
    FOnTimelineFloat TimelineCallback;
    TimelineCallback.BindUFunction(this, FName("OpenDoorTimeline"));
    //FOnTimelineEvent TimelineFinishedCallback;

    if(OpenDoorCurve)
    {
        TimelineComp->AddInterpFloat(OpenDoorCurve, TimelineCallback);
        TimelineComp->SetTimelineLength(1.5f);
        TimelineComp->SetLooping(false);
    }

	
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
    // TODO : 문이 닫혀잇는지 열려 있는지 확인, 한번 열리면 왜 또 실행되지 않는걸까 
    NetMulticast_OpenDoor();
}

void AHHR_Gate::NetMulticast_OpenDoor_Implementation()
{
    P_LOG(PolluteLog, Warning, TEXT("NetMulticast Open"));
    // OpenDoorTimeline 실행 
    TimelineComp->Play();
}

// Timeline의 델리게이트에 바인딩 
void AHHR_Gate::OpenDoorTimeline(float value)
{
    // Timeline 실행될때 매 프레임마다 호출되는 함수
    // 회전
    RightDoorMeshComp->SetRelativeRotation(FRotator(0, value, 0));
    LeftDoorMeshComp->SetRelativeRotation(FRotator(0, -value, 0));
}

void AHHR_Gate::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

    // ! 가끔 충돌 처리안될때 bp 삭제하고 다시 하는 게 나음 -> 세팅 c++로 해놓는게 낫다
    // 플레이어 충돌시 sequence 실행(자기 자신만 보이면 됨)
    // ! 충돌한 pawn이 자기 자신 (firstController여야 함) 
    // sequence 싫행 후 spector 모드로 전환

    // 플레이어 확인 -> Spector 모드면 실행 안됨 
    ALCU_PlayerCharacter* PlayerCharacter = Cast<ALCU_PlayerCharacter>(OtherActor);
    if(PlayerCharacter)
    {
        // !pawn이여야 하고 그 pawn이 First Controller여야 Sequence 실행
        if(PlayerCharacter->GetController() == GetWorld()->GetFirstPlayerController())
        {
            // Sequence 실행 -> PlayerType에 따라 다른 시퀀스 실행 
            P_LOG(PolluteLog, Warning, TEXT("pawn owner : %s"), *PlayerCharacter->GetOwner()->GetName());
            // Sequence bp에 구현 
            PlayEnding(PlayerCharacter);
                
            // spector 모드로 전환
            ALCU_PlayerController* pc = Cast<ALCU_PlayerController>(PlayerCharacter->GetController());
            pc->ServerRPC_ChangeToSpector();
        }
    }
}




