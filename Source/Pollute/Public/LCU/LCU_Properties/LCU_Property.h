// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"    // UObject 매크로 관련 정의
#include "LCU_Property.generated.h" // UHT에서 사용하는 헤더 반드시 포함

// 성별 애니메이션에 사용
UENUM(BlueprintType)
enum class EGender : uint8
{
	EMM UMETA(DisplayName = "Male"),
	EMF UMETA(DisplayName = "Female")
};

// 플레이어의 모드 상태 
UENUM(BlueprintType)
enum class EPlayerMode : uint8
{
	Human UMETA(DisplayName = "Human"),
	Monster UMETA(DisplayName = "Monster"),
	Spector UMETA(DisplayName = "Spector")
};
