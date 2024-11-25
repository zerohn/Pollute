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
    // 힌트의 이름
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hint")
    FName HintName;

    // 힌트 메시
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hint")
    UStaticMesh* HintMesh;

    // 힌트 표시 문구
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hint")
    FText HintDescription;
};