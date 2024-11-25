#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Engine/StaticMesh.h"
#include "NSK_ItemDataStruct.generated.h"

USTRUCT(BlueprintType)
struct FSpawnItemData : public FTableRowBase
{
    GENERATED_BODY()

public:
    // ������ �̸�
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName ItemName;

    // ����ƽ �޽� ���
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UStaticMesh* ItemMesh;

    // ������ ����
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText ItemDescription;

    // ī�װ� (����(Altar), ����, Ż�� ��)
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ItemCategory;
};