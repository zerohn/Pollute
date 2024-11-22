// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LCU_BaseCharacter.h"
#include "LCU_Human.generated.h"

UCLASS()
class POLLUTE_API ALCU_Human : public ALCU_BaseCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ALCU_Human();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
