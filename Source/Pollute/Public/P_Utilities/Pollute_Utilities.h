// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

// 로그 카테고리 선언
DECLARE_LOG_CATEGORY_EXTERN(PolluteLog, Log, All);

// 디버그 로그 매크로
#define P_LOG(CategoryName, Verbosity, Format, ...) \
UE_LOG(CategoryName, Verbosity, TEXT("[%s::%s] ") Format, *GetNameSafe(this), *FString(__FUNCTION__), ##__VA_ARGS__)

// 화면 디버그 용 매크로
// #define GET_SCREEN_DEBUG_OVERRIDE_MACRO(_1, _2, _3, _4, _5, _6, NAME, ...) NAME
// #define P_SCREEN(...) GET_SCREEN_DEBUG_OVERRIDE_MACRO(__VA_ARGS__, P_SCREEN_CUSTOM_KEY, P_SCREEN_DEFAULT_KEY)(__VA_ARGS__)

#define P_SCREEN(Time, Color, Format, ...) \
if (GEngine) \
{ \
	FString DebugMessage = FString::Printf(TEXT("[%s::%s] " Format), *GetNameSafe(this), *FString(__FUNCTION__), ##__VA_ARGS__); \
	GEngine->AddOnScreenDebugMessage(1, Time, Color, DebugMessage); \
}

// #define P_SCREEN_CUSTOM_KEY(Key, Time, Color, Format, ...) \
// if (GEngine) \
// { \
// 	FString DebugMessage = FString::Printf(TEXT("[%s::%s] " Format), *GetNameSafe(this), *FString(__FUNCTION__), ##__VA_ARGS__); \
// 	GEngine->AddOnScreenDebugMessage(Key, Time, Color, DebugMessage); \
// }
