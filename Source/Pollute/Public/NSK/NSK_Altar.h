#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NSK_Altar.generated.h"

UCLASS()
class POLLUTE_API ANSK_Altar : public AActor
{
	GENERATED_BODY()
	
public:
	ANSK_Altar();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Altar") // 블프에서 제단 메시를 설정
	UStaticMeshComponent* AltarMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Altar") // 제단의 이름
	FName AltarName;
};
