// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "P_GameInstance.generated.h"

/**
 * 
 */
UCLASS()
class POLLUTE_API UP_GameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;

	UFUNCTION()
	void CreateOwnSession(FName SessionName, int32 PlayerLimit);

protected:
	FName SessionKey;
	IOnlineSessionPtr Session;
private:
	
};
