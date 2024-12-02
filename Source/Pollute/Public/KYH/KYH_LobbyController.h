// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "KYH_LobbyController.generated.h"

class UKYH_CommonUserLobby;
/**
 * 
 */
UCLASS()
class POLLUTE_API AKYH_LobbyController : public APlayerController
{
	GENERATED_BODY()

public:

    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, Category = "Session")
    UKYH_CommonUserLobby* LobbyWidget;
    
};