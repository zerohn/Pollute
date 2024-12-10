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

    UPROPERTY(EditDefaultsOnly, Category = "Session")
    TSubclassOf<UKYH_CommonUserLobby> LobbyWidgetClass;

    UPROPERTY()
    UKYH_CommonUserLobby* LobbyWidget;
    
public:
    
	virtual void Tick(float DeltaSeconds) override;    

    UFUNCTION(Server, Reliable)
    void ServerRPC_SendChat(const FString& Message);

    UFUNCTION(NetMulticast, Reliable)
    void MulticastRPC_Chat(const FString& Message);
    
    void ServerRPC_UpdateLobbyUI();
    UFUNCTION(Client, Reliable)
    void MakeUI(const TArray<FName>& PlayerNames);
};
