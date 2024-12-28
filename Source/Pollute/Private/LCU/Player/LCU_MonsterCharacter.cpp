#include "LCU/Player/LCU_MonsterCharacter.h"

#include "EnhancedInputComponent.h"
#include "TimerManager.h"
#include "Animation/AnimInstance.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HHR/HHR_WeaponItem.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "LCU/Animation/LCU_MonsterAnim.h"
#include "LCU/Interfaces/LCU_InteractInterface.h"
#include "LCU/Player/LCU_PlayerController.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ALCU_MonsterCharacter::ALCU_MonsterCharacter()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    bUseControllerRotationYaw = true;
}

// Called when the game starts or when spawned
void ALCU_MonsterCharacter::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void ALCU_MonsterCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if(AttackStart && IsLocallyControlled())
    {
        OnNotifyAttack();
    }

    if(!bCanSkill)
    {
        DashCooldown -= DeltaTime;
        if(DashCooldown <= 0.f)
        {
            bCanSkill = true;
            DashCooldown = MaxDashTime;
        }
    }
}

// Called to bind functionality to input
void ALCU_MonsterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // Set up action bindings
    if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        EnhancedInputComponent->BindAction(IA_Attack, ETriggerEvent::Started, this, &ALCU_MonsterCharacter::Attack);
        EnhancedInputComponent->BindAction(IA_Skill1, ETriggerEvent::Started, this, &ALCU_MonsterCharacter::DashSkill);
    }
}

void ALCU_MonsterCharacter::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

float ALCU_MonsterCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
    class AController* EventInstigator, AActor* DamageCauser)
{
    float damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

    AHHR_WeaponItem* item = Cast<AHHR_WeaponItem>(DamageCauser);
    if(item->GetWeaponType() == EWeaponType::Knife)
    {        
        DieProcess();
    }
    else if(item->GetWeaponType() == EWeaponType::TaserGun)
    {
        ApplyStun();
    }
    
    return damage;
}

void ALCU_MonsterCharacter::Attack()
{
    if(GetMesh()->GetAnimInstance()->IsAnyMontagePlaying())
    {
        return;
    }
    
    ServerRPC_Attack();
}

void ALCU_MonsterCharacter::NetMulticast_OnSuccessHit_Implementation(FHitResult HitResult, bool bHit)
{
    if (bHit)
    {
        AActor* HitActor = HitResult.GetActor();

        // 인터페이스 호출
        ILCU_InteractInterface* InteractInterface = Cast<ILCU_InteractInterface>(HitActor);
        if (InteractInterface )
        {
            InteractInterface->Interact();
        }
    }
}

void ALCU_MonsterCharacter::ServerRPC_OnSuccessHit_Implementation(FHitResult HitResult, bool bHit)
{
    NetMulticast_OnSuccessHit(HitResult, bHit);
}

void ALCU_MonsterCharacter::OnNotifyAttack()
{
    if(!IsLocallyControlled()) return;

    FVector StartLocation = GetActorLocation();
    FVector EndLocation = StartLocation + GetActorForwardVector()* 200.f;

    // 박스 크기 설정
    FVector BoxHalfSize = FVector(100.0f, 100.0f, 50.0f); // 

    // 트레이스할 객체 유형 설정
    TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
    ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));

    // 무시할 액터 설정
    TArray<AActor*> IgnoredActors;
    IgnoredActors.Add(this); // 자신 제외

    FHitResult HitResult;

    // BoxTraceSingleForObjects 호출
    bool bHit = UKismetSystemLibrary::BoxTraceSingleForObjects(
        GetWorld(),
        StartLocation,
        EndLocation,
        BoxHalfSize,
        FRotator::ZeroRotator, // 방향은 ForwardVector로 이미 설정했으므로 필요 없음
        ObjectTypes,
        false,
        IgnoredActors,
        EDrawDebugTrace::None, // 디버그용 선
        HitResult,
        true,
        FLinearColor::Red,
        FLinearColor::Green,
        4.0f
    );

    ServerRPC_OnSuccessHit(HitResult, bHit);
}

void ALCU_MonsterCharacter::DashSkill()
{
    if(bCanSkill)
    {
        bCanSkill= false;
        ServerRPC_DashSkill();
    }
}

void ALCU_MonsterCharacter::ServerRPC_DashSkill_Implementation()
{
    Multicast_DashSkill();
}

void ALCU_MonsterCharacter::Multicast_DashSkill_Implementation()
{
    // 대쉬 방향 계산 (Z 축 제거)
    FVector DashDirection = GetActorForwardVector();
    DashDirection.Z = 0; // Z 축 제거
    DashDirection = DashDirection.GetSafeNormal() * DashStrength; // 크기 정규화 후 스케일 적용

    // LaunchCharacter 호출 (Z 축 없음)
    LaunchCharacter(DashDirection, false, false);
}

void ALCU_MonsterCharacter::CallChange(UAnimMontage* Montage, bool bInterrupted)
{
    if(Montage != DieMontage) return;
    ALCU_PlayerController* MonController = Cast<ALCU_PlayerController>(GetWorld()->GetFirstPlayerController());
   
    if(IsLocallyControlled() && MonController)
    {
        EnableInput(MonController);
        MonController->ServerRPC_ChangeToSpector();
    }
}

void ALCU_MonsterCharacter::DieProcess()
{
    if(!DieMontage) return;
    ALCU_PlayerController* MonController = Cast<ALCU_PlayerController>(GetController());
    if(IsLocallyControlled() && MonController)
    {
        DisableInput(MonController);
    }
    UAnimInstance* animInstance = GetMesh()->GetAnimInstance();
    if(animInstance && animInstance->IsAnyMontagePlaying())
    {
        
        
        animInstance->StopAllMontages(0.f);
    }
    ULCU_MonsterAnim* monAnim = Cast<ULCU_MonsterAnim>(animInstance);
    monAnim->bDeath = true;
    animInstance->Montage_Play(DieMontage);

    if(IsLocallyControlled())
    {
        animInstance->OnMontageEnded.AddDynamic(this, &ALCU_MonsterCharacter::CallChange);
    }
}

void ALCU_MonsterCharacter::DieAndChandChar()
{
    ALCU_PlayerController* MonController = Cast<ALCU_PlayerController>(GetWorld()->GetFirstPlayerController());
   
    if(IsLocallyControlled() && MonController)
    {
        EnableInput(MonController);
    }
}

void ALCU_MonsterCharacter::ApplyStun()
{
    if(bIsStunned) return;

    bIsStunned = true;

    GetCharacterMovement()->DisableMovement();

    GetWorld()->GetTimerManager().SetTimer(StunTimerHandle, this, &ALCU_MonsterCharacter::ClearStun, StunTime);
}

void ALCU_MonsterCharacter::ClearStun()
{
    bIsStunned = false;
    GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void ALCU_MonsterCharacter::ServerRPC_Attack_Implementation()
{
    Multicast_Attack();
}

void ALCU_MonsterCharacter::Multicast_Attack_Implementation()
{
    if(!AttackMontage) return;
    PlayAnimMontage(AttackMontage);
}