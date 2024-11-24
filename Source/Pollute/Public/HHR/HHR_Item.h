// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HHR_ItemData.h"
#include "GameFramework/Actor.h"
#include "HHR_Item.generated.h"


UCLASS()
class POLLUTE_API AHHR_Item : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHHR_Item();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


// Components
protected:
	
	// Item mesh comp
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Component")
	UStaticMeshComponent* ItemMehsComp;

	// Item Collision
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Component")
	class USphereComponent* ItemSphereComp;

	
protected:
	// Item Data
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	FItemData ItemData;


// Functions
public:
	void SetItemData(const FItemData& data);
	
	// TODO : Item Use

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
	class UUserWidget* TestPlayerHUD;

};
