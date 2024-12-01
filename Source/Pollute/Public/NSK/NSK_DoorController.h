#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/StaticMeshActor.h"

#include "NSK_DoorController.generated.h"

UCLASS()
class POLLUTE_API ANSK_DoorController : public AActor
{
	GENERATED_BODY()
	
public:	
	ANSK_DoorController();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door")
    TArray<AStaticMeshActor*> DoorMeshes;

    UFUNCTION(BlueprintCallable, Category = "Door")
    void OpenDoor();

    UFUNCTION(BlueprintCallable, Category = "Door")
    void CloseDoor();

protected:
	virtual void BeginPlay() override;

private:
    bool bIsDoorOpen = false;
};
