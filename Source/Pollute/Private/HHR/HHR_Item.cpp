#include "HHR/HHR_Item.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "EntitySystem/MovieSceneEntitySystemRunner.h"
#include "GameFramework/Character.h"
#include "HHR/HHR_ItemManager.h"
#include "HHR/UI/HHR_PlayerHUD.h"
#include "LCU/Player/LCU_PlayerCharacter.h"
#include "Net/UnrealNetwork.h"
#include "NSK/NSK_SpawnManager.h"

// Sets default values
AHHR_Item::AHHR_Item()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Components 생성
	ItemSphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("ItemSphere"));
	ItemSphereComp->InitSphereRadius(50);
	RootComponent = ItemSphereComp;

	ItemMehsComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMehs"));
	ItemMehsComp->SetupAttachment(ItemSphereComp);
	ItemMehsComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	ItemSphereComp->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);

	// Overlap 델리게이트 바인딩
	ItemSphereComp->OnComponentBeginOverlap.AddDynamic(this, &AHHR_Item::OnComponentBeginOverlap);
	ItemSphereComp->OnComponentEndOverlap.AddDynamic(this, &AHHR_Item::OnComponentEndOverlap);
    
    // Init Item Interact Widget
    ItemInteractWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("ItemInteractWidget"));
    ItemInteractWidgetComp->SetupAttachment(RootComponent);
    ItemInteractWidgetComp->SetRelativeLocation(FVector(0, 0, 100.0f));
    ItemInteractWidgetComp->SetDrawSize(FVector2d(50, 50));
    ItemInteractWidgetComp->SetCollisionProfileName(TEXT("NoCollision"));

    bReplicates = true;
    //SetReplicateMove(true);
    
        
}

// Called when the game starts or when spawned
void AHHR_Item::BeginPlay()
{
	Super::BeginPlay();

    ItemInteractWidgetComp->SetWidgetClass(ItemWidgetClass);
    ItemInteractWidgetComp->SetVisibility(false);
}

void AHHR_Item::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AHHR_Item, DataIdx);
    DOREPLIFETIME(AHHR_Item, ItemManager);
}

// Called every frame
void AHHR_Item::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHHR_Item::OnRep_ChangeIdx()
{
    // Data 넣어줌
    if(ItemManager)
    {
        SetItemData(ItemManager->ItemDataMap[DataIdx]);
    }
    /*if(SpawnManager)
    {
        SetItemData(*(SpawnManager->SpawnedItems[DataIdx]));
    }*/
}

// ItemSpawnManager에서 넣어주는 게 나을듯 
void AHHR_Item::SetItemData(const FItemData& data)
{
	if(ItemData.ItemID == -1)
	{
		// Item이 할당 안되어 있으면 할당
		// StaticMesh도 할당
		ItemData = data;
		ItemMehsComp->SetStaticMesh(ItemData.ItemMesh);
	}
}

// 안쓰임 
void AHHR_Item::Interact()
{
	// 추후 사용
    
	// 테스트 용 임시 UI 띄우기
	//SetVisibilityUI(true);

	// 아이템과 캐릭터 상호작용
	// -> 우리 프젝에서는 캐릭터에서 주로 기능을 수행할듯
	// => 그래서 사실 Interact() 함수는 안 사용할 듯..
	// => ?? InteractInterface에 SetvisibilityUI()를 만들어주는 게 맞는지 의문?
}

// 안쓰임 
void AHHR_Item::SetVisibilityUI(bool Visible)
{
	if(!PlayerHUD) return;
	
	// Item
	if(Visible)
	{
		PlayerHUD->SetItemDialogText(ItemData.ItemUIName);
		PlayerHUD->SetItemDialogVisibility(true);
	}
	else
	{
		PlayerHUD->SetItemDialogVisibility(false);
	}

}



void AHHR_Item::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    // local player에만 보여야함
    APawn* playerpawn = Cast<APawn>(OtherActor);
    if(playerpawn && playerpawn->IsLocallyControlled())
    {
        ItemInteractWidgetComp->SetVisibility(true);
    }
    
    
	// UI 처리 따로 함수로 빼서 Player Character에서 호출 
	/*if(Cast<ACharacter>(OtherActor))
	{
		// Character이면 UI 띄우기
		// 임시로 직접 가져와서 띄우기

		P_LOG(PolluteLog, Warning, TEXT("UI 띄우기"));
		if(PlayerHUD)

		{
			// TODO : 수정
			P_LOG(PolluteLog, Warning, TEXT("Player HUD 있음"));
			if(PlayerHUD)
			{
				PlayerHUD->SetItemDialogText(ItemData.ItemUIName);
				PlayerHUD->SetItemDialogVisibility(true);
			}
		}
	}*/
	
}

void AHHR_Item::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

    // local player에만 보여야함
    APawn* playerpawn = Cast<APawn>(OtherActor);
    if(playerpawn && playerpawn->IsLocallyControlled())
    {
        ItemInteractWidgetComp->SetVisibility(false);
    }
    
    
	// TODO : 필요 없음 ㅇㅅㅇ
	// 테스트용
	//SetVisibilityUI(false);

	/*if(Cast<ACharacter>(OtherActor))
	{
		//P_LOG(PolluteLog, Warning, TEXT("UI 안띄우기"));
		/*if(TestPlayerHUD)
		{
			UHHR_TestPlayerHUD* PlayerHUD = Cast<UHHR_TestPlayerHUD>(TestPlayerHUD);
			if(PlayerHUD)
			{
				PlayerHUD->SetItemDialogVisibility(false);
			}

		}
	}*/

}

