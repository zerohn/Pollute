// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LCU/LCU_Properties/LCU_Property.h"
#include "LCU_PlayerController.generated.h"

class UInputAction;
class UInputMappingContext;
enum class EVoiceChannel : uint8;
class ALCU_UIManager;

/**
 * 
 */
UCLASS()
class POLLUTE_API ALCU_PlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	ALCU_PlayerController();

public:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Tick(float DeltaTime) override;

    virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(Server, Reliable)
	void ServerRPC_ChangeToSpector();

	void ChangeToSpector();
	UFUNCTION()
	void ChangeToMonster();

     

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player State")
	EPlayerMode PlayerMode = EPlayerMode::Human;

	// 몬스터 클래스 TSubclassOf 로 생성하기
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player State")
    TSubclassOf<class ALCU_MonsterCharacter> MonChar;

    // 음성 채팅
    UPROPERTY(EditDefaultsOnly, Category = "VoiceChat")
    UInputMappingContext* IMC_VoiceChat;
    UPROPERTY(EditDefaultsOnly, Category = "VoiceChat")
    UInputAction* IA_PushToTalk;

    UFUNCTION()
    void EnableVoiceChat();
    UFUNCTION()
    void DisableVoiceChat();

    virtual void SetupInputComponent() override;
    
    EVoiceChannel CurrentVoiceChannel;

    // UI 관련 작업
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UIManager")
    TSubclassOf<ALCU_UIManager> UIManagerFactory;
    
    UPROPERTY()
    ALCU_UIManager* UIManager;

    UFUNCTION(Client, Reliable)
    void ClientRPC_ItemUIOff();

    UFUNCTION()
    void CurseUISet(bool bShow);
    UFUNCTION(Client, Reliable)
    void ClientRPC_CurseUISet(bool bShow);
    
    // UI 관련 작업 END
};
