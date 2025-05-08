// Copyright 2025-Present Xiao Lan fei. All Rights Reserved.


#include "CBAudio2FaceNodes.h"

#include "CBAudio2Face.h"
#include "CBAudio2FaceSettings.h"
#include "CBAudio2FaceSubsystem.h"
#include "CBAudio2FaceTypes.h"
#include "CBHTTPRequset.h"
#include "CBHTTPResponse.h"
#include "CBHTTPTypes.h"

void USendA2FRequestProxyBase::OnErrorInternal(UCBHTTPResponse* const Response)
{
	UE_LOG(LogCBAudio2Face,Error,TEXT("%s"),*Response->GetContentAsString())
	OnError.Broadcast();
	OnErrorC.ExecuteIfBound();
}

void USendA2FRequestProxyBase::OnSuccessInternal(UCBHTTPResponse* const Response)
{
	FString ContentString = Response->GetContentAsString();
	
	if (ContentString.Contains(TEXT("OK"), ESearchCase::IgnoreCase) && Response->GetResponseCode()==200)
	{
		OnResponse.Broadcast();
		OnResponseC.ExecuteIfBound();
		
	}
	else
	{
		OnError.Broadcast();
		OnErrorC.ExecuteIfBound();
		UE_LOG(LogCBAudio2Face,Error,TEXT("%s"),*ContentString)
	}
	
}


USendA2FRequestProxy* USendA2FRequestProxy::SendHttp2FStatusRequest()
{
	USendA2FRequestProxy* const Proxy = NewObject<USendA2FRequestProxy>();

	UCBHTTPRequset* const Request = Proxy->GetRequest();

	if(UCBAudio2FaceSettings* Settings=UCBAudio2FaceSettings::Get())
	{
		Request->SetURL(Settings->ServerURL+TEXT("/status"));
		Request->SetMimeType(EHttpMimeType::json);
		Request->SetVerb(EHttpVerb::GET);
		Proxy->SendRequest();
	}
	return Proxy;
}


USendA2FRequestProxy* USendA2FRequestProxy::SendHttp2FSetUSDRequest(const FString& USDFilePath)
{
	USendA2FRequestProxy* const Proxy = NewObject<USendA2FRequestProxy>();

	UCBHTTPRequset* const Request = Proxy->GetRequest();

	if(UCBAudio2FaceSettings* Settings=UCBAudio2FaceSettings::Get())
	{
		Request->SetURL(Settings->ServerURL+TEXT("/A2F/USD/Load"));
		Request->SetMimeType(EHttpMimeType::json);
		Request->SetVerb(EHttpVerb::POST);
		FString USDPath = USDFilePath;
		FPaths::NormalizeFilename(USDPath);
		FString RequestString=FString::Printf(TEXT("{\"file_name\":\"%s\"}"),*USDPath);
		Request->SetContentAsString(RequestString);
		Proxy->SendRequest();
	}
	return Proxy;
}

USendA2FRequestProxy* USendA2FRequestProxy::SendHttp2FSetSettingsRequest(const FString& Instance,
	const FAudio2FaceSettings& InSettings)
{
	USendA2FRequestProxy* const Proxy = NewObject<USendA2FRequestProxy>();

	UCBHTTPRequset* const Request = Proxy->GetRequest();

	if(UCBAudio2FaceSettings* Settings=UCBAudio2FaceSettings::Get())
	{
		Request->SetURL(Settings->ServerURL+TEXT("/A2F/SetSettings"));
		Request->SetMimeType(EHttpMimeType::json);
		Request->SetVerb(EHttpVerb::POST);
		FString RequestString;
		Proxy->Audio2FaceSettingsToJsonString(Instance,InSettings,RequestString);
		Request->SetContentAsString(RequestString);
		Proxy->SendRequest();
	}
	return Proxy;
}

USendA2FRequestProxy* USendA2FRequestProxy::SendHttp2FSetSettingsRequestByKeys(const FString& Instance,
	const TMap<FString, float>& InSettings)
{
	USendA2FRequestProxy* const Proxy = NewObject<USendA2FRequestProxy>();

	UCBHTTPRequset* const Request = Proxy->GetRequest();

	if(UCBAudio2FaceSettings* Settings=UCBAudio2FaceSettings::Get())
	{
		Request->SetURL(Settings->ServerURL+TEXT("/A2F/SetSettings"));
		Request->SetMimeType(EHttpMimeType::json);
		Request->SetVerb(EHttpVerb::POST);
		FString RequestString;
		Proxy->Audio2FaceSettingsToJsonString(Instance,InSettings,RequestString);
		Request->SetContentAsString(RequestString);
		Proxy->SendRequest();
	}
	return Proxy;
}

USendA2FRequestProxy* USendA2FRequestProxy::SendHttpA2FSetFrameRequest(const FString& Instance, int32 InFrame /**=25*/,
                                                                       bool bAsTimestamp /**=false*/)
{
	USendA2FRequestProxy* const Proxy = NewObject<USendA2FRequestProxy>();

	UCBHTTPRequset* const Request = Proxy->GetRequest();

	if(UCBAudio2FaceSettings* Settings=UCBAudio2FaceSettings::Get())
	{
		Request->SetURL(Settings->ServerURL+TEXT("/A2F/SetFrame"));
		Request->SetMimeType(EHttpMimeType::json);
		Request->SetVerb(EHttpVerb::POST);

		FString RequestString = FString::Printf(TEXT("{\"a2f_instance\":\"%s\",\"frame\":\"%i\",\"as_timestamp\":\"%s\"}"),
			*Instance, 
			InFrame,
			(bAsTimestamp ? TEXT("true") : TEXT("false")));
		
		Request->SetContentAsString(RequestString);
		
		Proxy->SendRequest();
	}
	return Proxy;
}

USendA2FRequestProxy* USendA2FRequestProxy::SendHttpA2FSetEmotionWeightsByFrameRequest(const FString& Instance,
	const FA2FEmotion& InEmotion)
{
	USendA2FRequestProxy* const Proxy = NewObject<USendA2FRequestProxy>();

	UCBHTTPRequset* const Request = Proxy->GetRequest();

	if(UCBAudio2FaceSettings* Settings=UCBAudio2FaceSettings::Get())
	{
		Request->SetURL(Settings->ServerURL+TEXT("/A2F/A2E/SetEmotion"));
		Request->SetMimeType(EHttpMimeType::json);
		Request->SetVerb(EHttpVerb::POST);
		FString RequestString;
		Proxy->Audio2FaceSetEmotionToJsonString(Instance,InEmotion,RequestString);
		
		Request->SetContentAsString(RequestString);
		Proxy->SendRequest();
	}
	return Proxy;
}

USendA2FRequestProxy* USendA2FRequestProxy::SendHttpA2FSetEmotionWeightsByNameRequest(const FString& Instance,
	const TMap<FString, float>& InEmotion)
{
	USendA2FRequestProxy* const Proxy = NewObject<USendA2FRequestProxy>();

	UCBHTTPRequset* const Request = Proxy->GetRequest();

	if(UCBAudio2FaceSettings* Settings=UCBAudio2FaceSettings::Get())
	{
		Request->SetURL(Settings->ServerURL+TEXT("/A2F/A2E/SetEmotionByName"));
		Request->SetMimeType(EHttpMimeType::json);
		Request->SetVerb(EHttpVerb::POST);
		FString RequestString;
		Proxy->Audio2FaceSetEmotionNameToJsonString(Instance,InEmotion,RequestString);
		
		Request->SetContentAsString(RequestString);
		Proxy->SendRequest();
	}
	return Proxy;
}

USendA2FRequestProxy* USendA2FRequestProxy::SendHttpA2FSetEnableStreamingRequest(const FString& Instance, bool bEnable)
{
	USendA2FRequestProxy* const Proxy = NewObject<USendA2FRequestProxy>();

	UCBHTTPRequset* const Request = Proxy->GetRequest();

	if(UCBAudio2FaceSettings* Settings=UCBAudio2FaceSettings::Get())
	{
		Request->SetURL(Settings->ServerURL+TEXT("/A2F/A2E/EnableStreaming"));
		Request->SetMimeType(EHttpMimeType::json);
		Request->SetVerb(EHttpVerb::POST);
		FString RequestString = FString::Printf(TEXT("{\"a2f_instance\":\"%s\",\"enable\":\"%s\"}"), *Instance,(bEnable ? TEXT("true") : TEXT("false")));
		Request->SetContentAsString(RequestString);
		
		Proxy->SendRequest();
	}
	return Proxy;
}

USendA2FRequestProxy* USendA2FRequestProxy::SendHttpA2FSetEmotionSettingsRequest(const FString& Instance,
	const FAudio2EmotionSettings& InSettings)
{
	USendA2FRequestProxy* const Proxy = NewObject<USendA2FRequestProxy>();

	UCBHTTPRequset* const Request = Proxy->GetRequest();

	if(UCBAudio2FaceSettings* Settings=UCBAudio2FaceSettings::Get())
	{
		Request->SetURL(Settings->ServerURL+TEXT("/A2F/A2E/SetSettings"));
		Request->SetMimeType(EHttpMimeType::json);
		Request->SetVerb(EHttpVerb::POST);
		
		FString RequestString;
		Proxy->Audio2FaceSetEmotionSettingToJsonString(Instance,InSettings,RequestString);
		Request->SetContentAsString(RequestString);
		
		Proxy->SendRequest();
	}
	return Proxy;
}

