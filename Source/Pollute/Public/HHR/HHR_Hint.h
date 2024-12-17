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

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

// #### Variables ####
protected:
    UPROPERTY(VisibleDefaultsOnly, Category = "Components")
    class UStaticMeshComponent* FrameMeshComp;


private:
    // item 해당되는 id
    int32 ItemID;

    // altar가 아닐때 생성되어야 하는지 확인 변수
    bool bIsHideHint = false;



};
