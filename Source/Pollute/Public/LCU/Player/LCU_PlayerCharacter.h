// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LCU/Interfaces/LCU_InteractInterface.h"
#include "Pollute/TP_ThirdPerson/TP_ThirdPersonCharacter.h"
#include "Pollute/Public/LCU/LCU_Properties/LCU_Property.h"
#include "NSK/NSK_Altar.h"

#include "LCU_PlayerCharacter.generated.h"

UCLASS()
class POLLUTE_API ALCU_PlayerCharacter : public ATP_ThirdPersonCharacter , public ILCU_InteractInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ALCU_PlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	void UpdateCameraTransform();

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	// ILCU_InteractInterface 의 메서드
	virtual  void Interact() override;
	// ILCU_InteractInterface 의 메서드 끝

	// 박스 콜리전과 관련한 함수
	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
						   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
						   bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
						 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// 박스 콜리전 안에 들어온 플레이어들을 추가제거
	void AddOverlappingPlayer(AActor* otherActor);
	void RemoveOverlappingPlayer(AActor* otherActor);

	// 박스 콜리전 안에 들어온 아이템들을 추가제거
	void AddOverlappingItem(AActor* otherActor);
	void RemoveOverlappingItem(AActor* otherActor);

	AActor* GetClosestActorToCamera(const TSet<AActor*>& Actors);
	// 박스 콜리전과 관련 함수 끝
public:
	// Get, Set
	EGender GetGender() {return Gender;}

	bool GetHasCurse() {return bHasCurse;}
	void SetHasCurse(bool bCurse) {bHasCurse = bCurse;}

    // ## 아이템 Get
    class AHHR_Item* GetItem() {return ItemInHand;}
    // 아이템 초기화 함수
    void InitItem() {ItemInHand = nullptr;}
    
	// Get, Set 끝

	void CarryCurse();
	UFUNCTION(Server, Reliable)
	void ServerRPC_CarryCurse();
	UFUNCTION(NetMulticast, Reliable)
	void NetMulticast_CarryCurse();
	void PickUpDropDown(AHHR_Item* Item);
	void ShootTrace();

  // 죽으면 부르는 함수
  void DieProcess();

  // IA에 Bind될 함수
  void Attack();

private:
	// 아이템 및 캐릭터와의 충돌처리하는 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* BoxComponent;

	// 오버랩되는 액터를 클래스별로 정리
	UPROPERTY()
	TSet<AActor*> OverlappingPlayers;
	UPROPERTY()
	TSet<AActor*> OverlappingItems;

	// 최종 선택된 액터를 관리하는 변수
	UPROPERTY()
	ALCU_PlayerCharacter* FinalOverapPlayer;
	UPROPERTY()
	AActor* FinalOverapItem;
    UPROPERTY()
    class AHHR_Item* ItemInHand;
	
	// 성별 변수인데 성별따라 사용하는 애니메이션이 좀다를것
	// 애님 인스턴스에서 사용되어짐
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	EGender Gender;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UInputAction* IA_CarryCurse;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UInputAction* IA_PickUpDropDown;

    // HHR
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category = "Input")
    UInputAction* IA_Attack;
    
    // Montage
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category = "Anim")
    class UAnimMontage* KnifeAttackMontage;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category = "Anim")
    class UAnimMontage* GunAttackMontage;

	UPROPERTY()
	FTimerHandle TraceHandle;

    UPROPERTY(Replicated)
    int32 HealthCount = 4;

	bool bHasCurse = false;

    // NSK
    public:

        bool bHasItem = false;

        // 제단 상호작용 키
        UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
        UInputAction* IA_G;

        // NSK G키 상호작용 처리 함수
        void OnInteract();

        // 현재 근처 제단
        UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
        ANSK_Altar* NearbyAltar;

        // 근처 제단 설정 함수
        void SetNearbyAltar(ANSK_Altar* Altar);

        // 현재 플레이어가 들고 있는 아이템 데이터
        UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Inventory")
        FItemData HeldItem;

        // 플레이어가 들고 있는 아이템 반환 함수
        FItemData GetHeldItem() const;
};