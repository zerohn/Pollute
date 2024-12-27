#pragma once

#include "CoreMinimal.h"
#include "LCU/Interfaces/LCU_InteractInterface.h"
#include "Pollute/TP_ThirdPerson/TP_ThirdPersonCharacter.h"
#include "Pollute/Public/LCU/LCU_Properties/LCU_Property.h"

#include "NSK/NSK_Altar.h"
#include "NSK/NSK_Parachute.h"
#include "P_Settings/PlayData.h"
#include "LCU_PlayerCharacter.generated.h"

class ALCU_PlayerController;
class UMaterialInstanceDynamic;
class UMaterialInstance;
enum class EPlayerType : uint8;


// Delegates by hhr
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnAttachItemOnAltar, class AHHR_Item*, Item);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnDettachItemOnAltar, class AHHR_Item*, Item);

UCLASS()
class POLLUTE_API ALCU_PlayerCharacter : public ATP_ThirdPersonCharacter , public ILCU_InteractInterface
{
	GENERATED_BODY()

public:
	ALCU_PlayerCharacter();

protected:
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    virtual void PossessedBy(AController* NewController) override;
	void UpdateCameraTransform();

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

    // 움직임 관련 함수들
    virtual void Move(const FInputActionValue& Value) override;
    void RunShiftToggle();
    UFUNCTION(Server, Reliable)
    void ServerRPC_SetRunning(bool run);
    UFUNCTION(Server, Reliable)
    void ServerRPC_UpdateSpeed(float MoveForwardValue, float MoveRightValue);
    UFUNCTION(NetMulticast, Reliable)
    void NetMulticast_UpdateSpeed(float NewSpeed);
	
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

    void SetCarryCurseCool(bool Cool) { StartCurseCool = Cool;}

    // ## 아이템 Get
    class AHHR_Item* GetItem() {return ItemInHand;}
    // 아이템 초기화 함수
    
    
	// Get, Set 끝

	void CarryCurse();
	UFUNCTION(Server, Reliable)
	void ServerRPC_CarryCurse();

    bool IsInMatToCamera(UMaterialInstanceDynamic* DynaminMat);
    
    
    UFUNCTION(Client, Reliable)
    void ClientRPC_SetCurseMat(bool bShow);
    UFUNCTION(Client, Reliable)
    void ClientRPC_SetCurseScalar(float scalar);
    
    UFUNCTION(NetMulticast, Reliable)
    void NetMulticast_AttachItem();
    void AttachItem();
    UFUNCTION(NetMulticast, Reliable)
    void NetMulticast_DetachItem();
    void DetachItem();
    
    void DropDown();
	void PickUpDropDown();
    
    UFUNCTION(Server, Reliable)
    void ServerRPC_PickUpDropDown();

    // 죽으면 부르는 함수
    void DieProcess();
    // IA에 Bind될 함수
    void Attack();
    // rpc attack
    UFUNCTION(Server, Reliable)
    void ServerRPC_Attack();
    UFUNCTION(NetMulticast, Reliable)
    void NetMulticast_Attack();

    void InitItem();
private:

    // 플레이어 컨트롤러
    UPROPERTY(Replicated)
    ALCU_PlayerController* LCU_Pc;
    
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
    UPROPERTY(Replicated)
    class AHHR_Item* ItemInHand;
	
	// 성별 변수인데 성별따라 사용하는 애니메이션이 좀다를것
	// 애님 인스턴스에서 사용되어짐
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	EGender Gender;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category = "Input")
	UInputAction* IA_CarryCurse;
	
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category = "Input")
	UInputAction* IA_PickUpDropDown;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category = "Input")
    UInputAction* IA_Attack;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category = "Input")
    UInputAction* IA_RunToggle;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category = "Input")
    UInputAction* IA_Ladder;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category = "Input")
    UInputAction* IA_ClimingLadder;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category = "Input")
    UInputAction* IA_Parachute;
    
    // Montage
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category = "Anim")
    class UAnimMontage* KnifeAttackMontage;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category = "Anim")
    class UAnimMontage* GunAttackMontage;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category = "Anim")
    UAnimMontage* HitMontage;

	UPROPERTY()
	FTimerHandle TraceHandle;

    UPROPERTY(Replicated)
    int32 HealthCount = 4;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
    UMaterialInstance* CurseMaterial;

    UPROPERTY()
    UMaterialInstanceDynamic* CurseMatInstance;

    UPROPERTY()
    float CarryCurseCool = 20.f;
    UPROPERTY()
    float MaxCurseCool = 20.f;
    UPROPERTY(Replicated)
    bool StartCurseCool = false;
    UPROPERTY(Replicated)
	bool bHasCurse = false;
    
    UPROPERTY(EditAnywhere)
    float WalkSpeed = 400.f;
    UPROPERTY(EditAnywhere)
    float RunSpeed = 700.f;
    UPROPERTY(EditAnywhere, Replicated)
    bool bIsRunning = false;
    