USendA2FRequestProxy* USendA2FRequestProxy::SendHttpA2FSetPlayerRootPathRequest(const FString& PlayerInstance,
	const FString& DirPath)
{
	USendA2FRequestProxy* const Proxy = NewObject<USendA2FRequestProxy>();

	FString Path=DirPath;
	FPaths::NormalizeFilename(Path);
	UCBHTTPRequset* const Request = Proxy->GetRequest();
	if(UCBAudio2FaceSettings* Settings=UCBAudio2FaceSettings::Get())
	{
		Request->SetURL(Settings->ServerURL+TEXT("/A2F/Player/SetRootPath"));
		Request->SetMimeType(EHttpMimeType::json);
		Request->SetVerb(EHttpVerb::POST);
		
		FString RequestString = FString::Printf(TEXT(
		"{\
		\"a2f_player\":\"%s\",\
		\"dir_path\": \"%s\"\
		}"),
		*PlayerInstance,
		*Path);
		Request->SetContentAsString(RequestString);
		
		Proxy->SendRequest();
	}
	return Proxy;
}

USendA2FRequestProxy* USendA2FRequestProxy::SendHttpA2FSetPlayerTrackRequest(const FString& PlayerInstance,
	const FA2FPlayerTrack& PlayerTrack)
{
	USendA2FRequestProxy* const Proxy = NewObject<USendA2FRequestProxy>();

	UCBHTTPRequset* const Request = Proxy->GetRequest();

	if(UCBAudio2FaceSettings* Settings=UCBAudio2FaceSettings::Get())
	{
		Request->SetURL(Settings->ServerURL+TEXT("/A2F/Player/SetTrack"));
		Request->SetMimeType(EHttpMimeType::json);
		Request->SetVerb(EHttpVerb::POST);
		
		FString RequestString;
		Proxy->Audio2FaceSetPlayerTrackToJsonString(PlayerInstance,PlayerTrack,RequestString);
		Request->SetContentAsString(RequestString);
		Proxy->SendRequest();
	}
	return Proxy;
}

USendA2FRequestProxy* USendA2FRequestProxy::SendHttpA2FSetPlayerRangeRequest(const FString& PlayerInstance, float Start,
	float End /**=-1.0f*/)
{
	USendA2FRequestProxy* const Proxy = NewObject<USendA2FRequestProxy>();

	UCBHTTPRequset* const Request = Proxy->GetRequest();

	if(UCBAudio2FaceSettings* Settings=UCBAudio2FaceSettings::Get())
	{
		Request->SetURL(Settings->ServerURL+TEXT("/A2F/Player/SetRange"));
		Request->SetMimeType(EHttpMimeType::json);
		Request->SetVerb(EHttpVerb::POST);
		
		FString RequestString=FString::Printf(TEXT(
		"{\
			\"a2f_player\":\"%s\",\
			\"start\":\"%f\",\
			\"end\":\"%f\"\
		}"),
		*PlayerInstance,
		Start, 
		End);

		Request->SetContentAsString(RequestString);
		Proxy->SendRequest();
	}
	return Proxy;
}

USendA2FRequestProxy* USendA2FRequestProxy::SendHttpA2FSetPlayerCurrentTimeRequest(const FString& PlayerInstance,
	float Time)
{
	USendA2FRequestProxy* const Proxy = NewObject<USendA2FRequestProxy>();

	UCBHTTPRequset* const Request = Proxy->GetRequest();

	if(UCBAudio2FaceSettings* Settings=UCBAudio2FaceSettings::Get())
	{
		Request->SetURL(Settings->ServerURL+TEXT("/A2F/Player/SetTime"));
		Request->SetMimeType(EHttpMimeType::json);
		Request->SetVerb(EHttpVerb::POST);
		
		FString RequestString=FString::Printf(TEXT(
		"{\
			\"a2f_player\":\"%s\",\
			\"time\":\"%f\"\
		}"),
		*PlayerInstance,
		Time);

		Request->SetContentAsString(RequestString);
		Proxy->SendRequest();
	}
	return Proxy;
}

void USendA2FPlayerProxy::OnErrorInternal(UCBHTTPResponse* const Response)
{
	UE_LOG(LogCBAudio2Face,Error,TEXT("%s"),*Response->GetContentAsString())
	OnError.Broadcast();
	OnErrorC.ExecuteIfBound();
}

void USendA2FPlayerProxy::OnSuccessInternal(UCBHTTPResponse* const Response)
{
	FString ContentString = Response->GetContentAsString();
	
	if (ContentString.Contains(TEXT("OK"), ESearchCase::IgnoreCase) && Response->GetResponseCode()==200)
	{
		OnResponse.Broadcast();
		OnResponseC.ExecuteIfBound();

		if(UCBAudio2FaceSubsystem* const Subsystem = GEngine->GetEngineSubsystem<UCBAudio2FaceSubsystem>())
		{
			Subsystem->bA2FPlay=true;
		}
	}
	else
	{
		OnError.Broadcast();
		OnErrorC.ExecuteIfBound();
		UE_LOG(LogCBAudio2Face,Error,TEXT("%s"),*ContentString)
	}
}

USendA2FPlayerProxy* USendA2FPlayerProxy::SendHttpA2FPlayerPlayRequest(const FString& PlayerInstance)
{
	USendA2FPlayerProxy* const Proxy = NewObject<USendA2FPlayerProxy>();

	UCBHTTPRequset* const Request = Proxy->GetRequest();

	if(UCBAudio2FaceSettings* Settings=UCBAudio2FaceSettings::Get())
	{
		Request->SetURL(Settings->ServerURL+TEXT("/A2F/Player/Play"));
		Request->SetMimeType(EHttpMimeType::json);
		Request->SetVerb(EHttpVerb::POST);
		
		FString RequestString=FString::Printf(TEXT(
		"{\
			\"a2f_player\":\"%s\"\
		}"),
		*PlayerInstance);

		Request->SetContentAsString(RequestString);
		Proxy->SendRequest();
		
	}
	return Proxy;
}

USendA2FRequestProxy* USendA2FRequestProxy::SendHttpA2FPlayerPauseRequest(const FString& PlayerInstance)
{
	USendA2FRequestProxy* const Proxy = NewObject<USendA2FRequestProxy>();

	UCBHTTPRequset* const Request = Proxy->GetRequest();

	if(UCBAudio2FaceSettings* Settings=UCBAudio2FaceSettings::Get())
	{
		Request->SetURL(Settings->ServerURL+TEXT("/A2F/Player/Pause"));
		Request->SetMimeType(EHttpMimeType::json);
		Request->SetVerb(EHttpVerb::POST);
		
		FString RequestString=FString::Printf(TEXT(
		"{\
			\"a2f_player\":\"%s\"\
		}"),
		*PlayerInstance);

		Request->SetContentAsString(RequestString);
		Proxy->SendRequest();
	}
	return Proxy;	
}

USendA2FRequestProxy* USendA2FRequestProxy::SendHttpA2FPlayerRewindRequest(const FString& PlayerInstance)
{
	USendA2FRequestProxy* const Proxy = NewObject<USendA2FRequestProxy>();

	UCBHTTPRequset* const Request = Proxy->GetRequest();

	if(UCBAudio2FaceSettings* Settings=UCBAudio2FaceSettings::Get())
	{
		Request->SetURL(Settings->ServerURL+TEXT("/A2F/Player/Rewind"));
		Request->SetMimeType(EHttpMimeType::json);
		Request->SetVerb(EHttpVerb::POST);
		
		FString RequestString=FString::Printf(TEXT(
		"{\
			\"a2f_player\":\"%s\"\
		}"),
		*PlayerInstance);

		Request->SetContentAsString(RequestString);
		Proxy->SendRequest();
	}
	return Proxy;	
}

USendA2FRequestProxy* USendA2FRequestProxy::SendHttpA2FSetPlayerLoopingRequest(const FString& PlayerInstance,
	bool bLoop)
{
	USendA2FRequestProxy* const Proxy = NewObject<USendA2FRequestProxy>();

	UCBHTTPRequset* const Request = Proxy->GetRequest();

	if(UCBAudio2FaceSettings* Settings=UCBAudio2FaceSettings::Get())
	{
		Request->SetURL(Settings->ServerURL+TEXT("/A2F/Player/SetLooping"));
		Request->SetMimeType(EHttpMimeType::json);
		Request->SetVerb(EHttpVerb::POST);
		
		FString RequestString= FString::Printf(TEXT(
		"{\
			\"a2f_player\":\"%s\",\
			\"loop_audio\":\"%s\"\
		}"),
		*PlayerInstance,
		bLoop ? TEXT("true") : TEXT("false"));

		Request->SetContentAsString(RequestString);
		Proxy->SendRequest();
	}
	return Proxy;
}

