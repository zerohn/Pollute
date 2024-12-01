#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Components/BoxComponent.h>

#include "NSK_EscapeCollision.generated.h"

UCLASS()
class POLLUTE_API ANSK_EscapeCollision : public AActor
{
	GENERATED_BODY()
	
public:	
	ANSK_EscapeCollision();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
    UBoxComponent* EscapeTrigger;

protected:
	virtual void BeginPlay() override;

private:
    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};