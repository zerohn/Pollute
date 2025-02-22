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

    SetSessionKey(FName("Pollute"));
	// 서브시스템 참조
	IOnlineSubsystem* Subsys = IOnlineSubsystem::Get();
	if (Subsys)
	{
		// 서브 시스템 세션 인터페이스 가져오기
		SessionInterface = Subsys->GetSessionInterface();
		// 세션 델리게이트 바인딩 (생성, 파괴, 검색, 참가)
		SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UP_GameInstance::OnCreateSessionComplete);
		SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UP_GameInstance::OnDestroySessionComplete);
		SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UP_GameInstance::OnFindSessionsComplete);
		SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UP_GameInstance::OnJoinSessionComplete);
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
    // 세션 이름 저장
    JoinedSessionName = SessionName;
	// 클라이언트의 Unique Net ID 가져오기
	FUniqueNetIdPtr NetID = GetWorld()->GetFirstLocalPlayerFromController()->GetUniqueNetIdForPlatformUser().GetUniqueNetId();
	// 세션 생성
	SessionInterface->CreateSession(*NetID, SessionName, SessionSettings);
	
}

void UP_GameInstance::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		// 세션 만든 클라이언트에서 만들어진 세션의 정해진 시작 레벨로 이동
		GetWorld()->ServerTravel(SessionLobbyLevelURL + TEXT("?Listen"));
	}
	else
	{
	    OnCreateCompleteDelegates.ExecuteIfBound(false);
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
		//P_SCREEN(3.0f, FColor::Green, TEXT("세션 파괴 성공"));
	}
	else
	{
		//P_SCREEN(3.0f, FColor::Orange, TEXT("세션 파괴 실패"));
	}
}

void UP_GameInstance::FindSessions()
{
	//P_SCREEN(3.0f, FColor::Black, TEXT("세션 검색 시작"));
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

		for (int32 i = 0; i < SearchResults.Num(); i++)
		{
			FOnlineSessionSearchResult SearchResult = SearchResults[i];
			// 세션 이름 가져오기
			FString SessionName;
			SearchResult.Session.SessionSettings.Get(SessionKey, SessionName);

			// 세션을 만든 유저 이름 가져오기
			FString SessionCreator = SearchResult.Session.OwningUserName;

			//FString SessionInfo = FString::Printf(TEXT("%s - %s"), *SessionName, *SessionCreator);
			OnAddSessionDelegates.ExecuteIfBound(i, SessionName, SessionCreator);
		}
	}
	OnFindCompleteDelegates.ExecuteIfBound(true);
}

void UP_GameInstance::JoinSelectSession(int32 Idx)
{
	TArray<FOnlineSessionSearchResult> SearchResults = SessionSearch->SearchResults;
	FString SessionName;
	// 세션 정보 받아오기
	SearchResults[Idx].Session.SessionSettings.Get(SessionKey, SessionName);
    JoinedSessionName = FName(SessionName);
	// 세션 정보와 일치하는 세션에 참가하기
	SessionInterface->JoinSession(0, FName(SessionName), SearchResults[Idx]);
}

void UP_GameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	// 세션 참가 성공 시 동작
	if (Result == EOnJoinSessionCompleteResult::Success)
	{
	    JoinedSessionName = SessionName;
		// 세션 레벨 정보 URL 검색
		FString URL;
		SessionInterface->GetResolvedConnectString(SessionName, URL);
		// 클라이언트에서 참가한 세션 시작 레벨 URL의 경로에 있는 레벨 입장
		APlayerController* PC = GetWorld()->GetFirstPlayerController();
		PC->ClientTravel(URL, ETravelType::TRAVEL_Absolute);
	}
}