//#pragma once
//
//#include "CoreMinimal.h"
//#include "Engine/DataTable.h"
//#include "Engine/StaticMesh.h"
//#include "NSK_ItemDataStruct.generated.h"
//
//USTRUCT(BlueprintType)
//struct FSpawnItemData : public FTableRowBase
//{
//    GENERATED_BODY()
//
//public:
//    // 아이템 이름
//    UPROPERTY(EditAnywhere, BlueprintReadWrite)
//    FName ItemName;
//
//    // 스태틱 메시 경로
//    UPROPERTY(EditAnywhere, BlueprintReadWrite)
//    UStaticMesh* ItemMesh;
//
//    // 아이템 설명
//    UPROPERTY(EditAnywhere, BlueprintReadWrite)
//    FText ItemDescription;
//
//    // 카테고리 (제단(Altar), 무기, 탈출 등)
//    UPROPERTY(EditAnywhere, BlueprintReadWrite)
//    FString ItemCategory;
//};