// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "P_GameInstance.generated.h"


/**
 * 
 */

DECLARE_DELEGATE_OneParam(FAddSeesion, FString)

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

protected:
	// 세션 생성 키 값
	FName SessionKey = NAME_None;
	// 온라인 세션 인터페이스
	IOnlineSessionPtr SessionInterface;
	// 세션 검색 처리
	TSharedPtr<FOnlineSessionSearch> SessionSearch;
	// 세션이 검색 되었을 때 각 세션의 정보를 전달해주는 델리게이트
	//FAddSession OnAddSession;
private:
	
};
