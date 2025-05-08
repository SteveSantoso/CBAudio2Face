// Copyright Epic Games, Inc. All Rights Reserved.

#include "CBAudio2Face.h"

#ifdef _WIN64
#include "Windows/AllowWindowsPlatformTypes.h"
#include "Windows.h"
#include "stdio.h"
#include "TlHelp32.h"
#include "Misc/Paths.h"
#include "Containers/Array.h"
#include "Algo/Reverse.h"
#include "tchar.h"
#include "Winternl.h"
#include "cstdio"
#include "iostream"
#include "vector"
#endif

#include "CBAudio2FaceProcess.h"
#include "CBAudio2FaceSettings.h"

#define LOCTEXT_NAMESPACE "FCBAudio2FaceModule"

DEFINE_LOG_CATEGORY(LogCBAudio2Face);
DEFINE_LOG_CATEGORY(LogCBAudioToLipsync);

void FCBAudio2FaceModule::StartupModule()
{
	if(!UCBAudio2FaceSettings::Get()->bActive)
	{
		return;
	}
	
	A2FApp=MakeShared<FCBAudio2FaceProcess>();

	TSharedPtr<FCBAudio2FaceProcess> A2FAppLocal = A2FApp;

	FCoreDelegates::OnFEngineLoopInitComplete.AddLambda([A2FAppLocal]()
	{
		A2FAppLocal->Start();
	});
}

void FCBAudio2FaceModule::ShutdownModule()
{

	if(!UCBAudio2FaceSettings::Get()->bActive)
	{
		return;
	}

	if(!A2FApp)
	{
		return;
	}
	
	A2FApp->Shutdown();

	FCoreDelegates::OnFEngineLoopInitComplete.RemoveAll(this);

	if(IsProcessRunning(TEXT("kit.exe")))
	{
		FString Filter;
		ExeTerminator(TEXT("kit.exe"),false,true,true,Filter);
	}
	
#if WITH_EDITOR
	FCoreDelegates::OnPostEngineInit.RemoveAll(this);
#endif
}

bool FCBAudio2FaceModule::IsProcessRunning(FString Name)
{
	bool LocalBool = false;
#ifdef _WIN64
	HANDLE LocalHANDLE = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (LocalHANDLE != INVALID_HANDLE_VALUE)
	{
		PROCESSENTRY32 LocalPROCESSENTRY32;
		LocalPROCESSENTRY32.dwSize = sizeof(LocalPROCESSENTRY32);
		if (::Process32First(LocalHANDLE, &LocalPROCESSENTRY32))
		{
			do {
				if (LocalPROCESSENTRY32.szExeFile == Name)
				{
					LocalBool = true;
				}
			} while (::Process32Next(LocalHANDLE, &LocalPROCESSENTRY32));
		}

	}
	::CloseHandle(LocalHANDLE);
#endif
	return LocalBool;
}

bool FCBAudio2FaceModule::ExeTerminator(FString Name, bool bUsePID, bool bForceToBeTerminated,
	bool bTerminateAllLaunchedChildProcesses, FString Filter)
{
#ifdef _WIN64
	bool LocalBool = false;
	HANDLE LocalHANDLE = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (LocalHANDLE != INVALID_HANDLE_VALUE)
	{
		PROCESSENTRY32 LocalPROCESSENTRY32;
		LocalPROCESSENTRY32.dwSize = sizeof(LocalPROCESSENTRY32);
		if (::Process32First(LocalHANDLE, &LocalPROCESSENTRY32))
		{
			do {
				if (bUsePID)
				{
					if (LocalPROCESSENTRY32.th32ProcessID == FCString::Atoi(*Name))
					{
						LocalBool = true;
					}
				}
				else
				{
					if (LocalPROCESSENTRY32.szExeFile == Name)
					{
						LocalBool = true;
					}
				}
			} while (::Process32Next(LocalHANDLE, &LocalPROCESSENTRY32));
		}
	}
	::CloseHandle(LocalHANDLE);
	if (LocalBool)
	{
		FString LocalString = ("taskkill");
		if (bForceToBeTerminated)
		{
			LocalString += (" /f");
		}
			
		if (bUsePID)
		{
			LocalString += (" /pid " + Name);
		}
		else
		{
			LocalString += (" /im " + Name);
		}
		if (bTerminateAllLaunchedChildProcesses)
		{
			LocalString += (" /t");
		}
			
		if (Filter.Len() > 0)
		{
			LocalString += (" /fi \"" + Filter + "\"");
		}
			
		char* LocalChar = TCHAR_TO_ANSI(*LocalString);
		system(LocalChar);
		return true;
	}
#endif
	
	return false;
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FCBAudio2FaceModule, CBAudio2Face)