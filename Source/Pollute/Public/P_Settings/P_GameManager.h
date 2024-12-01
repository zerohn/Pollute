// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "P_GameManager.generated.h"

class ALCU_PlayerCharacter;

UCLASS()
class POLLUTE_API AP_GameManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AP_GameManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
