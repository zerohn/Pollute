// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "P_PlayerState.generated.h"

class UKYH_CommonUserLobby;
class UKYH_CommonUserChat;

UCLASS()
class POLLUTE_API AP_PlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	AP_PlayerState(const FObjectInitializer& ObjectInitializer);

protected:
    
	virtual void BeginPlay() override;

public:
    
	virtual void Tick(float DeltaSeconds) override;

    UFUNCTION(Server, Reliable)
    void ServerRPC_SendChat(const FString& Message);

    UFUNCTION(NetMulticast, Reliable)
    void MulticastRPC_Chat(const FString& Message);
};
