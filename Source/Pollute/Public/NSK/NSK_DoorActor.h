// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NSK_DoorActor.generated.h"

UCLASS()
class POLLUTE_API ANSK_DoorActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANSK_DoorActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    // 문이 왼쪽인지 오른쪽인지 구별하는 변수
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door")
    bool bIsLeftDoor;  // 왼쪽 문인지 확인하는 플래그

    // 스태틱 메시 컴포넌트
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door")
    UStaticMeshComponent* DoorMesh;

    // 문이 열릴 목표 위치를 계산하기 위한 변수
    FVector TargetPosition;

    // 문이 열릴 때의 회전 (0도에서 90도 회전)
    FRotator TargetRotation;

    // 문 열기 및 닫기
    UFUNCTION(BlueprintCallable, Category = "Door")
    void OpenDoor();

    // 문 열리고 있을때 처리
    bool bIsOpening;

private:

    // 문이 열릴 목표 위치 및 회전 설정
    void SetTargetPositionAndRotation();

};
