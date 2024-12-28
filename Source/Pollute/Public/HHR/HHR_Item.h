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

    UPROPERTY(EditDefaultsOnly, Category = "Component")
    TSubclassOf<class UUserWidget> ItemWidgetClass;
public:
    
	// Item Data
	UPROPERTY(BlueprintReadOnly, Category="Item")
	FItemData ItemData;
    
private:
    // 제단 아이템 확인
    //UPROPERTY(Replicated)
    UPROPERTY(Replicated)
    bool bIsAltarItem = false;

public:
    // 임시
    UPROPERTY(EditAnywhere, Category="Material")
    class UMaterialInterface* ItemOverlayMaterial;
    
    
// ###### Functions #######
public:
    // set, get
    FORCEINLINE UWidgetComponent* GetItemInteractWidgetComponent() { return ItemInteractWidgetComp; }
    FORCEINLINE void SetIsAltarItem(bool bIsAltar) {bIsAltarItem = bIsAltar;}
    FORCEINLINE bool GetIsAltarItem() {return bIsAltarItem;}
    // 

	// Item Data Sets
	void SetItemData(const FItemData& data);
	
    virtual void Interact() override;

};