USendA2FRequestProxy* USendA2FRequestProxy::SendHttpA2FStreamLivelinkSettingsRequest(const FString& NodePath,
	const FStreamLivelinkSettings& LivelinkSettings)
{
	USendA2FRequestProxy* const Proxy = NewObject<USendA2FRequestProxy>();

	UCBHTTPRequset* const Request = Proxy->GetRequest();

	if(UCBAudio2FaceSettings* Settings=UCBAudio2FaceSettings::Get())
	{
		Request->SetURL(Settings->ServerURL+TEXT("/A2F/Exporter/SetStreamLivelinkSettings"));
		Request->SetMimeType(EHttpMimeType::json);
		Request->SetVerb(EHttpVerb::POST);
		
		FString RequestString;
		Proxy->Audio2FaceStreamLivelinkSettingsToJsonString(NodePath,LivelinkSettings,RequestString);

		Request->SetContentAsString(RequestString);
		Proxy->SendRequest();
	}
	return Proxy;	
}

USendA2FRequestProxy* USendA2FRequestProxy::SendHttpA2FExportBlendshapesRequest(const FString& SolverNode,
	const FA2FBlendshapeParam& Blendshape)
{
	USendA2FRequestProxy* const Proxy = NewObject<USendA2FRequestProxy>();

	UCBHTTPRequset* const Request = Proxy->GetRequest();

	if(UCBAudio2FaceSettings* Settings=UCBAudio2FaceSettings::Get())
	{
		Request->SetURL(Settings->ServerURL+TEXT("/A2F/Exporter/ExportBlendshapes"));
		Request->SetMimeType(EHttpMimeType::json);
		Request->SetVerb(EHttpVerb::POST);
		
		FString RequestString;
		Proxy->Audio2FaceExportBlendshapesToJsonString(SolverNode,Blendshape,RequestString);
		
		Request->SetContentAsString(RequestString);
		Proxy->SendRequest();
	}
	return Proxy;	
}

USendA2FRequestProxy* USendA2FRequestProxy::SendHttpA2FEnableAutoGenerateOnTrackChangeRequest(const FString& Instance,
	bool bEnable)
{
	USendA2FRequestProxy* const Proxy = NewObject<USendA2FRequestProxy>();

	UCBHTTPRequset* const Request = Proxy->GetRequest();

	if(UCBAudio2FaceSettings* Settings=UCBAudio2FaceSettings::Get())
	{
		Request->SetURL(Settings->ServerURL+TEXT("/A2F/A2E/EnableAutoGenerateOnTrackChange"));
		Request->SetMimeType(EHttpMimeType::json);
		Request->SetVerb(EHttpVerb::POST);
		FString RequestString = FString::Printf(TEXT("{\"a2f_instance\":\"%s\",\"enable\":\"%s\"}"), *Instance,(bEnable ? TEXT("true") : TEXT("false")));
		Request->SetContentAsString(RequestString);
		
		Proxy->SendRequest();
	}
	return Proxy;
}

USendA2FRequestProxy* USendA2FRequestProxy::SendHttpA2FActivateStreamLivelinkRequest(const FString& NodePath,
                                                                                     bool bActive)
{
	USendA2FRequestProxy* const Proxy = NewObject<USendA2FRequestProxy>();

	UCBHTTPRequset* const Request = Proxy->GetRequest();

	if(UCBAudio2FaceSettings* Settings=UCBAudio2FaceSettings::Get())
	{
		Request->SetURL(Settings->ServerURL+TEXT("/A2F/Exporter/ActivateStreamLivelink"));
		Request->SetMimeType(EHttpMimeType::json);
		Request->SetVerb(EHttpVerb::POST);
		
		FString RequestString= FString::Printf(TEXT(
		"{\
			\"node_path\":\"%s\",\
			\"value\":\"%s\"\
		}"),
		*NodePath,
		bActive ? TEXT("true") : TEXT("false"));

		Request->SetContentAsString(RequestString);
		Proxy->SendRequest();
	}
	return Proxy;	
}

bool USendA2FRequestProxy::Audio2FaceSettingsToJsonString(const FString& Instance,
                                                          const FAudio2FaceSettings& InSettings, FString& OutJson)
{
	TSharedPtr<FJsonObject> HeardJsonObject = MakeShareable(new FJsonObject);
        HeardJsonObject->SetStringField(TEXT("a2f_instance"), Instance);

        TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
        {
            double Offset = 0.00001; //不能直接传递int 会报错

            JsonObject->SetNumberField(TEXT("keyframer_fps"), InSettings.KeyframerFps + Offset);
            JsonObject->SetNumberField(TEXT("lower_face_smoothing"), InSettings.LowerFaceSmoothing + Offset);
            JsonObject->SetNumberField(TEXT("upper_face_smoothing"), InSettings.UpperFaceSmoothing + Offset);
            JsonObject->SetNumberField(TEXT("lower_face_strength"), InSettings.LowerFaceStrength + Offset);
            JsonObject->SetNumberField(TEXT("upper_face_strength"), InSettings.UpperFaceStrength + Offset);
            JsonObject->SetNumberField(TEXT("blink_strength"), InSettings.BlinkStrength + Offset);
            JsonObject->SetNumberField(TEXT("face_mask_level"), InSettings.FaceMaskLevel + Offset);
            JsonObject->SetNumberField(TEXT("face_mask_softness"), InSettings.FaceMaskSoftness + Offset);
            JsonObject->SetNumberField(TEXT("input_strength"), InSettings.InputStrength + Offset);
            JsonObject->SetNumberField(TEXT("prediction_delay"), InSettings.PredictionDelay + Offset);
            JsonObject->SetNumberField(TEXT("skin_strength"), InSettings.SkinStrength + Offset);
            JsonObject->SetNumberField(TEXT("lower_teeth_strength"), InSettings.LowerTeethStrength + Offset);
            JsonObject->SetNumberField(TEXT("lower_teeth_height_offset"), InSettings.LowerTeethHeightOffset + Offset);
            JsonObject->SetNumberField(TEXT("lower_teeth_depth_offset"), InSettings.LowerTeethDepthOffset + Offset);
            JsonObject->SetNumberField(TEXT("lip_open_offset"), InSettings.LipOpenOffset + Offset);
            JsonObject->SetNumberField(TEXT("tongue_strength"), InSettings.TongueStrength + Offset);
            JsonObject->SetNumberField(TEXT("tongue_height_offset"), InSettings.TongueHeightOffset + Offset);
            JsonObject->SetNumberField(TEXT("tongue_depth_offset"), InSettings.TongueDepthOffset + Offset);
            JsonObject->SetNumberField(TEXT("eyeballs_strength"), InSettings.EyeballsStrength + Offset);
            JsonObject->SetNumberField(TEXT("saccade_strength"), InSettings.SaccadeStrength + Offset);
            JsonObject->SetNumberField(TEXT("right_eye_rot_x_offset"), InSettings.RightEyeRotXOffset + Offset);
            JsonObject->SetNumberField(TEXT("right_eye_rot_y_offset"), InSettings.RightEyeRotYOffset + Offset);
            JsonObject->SetNumberField(TEXT("left_eye_rot_x_offset"), InSettings.LeftEyeRotXOffset + Offset);
            JsonObject->SetNumberField(TEXT("left_eye_rot_y_offset"), InSettings.LeftEyeRotYOffset + Offset);
            JsonObject->SetNumberField(TEXT("eyelid_open_offset"), InSettings.EyelidOpenOffset + Offset);
            JsonObject->SetNumberField(TEXT("blink_interval"), InSettings.BlinkInterval + Offset);
            JsonObject->SetNumberField(TEXT("a2e_window_size"), InSettings.A2EWindowSize + Offset);
            JsonObject->SetNumberField(TEXT("a2e_stride"), InSettings.A2EStride + Offset);
            JsonObject->SetNumberField(TEXT("a2e_emotion_strength"), InSettings.A2EEmotionStrength + Offset);
            JsonObject->SetNumberField(TEXT("a2e_smoothing_exp"), InSettings.A2ESmoothingExp + Offset);
            JsonObject->SetNumberField(TEXT("a2e_contrast"), InSettings.A2EContrast + Offset);
            JsonObject->SetNumberField(TEXT("a2e_streaming_smoothing"), InSettings.A2EStreamingSmoothing + Offset);
            JsonObject->SetNumberField(TEXT("a2e_streaming_transition_time"), InSettings.A2EStreamingTransitionTime + Offset);
            JsonObject->SetNumberField(TEXT("a2e_preferred_emotion_strength"), InSettings.A2EPreferredEmotionStrength + Offset);
           
            JsonObject->SetNumberField(TEXT("source_frame"), InSettings.SourceFrame);

            //int32
            JsonObject->SetNumberField(TEXT("eye_saccade_seed"), InSettings.EyeSaccadeSeed);
            JsonObject->SetNumberField(TEXT("a2e_smoothing_kernel_radius"), InSettings.A2ESmoothingKernelRadius);
            JsonObject->SetNumberField(TEXT("a2e_max_emotions"), InSettings.A2EMaxEmotions);
            JsonObject->SetNumberField(TEXT("a2e_streaming_update_period"), InSettings.A2EStreamingUpdatePeriod);

            //传递服务器会报错 先注销
          /**  { 
                JsonObject->SetStringField(TEXT("device_id"), InSettings.DeviceId);
                JsonObject->SetStringField(TEXT("a2e_network_path"), InSettings.A2ENetworkPath);
                JsonObject->SetStringField(TEXT("a2e_network_name"), InSettings.A2ENetworkName);
                JsonObject->SetStringField(TEXT("network_root_path"), InSettings.NetworkRootPath);
                JsonObject->SetStringField(TEXT("network_name"), InSettings.NetworkName);

                JsonObject->SetBoolField(TEXT("a2e_preferred_emotion_is_set"), InSettings.bA2EPreferredEmotionIsSet);
                JsonObject->SetBoolField(TEXT("a2e_auto_generate"), InSettings.bA2EAutoGenerate);
                JsonObject->SetBoolField(TEXT("a2e_streaming_live_mode"), InSettings.bA2EStreamingLiveMode);
                JsonObject->SetBoolField(TEXT("a2e_force_set_keys"), InSettings.bA2EForceSetKeys);

                TArray<TSharedPtr<FJsonValue>> A2EPreferredEmotionArray;
                for (int32 EmotionValue : InSettings.A2EPreferredEmotion)
                {
                    A2EPreferredEmotionArray.Add(MakeShareable(new FJsonValueNumber(EmotionValue)));
                }
                JsonObject->SetArrayField(TEXT("a2e_preferred_emotion"), A2EPreferredEmotionArray);

                TArray<TSharedPtr<FJsonValue>> EmotionArray;
                for (int32 EmotionValue : InSettings.Emotion)
                {
                    EmotionArray.Add(MakeShareable(new FJsonValueNumber(EmotionValue)));
                }
                JsonObject->SetArrayField(TEXT("emotion"), EmotionArray);

                JsonObject->SetStringField(TEXT("source_shot"), InSettings.SourceShot);
            }*/
        }
       
        HeardJsonObject->SetObjectField(TEXT("settings"), JsonObject);

        TSharedRef<TJsonWriter<TCHAR>> JsonWriter = TJsonWriterFactory<TCHAR>::Create(&OutJson);
        return FJsonSerializer::Serialize(HeardJsonObject.ToSharedRef(), JsonWriter);
}

