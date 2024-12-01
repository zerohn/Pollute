// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LCU_Curse.generated.h"

class ALCU_PlayerCharacter;

UCLASS()
class POLLUTE_API ALCU_Curse : public AActor
{
	GENERATED_BODY()

public:
	// 싱글톤으로 저주를 관리한다.
	static ALCU_Curse* GetInstance(UWorld* world);

public:
	// Sets default values for this actor's properties
	ALCU_Curse();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	// 저주 시간을 초기화
	void InitCurseTime();

public:
	// Get, Set
	void SetCharacter(ALCU_PlayerCharacter* character);
	ALCU_PlayerCharacter* GetCharacter() { return OwnerCharacter; };

	// Get, Set 끝

	//  저주 카운트 시작
	void StartCurseTimer(ALCU_PlayerCharacter* player);

private:
	// 폭탄을 소유하고 있는 한 사람
	UPROPERTY(VisibleAnywhere, Replicated)
	ALCU_PlayerCharacter* OwnerCharacter;

	UPROPERTY()
	float EndCurseTime = 10.f;

	UPROPERTY()
	float CurrentCurseTime = 10.f;

	bool bStartCurseTime = false;
	
private:
	static ALCU_Curse* Instance;
	
};
