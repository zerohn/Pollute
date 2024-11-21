// Enum만 정의하고 다른 클래스의 상속을 필요로 하지 않을 때
// 별도의 Enum을 정의하기 위한 C++ 파일을 생성할 때 C++ 부모 클래스 선택을 None으로 하는 것이 적합하다.
// 코드의 간결성과 유지보수성을 높일 수 있다.

#pragma once

#include "CoreMinimal.h"
#include "NSK_ItemEnumType.generated.h"

// 제단 아이템 : 새의 깃털, 빨간 촛불, 인형, 흰종이, 종, 책, 못, 손수건

UENUM(BlueprintType) // 블루 프린트에서 사용 가능하게
enum class EItemEnumType : uint8
{
	None			UMETA(DisplayName = "None"),	 // 기본값으로 사용
	feather			UMETA(DisplayName = "새의 깃털"), // 새의 깃털
	candle			UMETA(DisplayName = "빨간 촛불"), // 빨간 촛불
	doll			UMETA(DisplayName = "인형"),		 // 인형
	paper			UMETA(DisplayName = "흰종이"),	 // 흰종이
	bell			UMETA(DisplayName = "종"),		 // 종
	book			UMETA(DisplayName = "책"),		 // 책
	nail			UMETA(DisplayName = "못"),		 // 못
	handkerchief	UMETA(DisplayName = "손수건")	 // 손수건
};

// 무기 아이템 : 마법 단검, 마법총

// 탈출 아이템 : 사다리, 낙하산, 저주 아이템(제단 아이템 랜덤 부여)