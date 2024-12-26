// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pollute/TP_ThirdPerson/TP_ThirdPersonCharacter.h"
#include "LCU_MonsterCharacter.generated.h"

UCLASS()
class POLLUTE_API ALCU_MonsterCharacter : public ATP_ThirdPersonCharacter
{
    GENERATED_BODY()

public:
    // Sets default values for this character's properties
    ALCU_MonsterCharacter();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

    virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
    
public:
    // 공격 관련 함수
    void Attack();
    UFUNCTION(Server, Reliable)
    void ServerRPC_OnSuccessHit(FHitResult HitResult, bool bHit);
    UFUNCTION(NetMulticast, Reliable)
    void NetMulticast_OnSuccessHit(FHitResult HitResult, bool bHit);
    UFUNCTION(Server, Reliable)
    void ServerRPC_Attack();
    UFUNCTION(NetMulticast, Reliable)
    void Multicast_Attack();    
    UFUNCTION()
    void OnNotifyAttack();

    void DashSkill();
    UFUNCTION(Server, Reliable)
    void ServerRPC_DashSkill();
    UFUNCTION(NetMulticast, Reliable)
    void Multicast_DashSkill();

    UFUNCTION()
    void CallChange(UAnimMontage* Montage, bool bInterrupted);
    // 공격 받기
    void DieProcess();
    void DieAndChandChar();
    void ApplyStun();
    void ClearStun();
    
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
    class UInputAction* IA_Attack;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
    class UInputAction* IA_Skill1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
    class UAnimMontage* AttackMontage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
    UAnimMontage* DieMontage;
        
    FTimerHandle StunTimerHandle;
    UPROPERTY(EditAnywhere)
    float StunTime = 5.f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float DashStrength = 5000.f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float DashCooldown = 10.f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MaxDashTime = 10.f;
    
    UPROPERTY()
    bool bIsStunned = false;
    
    UPROPERTY()
    bool bCanAttack = true;

    UPROPERTY()
    bool AttackStart = false;

    UPROPERTY()
    bool bCanSkill = true;

    UPROPERTY(BlueprintReadWrite)
    bool bDeath = false;

  
};