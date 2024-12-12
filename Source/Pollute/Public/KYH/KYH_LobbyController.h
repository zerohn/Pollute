// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "KYH_LobbyController.generated.h"

enum class EPlayerType : uint8;
class UKYH_CommonUserLobby;

/**
 * 
 */

UCLASS()
class POLLUTE_API AKYH_LobbyController : public APlayerController
{
	GENERATED_BODY()

public:

protected:

    virtual void BeginPlay() override;
    
    UPROPERTY(EditDefaultsOnly, Category = "Session")
    TSubclassOf<UKYH_CommonUserLobby> LobbyWidgetClass;

    UPROPERTY()
    UKYH_CommonUserLobby* LobbyWidget;

    EPlayerType CurrentPlayerType;

public:

    UFUNCTION(Server, Reliable)
    void InitLobbyWidget();
    UFUNCTION(Client, Reliable)
    void UpdatePlayerSlot(TArray<TPair<int32, int32>> PlayerTypeMap);

    UFUNCTION(Server, Reliable)
    void ServerRPC_SendChat(const FString& Message);
    UFUNCTION(NetMulticast, Reliable)
    void NetMulticastRPC_UpdateChat(const FString& Message);
};
