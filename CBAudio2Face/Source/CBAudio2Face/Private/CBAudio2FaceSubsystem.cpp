// Copyright 2025-Present Xiao Lan fei. All Rights Reserved.


#include "CBAudio2FaceSubsystem.h"

#include "CBAudio2Face.h"
#include "CBAudio2FaceNodes.h"
#include "CBAudio2FaceSettings.h"
#include "Kismet/KismetStringLibrary.h"
#include "Library/CBFileHeplerBPLibrary.h"

UCBAudio2FaceSubsystem::UCBAudio2FaceSubsystem()
{
	bA2FPlay=false;
	bA2FLoadedCompleted=false;
	bCalculating=false;
	PlayTime=0.0f;
}

void UCBAudio2FaceSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	UE_LOG(LogCBAudio2Face, Display, TEXT("%s: CBAudio2Face Engine Subsystem initialized."), *FString(__func__));
}

void UCBAudio2FaceSubsystem::Deinitialize()
{
	UE_LOG(LogCBAudio2Face, Display, TEXT("%s: CBAudio2Face Engine Subsystem deinitialized."), *FString(__func__));
	
	Super::Deinitialize();
}

void UCBAudio2FaceSubsystem::Tick(float DeltaTime)
{
	if(bA2FLoadedCompleted)
	{
		if(bA2FPlay)
		{
			if(!bCalculating)
			{
				
				USendA2FRequestPlayerRangeProxy* TimeProxy=USendA2FRequestPlayerRangeProxy::SendHttpA2FGetPlayerIRangeRequest("/World/audio2face/Player");
				TimeProxy->OnResponseC.BindLambda([this](FA2FPlayerRange PlayerRange)
				{
					bCalculating = true;	
					PlayTime = PlayerRange.DefaultMax;
					//UE_LOG(LogTemp,Warning,TEXT("%f"),PlayTime);
				});
			}
			else
			{
				if(CurrentPlayTime>=PlayTime)
				{
					OnA2FPlayCompleted.Broadcast();
					OnA2FPlayCompletedNative.ExecuteIfBound();
					bA2FPlay=false;
					bCalculating=false;
					CurrentPlayTime=0.0f;
					PlayTime=0.0f;
				}
				else
				{
					CurrentPlayTime+=DeltaTime;
				}
			}

			
		}
	}
}

bool UCBAudio2FaceSubsystem::IsTickable() const
{
	return true;
}

TStatId UCBAudio2FaceSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UTickableObject, STATGROUP_Tickables);
}

void UCBAudio2FaceSubsystem::InitA2F()
{

	if(UCBAudio2FaceSettings* Settings=UCBAudio2FaceSettings::Get())
	{
		FString USDFile=UCBFileHeplerBPLibrary::GetExtrasDir("CBAudio2Face","USD");
		USDFile=FPaths::Combine(USDFile,Settings->USDFile);
		if(!FPaths::FileExists(USDFile))
		{
			UE_LOG(LogCBAudio2Face, Warning, TEXT("%s: USD file does not exist,please check"), *FString(__func__));
			OnA2FLoadedCompleted.Broadcast(false);
			OnA2FLoadedCompletedNative.ExecuteIfBound(false);
			return;
		}
	
		USendA2FRequestProxy* USDProxy=USendA2FRequestProxy::SendHttp2FSetUSDRequest(USDFile);
		USDProxy->OnErrorC.BindLambda([this]
		{
			OnA2FLoadedCompleted.Broadcast(false);
			OnA2FLoadedCompletedNative.ExecuteIfBound(false);
		});
	
		USDProxy->OnResponseC.BindLambda([this]()
		{
			USendA2FRequestGetArrayProxy* LiveLinkNodeProxy=USendA2FRequestGetArrayProxy::SendHttpA2FGetStreamLivelinkNodesRequest();

			LiveLinkNodeProxy->OnErrorC.BindLambda([this](TArray<FString> ArrayNode) 
				{
					OnA2FLoadedCompleted.Broadcast(false);
					OnA2FLoadedCompletedNative.ExecuteIfBound(false);
				});
		
			LiveLinkNodeProxy->OnResponseC.BindLambda([this](TArray<FString> ArrayNode)
			{
				USendA2FRequestProxy* ActiveProxy=USendA2FRequestProxy::SendHttpA2FActivateStreamLivelinkRequest(ArrayNode[0],true);
				ActiveProxy->OnErrorC.BindLambda([this]
				{
					OnA2FLoadedCompleted.Broadcast(false);
					OnA2FLoadedCompletedNative.ExecuteIfBound(false);
				});
			
				ActiveProxy->OnResponseC.BindLambda([this]
				{
					OnA2FLoadedCompleted.Broadcast(true);
					OnA2FLoadedCompletedNative.ExecuteIfBound(true);
					bA2FLoadedCompleted=true;
				
					USendA2FRequestProxy *Ptah=USendA2FRequestProxy::SendHttpA2FSetPlayerRootPathRequest("/World/audio2face/Player",GetWavPath());

					Ptah->OnResponseC.BindLambda([this]
					{
						FA2FPlayerTrack PlayerTrack;
						PlayerTrack.FileName=TEXT("default.wav");
						USendA2FRequestProxy* PlayTrack= USendA2FRequestProxy::SendHttpA2FSetPlayerTrackRequest("/World/audio2face/Player",PlayerTrack);

						PlayTrack->OnErrorC.BindLambda([this]
						{
							UE_LOG(LogCBAudio2Face, Warning, TEXT("%s: Get Play Track Error."), *FString(__func__));
						});
					
				});
			});
		});
	});
	}
	
}

