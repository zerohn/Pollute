#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Components/StaticMeshComponent.h>

#include "NSK_SpawnPortPoint.generated.h"

class UBoxComponent;
class ALCU_PlayerCharacter;

UCLASS()
class POLLUTE_API ANSK_SpawnPortPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	ANSK_SpawnPortPoint();

protected:
	virtual void BeginPlay() override;

public:

    bool bSpawnPortIsUsed = false;

    UFUNCTION(BlueprintCallable, Category = "Port")
    void RandomSpawnPortPoint(UObject* WorldContextObject);

    UFUNCTION(BlueprintCallable, Category = "Port")
    void HideSpawnPortMesh();

    UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
    void Multicast_SpawnPortSelected(ANSK_SpawnPortPoint* SelectedSpawnPoint);

    UFUNCTION(NetMulticast, Reliable)
    void Multicast_DestroyPort();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Port")
    UStaticMeshComponent* SpawnPortMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
    UBoxComponent* OverlapBox;

private:

    bool bHasOverlapped = false;

    bool bIsPendingDestroy = false;

    UFUNCTION(BlueprintCallable)
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
