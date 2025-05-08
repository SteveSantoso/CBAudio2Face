// Copyright 2025-Present Xiao Lan fei. All Rights Reserved.


#include "CBAudio2FaceProcess.h"

#include "CBAudio2Face.h"
#include "CBAudio2FaceSettings.h"
#include "CBAudio2FaceSubsystem.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Interfaces/IPluginManager.h"

#define LOCTEXT_NAMESPACE "CBAudip2Face"

FCBAudio2FaceProcess::FCBAudio2FaceProcess()
	: Status(EProcessStatus::Stopped)
{
}

FCBAudio2FaceProcess::~FCBAudio2FaceProcess()
{
}

void FCBAudio2FaceProcess::Start()
{
	Shutdown();

	Status = EProcessStatus::Launching;

	/**找到对应启动文件描述*/
	FString Audio2faceConfig = IPluginManager::Get().FindPlugin("CBAudio2Face")->GetBaseDir() / TEXT("Prototype/audio2face_headless.kit");

	UCBAudio2FaceSettings* Settings = UCBAudio2FaceSettings::Get();
	if(!Settings)
	{
		UE_LOG(LogCBAudio2Face,Error,TEXT("Not find UCBAudio2FaceSettings"));
		return;
	}
	
	if (FPaths::DirectoryExists(Settings->Audio2FaceExePath) )
	{
		
		FPaths::NormalizeFilename(Audio2faceConfigPaths);
		
		FAsyncTaskNotificationConfig NotificationConfig;
		NotificationConfig.bKeepOpenOnFailure = true;
		NotificationConfig.TitleText = LOCTEXT("Audio2Face_Launch", "Launching Audio2Face");
#if !NO_LOGGING	
		NotificationConfig.LogCategory = &LogCBAudio2Face;
#endif
		NotificationConfig.bIsHeadless =false;
		TaskNotification = MakeUnique<FAsyncTaskNotification>(NotificationConfig);

		Thread = FRunnableThread::Create(this, TEXT("FCBAudio2FaceProcess"), 8 * 1024, TPri_BelowNormal);
	}
}

void FCBAudio2FaceProcess::Shutdown()
{
	Status = EProcessStatus::Stopped;

	if (Process.IsValid())
	{
		FPlatformProcess::TerminateProc(Process, true);
		Process.Reset();
	}

	if (Thread)
	{
		Thread->Kill(true);
		delete Thread;
		Thread = nullptr;
	}

	
}

FCBAudio2FaceProcess::EProcessStatus FCBAudio2FaceProcess::GetStatus() const
{
	return Status.load();
}

