// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HHR_ItemData.h"
#include "GameFramework/Actor.h"
#include "LCU/Interfaces/LCU_InteractInterface.h"
#include "HHR_Item.generated.h"


UCLASS()
class POLLUTE_API AHHR_Item : public AActor, public ILCU_InteractInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHHR_Item();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
    virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

// ###### Variables #######
public:
    UPROPERTY(ReplicatedUsing = OnRep_ChangeIdx)
    int32 DataIdx;
    UFUNCTION()
    void OnRep_ChangeIdx();

    UPROPERTY(Replicated)
    class AHHR_ItemManager* ItemManager;
    UPROPERTY(Replicated)
    class AHHR_ItemSpawnManager* ItemSpawnManager;
	
// Components
protected:
	// Item mesh Component
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Component")
	UStaticMeshComponent* ItemMehsComp;

	// Item Collision Component
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Component")
	class USphereComponent* ItemSphereComp;

    // Item Interaction Widget Component
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Component")
    class UWidgetComponent* ItemInteractWidgetComp;
    
public:
    
	// Item Data
	UPROPERTY(BlueprintReadOnly, Category="Item")
	FItemData ItemData;
    
    // Item Interaction Widget Data
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
    TSubclassOf<class UUserWidget> ItemWidgetClass;

private:
    // 제단 아이템 확인
    bool bIsAltarItem = false;
    
    
// ###### Functions #######
public:
    // set, get
    FORCEINLINE UWidgetComponent* GetItemInteractWidgetComponent() { return ItemInteractWidgetComp; }
    FORCEINLINE void SetIsAltarItem(bool bIsAltar) {bIsAltarItem = bIsAltar;}
    FORCEINLINE bool GetIsAltarItem() {return bIsAltarItem;}
    // 

    
	// Item Data Sets
	void SetItemData(const FItemData& data);
	
    // UI
	virtual void SetVisibilityUI(bool Visible) override;

    virtual void Interact() override;

protected:
	// 충돌 처리 함수
	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

// 임시 Player HUD
// TODO : UI Player Controller 혹은 다른 UI 관리자로부터 가져와서 변경
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI")
	class UHHR_PlayerHUD* PlayerHUD;



    
};
