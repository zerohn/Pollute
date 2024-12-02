#include "LCU/Player/LCU_MonsterCharacter.h"

#include "EnhancedInputComponent.h"
#include "Animation/AnimInstance.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "Kismet/KismetSystemLibrary.h"
#include "LCU/Interfaces/LCU_InteractInterface.h"
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
}

// Called to bind functionality to input
void ALCU_MonsterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // Set up action bindings
    if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        EnhancedInputComponent->BindAction(IA_Attack, ETriggerEvent::Started, this, &ALCU_MonsterCharacter::Attack);
    }
}

void ALCU_MonsterCharacter::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ALCU_MonsterCharacter, bCanAttack);
}

void ALCU_MonsterCharacter::Attack()
{
    if(GetMesh()->GetAnimInstance()->IsAnyMontagePlaying())
    {
        return;
    }
    
    ServerRPC_Attack();
}

void ALCU_MonsterCharacter::Multicast_OnNotifyAttack_Implementation()
{
    // AttackSocket의 위치와 방향 가져오기
    FVector SocketLocation = GetMesh()->GetSocketLocation(TEXT("AttackSocket"));
    FVector SocketForwardVector = GetMesh()->GetSocketRotation(TEXT("AttackSocket")).Vector();

    // 트레이스 시작점과 끝점 계산
    FVector Start = SocketLocation;
    FVector End = Start + SocketForwardVector * 300.0f; // 소켓 ForwardVector 방향으로 1000 단위 거리

    // 박스 크기 설정
    FVector BoxHalfSize = FVector(10.0f, 10.0f, 50.0f); // 박스 크기 (너비 20, 높이 100)

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
        Start,
        End,
        BoxHalfSize,
        FRotator::ZeroRotator, // 방향은 ForwardVector로 이미 설정했으므로 필요 없음
        ObjectTypes,
        false,
        IgnoredActors,
        EDrawDebugTrace::ForDuration, // 디버그용 선
        HitResult,
        true,
        FLinearColor::Red,
        FLinearColor::Green,
        5.0f
    );

    if (bHit)
    {
        AActor* HitActor = HitResult.GetActor();

        // 인터페이스 호출
        ILCU_InteractInterface* InteractInterface = Cast<ILCU_InteractInterface>(HitActor);
        if (InteractInterface && bCanAttack)
        {
            InteractInterface->Interact();
            bCanAttack = false;
        }
    }
}

void ALCU_MonsterCharacter::ServerRPC_OnNotifyAttack_Implementation()
{
    Multicast_OnNotifyAttack();
}

void ALCU_MonsterCharacter::ServerRPC_Attack_Implementation()
{
    Multicast_Attack();
}

void ALCU_MonsterCharacter::Multicast_Attack_Implementation()
{
    if(!AttackMontage) return;;
    PlayAnimMontage(AttackMontage);
}