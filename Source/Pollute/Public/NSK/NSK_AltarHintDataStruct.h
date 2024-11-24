#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Engine/StaticMesh.h"
#include "NSK_AltarHintDataStruct.generated.h"

USTRUCT(BlueprintType)
struct FAltarHintData : public FTableRowBase
{
    GENERATED_BODY()

public:
    // ��Ʈ�� �̸�
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hint")
    FName HintName;

    // ��Ʈ �޽�
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hint")
    UStaticMesh* HintMesh;

    // ��Ʈ ǥ�� ����
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hint")
    FText HintDescription;
};