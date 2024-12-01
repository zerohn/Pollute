#include "NSK/NSK_TESTPlayerCharacter.h"
#include "Components/InputComponent.h"
#include <Kismet/GameplayStatics.h>
#include "EnhancedInputComponent.h"
#include <EnhancedInputSubsystemInterface.h>

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

    // Enhanced Input 컴포넌트 확인
    UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
    if (!EnhancedInputComponent)
    {
        P_LOG(PolluteLog, Error, TEXT("EnhancedInputComponent가 없습니다."));
        return;
    }

    // G 키 입력 액션 바인딩
    if (IA_G)
    {
        EnhancedInputComponent->BindAction(IA_G, ETriggerEvent::Started, this, &ANSK_TESTPlayerCharacter::OnInteract);
        P_LOG(PolluteLog, Warning, TEXT("G 키 입력 액션 바인딩 완료"));
    }
    else
    {
        P_LOG(PolluteLog, Error, TEXT("IA_G 입력 액션이 설정되지 않았습니다."));
    }
}

void ANSK_TESTPlayerCharacter::OnInteract()
{
    P_LOG(PolluteLog, Warning, TEXT("OnInteract 호출"));
    if (NearbyAltar) // 근처에 제단이 있을 때만 상호작용
    {
        P_LOG(PolluteLog, Warning, TEXT("NearbyAltar 있음, 상호작용 진행"));
        NearbyAltar->OnInteract();
    }
    else
    {
        P_LOG(PolluteLog, Warning, TEXT("NearbyAltar 없음"));
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
    P_LOG(PolluteLog, Warning, TEXT("HeldItem: %s"), *HeldItem.ItemName.ToString());
    return HeldItem;
}

void ANSK_TESTPlayerCharacter::SetNearbyAltar(ANSK_Altar* Altar)
{
    NearbyAltar = Altar; // 근처 제단 참조 업데이트
    P_LOG(PolluteLog, Warning, TEXT("NearbyAltar 설정: %s"), Altar ? *Altar->GetName() : TEXT("nullptr"));
}