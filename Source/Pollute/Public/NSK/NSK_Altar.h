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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Altar") // �������� ���� �޽ø� ����
	UStaticMeshComponent* AltarMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Altar") // ������ �̸�
	FName AltarName;
};
