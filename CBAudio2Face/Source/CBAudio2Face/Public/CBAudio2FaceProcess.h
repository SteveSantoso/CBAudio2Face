// Copyright 2025-Present Xiao Lan fei. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Misc/AsyncTaskNotification.h"

class FCBAudio2FaceProcess : private FRunnable
{
public:
	
	enum class EProcessStatus : uint8
	{
		Stopped,
		Launching,
		Running,
		Error
	};

public:
	FCBAudio2FaceProcess();

	virtual ~FCBAudio2FaceProcess();

	void Start();

	void Shutdown();

	EProcessStatus GetStatus() const;

protected:
	virtual uint32 Run() override;

	bool IsA2FServerReady();
private:

	/**audio2face 进程的句柄*/
	FProcHandle Process;

	/**audio2face 进程进程*/
	FRunnableThread* Thread = nullptr;

	TUniquePtr<FAsyncTaskNotification> TaskNotification;

	std::atomic<EProcessStatus> Status;

	/**配置项路径*/
	FString Audio2faceConfigPaths;

	bool bSucceed=false;
};