bool USendA2FRequestProxy::Audio2FaceSettingsToJsonString(const FString& Instance,
	const TMap<FString, float>& InSettings, FString& OutJson)
{
	TSharedPtr<FJsonObject> HeardJsonObject = MakeShareable(new FJsonObject);
	HeardJsonObject->SetStringField(TEXT("a2f_instance"), Instance);

	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	{
		for(auto Setting:InSettings)
		{
			double Offset = 0.00001;
			JsonObject->SetNumberField(Setting.Key, Setting.Value+Offset);
		}
	}
	 
	HeardJsonObject->SetObjectField(TEXT("settings"), JsonObject);

	TSharedRef<TJsonWriter<TCHAR>> JsonWriter = TJsonWriterFactory<TCHAR>::Create(&OutJson);
	return FJsonSerializer::Serialize(HeardJsonObject.ToSharedRef(), JsonWriter);
}

bool USendA2FRequestProxy::Audio2FaceSetEmotionToJsonString(const FString& Instance, const FA2FEmotion& InEmotion,
                                                            FString& OutJson)
{
	TSharedPtr<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> JsonWriter =
			TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&OutJson);

	JsonWriter->WriteObjectStart();
	{
		JsonWriter->WriteValue(TEXT("a2f_instance"), Instance);

		JsonWriter->WriteArrayStart(TEXT("emotion"));
		{
			JsonWriter->WriteValue(InEmotion.Amazement);
			JsonWriter->WriteValue(InEmotion.Anger);
			JsonWriter->WriteValue( InEmotion.Cheekiness);
			JsonWriter->WriteValue( InEmotion.Disgust);
			JsonWriter->WriteValue( InEmotion.Fear);
			JsonWriter->WriteValue( InEmotion.Grief);
			JsonWriter->WriteValue( InEmotion.Joy);
			JsonWriter->WriteValue( InEmotion.OutOfBreath);
			JsonWriter->WriteValue(InEmotion.Pain);
			JsonWriter->WriteValue( InEmotion.Sadness);
		}
		JsonWriter->WriteArrayEnd();
	}
	JsonWriter->WriteObjectEnd();

	JsonWriter->Close();

	return OutJson.Len() > 0;
}

bool USendA2FRequestProxy::Audio2FaceSetEmotionNameToJsonString(const FString& Instance,
	const TMap<FString, float>& InEmotion, FString& OutJson)
{
	TSharedPtr<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> JsonWriter =
			TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&OutJson);

	JsonWriter->WriteObjectStart();
	{
		JsonWriter->WriteValue(TEXT("a2f_instance"), Instance);

		JsonWriter->WriteObjectStart(TEXT("emotions"));
		{
			for(auto& Elem:InEmotion)
			{
				JsonWriter->WriteValue(Elem.Key,Elem.Value);
			}
		}
		JsonWriter->WriteObjectEnd();
	}
	JsonWriter->WriteObjectEnd();

	JsonWriter->Close();

	return OutJson.Len() > 0;
}

bool USendA2FRequestProxy::Audio2FaceSetEmotionSettingToJsonString(const FString& Instance,
                                                                   const FAudio2EmotionSettings& InSetting, FString& OutJson)
{
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);

	JsonObject->SetStringField("a2f_instance", Instance);

	JsonObject->SetNumberField("a2e_streaming_smoothing", InSetting.StreamingSmoothing);
	JsonObject->SetNumberField("a2e_streaming_update_period", InSetting.StreamingUpdatePeriod);
	JsonObject->SetNumberField("a2e_streaming_transition_time", InSetting.StreamingTransitionTime);
	JsonObject->SetNumberField("a2e_window_size", InSetting.WindowSize);
	JsonObject->SetNumberField("a2e_stride", InSetting.Stride);
	JsonObject->SetNumberField("a2e_emotion_strength", InSetting.EmotionStrength);
	JsonObject->SetNumberField("a2e_smoothing_kernel_radius", InSetting.SmoothingKernelRadius);
	JsonObject->SetNumberField("a2e_smoothing_exp", InSetting.SmoothingExp);
	JsonObject->SetNumberField("a2e_max_emotions", InSetting.MaxEmotions);
	JsonObject->SetNumberField("a2e_contrast", InSetting.Contrast);
       
	JsonObject->SetNumberField("a2e_preferred_emotion_strength", InSetting.PreferredEmotionStrength);

	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutJson);
	return FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);
}

bool USendA2FRequestProxy::Audio2FaceSetPlayerTrackToJsonString(const FString& Instance,
	const FA2FPlayerTrack& InPlayerTrack, FString& OutJson)
{
	TSharedPtr<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> JsonWriter =
			TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&OutJson);

	JsonWriter->WriteObjectStart();
	{
		JsonWriter->WriteValue(TEXT("a2f_player"), Instance);
		JsonWriter->WriteValue(TEXT("file_name"),InPlayerTrack.FileName);
		JsonWriter->WriteArrayStart(TEXT("time_range"));
		{
			JsonWriter->WriteValue(InPlayerTrack.TimeRangeMin);
			JsonWriter->WriteValue(InPlayerTrack.TimeRangeMax);
		}
		JsonWriter->WriteArrayEnd();
	}
	JsonWriter->WriteObjectEnd();

	JsonWriter->Close();

	return OutJson.Len() > 0;
}

bool USendA2FRequestProxy::Audio2FaceStreamLivelinkSettingsToJsonString(const FString& InLivelinkNode,
	const FStreamLivelinkSettings& InSettings, FString& OutJson)
{
	TSharedPtr<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> JsonWriter =
		  TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&OutJson);

	JsonWriter->WriteObjectStart();
	{
		JsonWriter->WriteValue(TEXT("node_path"), InLivelinkNode);

		JsonWriter->WriteObjectStart(TEXT("values"));
		{
			JsonWriter->WriteValue(TEXT("audio_port"), InSettings.AudioPort);
			JsonWriter->WriteValue(TEXT("enable_audio_stream"), InSettings.bEnableAudioStream);
			JsonWriter->WriteValue(TEXT("enable_gaze"), InSettings.bEnableGaze);
			JsonWriter->WriteValue(TEXT("enable_idle_face"), InSettings.bEnableIdleFace);
			JsonWriter->WriteValue(TEXT("enable_idle_head"), InSettings.bEnableIdleHead);
			JsonWriter->WriteValue(TEXT("idle_face_multiplier"), InSettings.IdleFaceMultiplier);
			JsonWriter->WriteValue(TEXT("idle_head_multiplier"), InSettings.IdleHeadMultiplier);
			JsonWriter->WriteValue(TEXT("idle_motion_fps"), InSettings.IdleMotionFps);
			JsonWriter->WriteValue(TEXT("livelink_host"), InSettings.LivelinkHost);
			JsonWriter->WriteValue(TEXT("livelink_port"), InSettings.LivelinkPort);
			JsonWriter->WriteValue(TEXT("livelink_subject"), InSettings.LivelinkSubject);
		}
		JsonWriter->WriteObjectEnd();
	}
	JsonWriter->WriteObjectEnd();

	JsonWriter->Close();

	return OutJson.Len() > 0;
}

