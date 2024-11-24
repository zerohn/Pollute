// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LCU_TestActor.generated.h"

UCLASS()
class POLLUTE_API ALCU_TestActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALCU_TestActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