void UCBAudio2FaceSubsystem::PlayA2F(FString InPath,const FA2FPlayerTrack PlayerTrack)
{
	FString TrackPath;
	if(!InPath.IsEmpty())
	{
		TrackPath=FPaths::Combine(GetWavPath(),InPath);
	}
	else
	{
		TrackPath=GetWavPath();
	}
	
	USendA2FRequestProxy *Ptah=USendA2FRequestProxy::SendHttpA2FSetPlayerRootPathRequest("/World/audio2face/Player",TrackPath);

	Ptah->OnResponseC.BindLambda([this,PlayerTrack]
	{
		USendA2FRequestProxy* PlayTrack= USendA2FRequestProxy::SendHttpA2FSetPlayerTrackRequest("/World/audio2face/Player",PlayerTrack);

		PlayTrack->OnErrorC.BindLambda([this]
		{
			UE_LOG(LogCBAudio2Face, Warning, TEXT("%s: Get Play Track Error."), *FString(__func__));

			OnA2FPlayError.Broadcast();
			OnA2FPlayErrorNative.ExecuteIfBound();
			return;
		});

		PlayTrack->OnResponseC.BindLambda([this]
		{
			USendA2FPlayerProxy* Player=USendA2FPlayerProxy::SendHttpA2FPlayerPlayRequest("/World/audio2face/Player");

			Player->OnErrorC.BindLambda([this]
		{
			UE_LOG(LogCBAudio2Face, Warning, TEXT("%s: Get Play player Error."), *FString(__func__));
			OnA2FPlayError.Broadcast();
			OnA2FPlayErrorNative.ExecuteIfBound();
			return;
		});
		
		});
	});

	Ptah->OnErrorC.BindLambda([this]
	{
		UE_LOG(LogCBAudio2Face, Warning, TEXT("%s: Set Play Track Error."), *FString(__func__));
		OnA2FPlayError.Broadcast();
		OnA2FPlayErrorNative.ExecuteIfBound();
	});
}

void UCBAudio2FaceSubsystem::StopPlayA2F()
{

	USendA2FRequestProxy *Ptah=USendA2FRequestProxy::SendHttpA2FSetPlayerRootPathRequest("/World/audio2face/Player",GetWavPath());

	Ptah->OnResponseC.BindLambda([this]
	{
		FA2FPlayerTrack PlayerTrack;
		PlayerTrack.FileName=TEXT("default.wav");
		USendA2FRequestProxy* PlayTrack= USendA2FRequestProxy::SendHttpA2FSetPlayerTrackRequest("/World/audio2face/Player",PlayerTrack);

		PlayTrack->OnErrorC.BindLambda([this]
		{
			UE_LOG(LogCBAudio2Face, Warning, TEXT("%s: Get Play Track Error."), *FString(__func__));
		});

		OnA2FPlayStop.Broadcast();
		OnA2FPlayStopNative.ExecuteIfBound();
		bA2FPlay=false;
		bCalculating=false;
		CurrentPlayTime=0.0f;
		PlayTime=0.0f;
	});
	
}

FString UCBAudio2FaceSubsystem::GetWavPath()
{
	UCBAudio2FaceSettings* Settings = UCBAudio2FaceSettings::Get();

	if(!Settings)
	{
		return FString();
	}
	FString Path=Settings->Audio2FaceRootPath;
	const FString PathPrefix="LocalA2FWav://";
	if((UKismetStringLibrary::StartsWith(Path,PathPrefix,ESearchCase::IgnoreCase)))
	{
		FString File=UKismetStringLibrary::RightChop(Path,PathPrefix.Len());
		Path=FPaths::Combine(UCBFileHeplerBPLibrary::GetExtrasDir(TEXT("CBAudio2Face"),TEXT("WAV"))/File);
		FPaths::NormalizeFilename(Path);
	}
	return Path;
}
