// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"

UENUM(BlueprintType)
enum class EPlayerType : uint8
{
    Eric UMETA(DisplayName = "Eric"),
    Manuel UMETA(DisplayName = "Manuel"),
    Sophia UMETA(DisplayName = "Sophia"),
    Carla UMETA(DisplayName = "Carla"),
    Nathan UMETA(DisplayName = "Nathan"),
    Claudia UMETA(DisplayName = "Claudia"),
    Count UMETA(CLASS_Hidden)
};

UENUM(BlueprintType)
enum class EVoiceChannel : uint8
{
    PlayerChannel UMETA(DisplayName = "PlayerVoiceChannel"),
    MonsterChannel UMETA(DisplayName = "MonsterVoiceChannel"),
    SpectatorChannel UMETA(DisplayName = "SpectatorVoiceChannel")
};

/**
 * 
 */

class POLLUTE_API PlayData
{
public:
	PlayData();
	~PlayData();
};
