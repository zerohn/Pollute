// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "P_GameState.generated.h"

class ALCU_PlayerCharacter;
class ALCU_Curse;

UCLASS()
class POLLUTE_API AP_GameState : public AGameStateBase
{
	GENERATED_BODY()
public:
	AP_GameState();
	void SelectCursePlayer();

public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	// 플레이어 저장 및 삭제 함수들
	TObjectPtr<ALCU_PlayerCharacter> FindPlayer(ALCU_PlayerCharacter* player);
	
	void GetAllCharacters();

	void RemoveHumanPlayer(ALCU_PlayerCharacter* humanPlayer);
	// 플레이어 저장 및 삭제 함수들 끝

	// 저주를 걸 플레이어를 선택.
	UFUNCTION()
	void SelectPlayer();

	//UFUNCTION(NetMulticast, Reliable)
	UFUNCTION()
	void StartCurse(ALCU_PlayerCharacter* selectedPlayer);

	// 저주를 가진 사람의 사망 등의 이유로 다시 저주를 걸어야해요
	UFUNCTION(NetMulticast, Reliable)
	void RestartCurse(ALCU_PlayerCharacter* selectedPlayer);
	
protected:
	// 싱글턴 객체
	UPROPERTY(EditAnywhere)
	ALCU_Curse* Curse;
	
private:

	UPROPERTY(Replicated)
	TArray<TObjectPtr<ALCU_PlayerCharacter>> HumanPlayers;

	FTimerHandle CurseSelectedTimer;
};
