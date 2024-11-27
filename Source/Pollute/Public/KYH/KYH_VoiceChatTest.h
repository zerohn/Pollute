// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "KYH_VoiceChatTest.generated.h"

class UAudioCaptureComponent;
class USoundWave;

UCLASS()
class POLLUTE_API AKYH_VoiceChatTest : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AKYH_VoiceChatTest();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	float CurrentEnvelopeValue;

	float MicVolume = 100.0f;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio")
	UAudioCaptureComponent* AudioCaptureComp;

	UFUNCTION()
	void OnAudioEnvelopeValue(const float EnvelopeValue);
	UFUNCTION(BlueprintCallable)
	float GetEnvelopeValue() const { return CurrentEnvelopeValue; }
	UFUNCTION(BlueprintCallable)
	void SetMicVolume(const float NewVolume = 100.0f); 
};


