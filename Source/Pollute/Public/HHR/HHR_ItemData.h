// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "HHR_ItemData.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EItemType : uint8
{
	WeaponItem,
	CombineItem,
	EscapeItem,
};


// 아이템 구조체
USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase
{
	GENERATED_BODY()

	// 아이템 id
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 ItemID = -1;

	// 아이템 이름
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FText ItemName;

	// UI에 띄울 아이템 이름
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FText ItemUIName;
	
	// 아이템 타입
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	EItemType ItemType;


	// 아이템 staticmesh
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	class UStaticMesh* ItemMesh;

	// Hand UI에 표시될 아이템 image
    // Texture2D를 직접 가지는 것 보다 경로로 저장해서 나중에 로드 하는게 비용적으로 좋지 않을까 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	class UTexture2D* ItemImage;

	// 캐릭터 socket에 부착되기 위한 Rotation, Location 값
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    FVector ItemLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FRotator ItemRotation;
	
    // 비교 연산자 정의
    bool operator==(const FItemData& Other) const
    {
        return ItemID == Other.ItemID && ItemName.EqualTo(Other.ItemName);
    }
};


class POLLUTE_API HHR_ItemData
{
public:
	HHR_ItemData();
	~HHR_ItemData();
};
