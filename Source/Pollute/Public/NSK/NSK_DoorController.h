#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/StaticMeshActor.h"

#include "NSK_DoorController.generated.h"

class ANSK_DoorActor;

UCLASS()
class POLLUTE_API ANSK_DoorController : public AActor
{
	GENERATED_BODY()
	
public:	
	ANSK_DoorController();

protected:
    virtual void BeginPlay() override;

public:
    // 문 액터 레퍼런스
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door")
    TSubclassOf<ANSK_DoorActor> LeftDoorClass; // BP_NSK_L_DoorActor

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door")
    TSubclassOf<ANSK_DoorActor> RightDoorClass; // BP_NSK_R_DoorActor

    UPROPERTY(BlueprintReadOnly, Category = "Door")
    ANSK_DoorActor* LeftDoorInstance;

    UPROPERTY(BlueprintReadOnly, Category = "Door")
    ANSK_DoorActor* RightDoorInstance;

    // 문 열기 함수
    UFUNCTION(BlueprintCallable, Category = "Door")
    void OpenDoor();
};
