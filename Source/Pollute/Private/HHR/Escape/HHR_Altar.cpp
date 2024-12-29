// Fill out your copyright notice in the Description page of Project Settings.


#include "HHR/Escape/HHR_Altar.h"

#include "EngineUtils.h"
#include "TimerManager.h"
#include "HHR/HHR_Item.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Debugging/SlateDebugging.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "LCU/InteractActors/LCU_Curse.h"
#include "LCU/Player/LCU_PlayerCharacter.h"
#include "LCU/Player/LCU_PlayerController.h"
#include "Net/UnrealNetwork.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
AHHR_Altar::AHHR_Altar()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    // * Components 생성
    SphereCollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
    SetRootComponent(SphereCollisionComp);
    SphereCollisionComp->InitSphereRadius(115.f);
    // collision 세팅
    // collision 함수 바인딩
    SphereCollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AHHR_Altar::OnComponentBeginOverlap);
    SphereCollisionComp->OnComponentEndOverlap.AddDynamic(this, &AHHR_Altar::OnComponentEndOverlap);

    // mesh 설정
    AltarStaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AltarStaticMesh"));
    AltarStaticMeshComp->SetupAttachment(SphereCollisionComp);
    AltarStaticMeshComp->AddRelativeLocation(FVector(24.f, -94.f, 28.f));
    static ConstructorHelpers::FObjectFinder<UStaticMesh> altarMeshAsset(TEXT("/Script/Engine.StaticMesh'/Game/HHR/Item/ItemMesh/SM_Altar.SM_Altar'"));
    if(altarMeshAsset.Succeeded())
    {
        AltarStaticMeshComp->SetStaticMesh(altarMeshAsset.Object);
    }

    bReplicates = true;
    


}

// Called when the game starts or when spawned
void AHHR_Altar::BeginPlay()
{
	Super::BeginPlay();

    // 변수 세팅
    MaxItemCnt = ItemAttachPos.Num();
    AttachedItems.Init(nullptr, MaxItemCnt);

    // owner 설정 -> 딜레이 안주면 안되는듯... 레벨에 배치되어 있어서 contorller 생성보다 빠른듯

    // 서버에서만 해줌
    if(!HasAuthority())
    {
        return;
    }
    
    FTimerHandle timerHandle;
    GetWorld()->GetTimerManager().SetTimer(timerHandle, FTimerDelegate::CreateLambda([&]()
    {
        //SetOwner(GetWorld()->GetFirstPlayerController());
        //P_LOG(PolluteLog, Warning, TEXT("owner 설정 : %s"), *GetOwner()->GetName());

        // PlayerCharacter에 바인딩
        // 서버에서 모든 캐릭터 바인딩
        for (AActor* Actor : TActorRange<ALCU_PlayerCharacter>(GetWorld()))
        {
            ALCU_PlayerCharacter* PlayerCharacter = Cast<ALCU_PlayerCharacter>(Actor);
            if (PlayerCharacter)
            {
                // Delegate 바인딩
                PlayerCharacter->OnAttachItemOnAltar.BindDynamic(this, &AHHR_Altar::OnAttachItem);
                PlayerCharacter->OnDettachItemOnAltar.BindDynamic(this, &AHHR_Altar::OnDetatchItem);
                UE_LOG(LogTemp, Log, TEXT("Bound delegate to PlayerCharacter: %s"), *PlayerCharacter->GetName());
            }
        }
        
    }), 3.0f, false);


}

void AHHR_Altar::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    //DOREPLIFETIME(AHHR_Altar, CurrentItemCnt);
}

// Called every frame
void AHHR_Altar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    
}

void AHHR_Altar::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                         UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    // TODO : 종속성..
    // Player의 bNearByAltar 변수 변경
    // TODO : Altar에 아이템이 다 차 있으면 굳이 안 변경해줘도 될듯 
    ALCU_PlayerCharacter* player = Cast<ALCU_PlayerCharacter>(OtherActor);
    if(player)
    {
        player->SetNearByAltar(true);
    }
}

void AHHR_Altar::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    // Player의 bNearByAltar 변수 변경
    ALCU_PlayerCharacter* player = Cast<ALCU_PlayerCharacter>(OtherActor);
    if(player)
    {
        player->SetNearByAltar(false);
    }
}

