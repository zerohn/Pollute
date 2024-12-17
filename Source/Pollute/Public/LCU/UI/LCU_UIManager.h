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

    ALCU_UIManager* GetInstance(UWorld* world, TSubclassOf<ALCU_UIManager> UIManagerFactory);
    ALCU_UIManager* GetInstance(UWorld* world);

public:
    // 저주 관련 위젯
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category= "Widget")
    TSubclassOf<class ULCU_CurseWidget> LCU_CurseWidgetFactory;

    ULCU_CurseWidget* LCU_CurseWidget;

    UFUNCTION()
    void ShowCurseWidget(bool bShow);
    UFUNCTION(Client, Reliable)
    void ClientRPC_ShowCurseWidget(bool bShow);

private:
    // 싱글턴
    static ALCU_UIManager* Instance;

};