bool USendA2FRequestProxy::Audio2FaceExportBlendshapesToJsonString(const FString& InBlendshapeNode,
	const FA2FBlendshapeParam& Blendshape, FString& OutJson)
{
	FPaths::NormalizeFilename(*const_cast<FString*>(&Blendshape.ExportDirectory));

	TSharedPtr<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> JsonWriter =
		TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&OutJson);

	JsonWriter->WriteObjectStart();
	{
		JsonWriter->WriteValue(TEXT("solver_node"), InBlendshapeNode);

		JsonWriter->WriteValue(TEXT("export_directory"), Blendshape.ExportDirectory);
		JsonWriter->WriteValue(TEXT("file_name"), Blendshape.Filename);
		JsonWriter->WriteValue(TEXT("format"), Blendshape.Format);
		JsonWriter->WriteValue(TEXT("batch"), Blendshape.bBatch);
		JsonWriter->WriteValue(TEXT("fps"), Blendshape.FPS);
	}
	JsonWriter->WriteObjectEnd();

	JsonWriter->Close();

	return OutJson.Len() > 0;
}

USendA2FRequestGetArrayProxy* USendA2FRequestGetArrayProxy::SendHttpA2FGetInstanceRequest()
{
	USendA2FRequestGetArrayProxy* const Proxy = NewObject<USendA2FRequestGetArrayProxy>();

	UCBHTTPRequset* const Request = Proxy->GetRequest();

	if(UCBAudio2FaceSettings* Settings=UCBAudio2FaceSettings::Get())
	{
		Request->SetURL(Settings->ServerURL+TEXT("/A2F/GetInstances"));
		Request->SetMimeType(EHttpMimeType::json);
		Request->SetVerb(EHttpVerb::GET);
		Proxy->SendRequest();
	}
	return Proxy;
}

USendA2FRequestGetArrayProxy* USendA2FRequestGetArrayProxy::SendHttpA2FGetSettingNamesRequest(const FString& Instance)
{
	USendA2FRequestGetArrayProxy* const Proxy = NewObject<USendA2FRequestGetArrayProxy>();

	UCBHTTPRequset* const Request = Proxy->GetRequest();

	if(UCBAudio2FaceSettings* Settings=UCBAudio2FaceSettings::Get())
	{
		Request->SetURL(Settings->ServerURL+TEXT("/A2F/GetSettingNames"));
		Request->SetMimeType(EHttpMimeType::json);
		Request->SetVerb(EHttpVerb::POST);
		FString RequestString = FString::Printf(TEXT("{\"a2f_instance\":\"%s\"}"), *Instance);
		Request->SetContentAsString(RequestString);
		
		Proxy->SendRequest();
	}
	return Proxy;
}

USendA2FRequestGetArrayProxy* USendA2FRequestGetArrayProxy::SendHttpA2FGetEmotionNamesRequest()
{
	USendA2FRequestGetArrayProxy* const Proxy = NewObject<USendA2FRequestGetArrayProxy>();

	UCBHTTPRequset* const Request = Proxy->GetRequest();

	if(UCBAudio2FaceSettings* Settings=UCBAudio2FaceSettings::Get())
	{
		Request->SetURL(Settings->ServerURL+TEXT("/A2F/A2E/GetEmotionNames"));
		Request->SetMimeType(EHttpMimeType::json);
		Request->SetVerb(EHttpVerb::GET);
		Proxy->SendRequest();
	}
	return Proxy;
}

USendA2FRequestGetArrayProxy* USendA2FRequestGetArrayProxy::SendHttpA2FGetBlendShapesolvesRequest()
{
	USendA2FRequestGetArrayProxy* const Proxy = NewObject<USendA2FRequestGetArrayProxy>();

	UCBHTTPRequset* const Request = Proxy->GetRequest();

	if(UCBAudio2FaceSettings* Settings=UCBAudio2FaceSettings::Get())
	{
		Request->SetURL(Settings->ServerURL+TEXT("/A2F/Exporter/GetBlendShapeSolvers"));
		Request->SetMimeType(EHttpMimeType::json);
		Request->SetVerb(EHttpVerb::GET);
		Proxy->SendRequest();
	}
	return Proxy;
}

USendA2FRequestGetArrayProxy* USendA2FRequestGetArrayProxy::SendHttpA2FGetStreamLivelinkNodesRequest()
{
	USendA2FRequestGetArrayProxy* const Proxy = NewObject<USendA2FRequestGetArrayProxy>();

	UCBHTTPRequset* const Request = Proxy->GetRequest();

	if(UCBAudio2FaceSettings* Settings=UCBAudio2FaceSettings::Get())
	{
		Request->SetURL(Settings->ServerURL+TEXT("/A2F/Exporter/GetStreamLivelinkNodes"));
		Request->SetMimeType(EHttpMimeType::json);
		Request->SetVerb(EHttpVerb::GET);
		Proxy->SendRequest();
	}
	return Proxy;
}

USendA2FRequestGetArrayProxy* USendA2FRequestGetArrayProxy::SendHttpA2FGetPlayerTracksRequest(
	const FString& PlayerInstance)
{
	USendA2FRequestGetArrayProxy* const Proxy = NewObject<USendA2FRequestGetArrayProxy>();

	UCBHTTPRequset* const Request = Proxy->GetRequest();

	if(UCBAudio2FaceSettings* Settings=UCBAudio2FaceSettings::Get())
	{
		Request->SetURL(Settings->ServerURL+TEXT("/A2F/Player/GetTracks"));
		Request->SetMimeType(EHttpMimeType::json);
		Request->SetVerb(EHttpVerb::POST);

		FString ContentString = FString::Printf(TEXT(
		"{\
			\"a2f_player\":\"%s\"\
		}"),
		*PlayerInstance);
		Request->SetContentAsString(ContentString);
		Proxy->SendRequest();
	}
	return Proxy;
}

void USendA2FRequestGetArrayProxy::OnErrorInternal(UCBHTTPResponse* const Response)
{
	TArray<FString> Instance;
	UE_LOG(LogCBAudio2Face,Error,TEXT("%s"),*Response->GetContentAsString())
	OnError.Broadcast(Instance);
	OnErrorC.ExecuteIfBound(Instance);
}

void USendA2FRequestGetArrayProxy::OnSuccessInternal(UCBHTTPResponse* const Response)
{
	
	TArray<FString> Instance;
	if(!(Response->GetResponseCode()==200))
	{
		UE_LOG(LogCBAudio2Face,Error,TEXT("%s"),*Response->GetContentAsString())
		OnError.Broadcast(Instance);
		OnErrorC.ExecuteIfBound(Instance);
	}
	else
	{
		FString ContentString = Response->GetContentAsString();
		GetResponseInstance(Instance,ContentString);
		OnResponse.Broadcast(Instance);
		OnResponseC.ExecuteIfBound(Instance);
	}
	
}

bool USendA2FRequestGetArrayProxy::GetResponseInstance(TArray<FString>& Instance,const FString& Response)
{
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(Response);
	TSharedPtr<FJsonValue> ReadRoot;

	bool bOk=false;
	if (FJsonSerializer::Deserialize(JsonReader, ReadRoot))
	{
		if (TSharedPtr<FJsonObject> InJsonObject = ReadRoot->AsObject())
		{

			FString Status= InJsonObject->GetStringField(TEXT("status"));

			if(Status==TEXT("OK"))
			{
				bOk=true;
			}
			else
			{
				UE_LOG(LogCBAudio2Face,Error,TEXT("%s"),*Response);
				return false;
			}
			
			if(InJsonObject)
			{
				 const TSharedPtr<FJsonObject>* ResultObject;
				if(InJsonObject->TryGetObjectField(TEXT("result"),ResultObject))
				{
					const TArray<TSharedPtr<FJsonValue>>& FullFaceInstancesArrays = (*ResultObject)->GetArrayField(TEXT("fullface_instances"));
					for (auto &JsonValue : FullFaceInstancesArrays)
					{
						Instance.Add(JsonValue->AsString());
					}
				}
				else
				{
					InJsonObject->TryGetStringArrayField(TEXT("result"),Instance);
				}
			}
			
			if(Status==TEXT("OK"))
			{
				bOk=true;
			}
		}
	}

	return bOk;
}


void USendA2FRequestGetTimeProxy::OnErrorInternal(UCBHTTPResponse* const Response)
{
	float Time=-1.0f;
	UE_LOG(LogCBAudio2Face,Error,TEXT("%s"),*Response->GetContentAsString())
	OnError.Broadcast(Time);
	OnErrorC.ExecuteIfBound(Time);
}

void USendA2FRequestGetTimeProxy::OnSuccessInternal(UCBHTTPResponse* const Response)
{
	float Time=-1.0f;
	if(!(Response->GetResponseCode()==200))
	{
		UE_LOG(LogCBAudio2Face,Error,TEXT("%s"),*Response->GetContentAsString())
		OnError.Broadcast(Time);
		OnErrorC.ExecuteIfBound(Time);
	}
	else
	{
		FString ContentString = Response->GetContentAsString();
		GetResponse(Time,ContentString);
		OnResponse.Broadcast(Time);
		OnResponseC.ExecuteIfBound(Time);
	}
}

bool USendA2FRequestGetTimeProxy::GetResponse(float& Result, const FString& Response)
{
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(Response);
	TSharedPtr<FJsonValue> ReadRoot;

	bool bOk=false;
	if (FJsonSerializer::Deserialize(JsonReader, ReadRoot))
	{
		if (TSharedPtr<FJsonObject> InJsonObject = ReadRoot->AsObject())
		{
			FString Status= InJsonObject->GetStringField(TEXT("status"));
			if(Status==TEXT("OK"))
			{
				bOk=true;
				Result=InJsonObject->GetNumberField(TEXT("result"));
			}
			else
			{
				UE_LOG(LogCBAudio2Face,Error,TEXT("%s"),*Response);
				return false;
			}
		}
	}

	return bOk;
}

