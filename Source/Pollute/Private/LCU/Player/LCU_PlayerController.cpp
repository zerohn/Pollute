// Fill out your copyright notice in the Description page of Project Settings.


#include "LCU/Player/LCU_PlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "OnlineSubsystemUtils.h"
#include "NavigationSystemTypes.h"
#include "VoiceModule.h"
#include "Animation/AnimInstance.h"
#include "Components/AudioComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/SpectatorPawn.h"
#include "HHR/UI/HHR_PlayerHUD.h"
#include "Kismet/GameplayStatics.h"
#include "LCU/Player/LCU_MonsterCharacter.h"
#include "LCU/Player/LCU_PlayerCharacter.h"
#include "LCU/UI/LCU_UIManager.h"
#include "Net/UnrealNetwork.h"
#include "P_Settings/P_GameState.h"
#include "P_Settings/P_PlayerState.h"
#include "Runtime/Online/Voice/Public/Interfaces/VoiceCapture.h"
#include "Sound/SoundWaveProcedural.h"

ALCU_PlayerController::ALCU_PlayerController()
{
    bReplicates = true;

    VoiceChatComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("VoiceChatComponent"));
    VoiceChatComponent->bAutoActivate = false;
    VoiceChatComponent->bAllowSpatialization = true;

    VoiceChatSoundWave = NewObject<USoundWaveProcedural>();
    VoiceChatSoundWave->SetSampleRate(16000);
    VoiceChatSoundWave->NumChannels = 2;
    VoiceChatSoundWave->Duration = INDEFINITELY_LOOPING_DURATION;

    VoiceChatComponent->SetSound(VoiceChatSoundWave);
}

void ALCU_PlayerController::BeginPlay()
{
	Super::BeginPlay();

    // 음성채팅 입력 바인딩
    UEnhancedInputLocalPlayerSubsystem* Subsys = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(this->GetLocalPlayer());
    if (Subsys && IMC_VoiceChat)
    {
        Subsys->AddMappingContext(IMC_VoiceChat, 1);
    }

    // if (VoiceChatComponent)
    // {
    //     VoiceChatComponent->Activate();
    // }
    
    if(!IsLocalController()) return;
    if(UIManagerFactory)
    {
        UIManager = GetWorld()->SpawnActor<ALCU_UIManager>(UIManagerFactory);
        UIManager->SetOwner(this);
        UIManager->Init();
    }
}

void ALCU_PlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void ALCU_PlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALCU_PlayerController::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

}

void ALCU_PlayerController::ServerRPC_ChangeToSpector_Implementation()
{
	// 현재 Possess 하고 있는 폰 가져오기
	APawn* player = GetPawn();
    ALCU_PlayerCharacter* PlayerCharacter = Cast<ALCU_PlayerCharacter>(player);
    if(PlayerCharacter && HasAuthority())
    {
        PlayerCharacter->NetMulticast_DetachItem();
    }
    
    if(PlayerCharacter && PlayerCharacter->GetHasCurse())
    {
        AP_GameState* GS = Cast<AP_GameState>(UGameplayStatics::GetGameState(GetWorld()));
        if (GS)
        {
            GS->RestartCurse();
        }
    }
    
    ClientRPC_ItemUIOff();
	UnPossess();

	// Spector Pawn 생성
	AGameModeBase* gm = GetWorld()->GetAuthGameMode();
	ASpectatorPawn* spector = GetWorld()->SpawnActor<ASpectatorPawn>(gm->SpectatorClass,player->GetActorTransform());

	// 생성된 Spector 로 Possess
	Possess(spector);
    
    if (IsValid(player))
    {
        player->Destroy();
    }
    
    AP_GameState* GS =Cast<AP_GameState>(UGameplayStatics::GetGameState(GetWorld()));
    if (GS)
    {
        GS->GetAllCharacters();
    }
}

