// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "P_LobbyGameState.generated.h"

class UKYH_CommonUserLobby;
/**
 * 
 */
UCLASS()
class POLLUTE_API AP_LobbyGameState : public AGameStateBase
{
	GENERATED_BODY()
    
public:

    virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, Category = "Session")
    TSubclassOf<UKYH_CommonUserLobby> LobbyWidgetClass;

    UPROPERTY()
    UKYH_CommonUserLobby* LobbyWidget;
};
