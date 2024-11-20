// Copyright Epic Games, Inc. All Rights Reserved.

#include "Pollute.h"
#include "Modules/ModuleManager.h"
#include "Pollute/Public/P_Utilities/PTimer.h"

IMPLEMENT_PRIMARY_GAME_MODULE(FDefaultGameModuleImpl, Pollute, "Pollute");

void FPolluteModule::StartupModule()
{
	IModuleInterface::StartupModule();
	PTimer::Get();
}

void FPolluteModule::ShutdownModule()
{
	PTimer::Destroy();
	IModuleInterface::ShutdownModule();
}
