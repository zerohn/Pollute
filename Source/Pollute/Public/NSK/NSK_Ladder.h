#pragma once

#include "CoreMinimal.h"
#include "HHR/HHR_Item.h"
#include "NSK_Ladder.generated.h"

UCLASS()
class POLLUTE_API ANSK_Ladder : public AHHR_Item
{
	GENERATED_BODY()
	
public:
    void SetLadderRotation(FRotator Rotation);
};
