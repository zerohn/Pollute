// Fill out your copyright notice in the Description page of Project Settings.


#include "Pollute/P_Utilities/Public/PTimer.h"

#include "AnalyticsProviderETEventCache.h"
#include "HAL/PlatformTime.h"


PTimer* PTimer::Instance = nullptr;

PTimer::PTimer()
{
}

PTimer::~PTimer()
{
}

bool PTimer::StartTimerWithTryLock(const FString& Tag)
{
	if(CriticalSection.TryLock())
	{
		if(!TimerMap.Contains(Tag))
		{
			TimerMap.Add(Tag, FPlatformTime::Seconds());
			CriticalSection.Unlock();
			return true;
		}
		else
		{
			UE_LOG(PolluteLog, Log, TEXT("Already Contains TimerMap this Tag"));
			return false;
		}
	}
	else
	{
		UE_LOG(PolluteLog, Log, TEXT("StartTimerWithTryLock: Already Contains TimerMap this Tag"));
		return false;
	}
}

double PTimer::StopTimerWithTryLock(const FString& Tag)
{
	if(CriticalSection.TryLock())
	{
		if(TimerMap.Contains(Tag))
		{
			double StartTime = TimerMap[Tag];
			TimerMap.Remove(Tag);
			CriticalSection.Unlock();
			return FPlatformTime::Seconds() - StartTime;
		}
		else
		{
			UE_LOG(PolluteLog, Log, TEXT("EndTimerWithTryLock: No timer found with tag '%s'"), *Tag);			
			return -1.f;
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("EndTimerWithTryLock: Unable to lock TimerMap for tag '%s'"), *Tag);
		return -1.f;
	}
}

double PTimer::GetElapsedTime(const FString& Tag)
{
	FScopeLock TimerLock(&CriticalSection);

	if(TimerMap.Contains(Tag))
	{
		double StartTime = TimerMap[Tag];
		return FPlatformTime::Seconds() - StartTime;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("GetElapsedTime: No timer found with tag '%s'."), *Tag);
		return -1.f;
	}
	
	return 0;
}
