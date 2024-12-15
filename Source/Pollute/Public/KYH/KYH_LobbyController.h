// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "KYH_LobbyController.generated.h"

enum class EPlayerType : uint8;
class UKYH_CommonUserLobby;

USTRUCT(BlueprintType)
struct FPlayerLobbyInfo
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    int32 PlayerId;
    UPROPERTY(BlueprintReadWrite)
    EPlayerType PlayerType;
};

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

    UPROPERTY()
    EPlayerType CurrentPlayerType;
    
public:

    UFUNCTION(Server, Reliable)
    void InitLobbyWidget();
    UFUNCTION(Client, Reliable)
    void UpdatePlayerSlot(const TArray<FPlayerLobbyInfo>& PlayerInfoArray);

    UFUNCTION(Server, Reliable)
    void ServerRPC_SendChat(const FString& Message);
    UFUNCTION(Client, Reliable)
    void ClientRPC_UpdateChat(const FString& Message);

    UFUNCTION(Server, Reliable)
    void ServerRPC_SetPlayerType(EPlayerType InPlayerType);

    UFUNCTION()
    EPlayerType GetCurrentPlayerType() { return CurrentPlayerType; }
};
