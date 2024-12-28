// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LCU_UIManager.generated.h"

UCLASS()
class POLLUTE_API ALCU_UIManager : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    ALCU_UIManager();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    void Init();

public:
    // 사람 HUD!

    void ShowCurseWidget(bool bShow);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category= "Widget")
    TSubclassOf<class UHHR_PlayerHUD> PlayerHUDFactory;
    
    UPROPERTY()
    UHHR_PlayerHUD* PlayerHUD;
    
    // 사람 HUD END!!

    // 공통으로 사용할 멤버 및 메서드
    UPROPERTY()
    class ALCU_PlayerController* LCU_Pc;
};