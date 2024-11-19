// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleInterface.h"
#include "Pollute/P_Utilities/Public/Pollute_Utilities.h"

class FPolluteModule : public IModuleInterface
{
	public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};