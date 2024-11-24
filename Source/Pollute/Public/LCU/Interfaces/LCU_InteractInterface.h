// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "LCU_InteractInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class ULCU_InteractInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class POLLUTE_API ILCU_InteractInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// 상호작용을 받는 입장에서 호출
	virtual void Interact() = 0;	
};
