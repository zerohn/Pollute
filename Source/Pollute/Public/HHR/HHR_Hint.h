// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HHR_Hint.generated.h"

UCLASS()
class POLLUTE_API AHHR_Hint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHHR_Hint();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
    virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

// #### Variables ####
protected:
    UPROPERTY(VisibleDefaultsOnly, Category = "Components")
    class UStaticMeshComponent* FrameMeshComp;
    UPROPERTY(VisibleDefaultsOnly, Category = "Components")
    class UStaticMeshComponent* PictureMeshComp;


    // # Setting 변수
private:
    // item 해당되는 id
    UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"), Category = "Setting")
    int32 ItemID;

    // altar가 아닐때 생성되어야 하는지 확인 변수
    UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"), Category = "Setting")
    bool bIsHideHint = false;

    // 스폰될 위치
    UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"), Category = "Setting")
    FTransform HintSpawnTransform;

public:
    // 동기화를 위한 변수
    UPROPERTY(ReplicatedUsing=OnRep_InvisiblePicture)
    bool bInvisiblePicture = false;
    UFUNCTION()
    void OnRep_InvisiblePicture();


// #### Function ####
    // Get, Set 함수
public:
    // Transform 반환
    FTransform* GetHintSpawnTransform(){return &HintSpawnTransform;}
    // HideHint 
    bool GetIsHideHint(){return bIsHideHint;}
    //
    void SetInvisiblePicture(bool b){bInvisiblePicture = b;}

public:
    // 그림 안보이게 하기 
    void InvisiblePicture();



};
