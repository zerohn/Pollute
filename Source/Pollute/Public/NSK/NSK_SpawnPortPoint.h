#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Components/StaticMeshComponent.h>

#include "NSK_SpawnPortPoint.generated.h"

class UBoxComponent;

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

    // 서버에서 클라로 삭제를 알리는 RPC 함수
    UFUNCTION(Server, Reliable)
    void ServerNotifyPortDestruction();

    // 클라에서 서버로 포트 삭제 요청 함수
    UFUNCTION(Client, Reliable)
    void ClientNotifyPortDestruction();

private:

    bool bHasOverlapped;

    bool bIsPendingDestroy = false;

    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
