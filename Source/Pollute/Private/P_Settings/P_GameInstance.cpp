// Fill out your copyright notice in the Description page of Project Settings.


#include "P_Settings/P_GameInstance.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Engine/Engine.h"
#include "Engine/LocalPlayer.h"
#include "Engine/World.h"
#include "Online/OnlineSessionNames.h"

void UP_GameInstance::Init()
{
	Super::Init();
	
	// 서브시스템 참조
	IOnlineSubsystem* Subsys = IOnlineSubsystem::Get();
	if (Subsys)
	{
		// 서브 시스템 세션 인터페이스 가져오기
		SessionInterface = Subsys->GetSessionInterface();
		// 세션 델리게이트 바인딩 (생성, 파괴, 검색)
		SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UP_GameInstance::OnCreateSessionComplete);
		SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UP_GameInstance::OnDestroySessionComplete);
		SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UP_GameInstance::OnFindSessionsComplete);
	}
}

void UP_GameInstance::CreateOwnSession(FName SessionName, int32 PlayerLimit)
{
	// 세션 옵션 설정용 변수
	FOnlineSessionSettings SessionSettings;

	// 온라인 서브 시스템 이름 확인 (NULL인 경우 LANMacth로 동작 될 수 있게) 
	FName SubsysName = IOnlineSubsystem::Get()->GetSubsystemName();
	SessionSettings.bIsLANMatch = SubsysName.IsEqual(FName("NULL"));

	// Lobby 사용 여부 설정
	SessionSettings.bUseLobbiesIfAvailable = true;
	// 세션 검색 허용 여부 설정
	SessionSettings.bShouldAdvertise = true;
	// 세션이 진행 중일 때 참가 여부 설정
	SessionSettings.bAllowJoinInProgress = false;
	// 세션의 최대 참가 인원 설정
	SessionSettings.NumPublicConnections = PlayerLimit;
	// 세션 정보 설정 (세션 키값, 이름, 노출 정보 설정) 
	SessionSettings.Set(SessionKey, SessionName.ToString(), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	// 클라이언트의 Unique Net ID 가져오기
	FUniqueNetIdPtr NetID = GetWorld()->GetFirstLocalPlayerFromController()->GetUniqueNetIdForPlatformUser().GetUniqueNetId();
	// 세션 생성
	SessionInterface->CreateSession(*NetID, SessionName, SessionSettings);
	
}

void UP_GameInstance::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		P_SCREEN(3.0f, FColor::Green, TEXT("세션 생성 성공"));
	}
	else
	{
		P_SCREEN(3.0f, FColor::Orange, TEXT("세션 생성 실패"));
	}
}

void UP_GameInstance::DestroyOwnSession(FName SessionName)
{
	// 세션 파괴
	SessionInterface->DestroySession(SessionName);
}

void UP_GameInstance::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		P_SCREEN(3.0f, FColor::Green, TEXT("세션 파괴 성공"));
	}
	else
	{
		P_SCREEN(3.0f, FColor::Orange, TEXT("세션 파괴 실패"));
	}
}

void UP_GameInstance::FindSessions()
{
	P_SCREEN(3.0f, FColor::Black, TEXT("세션 검색 시작"));
	// 세션 검색 변수 Init
	SessionSearch = MakeShared<FOnlineSessionSearch>();
	
	// LAN Query 여부 설정
	FName SubsysName = IOnlineSubsystem::Get()->GetSubsystemName();
	SessionSearch->bIsLanQuery = SubsysName.IsEqual(FName("NULL"));

	// 활성 세션 만 검색
	SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

	// 세션 검색 수 제한 설정
	SessionSearch->MaxSearchResults = 100;

	// 세션 검색
	SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
}

void UP_GameInstance::OnFindSessionsComplete(bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		TArray<FOnlineSessionSearchResult> SearchResults = SessionSearch->SearchResults;

		for (FOnlineSessionSearchResult SearchResult : SearchResults)
		{
			// 세션 이름 가져오기
			FString SessionName;
			SearchResult.Session.SessionSettings.Get(SessionKey, SessionName);

			// 세션을 만든 유저 이름 가져오기
			FString SessionCreator = SearchResult.Session.OwningUserName;
		}
	}
}
