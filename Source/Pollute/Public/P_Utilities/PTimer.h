// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HAL/CriticalSection.h"

/**
 * 
 */
// 싱글턴 입니다.
class POLLUTE_API PTimer
{
public:
	PTimer();
	~PTimer();


private:
	TMap<FString, double> TimerMap;
	FCriticalSection CriticalSection;

	static PTimer* Instance;

public:
	static PTimer& Get()
	{
		if(!Instance)
		{
			Instance = new PTimer();
		}
		return *Instance;
	}

	static void Destroy()
	{
		delete Instance;
		Instance = nullptr;
	}

	//
	bool StartTimerWithLock(const FString& Tag);
	TOptional<double> StopTimerWithLock(const FString& Tag);

	// 특정 타이머의 경과 시간을 반환
	double GetElapsedTime(const FString& Tag);
};
