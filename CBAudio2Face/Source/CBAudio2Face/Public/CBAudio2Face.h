// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FCBAudio2FaceProcess;

DECLARE_LOG_CATEGORY_EXTERN(LogCBAudio2Face, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogCBAudioToLipsync, Log, All);
class FCBAudio2FaceModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	TSharedPtr<FCBAudio2FaceProcess> A2FApp;

	/** A2FServer Start Delegate */
	FDelegateHandle A2FServerStartedDelegate;
public:
	static bool IsProcessRunning(FString Name);
	static bool ExeTerminator(FString Name, bool bUsePID, bool bForceToBeTerminated, bool bTerminateAllLaunchedChildProcesses, FString Filter);
};
