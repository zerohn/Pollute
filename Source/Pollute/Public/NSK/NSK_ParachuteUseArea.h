#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"

#include "NSK_ParachuteUseArea.generated.h"

UCLASS()
class POLLUTE_API ANSK_ParachuteUseArea : public AActor
{
	GENERATED_BODY()
	
public:	
	ANSK_ParachuteUseArea();

    virtual void BeginPlay() override;

public:	
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Parachute")
    UBoxComponent* ParachuteUseArea;

    UFUNCTION()
    void BeginOverlap(
        UPrimitiveComponent* OverlappedComp,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult& SweepResult);

    UFUNCTION()
    void EndOverlap(
        UPrimitiveComponent* OverlappedComponent,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex);
};
