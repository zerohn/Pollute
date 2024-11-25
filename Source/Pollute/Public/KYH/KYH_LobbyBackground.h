// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/DataAsset.h"
#include "KYH_LobbyBackground.generated.h"

class UObject;
class UWorld;

/**
 * 
 */
UCLASS(config=EditorPerProjectUserSettings, MinimalAPI)
class UKYH_LobbyBackground : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UWorld> BackgroundLevel;
	
};