public:
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bInjuredBody = false;


    // NSK
public:

    bool bHasItem = false;

    // 제단 상호작용 키
    /*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    UInputAction* IA_G;

    // NSK G키 상호작용 처리 함수
    UFUNCTION()
    void OnInteract();*/

    // 현재 근처 제단
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
    ANSK_Altar* NearbyAltar;
    
    int32 SelectedSlotIndex;

    // 슬롯 인덱스를 설정하는 함수
    void SetCurrentSlotIndex(int32 SlotIndex);

    // 현재 슬롯 인덱스를 초기화하는 함수
    void ClearCurrentSlotIndex();

    // 근처 제단 설정 함수
    void SetNearbyAltar(ANSK_Altar* Altar, int32 SlotIndex);
    void ClearNearbyAltar();

    // 현재 플레이어가 들고 있는 아이템 데이터
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Inventory")
    FItemData HeldItem;

    // 플레이어가 들고 있는 아이템 반환 함수
    FItemData GetHeldItem() const;
    
    

    // NSK Ladder
    void OnInstallLadder();

    void InstallAndDeleteItem();

    UFUNCTION()
    void InteractWithLadder(const FInputActionValue& Value);

    // 사다리 설치 서버 RPC
    UFUNCTION(Server, Reliable)
    void ServerInstallLadder(ANSK_LadderInstallPoint* InstallPoint);

    // 상호작용 서버 RPC
    UFUNCTION(Server, Reliable)
    void Server_InteractWithLadder();

    // 상호작용 멀티캐스트 RPC
    UFUNCTION(NetMulticast, Reliable)
    void Multicast_InteractWithLadder(const FVector& TopLocation);

    // NSK Parachute
    UFUNCTION(BlueprintCallable)
    void InteractWithParachute();

    UFUNCTION()
    void CanUseParachute(bool bCanUse);

    UFUNCTION(Server, Reliable, WithValidation)
    void ServerDestroyParachute(ANSK_Parachute* Parachute);

    UFUNCTION(NetMulticast, Reliable)
    void MulticastDestroyParachute(ANSK_Parachute* Parachute);

    bool bCanUseParachute;

public:

    // KYH
    UPROPERTY(EditAnywhere)
    EPlayerType PlayerType = EPlayerType::Eric;
    UPROPERTY(EditDefaultsOnly)
    TArray<USkeletalMesh*> PlayerMeshType;

    UFUNCTION(Server, Reliable)
    void ServerRPC_SetPlayerType(EPlayerType InPlayerType);
    UFUNCTION(NetMulticast, Reliable)
    void MulticastRPC_UpdatePlayerMesh(EPlayerType InPlayerType);



 
// ** 제단 ** by HHR
    // Delegates
public:
    FOnAttachItemOnAltar OnAttachItemOnAltar;
    FOnDettachItemOnAltar OnDettachItemOnAltar;
    
private:
    // IA
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category = "Input")
    UInputAction* IA_PutItemOnAltar;

    // altar 근처에 있는지 체크 
    UPROPERTY(Replicated)
    bool bNearByAltar = false;

// *Get, Set*
public:

    FORCEINLINE bool GetNearByAltar() const {return bNearByAltar;}
    FORCEINLINE void SetNearByAltar(bool InNearByAltar){bNearByAltar = InNearByAltar;}
    UFUNCTION(BlueprintCallable)
    FORCEINLINE EPlayerType GetPlayerType() const {return PlayerType;}
//*RPC*
public:
    UFUNCTION(Server, Reliable)
    void ServerRPC_PutItemOnAltar();
    
protected:
    // Input system에 바인딩될 함수
    void PutItemOnAltar();
// ** hhr
    

// 임시 playerhud
public:
    class UHHR_PlayerHUD* PlayerHUD;

    // 픽업 대상 아이템 by lcu
    class AHHR_Item* RetrievedItem;

};