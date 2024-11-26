// Fill out your copyright notice in the Description page of Project Settings.


#include "KYH/KYH_VoiceChatTest.h"
#include "AudioCaptureComponent.h"

// Sets default values
AKYH_VoiceChatTest::AKYH_VoiceChatTest()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AudioCaptureComp = CreateDefaultSubobject<UAudioCaptureComponent>("AudioCapture Comp");
	AudioCaptureComp->SetupAttachment(RootComponent);
	AudioCaptureComp->OnAudioEnvelopeValue.AddDynamic(this, &AKYH_VoiceChatTest::OnAudioEnvelopeValue);
}

// Called when the game starts or when spawned
void AKYH_VoiceChatTest::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AKYH_VoiceChatTest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AKYH_VoiceChatTest::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AKYH_VoiceChatTest::OnAudioEnvelopeValue(const float EnvelopeValue)
{
	P_LOG(PolluteLog, Warning, TEXT("Volume : %f"), EnvelopeValue);
}
