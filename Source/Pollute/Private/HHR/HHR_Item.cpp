#include "HHR/HHR_Item.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "EntitySystem/MovieSceneEntitySystemRunner.h"
#include "GameFramework/Character.h"
#include "HHR/HHR_ItemManager.h"
#include "HHR/HHR_ItemSpawnManager.h"
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

    //
    //SetOwner(GetWorld()->GetFirstPlayerController());

}

void AHHR_Item::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AHHR_Item, DataIdx);
    DOREPLIFETIME(AHHR_Item, ItemManager);
    // REplicated 무조건 해줘야 함 
    DOREPLIFETIME(AHHR_Item, ItemSpawnManager);

    DOREPLIFETIME(AHHR_Item, bIsAltarItem);
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
    if(ItemSpawnManager)
    {
        SetItemData(ItemSpawnManager->ItemDataMap[DataIdx]);
    }
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
	    // 임시
	    if(ItemOverlayMaterial)
	    {
	        ItemMehsComp->SetOverlayMaterial(ItemOverlayMaterial);
	    }
	}
}

void AHHR_Item::Interact()
{
}