void ALCU_PlayerController::ChangeToSpector()
{
    // 현재 Possess 하고 있는 폰 가져오기
    APawn* player = GetPawn();
    ALCU_PlayerCharacter* PlayerCharacter = Cast<ALCU_PlayerCharacter>(player);
    if(PlayerCharacter && HasAuthority())
    {
        PlayerCharacter->NetMulticast_DetachItem();
    }
    
    if(PlayerCharacter && PlayerCharacter->GetHasCurse())
    {
        AP_GameState* GS = Cast<AP_GameState>(UGameplayStatics::GetGameState(GetWorld()));
        if (GS)
        {
            GS->RestartCurse();
        }
    }
    
    ClientRPC_ItemUIOff();
    UnPossess();

    // Spector Pawn 생성
    AGameModeBase* gm = GetWorld()->GetAuthGameMode();
    ASpectatorPawn* spector = GetWorld()->SpawnActor<ASpectatorPawn>(gm->SpectatorClass,player->GetActorTransform());

    // 생성된 Spector 로 Possess
    Possess(spector);

    player->Destroy();

    AP_GameState* GS =Cast<AP_GameState>(UGameplayStatics::GetGameState(GetWorld()));
    if (GS)
    {
        GS->GetAllCharacters();
    }
}

void ALCU_PlayerController::ChangeToMonster()
{
    if(!IsValid(MonChar)) return;
    // 현재 Possess 하고 있는 폰 가져오기
    APawn* player = GetPawn();
    ALCU_PlayerCharacter* PlayerCharacter = Cast<ALCU_PlayerCharacter>(player);
    if(PlayerCharacter && HasAuthority())
    {
        PlayerCharacter->NetMulticast_DetachItem();
    }
    ClientRPC_ItemUIOff();
    UnPossess();

    // Spector Pawn 생성
    AGameModeBase* gm = GetWorld()->GetAuthGameMode();
    ALCU_MonsterCharacter* MonSter = GetWorld()->SpawnActor<ALCU_MonsterCharacter>(MonChar,player->GetActorTransform());

    // 생성된 Spector 로 Possess
    Possess(MonSter);

    player->Destroy();
    
    AP_GameState* GS =Cast<AP_GameState>(UGameplayStatics::GetGameState(GetWorld()));
    if (GS)
    {
        GS->GetAllCharacters();
    }
}

void ALCU_PlayerController::CaptureVoiceData()
{
    TSharedPtr<IVoiceCapture> VoiceCapture = FVoiceModule::Get().CreateVoiceCapture(GetPlayerState<AP_PlayerState>()->GetPlayerName());
    
}

void ALCU_PlayerController::PlayVoiceData(const TArray<uint8>& VoiceData)
{
}

void ALCU_PlayerController::EnableVoiceChat()
{
    P_LOG(PolluteLog, Warning, TEXT("EnableVoiceChat"))
     IOnlineSubsystem* OnlineSubsystem = Online::GetSubsystem(GetWorld());
     if (OnlineSubsystem)
     {
         IOnlineVoicePtr VoiceInterface = OnlineSubsystem->GetVoiceInterface();
         if (VoiceInterface.IsValid())
         {
             VoiceInterface->StartNetworkedVoice(GetLocalPlayer()->GetControllerId());
         }
     }
}

void ALCU_PlayerController::DisableVoiceChat()
{
    P_LOG(PolluteLog, Warning, TEXT("DisableVoiceChat"))
     IOnlineSubsystem* OnlineSubsystem = Online::GetSubsystem(GetWorld());
     if (OnlineSubsystem)
     {
         IOnlineVoicePtr VoiceInterface = OnlineSubsystem->GetVoiceInterface();
         if (VoiceInterface.IsValid())
         {
             VoiceInterface->StopNetworkedVoice(GetLocalPlayer()->GetControllerId());
         }
     }
}

void ALCU_PlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
    {
        EnhancedInputComponent->BindAction(IA_PushToTalk, ETriggerEvent::Started, this, &ALCU_PlayerController::EnableVoiceChat);
        EnhancedInputComponent->BindAction(IA_PushToTalk, ETriggerEvent::Completed, this, &ALCU_PlayerController::DisableVoiceChat);
    }
}

void ALCU_PlayerController::CurseUISet(bool bShow)
{
    if(!UIManager) return;

    UIManager->ShowCurseWidget(bShow);
}

void ALCU_PlayerController::ClientRPC_CurseUISet_Implementation(bool bShow)
{
    if(!UIManager) return;

    UIManager->ShowCurseWidget(bShow);
}

void ALCU_PlayerController::ClientRPC_ItemUIOff_Implementation()
{
    if(!UIManager) return;
    if(!UIManager->PlayerHUD) return;
    UIManager->PlayerHUD->SetVisibility(ESlateVisibility::Hidden);
}
