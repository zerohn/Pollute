// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HHR_Gate.generated.h"

class UBoxComponent;
class UStaticMeshComponent;
UCLASS()
class POLLUTE_API AHHR_Gate : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHHR_Gate();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

// #### Variables ####
//*Components*
protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
    UBoxComponent* BoxColliderComp;
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
    UStaticMeshComponent* FenceMeshComp;
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
    UStaticMeshComponent* LeftDoorMeshComp;
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
    UStaticMeshComponent* RightDoorMeshComp;

    // Timeline Component
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
    class UTimelineComponent* TimelineComp;

// *Timeline 관련 변수*
protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Timeline")
    class UCurveFloat* OpenDoorCurve;
    

// #### Functions ####
// *문 여는 함수* (Altar의 델리게이트에 바인딩)
private:
    UFUNCTION()
    void OpenDoor();

// *RPC 함수*
private:
    UFUNCTION(NetMulticast, Reliable)
    void NetMulticast_OpenDoor();

//*Timeline 관련된 함수*
private:
    UFUNCTION()
    void OpenDoorTimeline(float value);

// *충돌 처리 함수*
protected:
    UFUNCTION()
    void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                    bool bFromSweep, const FHitResult & SweepResult);


    

};
