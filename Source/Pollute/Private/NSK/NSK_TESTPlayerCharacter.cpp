#include "NSK/NSK_TESTPlayerCharacter.h"
#include "Components/InputComponent.h"
#include <Kismet/GameplayStatics.h>

ANSK_TESTPlayerCharacter::ANSK_TESTPlayerCharacter()
    : bHasItem(false), NearbyAltar(nullptr) // 제단 참조 초기화
{

}

void ANSK_TESTPlayerCharacter::BeginPlay()
{
    Super::BeginPlay();
}

void ANSK_TESTPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // G키 입력을 제단 상호작용에 연결
    PlayerInputComponent->BindAction("IA_G", IE_Pressed, this, &ANSK_TESTPlayerCharacter::OnInteract);
}

void ANSK_TESTPlayerCharacter::OnInteract()
{
    if (NearbyAltar) // 근처에 제단이 있을 때만 상호작용
    {
        NearbyAltar->HandlePlayerInteraction(this);
    }
}

void ANSK_TESTPlayerCharacter::ResetHeldItem()
{
    // 들고 있는 아이템 초기화
    HeldItem = FItemData();
    bHasItem = false;
}

FItemData ANSK_TESTPlayerCharacter::GetHeldItem() const
{
    return HeldItem;
}

void ANSK_TESTPlayerCharacter::SetNearbyAltar(ANSK_Altar* Altar)
{
    NearbyAltar = Altar; // 근처 제단 참조 업데이트
}