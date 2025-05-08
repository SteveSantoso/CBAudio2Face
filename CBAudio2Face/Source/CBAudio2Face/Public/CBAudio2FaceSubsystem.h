// Copyright 2025-Present Xiao Lan fei. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/EngineSubsystem.h"
#include "CBAudio2FaceSubsystem.generated.h"

/**
 * 
 */

struct FA2FPlayerTrack;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnA2FStartUpEvent, bool, bSucceed);
DECLARE_DELEGATE_OneParam(FOnA2FStartUpEventNative,bool);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnA2FLoadedCompletedEvent,bool,bSucceed);
DECLARE_DELEGATE_OneParam(FOnA2FLoadedCompletedEventNative,bool);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnA2FPlayEvent);
DECLARE_DELEGATE(FOnA2FPlayEventNative);
UCLASS()
class CBAUDIO2FACE_API UCBAudio2FaceSubsystem : public UEngineSubsystem,public FTickableGameObject
{
	GENERATED_BODY()

public:

	UCBAudio2FaceSubsystem();
	// Begin USubsystem
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// End USubsystem

	// Begin TickableGameObject
	virtual void Tick(float DeltaTime)override ;
	virtual bool IsTickable()const override ;
	virtual TStatId GetStatId() const override;
	// End TickableGameObject
public:
	/**初始化a2f
	 * 
	 */
	UFUNCTION(BlueprintCallable,Category="AIGC|Audio2Face",DisplayName="Init A2F")
	void InitA2F();

	/**播放a2f*/
	UFUNCTION(BlueprintCallable,Category="AIGC|Audio2Face",DisplayName="Play A2F")
	void PlayA2F(FString Path,const FA2FPlayerTrack PlayerTrack);

	/**停止播放a2f*/
	UFUNCTION(BlueprintCallable,Category="AIGC|Audio2Face",DisplayName="Stop Play A2F")
	void StopPlayA2F();

	/**获取放置wav路径*/
	UFUNCTION( BlueprintPure,BlueprintCallable,Category="AIGC|Audio2Face",DisplayName="Get Wav Path")
	FString GetWavPath();
public:

	/**a2f 开始初始化*/
	UPROPERTY(BlueprintAssignable,Category="AIGC|Audio2Face")
	FOnA2FStartUpEvent OnA2FStartUp;

	/**a2f 加载完成*/
	UPROPERTY(BlueprintAssignable,Category="AIGC|Audio2Face")
	FOnA2FLoadedCompletedEvent OnA2FLoadedCompleted;

	/**a2f 播放完成回调*/
	UPROPERTY(BlueprintAssignable,Category="AIGC|Audio2Face")
	FOnA2FPlayEvent OnA2FPlayCompleted;

	/**a2f 手动结束回调*/
	UPROPERTY(BlueprintAssignable,Category="AIGC|Audio2Face")
	FOnA2FPlayEvent OnA2FPlayStop;
	
	/**a2f 播放错误回调*/
	UPROPERTY(BlueprintAssignable,Category="AIGC|Audio2Face")
	FOnA2FPlayEvent OnA2FPlayError;
	
	FOnA2FStartUpEventNative OnA2FStartUpNative;
	FOnA2FLoadedCompletedEventNative OnA2FLoadedCompletedNative;
	FOnA2FPlayEventNative OnA2FPlayCompletedNative;
	FOnA2FPlayEventNative OnA2FPlayStopNative;
	FOnA2FPlayEventNative OnA2FPlayErrorNative;

public:
	bool bA2FPlay;
private:
	bool bA2FLoadedCompleted;
	bool bCalculating;

	float PlayTime;
	float CurrentPlayTime;
};