USendA2FRequestGetNumProxy* USendA2FRequestGetNumProxy::SendHttpA2FGetFrameRequest(const FString& Instance,
                                                                                   bool bAsTimestamp /**=false*/)
{
	USendA2FRequestGetNumProxy* const Proxy = NewObject<USendA2FRequestGetNumProxy>();

	UCBHTTPRequset* const Request = Proxy->GetRequest();

	if(UCBAudio2FaceSettings* Settings=UCBAudio2FaceSettings::Get())
	{
		Request->SetURL(Settings->ServerURL+TEXT("/A2F/GetFrame"));
		Request->SetMimeType(EHttpMimeType::json);
		Request->SetVerb(EHttpVerb::POST);

		FString RequestString = FString::Printf(TEXT("{\"a2f_instance\":\"%s\",\"as_timestamp\":\"%s\"}"), *Instance,(bAsTimestamp ? TEXT("true") : TEXT("false")));
		Request->SetContentAsString(RequestString);
		
		Proxy->SendRequest();
	}
	return Proxy;
}

USendA2FRequestGetTimeProxy* USendA2FRequestGetTimeProxy::SendHttpA2FGetPlayerTimeRequest(const FString& PlayerInstance)
{
	USendA2FRequestGetTimeProxy* const Proxy = NewObject<USendA2FRequestGetTimeProxy>();

	UCBHTTPRequset* const Request = Proxy->GetRequest();

	if(UCBAudio2FaceSettings* Settings=UCBAudio2FaceSettings::Get())
	{
		Request->SetURL(Settings->ServerURL+TEXT("/A2F/Player/GetTime"));
		Request->SetMimeType(EHttpMimeType::json);
		Request->SetVerb(EHttpVerb::POST);

		FString RequestString = FString::Printf(TEXT("{\"a2f_player\":\"%s\"}"), *PlayerInstance);
		Request->SetContentAsString(RequestString);
		
		Proxy->SendRequest();
	}
	return Proxy;
}

void USendA2FRequestGetNumProxy::OnErrorInternal(UCBHTTPResponse* const Response)
{
	int32 Fream=-1.0;
	UE_LOG(LogCBAudio2Face,Error,TEXT("%s"),*Response->GetContentAsString())
	OnError.Broadcast(Fream);
	OnErrorC.ExecuteIfBound(Fream);
}

void USendA2FRequestGetNumProxy::OnSuccessInternal(UCBHTTPResponse* const Response)
{
	
	int32 Fream=-1.0;
	if(!(Response->GetResponseCode()==200))
	{
		UE_LOG(LogCBAudio2Face,Error,TEXT("%s"),*Response->GetContentAsString())
		OnError.Broadcast(Fream);
		OnErrorC.ExecuteIfBound(Fream);
	}
	else
	{
		FString ContentString = Response->GetContentAsString();
		GetFrameResponse(Fream,ContentString);
		OnResponse.Broadcast(Fream);
		OnResponseC.ExecuteIfBound(Fream);
	}
	
}

bool USendA2FRequestGetNumProxy::GetFrameResponse(int32& Result , const FString& Response)
{
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(Response);
	TSharedPtr<FJsonValue> ReadRoot;

	bool bOk=false;
	if (FJsonSerializer::Deserialize(JsonReader, ReadRoot))
	{
		if (TSharedPtr<FJsonObject> InJsonObject = ReadRoot->AsObject())
		{
			FString Status= InJsonObject->GetStringField(TEXT("status"));
			if(Status==TEXT("OK"))
			{
				bOk=true;
				Result=InJsonObject->GetNumberField(TEXT("result"));
			}
			else
			{
				UE_LOG(LogCBAudio2Face,Error,TEXT("%s"),*Response);
				return false;
			}
		}
	}

	return bOk;
}

USendA2FRequestEmotionWeightsProxy* USendA2FRequestEmotionWeightsProxy::SendHttpA2FGetEmotionWeightsByFrameRequest(
	const FString& Instance, int32 Frame,bool bAsVector,bool bAsTimestamp)
{
	USendA2FRequestEmotionWeightsProxy* const Proxy = NewObject<USendA2FRequestEmotionWeightsProxy>();

	UCBHTTPRequset* const Request = Proxy->GetRequest();

	if(UCBAudio2FaceSettings* Settings=UCBAudio2FaceSettings::Get())
	{
		Request->SetURL(Settings->ServerURL+TEXT("/A2F/A2E/GetEmotion"));
		Request->SetMimeType(EHttpMimeType::json);
		Request->SetVerb(EHttpVerb::POST);

		FString RequestString =  FString::Printf(TEXT(
		"{\
		\"a2f_instance\":\"%s\",\
		\"as_vector\":%s,\
		\"frame\":%i,\
		\"as_timestamp\":%s\
	}"),
	*Instance,
	bAsVector ? TEXT("true") : TEXT("false"),
	Frame,
	bAsTimestamp ? TEXT("true") : TEXT("false")
	);
		Request->SetContentAsString(RequestString);
		
		Proxy->SendRequest();
	}
	return Proxy;
	
}

void USendA2FRequestEmotionWeightsProxy::OnErrorInternal(UCBHTTPResponse* const Response)
{
	FA2FEmotion Emotion ;
	UE_LOG(LogCBAudio2Face,Error,TEXT("%s"),*Response->GetContentAsString())
	OnError.Broadcast(Emotion);
	OnErrorC.ExecuteIfBound(Emotion);
}

void USendA2FRequestEmotionWeightsProxy::OnSuccessInternal(UCBHTTPResponse* const Response)
{
	FA2FEmotion OutEmotion;
	if(!(Response->GetResponseCode()==200))
	{
		UE_LOG(LogCBAudio2Face,Error,TEXT("%s"),*Response->GetContentAsString())
		OnError.Broadcast(OutEmotion);
		OnErrorC.ExecuteIfBound(OutEmotion);
	}
	else
	{
		FString ContentString = Response->GetContentAsString();
		GetResponse(ContentString,OutEmotion);
		OnResponse.Broadcast(OutEmotion);
		OnResponseC.ExecuteIfBound(OutEmotion);
	}
}

bool USendA2FRequestEmotionWeightsProxy::GetResponse(FString& Result,FA2FEmotion& OutEmotion)
{
	TSharedPtr<FJsonObject> ReadRoot;
	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(Result);

	bool bOK;
	if (FJsonSerializer::Deserialize(JsonReader, ReadRoot))
	{

		FString Status= ReadRoot->GetStringField(TEXT("status"));

		if(Status==TEXT("OK"))
		{
			bOK=true;
		}
		else
		{
			UE_LOG(LogCBAudio2Face,Error,TEXT("%s"),*Result);
			return false;
		}
		if (TSharedPtr<FJsonObject> JsonObject = ReadRoot->GetObjectField(TEXT("result")))
		{
			OutEmotion.Amazement = JsonObject->GetNumberField("amazement");
			OutEmotion.Anger = JsonObject->GetNumberField("anger");
			OutEmotion.Cheekiness = JsonObject->GetNumberField("cheekiness");
			OutEmotion.Disgust = JsonObject->GetNumberField("disgust");
			OutEmotion.Fear = JsonObject->GetNumberField("fear");
			OutEmotion.Grief = JsonObject->GetNumberField("grief");
			OutEmotion.Joy = JsonObject->GetNumberField("joy");
			OutEmotion.OutOfBreath = JsonObject->GetNumberField("outofbreath");
			OutEmotion.Pain = JsonObject->GetNumberField("pain");
			OutEmotion.Sadness = JsonObject->GetNumberField("sadness");

			return true;
		}
	}

	return false;	
}

USendA2FRequestEmotionSettingsProxy* USendA2FRequestEmotionSettingsProxy::SendHttpA2FGetEmotionSettingsRequest(
	const FString& Instance)
{
	USendA2FRequestEmotionSettingsProxy* const Proxy = NewObject<USendA2FRequestEmotionSettingsProxy>();

	UCBHTTPRequset* const Request = Proxy->GetRequest();

	if(UCBAudio2FaceSettings* Settings=UCBAudio2FaceSettings::Get())
	{
		Request->SetURL(Settings->ServerURL+TEXT("/A2F/A2E/GetSettings"));
		Request->SetMimeType(EHttpMimeType::json);
		Request->SetVerb(EHttpVerb::POST);

		FString RequestString = FString::Printf(TEXT(
		"{\
			\"a2f_instance\":\"%s\"\
		}"),
		*Instance);
		Request->SetContentAsString(RequestString);
		
		Proxy->SendRequest();
	}
	return Proxy;
}

void USendA2FRequestEmotionSettingsProxy::OnErrorInternal(UCBHTTPResponse* const Response)
{
	FAudio2EmotionSettings OutEmotionSettings ;
	UE_LOG(LogCBAudio2Face,Error,TEXT("%s"),*Response->GetContentAsString())
	OnError.Broadcast(OutEmotionSettings);
	OnErrorC.ExecuteIfBound(OutEmotionSettings);
}