uint32 FCBAudio2FaceProcess::Run()
{

	if(FCBAudio2FaceModule::IsProcessRunning(TEXT("kit.exe")))
	{

		if(UCBAudio2FaceSubsystem* const Subsystem = GEngine->GetEngineSubsystem<UCBAudio2FaceSubsystem>())
		{
			bSucceed=true;
			Subsystem->OnA2FStartUp.Broadcast(bSucceed);
			Subsystem->OnA2FStartUpNative.ExecuteIfBound(bSucceed);

		}
		TaskNotification->SetComplete(
			LOCTEXT("Aduio2Face_SuccessTitle", "Aduio2Face is running"),
		FText::FromString(TEXT("Aduio2Face is running")),
			true);
		Status = EProcessStatus::Stopped;
		
		 return 0;
	}
	
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

#if PLATFORM_WINDOWS
	FString StartScript = UCBAudio2FaceSettings::Get()->Audio2FaceExePath / TEXT("kit/kit.exe");
	Audio2faceConfigPaths=UCBAudio2FaceSettings::Get()->Audio2FaceExePath /TEXT("apps/audio2face_headless.kit");
#else
	FString StartScript = Root / TEXT("kit/kit.sh");
#endif
	
	FText ErrorTitle = LOCTEXT("Aduio2Face_ErrorTitle", "Failed to Launch the Aduio2Face");

	if (!PlatformFile.FileExists(*StartScript))
	{
		TaskNotification->SetComplete(
			ErrorTitle,
			LOCTEXT("Aduio2Face_FilesMissing", "Missing files in Aduio2Face folder"),
			false
		);
		Status = EProcessStatus::Error;
		return 0;
	}

	void* ReadPipe = nullptr;
	void* WritePipe = nullptr;

	if (!FPlatformProcess::CreatePipe(ReadPipe, WritePipe))
	{
		TaskNotification->SetComplete(
			ErrorTitle,
			LOCTEXT("Aduio2Face__PipeFailed", "Failed to create Pipes for the Aduio2Face process"),
			false
		);
		Status = EProcessStatus::Error;
		return 0;
	}
	
	Process = FPlatformProcess::CreateProc(
		*StartScript,	/* Path to start script */
		*Audio2faceConfigPaths,			/* Arguments with port numbers */
		false,			/* bLaunchDetached */
		true,			/* bLaunchHidden */
		true,			/* bLaunchReallyHidden */
		nullptr,		/* OutProcessID */
		1,				/* PriorityModifier */
		nullptr,
		WritePipe,		/* PipeWriteChild */
		ReadPipe		/* PipeReadChild */
	);
	
if (!Process.IsValid())
	{
		FPlatformProcess::ClosePipe(ReadPipe, WritePipe);
		TaskNotification->SetComplete(
			ErrorTitle,
			LOCTEXT("Aduio2Face__PipeFailed", "Failed to start Aduio2Face process"),
			false
		);

		Status = EProcessStatus::Error;
		return 0;
	}

	bool bLoadDone = false;
	auto LogReadPipe = [&](void* InReadPipe) {
		FString ProcessOutput = FPlatformProcess::ReadPipe(InReadPipe);

		if (ProcessOutput.Len() > 0)
		{
			TArray<FString> Lines;
			ProcessOutput.ParseIntoArray(Lines, TEXT("\n"), false);

			for (const FString& Line : Lines)
			{
				if (Line.Len() == 0)
				{
					continue;
				}

				UE_LOG(LogCBAudio2Face,Warning,TEXT("%s"),*Line);
				if (!bLoadDone)
				{
					if (Line.StartsWith(TEXT("[Error]")))
					{
						bLoadDone = true;
						const FString ErrorMessage = Line.Mid(7);
						TaskNotification->SetComplete(ErrorTitle, FText::FromString(ErrorMessage), false);
						Status = EProcessStatus::Error;
					}
					else
					{
						TaskNotification->SetProgressText(FText::FromString(Line));
					}
				}
				else
				{
					UE_LOG(LogCBAudio2Face, Log, TEXT("%s"), *Line);
				}
			}
		}

		return ProcessOutput.Len();
	};

	UE_LOG(LogCBAudio2Face, Log, TEXT("Aduio2Face started, initial launch will take longer as it will be building the Aduio2Face"));

	while (!bLoadDone && FPlatformProcess::IsProcRunning(Process))
	{
		const int32 BytesRead = LogReadPipe(ReadPipe);

		bLoadDone=IsA2FServerReady();

		if (bLoadDone)
		{
 			TaskNotification->SetComplete(
			LOCTEXT("Aduio2Face_SuccessTitle", "Aduio2Face is running"),
		 FText::FromString(TEXT("Aduio2Face is running")),
		 true);
 			Status = EProcessStatus::Running;
		}
		
		if (!BytesRead)
		{
			FPlatformProcess::Sleep(0.5);
		}
	}

	// One last ReadPipe log for anything that the while loop didn't catch.
	LogReadPipe(ReadPipe);

	if (!bLoadDone)
	{
		TaskNotification->SetComplete(
			ErrorTitle,
			LOCTEXT("Aduio2Face_AppExited", "Aduio2Face exited"),
			false
		);
	}
	else
	{
		UE_LOG(LogCBAudio2Face, Log, TEXT("Aduio2Face exited"));
	}

	Status = EProcessStatus::Stopped;

	FPlatformProcess::ClosePipe(ReadPipe, WritePipe);
	Process.Reset();

	return 0;
};

bool FCBAudio2FaceProcess::IsA2FServerReady()
{
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(UCBAudio2FaceSettings::Get()->ServerURL+TEXT("/status"));
	Request->SetHeader(TEXT("Content-Type"),TEXT("application/json"));
	Request->SetVerb(TEXT("GET"));

	Request->OnProcessRequestComplete().BindLambda([&](FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded)
	{
		if(HttpResponse)
		{
			FString Respone=HttpResponse->GetContentAsString();	
			if(Respone.Contains(TEXT("OK")))
			{
				bSucceed= true;
			}
			else
			{
				bSucceed=false;
			}
		}
		
	});
	
	Request->ProcessRequest();

	if(UCBAudio2FaceSubsystem* const Subsystem = GEngine->GetEngineSubsystem<UCBAudio2FaceSubsystem>())
	{
		AsyncTask(ENamedThreads::GameThread,
			[this, Subsystem]
			{
				Subsystem->OnA2FStartUp.Broadcast(bSucceed);
				Subsystem->OnA2FStartUpNative.ExecuteIfBound(bSucceed);
				if(bSucceed)
				{
					Subsystem->InitA2F();
				}
			}
		);
	}
	
	return bSucceed;
}

#undef LOCTEXT_NAMESPACE
