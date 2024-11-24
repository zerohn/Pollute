#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "NSK_AltarHintPoint.generated.h"

UCLASS()
class POLLUTE_API ANSK_AltarHintPoint : public AActor
{
	GENERATED_BODY()
	
public:	
    ANSK_AltarHintPoint();

protected:
	virtual void BeginPlay() override;

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HintPoint")
    FName HintName; // ��Ʈ �̸�

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HintPoint")
	bool bHintPointIsUsed = false; // ���� ����Ʈ�� ���Ǿ����� ����

	UFUNCTION(BlueprintCallable, Category = "HintPoint") // �������� ���ü��� �����ؾ� �ϴ� ���
	void HideHintPointMesh(); // ���� ���� �� ���� ����Ʈ �޽ø� �����ϴ� �Լ�

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HintPoint")
	UStaticMeshComponent* HintPointMesh; // ����ƽ �޽� ������Ʈ
};