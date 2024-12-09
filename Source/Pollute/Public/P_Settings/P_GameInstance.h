// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "P_GameInstance.generated.h"

/**
 * 
 */

enum class EPlayerType : uint8;
DECLARE_DELEGATE_ThreeParams(FAddSession, int32, FString, FString)
DECLARE_DELEGATE_OneParam(FFindComplete, bool)
DECLARE_DELEGATE_OneParam(FCreateComplete, bool)

UCLASS()
class POLLUTE_API UP_GameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
    
	virtual void Init() override;

	UFUNCTION(BlueprintCallable, Category = "Session")
	void SetSessionKey(FName NewKey) { SessionKey = NewKey; }
	
	// 세션 생성, 델리게이트
	UFUNCTION()
	void CreateOwnSession(FName SessionName, int32 PlayerLimit);
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
	// 세션 파괴, 델리게이트
	UFUNCTION()
	void DestroyOwnSession(FName SessionName);
	void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);
	// 세션 검색, 델리게이트
	UFUNCTION()
	void FindSessions();
	void OnFindSessionsComplete(bool bWasSuccessful);
	// 세션 참가, 델리게이트
	UFUNCTION()
	void JoinSelectSession(int32 Idx);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

    UFUNCTION()
    FName GetCurrentSessionName() { return JoinedSessionName; }

    FString GetLobbyLevelURL() { return SessionLobbyLevelURL; }
    FString GetMainGameLevelURL() { return MainGameLevelURL; }
    
protected:
    
	// 세션 생성 키 값
	FName SessionKey = NAME_None;
	// 온라인 세션 인터페이스
	IOnlineSessionPtr SessionInterface;
	// 세션 검색 처리
	TSharedPtr<FOnlineSessionSearch> SessionSearch;
    FName JoinedSessionName;
    // 레벨 URL 변수
    UPROPERTY(EditAnywhere, Category = "Session")
    FString SessionLobbyLevelURL;
    UPROPERTY(EditAnywhere, Category = "Session")
    FString MainGameLevelURL;
    // 플레이어의 캐릭터 타입 저장 변수
    
public:
    
    TArray<EPlayerType> PlayerTypes;
    
	// 세션이 검색 되었을 때 각 세션의 정보를 전달해주는 델리게이트
	FAddSession OnAddSessionDelegates;
	// 세션 검색이 완료 될 때 전달해주는 델리게이트
	FFindComplete OnFindCompleteDelegates;
    // 세션 생성이 완료 될 때 전달해주는 델리게이트
    FCreateComplete OnCreateCompleteDelegates;

    // 저장된 플레이어의 캐릭터 타입을 반환해 주는 함수
    TArray<EPlayerType> GetPlayerTypes() const { return PlayerTypes; }
private:
	
};
