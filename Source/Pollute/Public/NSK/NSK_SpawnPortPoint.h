#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Components/StaticMeshComponent.h>

#include "NSK_SpawnPortPoint.generated.h"

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

    // 이렇게 하면 링크 오류남 블프에서 Run or Server 설정 관련
    //UFUNCTION(Server, Reliable, WithValidation)
    //void RandomSpawnPortPoint(UObject* WorldContextObject);

    UFUNCTION(BlueprintCallable, Category = "Port")
    void RandomSpawnPortPoint(UObject* WorldContextObject);

    UFUNCTION(BlueprintCallable, Category = "Port")
    void HideSpawnPortMesh();

    UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
    void Multicast_SpawnPortSelected(ANSK_SpawnPortPoint* SelectedSpawnPoint);

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Port")
    UStaticMeshComponent* SpawnPortMesh;
};
