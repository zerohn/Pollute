#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Components/StaticMeshComponent.h>

#include "NSK_UseParachute.generated.h"

UCLASS()
class POLLUTE_API ANSK_UseParachute : public AActor
{
	GENERATED_BODY()
	
public:	
	ANSK_UseParachute();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Parachute")
    UStaticMeshComponent* UseParachute;
};