// 델리게이트에 바인딩 되는 함수
// 캐릭터를 통해 서버에서 호출 
void AHHR_Altar::OnAttachItem(AHHR_Item* Item)
{
    // Item Attach
    // TODO : Item 부착 동기화 필요
    if(CurrentItemCnt < MaxItemCnt)
    {
        // !!!!! 클라에서는 Pawn이랑 Controller만 소유권 가짐.. 
        NetMulticast_AttachToAltar(Item);
        // TODO : Attach 해주면 그 Item의 collision 어떻게 해줄지 정해줘야 함 -> 일단 지금 구현으로는 안해도 됨
        
        // 만약 CurrentItemCnt가 MaxItemCnt이면 Altar 아이템 체크
        // ! 이거 체크는 서버에서만 해주면 됨
        if(CurrentItemCnt >= MaxItemCnt)
        {
            CheckAltar();
        }
    }

}

// 서버에서 무조건 실행
// altar에서 detatch update
void AHHR_Altar::OnDetatchItem(AHHR_Item* Item)
{
    // altar에 올려져 있는 아이템인지 확인
    // 틀리면 무시
    // 맞으면 arrary에 제거, 변수 update,
    int32 idx = FindItemIdx(Item);
    if(idx >= 0)
    {
        // 찾으면 attachedItem에서 제거
        // TODO : 변수 update도 동기화 해줘야 함 
        NetMulticast_Update(idx);

    }
    else
    {
        // 못찾으면
        // altar 위에 있는 item이 아닌 거
        return;
    }
    
}

void AHHR_Altar::NetMulticast_AttachToAltar_Implementation(AHHR_Item* Item)
{
    // Attach 가능
    // TODO : attach 할 수 있는 인덱스 찾고 해당 위치에 attach + attached item에 넣기
    int32 idx = FindAttachIdx();
    if(idx >= 0)
    {
        // Attach 가능한 인덱스에 attach
        //Item->SetOwner(this);
        Item->AttachToComponent(SphereCollisionComp, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
        Item->SetActorRelativeLocation(ItemAttachPos[idx]);
        // 정보 업뎃
        Item->SetOverlayMaterialNull();
        AttachedItems[idx] = Item;
        ++CurrentItemCnt;
    }
    

}

void AHHR_Altar::NetMulticast_Update_Implementation(int32 idx)
{
    AttachedItems[idx] = nullptr;
    --CurrentItemCnt;
}

//*private 함수
int32 AHHR_Altar::FindItemIdx(AHHR_Item* Item)
{
    // 값에 해당하는 Item Index 찾기
    // 없으면 -1 반환
    for(int32 i = 0; i < AttachedItems.Num(); i++)
    {
        if(Item == AttachedItems[i])
        {
            return i;
        }
    }
    return -1;
}

int32 AHHR_Altar::FindAttachIdx()
{
    for(int32 i = 0; i < AttachedItems.Num(); i++)
    {
        if(AttachedItems[i] == nullptr)
        {
            return i;
        }
    }
    return -1;
}

bool AHHR_Altar::CheckAltarItems()
{
    // 모든 아이테밍 altar item인지 체크 
    if(CurrentItemCnt >= MaxItemCnt)
    {
        for(AHHR_Item* item : AttachedItems)
        {
            if(!item->GetIsAltarItem())
            {
                return false;
            }
        }
        return true;
    }
    return false;
}

void AHHR_Altar::CheckAltar()
{
    // 만약 CurrentItemCnt가 MaxItemCnt이면 Altar 아이템 체크
    // > altar 아이템들이 맞으면 탈출 로직 수행
    // > 아니면 패널티 적용 
    // ! 이거 체크는 서버에서만 해주면 됨
    if(CheckAltarItems())
    {
        // TODO : 탈출 로직 수행
        // 델리게이트로 처리할까...
        if(OnOpenDoor.IsBound())
                 {
                     OnOpenDoor.Execute();
                 }
    }
    else
    {
        // 패널티 적용
        P_LOG(PolluteLog, Warning, TEXT("삑! 틀림 ~ Penalty"));
        ALCU_Curse::GetInstance(GetWorld())->GetPenalty();
    }
}



