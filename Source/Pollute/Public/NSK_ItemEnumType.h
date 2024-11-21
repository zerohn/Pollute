// Enum�� �����ϰ� �ٸ� Ŭ������ ����� �ʿ�� ���� ���� ��
// ������ Enum�� �����ϱ� ���� C++ ������ ������ �� C++ �θ� Ŭ���� ������ None���� �ϴ� ���� �����ϴ�.
// �ڵ��� ���Ἲ�� ������������ ���� �� �ִ�.

#pragma once

#include "CoreMinimal.h"
#include "NSK_ItemEnumType.generated.h"

// ���� ������ : ���� ����, ���� �к�, ����, ������, ��, å, ��, �ռ���

UENUM(BlueprintType) // ��� ����Ʈ���� ��� �����ϰ�
enum class EItemEnumType : uint8
{
	None			UMETA(DisplayName = "None"),	 // �⺻������ ���
	feather			UMETA(DisplayName = "���� ����"), // ���� ����
	candle			UMETA(DisplayName = "���� �к�"), // ���� �к�
	doll			UMETA(DisplayName = "����"),		 // ����
	paper			UMETA(DisplayName = "������"),	 // ������
	bell			UMETA(DisplayName = "��"),		 // ��
	book			UMETA(DisplayName = "å"),		 // å
	nail			UMETA(DisplayName = "��"),		 // ��
	handkerchief	UMETA(DisplayName = "�ռ���")	 // �ռ���
};

// ���� ������ : ���� �ܰ�, ������

// Ż�� ������ : ��ٸ�, ���ϻ�, ���� ������(���� ������ ���� �ο�)