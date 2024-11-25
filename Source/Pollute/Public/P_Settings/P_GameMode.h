// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "P_GameMode.generated.h"

class ALCU_PlayerCharacter;
class ALCU_Curse;
/** TODO
 *  일단 그냥 저주를 넣자.
 *  바꿀 때는 남아있는 사람 플레이어 들 중에서 적용이 가능하게 하자
 */
UCLASS()
class POLLUTE_API AP_GameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	AP_GameMode();

public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

public:
	// 플레이어 저장 및 삭제 함수들
	TObjectPtr<ALCU_PlayerCharacter> FindPlayer(ALCU_PlayerCharacter* player);
	
	void GetAllCharacters();
	// 플레이어 저장 및 삭제 함수들 끝

	// 저주를 걸 플레이어를 선택.
	void SelectPlayer();
protected:
	// 싱글턴 객체
	UPROPERTY(EditAnywhere)
	ALCU_Curse* Curse;
	
private:
	UPROPERTY()
	TArray<TObjectPtr<ALCU_PlayerCharacter>> AllPlayers;

	UPROPERTY()
	TArray<TObjectPtr<ALCU_PlayerCharacter>> HumanPlayers;

	FTimerHandle CurseSelectedTimer;
	
};
