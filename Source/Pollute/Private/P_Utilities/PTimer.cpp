// Fill out your copyright notice in the Description page of Project Settings.


#include "Pollute/Public/P_Utilities/PTimer.h"
#include "HAL/PlatformTime.h"


PTimer* PTimer::Instance = nullptr;

PTimer::PTimer()
{
}

PTimer::~PTimer()
{
}
bool PTimer::StartTimerWithLock(const FString& Tag)
{
	FScopeLock Lock(&CriticalSection);
	if (!TimerMap.Contains(Tag))
	{
		TimerMap.Add(Tag, FPlatformTime::Seconds());
		return true;
	}
	UE_LOG(PolluteLog, Log, TEXT("Already Contains TimerMap this Tag"));
	return false;
}

TOptional<double> PTimer::StopTimerWithLock(const FString& Tag)
{
	FScopeLock Lock(&CriticalSection);
	if (TimerMap.Contains(Tag))
	{
		double StartTime = TimerMap[Tag];
		TimerMap.Remove(Tag);
		return FPlatformTime::Seconds() - StartTime;
	}
	UE_LOG(PolluteLog, Log, TEXT("No timer found with tag '%s'"), *Tag);
	return TOptional<double>();
}

double PTimer::GetElapsedTime(const FString& Tag)
{
	FScopeLock Lock(&CriticalSection);
	if (TimerMap.Contains(Tag))
	{
		return FPlatformTime::Seconds() - TimerMap[Tag];
	}
	UE_LOG(LogTemp, Warning, TEXT("GetElapsedTime: No timer found with tag '%s'."), *Tag);
	return -1.0;
}