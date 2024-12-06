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
    Claudia UMETA(DisplayName = "Claudia")
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