void USendA2FRequestEmotionSettingsProxy::OnSuccessInternal(UCBHTTPResponse* const Response)
{
	FAudio2EmotionSettings OutEmotionSettings;
	if(!(Response->GetResponseCode()==200))
	{
		UE_LOG(LogCBAudio2Face,Error,TEXT("%s"),*Response->GetContentAsString())
		OnError.Broadcast(OutEmotionSettings);
		OnErrorC.ExecuteIfBound(OutEmotionSettings);
	}
	else
	{
		FString ContentString = Response->GetContentAsString();
		GetResponse(ContentString,OutEmotionSettings);
		OnResponse.Broadcast(OutEmotionSettings);
		OnResponseC.ExecuteIfBound(OutEmotionSettings);
	}
	
}

bool USendA2FRequestEmotionSettingsProxy::GetResponse(FString& Result, FAudio2EmotionSettings& Response)
{
	TSharedPtr<FJsonObject> ReadRoot;
	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(Result);

	if (FJsonSerializer::Deserialize(JsonReader, ReadRoot))
	{
		if (TSharedPtr<FJsonObject> JsonObject = ReadRoot->GetObjectField(TEXT("result")))
		{
			Response.StreamingSmoothing = JsonObject->GetNumberField("a2e_streaming_smoothing");
			Response.StreamingUpdatePeriod = JsonObject->GetIntegerField("a2e_streaming_update_period");
			Response.StreamingTransitionTime = JsonObject->GetNumberField("a2e_streaming_transition_time");
			Response.WindowSize = JsonObject->GetNumberField("a2e_window_size");
			Response.Stride = JsonObject->GetIntegerField("a2e_stride");
			Response.EmotionStrength = JsonObject->GetNumberField("a2e_emotion_strength");
			Response.SmoothingKernelRadius = JsonObject->GetIntegerField("a2e_smoothing_kernel_radius");
			Response.SmoothingExp = JsonObject->GetIntegerField("a2e_smoothing_exp");
			Response.MaxEmotions = JsonObject->GetIntegerField("a2e_max_emotions");
			Response.Contrast = JsonObject->GetIntegerField("a2e_contrast");
			Response.PreferredEmotionStrength = JsonObject->GetNumberField("a2e_preferred_emotion_strength");

			return true;
		}
	}

	return false;
}

USendA2FRequestPlayerInstanceProxy* USendA2FRequestPlayerInstanceProxy::SendHttpA2FGetPlayerInstanceRequest()
{
	USendA2FRequestPlayerInstanceProxy* const Proxy = NewObject<USendA2FRequestPlayerInstanceProxy>();

	UCBHTTPRequset* const Request = Proxy->GetRequest();

	if(UCBAudio2FaceSettings* Settings=UCBAudio2FaceSettings::Get())
	{
		Request->SetURL(Settings->ServerURL+TEXT("/A2F/Player/GetInstances"));
		Request->SetMimeType(EHttpMimeType::json);
		Request->SetVerb(EHttpVerb::GET);
		Proxy->SendRequest();
	}
	return Proxy;
}

void USendA2FRequestPlayerInstanceProxy::OnErrorInternal(UCBHTTPResponse* const Response)
{
	TArray<FString> OutRegularResponse;
	TArray<FString> OutStreamingResponse;
	UE_LOG(LogCBAudio2Face,Error,TEXT("%s"),*Response->GetContentAsString())
	OnError.Broadcast(OutRegularResponse,OutStreamingResponse);
	OnErrorC.ExecuteIfBound(OutRegularResponse,OutStreamingResponse);
}

void USendA2FRequestPlayerInstanceProxy::OnSuccessInternal(UCBHTTPResponse* const Response)
{
	TArray<FString> OutRegularResponse;
	TArray<FString> OutStreamingResponse;
	
	if(!(Response->GetResponseCode()==200))
	{
		UE_LOG(LogCBAudio2Face,Error,TEXT("%s"),*Response->GetContentAsString())
		OnError.Broadcast(OutRegularResponse,OutStreamingResponse);
		OnErrorC.ExecuteIfBound(OutRegularResponse,OutStreamingResponse);
	}
	else
	{
		FString ContentString = Response->GetContentAsString();
	
		GetResponse(ContentString,OutRegularResponse,OutStreamingResponse);
		OnResponse.Broadcast(OutRegularResponse,OutStreamingResponse);
		OnResponseC.ExecuteIfBound(OutRegularResponse,OutStreamingResponse);
	}
	
}

void USendA2FRequestPlayerInstanceProxy::GetResponse(FString& Result, TArray<FString>& OutRegularResponse,
	TArray<FString>& OutStreamingResponse)
{
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(Result);
	TSharedPtr<FJsonValue> ReadRoot;

	if (FJsonSerializer::Deserialize(JsonReader, ReadRoot))
	{
		if (TSharedPtr<FJsonObject> InJsonObject = ReadRoot->AsObject())
		{
			if (TSharedPtr<FJsonObject> InResultJsonObject = InJsonObject->GetObjectField(TEXT("result")))
			{
				const TArray<TSharedPtr<FJsonValue>>& InRegularArrays = InResultJsonObject->GetArrayField(TEXT("regular"));
				for (auto& Tmp : InRegularArrays)
				{
					OutRegularResponse.Add(Tmp->AsString());
				}

				const TArray<TSharedPtr<FJsonValue>>& InStreamingArrays = InResultJsonObject->GetArrayField(TEXT("streaming"));
				for (auto& Tmp : InStreamingArrays)
				{
					OutStreamingResponse.Add(Tmp->AsString());
				}
			}
		}
	}
}

USendA2FRequestGetStringProxy* USendA2FRequestGetStringProxy::SendHttpA2FGetPlayerRootPathRequest(
	const FString& PlayerInstance)
{
	USendA2FRequestGetStringProxy* const Proxy = NewObject<USendA2FRequestGetStringProxy>();

	UCBHTTPRequset* const Request = Proxy->GetRequest();

	if(UCBAudio2FaceSettings* Settings=UCBAudio2FaceSettings::Get())
	{
		Request->SetURL(Settings->ServerURL+TEXT("/A2F/Player/GetRootPath"));
		Request->SetMimeType(EHttpMimeType::json);
		Request->SetVerb(EHttpVerb::POST);
		FString ContenString = FString::Printf(TEXT(
		"{\
			\"a2f_player\":\"%s\"\
		}"),
		*PlayerInstance);
		Request->SetContentAsString(ContenString);
		Proxy->SendRequest();
	}
	return Proxy;
}

USendA2FRequestGetStringProxy* USendA2FRequestGetStringProxy::SendHttpA2FGetPlayerCurrentTrackRequest(
	const FString& PlayerInstance)
{
	USendA2FRequestGetStringProxy* const Proxy = NewObject<USendA2FRequestGetStringProxy>();

	UCBHTTPRequset* const Request = Proxy->GetRequest();

	if(UCBAudio2FaceSettings* Settings=UCBAudio2FaceSettings::Get())
	{
		Request->SetURL(Settings->ServerURL+TEXT("/A2F/Player/GetCurrentTrack"));
		Request->SetMimeType(EHttpMimeType::json);
		Request->SetVerb(EHttpVerb::POST);
		FString ContenString = FString::Printf(TEXT(
		"{\
			\"a2f_player\":\"%s\"\
		}"),
		*PlayerInstance);
		Request->SetContentAsString(ContenString);
		Proxy->SendRequest();
	}
	return Proxy;	
}

void USendA2FRequestGetStringProxy::OnErrorInternal(UCBHTTPResponse* const InResponse)
{
	FString Response;
	UE_LOG(LogCBAudio2Face,Error,TEXT("%s"),*InResponse->GetContentAsString())
	OnError.Broadcast(Response);
	OnErrorC.ExecuteIfBound(Response);
}

void USendA2FRequestGetStringProxy::OnSuccessInternal(UCBHTTPResponse* const InResponse)
{
	FString Response;
	if(!(InResponse->GetResponseCode()==200))
	{
		UE_LOG(LogCBAudio2Face,Error,TEXT("%s"),*InResponse->GetContentAsString())
		OnError.Broadcast(Response);
		OnErrorC.ExecuteIfBound(Response);
	}
	else
	{
		FString ContentString = InResponse->GetContentAsString();
	
		GetResponse(ContentString,Response);
		OnResponse.Broadcast(Response);
		OnResponseC.ExecuteIfBound(Response);
	}
}

void USendA2FRequestGetStringProxy::GetResponse(FString& Result, FString& InResponse)
{
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(Result);
	TSharedPtr<FJsonValue> ReadRoot;

	if (FJsonSerializer::Deserialize(JsonReader, ReadRoot))
	{
		if (TSharedPtr<FJsonObject> InJsonObject = ReadRoot->AsObject())
		{
			if(!InJsonObject->TryGetStringField(TEXT("result"),InResponse))
			{
				InResponse=FString();
			}
		}
	}
}

