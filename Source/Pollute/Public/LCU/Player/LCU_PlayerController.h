// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LCU/LCU_Properties/LCU_Property.h"
#include "LCU_PlayerController.generated.h"

enum class EVoiceChannel : uint8;
/**
 * 
 */
UCLASS()
class POLLUTE_API ALCU_PlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	ALCU_PlayerController();

public:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(Server, Reliable)
	void ServerRPC_ChangeToSpector();

	UFUNCTION()
	void ChangeToMonster();

    UFUNCTION(Client, Reliable)
    void ClientRPC_ItemUIOff();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player State")
	EPlayerMode PlayerMode = EPlayerMode::Human;

	// 몬스터 클래스 TSubclassOf 로 생성하기
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player State")
    TSubclassOf<class ALCU_MonsterCharacter> MonChar;

    // 음성 채팅
    EVoiceChannel CurrentVoiceChannel;
};
