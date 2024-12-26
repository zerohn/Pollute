// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HHR/HHR_Item.h"
#include "Iris/ReplicationSystem/ReplicationSystemTypes.h"
#include "HHR_WeaponItem.generated.h"

/**
 * 
 */

// WeaponType에 대한 Enum
UENUM(BlueprintType)
enum class EWeaponType : uint8
{
    Knife,
    TaserGun,
};



UCLASS()
class POLLUTE_API AHHR_WeaponItem : public AHHR_Item
{
	GENERATED_BODY()

protected:
    bool bIsUsed = false;
    EWeaponType WeaponType;

// Getter Setter
public:
    bool GetIsUsed() {return bIsUsed;}
    void SetIsUsed(bool value) {bIsUsed = value;}

    EWeaponType GetWeaponType() {return WeaponType;}
    void SetWeaponType(EWeaponType type) {WeaponType = type;}
    
// 공격 함수
public:
	virtual void Attack();

    // 한번 사용시 사라지는 함수
    void DestoryDelay(class ALCU_PlayerCharacter* player);

    UFUNCTION()
    void DelegateDestroy(class ALCU_PlayerCharacter* player);

    UFUNCTION(NetMulticast, Reliable)
    void Multicast_DelegateDestroy(class ALCU_PlayerCharacter* player);
    UFUNCTION(NetMulticast, Reliable)
    void NetMulticast_UInull();

};