USendA2FRequestPlayerRangeProxy* USendA2FRequestPlayerRangeProxy::SendHttpA2FGetPlayerIRangeRequest(const FString& PlayerInstance)
{
	USendA2FRequestPlayerRangeProxy* const Proxy = NewObject<USendA2FRequestPlayerRangeProxy>();

	UCBHTTPRequset* const Request = Proxy->GetRequest();

	if(UCBAudio2FaceSettings* Settings=UCBAudio2FaceSettings::Get())
	{
		Request->SetURL(Settings->ServerURL+TEXT("/A2F/Player/GetRange"));
		Request->SetMimeType(EHttpMimeType::json);
		Request->SetVerb(EHttpVerb::POST);
		FString ContenString = FString::Printf(TEXT(
		"{\
			\"a2f_player\":\"%s\"\
		}"),
		*PlayerInstance);

		Request->SetContentAsString(ContenString);
		
		Proxy->SendRequest();
	}
	return Proxy;
}

void USendA2FRequestPlayerRangeProxy::OnErrorInternal(UCBHTTPResponse* const InResponse)
{
	FA2FPlayerRange Response;
	UE_LOG(LogCBAudio2Face,Error,TEXT("%s"),*InResponse->GetContentAsString())
	OnError.Broadcast(Response);
	OnErrorC.ExecuteIfBound(Response);
}

void USendA2FRequestPlayerRangeProxy::OnSuccessInternal(UCBHTTPResponse* const InResponse)
{
	FA2FPlayerRange Response;
	if(!(InResponse->GetResponseCode()==200))
	{
		UE_LOG(LogCBAudio2Face,Error,TEXT("%s"),*InResponse->GetContentAsString())
		OnError.Broadcast(Response);
		OnErrorC.ExecuteIfBound(Response);
	}
	else
	{
		FString ContentString = InResponse->GetContentAsString();
	
		GetResponse(ContentString,Response);
		OnResponse.Broadcast(Response);
		OnResponseC.ExecuteIfBound(Response);
	}
}

void USendA2FRequestPlayerRangeProxy::GetResponse(FString& Result, FA2FPlayerRange& PlayerRange)
{
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(Result);
	TSharedPtr<FJsonValue> ReadRoot;

	if (FJsonSerializer::Deserialize(JsonReader, ReadRoot))
	{
		if (TSharedPtr<FJsonObject> InJsonObject = ReadRoot->AsObject())
		{
			if (TSharedPtr<FJsonObject> InResultJsonObject = InJsonObject->GetObjectField(TEXT("result")))
			{
				const TArray<TSharedPtr<FJsonValue>>& InDefaultArrays = InResultJsonObject->GetArrayField(TEXT("default"));
				if (InDefaultArrays.Num() >= 2)
				{
					PlayerRange.DefaultMin = InDefaultArrays[0]->AsNumber();
					PlayerRange.DefaultMax = InDefaultArrays[1]->AsNumber();
				}

				const TArray<TSharedPtr<FJsonValue>>& InWorkArrays = InResultJsonObject->GetArrayField(TEXT("work"));
				if (InWorkArrays.Num() >= 2)
				{
					PlayerRange.WorkMin = InWorkArrays[0]->AsNumber();
					PlayerRange.WorkMax = InWorkArrays[1]->AsNumber();
				}
			}
		}
	}
	
}

USendA2FRequestLivelinkNodeConnectedProxy* USendA2FRequestLivelinkNodeConnectedProxy::
SendHttpA2FLivelinkNodeConnectedRequest(const FString& NodePath)
{
	USendA2FRequestLivelinkNodeConnectedProxy* const Proxy = NewObject<USendA2FRequestLivelinkNodeConnectedProxy>();

	UCBHTTPRequset* const Request = Proxy->GetRequest();

	if(UCBAudio2FaceSettings* Settings=UCBAudio2FaceSettings::Get())
	{
		Request->SetURL(Settings->ServerURL+TEXT("/A2F/Exporter/IsStreamLivelinkConnected"));
		Request->SetMimeType(EHttpMimeType::json);
		Request->SetVerb(EHttpVerb::POST);
		FString ContenString = FString::Printf(TEXT(
		"{\
			\"node_path\":\"%s\"\
		}"),
		*NodePath);

		Request->SetContentAsString(ContenString);
		
		Proxy->SendRequest();
	}
	return Proxy;
}

void USendA2FRequestLivelinkNodeConnectedProxy::OnErrorInternal(UCBHTTPResponse* const Response)
{
	bool bConnected=false;
	UE_LOG(LogCBAudio2Face,Error,TEXT("%s"),*Response->GetContentAsString())
	OnError.Broadcast(bConnected);
	OnErrorC.ExecuteIfBound(bConnected);
}

void USendA2FRequestLivelinkNodeConnectedProxy::OnSuccessInternal(UCBHTTPResponse* const Response)
{
	bool bConnected=false;
	if(!(Response->GetResponseCode()==200))
	{
		UE_LOG(LogCBAudio2Face,Error,TEXT("%s"),*Response->GetContentAsString())
		OnError.Broadcast(bConnected);
		OnErrorC.ExecuteIfBound(bConnected);
	}
	else
	{
		FString ContentString = Response->GetContentAsString();
	
		JsonStringToResultBool(ContentString,bConnected);
		OnResponse.Broadcast(bConnected);
		OnResponseC.ExecuteIfBound(bConnected);
	}
}

bool USendA2FRequestLivelinkNodeConnectedProxy::JsonStringToResultBool(const FString& InString, bool& OutResult)
{
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(InString);
	TSharedPtr<FJsonValue> ReadRoot;

	if (FJsonSerializer::Deserialize(JsonReader, ReadRoot))
	{
		if (TSharedPtr<FJsonObject> InJsonObject = ReadRoot->AsObject())
		{
			OutResult = InJsonObject->GetBoolField(TEXT("result"));

			return true;
		}
	}

	return false;
}

USendA2FRequestStreamLivelinkSettingsProxy* USendA2FRequestStreamLivelinkSettingsProxy::
SendHttpA2FGetStreamLivelinkSettingsRequest(const FString& NodePath)
{
	USendA2FRequestStreamLivelinkSettingsProxy* const Proxy = NewObject<USendA2FRequestStreamLivelinkSettingsProxy>();

	UCBHTTPRequset* const Request = Proxy->GetRequest();

	if(UCBAudio2FaceSettings* Settings=UCBAudio2FaceSettings::Get())
	{
		Request->SetURL(Settings->ServerURL+TEXT("/A2F/Exporter/GetStreamLivelinkSettings"));
		Request->SetMimeType(EHttpMimeType::json);
		Request->SetVerb(EHttpVerb::POST);
		FString ContenString = FString::Printf(TEXT(
		"{\
			\"node_path\":\"%s\"\
		}"),
		*NodePath);

		Request->SetContentAsString(ContenString);
		
		Proxy->SendRequest();
	}
	return Proxy;	
}

void USendA2FRequestStreamLivelinkSettingsProxy::OnErrorInternal(UCBHTTPResponse* const Response)
{
	FStreamLivelinkSettings LivelinkSettings;
	UE_LOG(LogCBAudio2Face,Error,TEXT("%s"),*Response->GetContentAsString())
	OnError.Broadcast(LivelinkSettings);
	OnErrorC.ExecuteIfBound(LivelinkSettings);
}

void USendA2FRequestStreamLivelinkSettingsProxy::OnSuccessInternal(UCBHTTPResponse* const Response)
{
	FStreamLivelinkSettings LivelinkSettings;
	if(!(Response->GetResponseCode()==200))
	{
		UE_LOG(LogCBAudio2Face,Error,TEXT("%s"),*Response->GetContentAsString())
		OnError.Broadcast(LivelinkSettings);
		OnErrorC.ExecuteIfBound(LivelinkSettings);
	}
	else
	{
		FString ContentString = Response->GetContentAsString();
	
		GetResponse(ContentString,LivelinkSettings);
		OnResponse.Broadcast(LivelinkSettings);
		OnResponseC.ExecuteIfBound(LivelinkSettings);
	}
}

bool USendA2FRequestStreamLivelinkSettingsProxy::GetResponse(FString& Result, FStreamLivelinkSettings& Response)
{
	TSharedPtr<FJsonObject> RootObject;
	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(Result);
	if (FJsonSerializer::Deserialize(JsonReader, RootObject))
	{
		if (const TSharedPtr<FJsonObject>& JsonObject = RootObject->GetObjectField(TEXT("result")))
		{
			Response.AudioPort = JsonObject->GetNumberField(TEXT("audio_port"));
			Response.bEnableAudioStream = JsonObject->GetBoolField(TEXT("enable_audio_stream"));
			Response.bEnableGaze = JsonObject->GetBoolField(TEXT("enable_gaze"));
			Response.bEnableIdleFace = JsonObject->GetBoolField(TEXT("enable_idle_face"));
			Response.bEnableIdleHead = JsonObject->GetBoolField(TEXT("enable_idle_head"));
			Response.IdleFaceMultiplier = JsonObject->GetNumberField(TEXT("idle_face_multiplier"));
			Response.IdleHeadMultiplier = JsonObject->GetNumberField(TEXT("idle_head_multiplier"));
			Response.IdleMotionFps = JsonObject->GetNumberField(TEXT("idle_motion_fps"));
			Response.LivelinkHost = JsonObject->GetStringField(TEXT("livelink_host"));
			Response.LivelinkPort = JsonObject->GetNumberField(TEXT("livelink_port"));
			Response.LivelinkSubject = JsonObject->GetStringField(TEXT("livelink_subject"));

			return true;
		}
	}

	return false;	
}

