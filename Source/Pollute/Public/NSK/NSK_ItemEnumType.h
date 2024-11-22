// Enum�� �����ϰ� �ٸ� Ŭ������ ����� �ʿ�� ���� ���� ��
// ������ Enum�� �����ϱ� ���� C++ ������ ������ �� C++ �θ� Ŭ���� ������ None���� �ϴ� ���� �����ϴ�.
// �ڵ��� ���Ἲ�� ������������ ���� �� �ִ�.

// ����ü�� ������ ���̺��� Ȱ���Ͽ� ���� ���� XXXXXXXXXXXXXXX

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
UENUM(BlueprintType)
enum class EWeaponEnumType : uint8
{
	None            UMETA(DisplayName = "None"),		// �⺻������ ���
	MagicDagger     UMETA(DisplayName = "���� �ܰ�"),		// ���� �ܰ�
	MagicGun        UMETA(DisplayName = "������")		// ������
};

// Ż�� ������ : ��ٸ�, ���ϻ�, ���� ������(���� ������ ���� �ο�)
UENUM(BlueprintType)
enum class EEscapeEnumType : uint8
{
	None            UMETA(DisplayName = "None"),		// �⺻������ ���
	Ladder          UMETA(DisplayName = "��ٸ�"),		// ��ٸ�
	Parachute       UMETA(DisplayName = "���ϻ�"),		// ���ϻ�
	CurseItem       UMETA(DisplayName = "���� ������")	// ���� ������ ���� �ο�
};