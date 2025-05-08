// Copyright 2025-Present Xiao Lan fei. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CBAudio2FaceTypes.h"
#include "CBHTTPNodes.h"
#include "CBAudio2FaceNodes.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnA2FStatusRequestEvent);
DECLARE_DELEGATE(FOnA2FStatusRequestEventC)
UCLASS(meta = (NotPlaceable,ExposedAsyncProxy = RequestTask),Category=A2FRequest)
class CBAUDIO2FACE_API USendA2FRequestProxyBase : public USendHttpRequestProxyBase
{
	GENERATED_BODY()
public:
	/*从服务器发送成功响应时调用*/
	UPROPERTY(BlueprintAssignable)
	FOnA2FStatusRequestEvent OnResponse;

	/*发生错误时调用*/
	UPROPERTY(BlueprintAssignable)
	FOnA2FStatusRequestEvent OnError;

public:
	FOnA2FStatusRequestEventC OnResponseC;
	FOnA2FStatusRequestEventC OnErrorC;
protected:
	virtual void OnErrorInternal(UCBHTTPResponse* const Response) override;
	virtual void OnSuccessInternal(UCBHTTPResponse* const Response) override;
};


UCLASS(meta = (NotPlaceable,ExposedAsyncProxy = RequestTask),Category=A2FRequest)
class CBAUDIO2FACE_API USendA2FPlayerProxy final : public USendHttpRequestProxyBase
{
	GENERATED_BODY()

public:
	/*从服务器发送成功响应时调用*/
	UPROPERTY(BlueprintAssignable)
	FOnA2FStatusRequestEvent OnResponse;

	/*发生错误时调用*/
	UPROPERTY(BlueprintAssignable)
	FOnA2FStatusRequestEvent OnError;

public:
	FOnA2FStatusRequestEventC OnResponseC;
	FOnA2FStatusRequestEventC OnErrorC;
protected:
	virtual void OnErrorInternal(UCBHTTPResponse* const Response) override;
	virtual void OnSuccessInternal(UCBHTTPResponse* const Response) override;

public:
	/**
	* 播放音频
	* @param PlayerInstance PlayerInstance实例
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"),DisplayName="A2F Player Play",  Category =A2FRequest)
	static USendA2FPlayerProxy* SendHttpA2FPlayerPlayRequest(const FString& PlayerInstance);

};
UCLASS(meta = (NotPlaceable,ExposedAsyncProxy = RequestTask),Category=A2FRequest)
class CBAUDIO2FACE_API USendA2FRequestProxy final : public USendA2FRequestProxyBase
{
	GENERATED_BODY()
	
public:

	/**获取audio2face状态*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"),DisplayName="Get A2F Status ",  Category =A2FRequest)
	static USendA2FRequestProxy* SendHttp2FStatusRequest();

	/**设置USD file*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"),DisplayName="Set A2F USD ",  Category =A2FRequest)
	static USendA2FRequestProxy* SendHttp2FSetUSDRequest(const FString& USDFileName);

	/**
	 * 设置对应的instance
	 * @param Instance instce实例
	 * @param InSettings 设置项
	 */
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"),DisplayName="Set A2F Settings",  Category =A2FRequest)
	static USendA2FRequestProxy* SendHttp2FSetSettingsRequest(const FString& Instance,const FAudio2FaceSettings& InSettings);

	/**
	 * 设置对应的instance
	 * @param Instance instce实例
	 * @param InSettings 设置项
	 */
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"),DisplayName="Set A2F Settings ByKeys",  Category =A2FRequest)
	static USendA2FRequestProxy* SendHttp2FSetSettingsRequestByKeys(const FString& Instance,const TMap<FString,float>& InSettings);
	/**
	 * 设置instance的帧数
	 * @param Instance instance名
	 * @param Instance 选择的instance
	 * @param bAsTimestamp 是否作为时间戳,若false为帧数
	 */
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"),DisplayName="Set A2F Instance Frame",  Category =A2FRequest)
	static USendA2FRequestProxy* SendHttpA2FSetFrameRequest(const FString& Instance,int32 Frame=25, bool bAsTimestamp=false);

	/**
	 * 设置表情
	 * @param Instance instance实例
	 * @param InEmotion 表情参数
	 */
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"),DisplayName="Set A2F Emotion Weights By Frame",  Category =A2FRequest)
	static USendA2FRequestProxy* SendHttpA2FSetEmotionWeightsByFrameRequest(const FString& Instance,const FA2FEmotion& InEmotion);

	/**
	* 通过指定名设置表情
	* @param Instance instance实例
	 * @param InEmotion 表情参数 ,请在amazement，anger，cheekiness，disgust，fear，grief，joy，outofbreath，pain，sadness其中一个或者多个作为TMap的key
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"),DisplayName="Set A2F Emotion Weights By Name",  Category =A2FRequest)
	static USendA2FRequestProxy* SendHttpA2FSetEmotionWeightsByNameRequest(const FString& Instance,const TMap<FString,float>& InEmotion);

	/**
	* 设置流送
	* @param Instance instance实例
	* @param bEnable 是否可以流送
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"),DisplayName="Set A2F Enable Streaming",  Category =A2FRequest)
	static USendA2FRequestProxy* SendHttpA2FSetEnableStreamingRequest(const FString& Instance,bool bEnable);

	/**
	* 设置emotion
	* @param Instance instance实例
	* @param Settings 设置项
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"),DisplayName="Set A2F Emotion Settings",  Category =A2FRequest)
	static USendA2FRequestProxy* SendHttpA2FSetEmotionSettingsRequest(const FString& Instance,const FAudio2EmotionSettings& Settings);

	
	/**
	* 设置player的根路径
	* @param PlayerInstance instance实例
	* @param DirPath root路径以/为分割符
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"),DisplayName="Set A2F Player Root Path",  Category =A2FRequest)
	static USendA2FRequestProxy* SendHttpA2FSetPlayerRootPathRequest(const FString& PlayerInstance,const FString& DirPath);

	/**
	* 设置player需要播放的音频
	* @param PlayerInstance PlayerInstance实例
	* @param PlayerTrack 音频文件参数
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"),DisplayName="Set A2F Player Track",  Category =A2FRequest)
	static USendA2FRequestProxy* SendHttpA2FSetPlayerTrackRequest(const FString& PlayerInstance,const FA2FPlayerTrack& PlayerTrack);

	/**
	* 设置player需要播放的音频
	* @param PlayerInstance PlayerInstance实例
	* @param Start 起始时间
	* @param End 结束时间。-1则为默认完整音频时间
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"),DisplayName="Set A2F Player Range",  Category =A2FRequest)
	static USendA2FRequestProxy* SendHttpA2FSetPlayerRangeRequest(const FString& PlayerInstance,float Start,float End=-1.0f);

	/**
	* 设置player当前播放位置
	* @param PlayerInstance PlayerInstance实例
	* @param Time 播放位置,若大于整体播放时间，则按照最后播放点
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"),DisplayName="Set A2F Player Current Time",  Category =A2FRequest)
	static USendA2FRequestProxy* SendHttpA2FSetPlayerCurrentTimeRequest(const FString& PlayerInstance,float Time);


	/**
	* 暂停音频
	* @param PlayerInstance PlayerInstance实例
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"),DisplayName="A2F Player Pause",  Category =A2FRequest)
	static USendA2FRequestProxy* SendHttpA2FPlayerPauseRequest(const FString& PlayerInstance);

	/**
	* 倒带音频
	* @param PlayerInstance PlayerInstance实例
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"),DisplayName="A2F Player Rewind",  Category =A2FRequest)
	static USendA2FRequestProxy* SendHttpA2FPlayerRewindRequest(const FString& PlayerInstance);

	/**
	* 设置player循环
	* @param PlayerInstance PlayerInstance实例
	* @param bLoop 是否循环
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"),DisplayName="Set A2F Player Looping",  Category =A2FRequest)
	static USendA2FRequestProxy* SendHttpA2FSetPlayerLoopingRequest(const FString& PlayerInstance,bool bLoop);

	/**
	* 激活流式
	* @param NodePath NodePath 实例
	* @param bActive 是否激活
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"),DisplayName="A2F Activate Stream Livelink",  Category =A2FRequest)
	static USendA2FRequestProxy* SendHttpA2FActivateStreamLivelinkRequest(const FString& NodePath,bool bActive);
		
	/**
	* 设置StreamLivelinkSettings
	* @param NodePath streamlive路径
	* @param Settings 设置参数
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"),DisplayName="Set A2F Stream Livelink Settings",  Category =A2FRequest)
	static USendA2FRequestProxy* SendHttpA2FStreamLivelinkSettingsRequest(const FString& NodePath,const FStreamLivelinkSettings& Settings);

	/**
	* 导出blendshape
	* @param NodePath streamlive路径
	* @param Settings 设置参数
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "false"),DisplayName=" A2F Export Blendshapes",  Category =A2FRequest)
	static USendA2FRequestProxy* SendHttpA2FExportBlendshapesRequest(const FString& SolverNode,const FA2FBlendshapeParam& Settings);

	/**
	* 设置流送
	* @param Instance instance实例
	* @param bEnable 是否可以流送
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"),DisplayName="Set A2F Enable Auto Generate On Track Change",  Category =A2FRequest)
	static USendA2FRequestProxy* SendHttpA2FEnableAutoGenerateOnTrackChangeRequest(const FString& Instance,bool bEnable);


private:
	bool Audio2FaceSettingsToJsonString(const FString& Instance, const FAudio2FaceSettings& InSettings, FString& OutJson);
	bool Audio2FaceSettingsToJsonString(const FString& Instance, const TMap<FString,float>& InSettings, FString& OutJson);
	bool Audio2FaceSetEmotionToJsonString(const FString& Instance, const FA2FEmotion& InEmotion, FString& OutJson);
	bool Audio2FaceSetEmotionNameToJsonString(const FString& Instance, const TMap<FString,float>& InEmotion, FString& OutJson);
	bool Audio2FaceSetEmotionSettingToJsonString(const FString& Instance, const FAudio2EmotionSettings& InSetting, FString& OutJson);
	bool Audio2FaceSetPlayerTrackToJsonString(const FString& PlayerInstance, const FA2FPlayerTrack& InPlayerTrack, FString& OutJson);
	bool Audio2FaceStreamLivelinkSettingsToJsonString(const FString& InLivelinkNode, const FStreamLivelinkSettings& InSettings, FString& OutJson);
	bool Audio2FaceExportBlendshapesToJsonString(const FString& InBlendshapeNode, const FA2FBlendshapeParam& Blendshape, FString& OutJson);
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnA2FRequestGetarrayEvent,const TArray<FString>&,Array);
DECLARE_DELEGATE_OneParam(FOnA2FRequestGetarrayEventC,const TArray<FString>&);

UCLASS(meta = (NotPlaceable,ExposedAsyncProxy = RequestTask))
class CBAUDIO2FACE_API USendA2FRequestGetArrayProxy : public USendHttpRequestProxyBase
{
	GENERATED_BODY()
public:
	/*从服务器发送成功响应时调用*/
	UPROPERTY(BlueprintAssignable)
	FOnA2FRequestGetarrayEvent OnResponse;

	/*发生错误时调用*/
	UPROPERTY(BlueprintAssignable)
	FOnA2FRequestGetarrayEvent OnError;
	
	/**获取instance数组*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"),DisplayName="Get A2F Instance",  Category =A2FRequest)
	static USendA2FRequestGetArrayProxy* SendHttpA2FGetInstanceRequest();

	/**获取Settings
	* @param Instance instance名
	 */
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"),DisplayName="Get A2F Setting Names",  Category =A2FRequest)
	static USendA2FRequestGetArrayProxy* SendHttpA2FGetSettingNamesRequest(const FString& Instance);

	/**
	 * 获取Emotion名字
	 */
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"),DisplayName="Get A2F Emotion Names",  Category =A2FRequest)
	static USendA2FRequestGetArrayProxy* SendHttpA2FGetEmotionNamesRequest();
	
	/**
	* 获取tracks
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"),DisplayName="Get A2F Player Tracks",  Category =A2FRequest)
	static USendA2FRequestGetArrayProxy* SendHttpA2FGetPlayerTracksRequest(const FString& PlayerInstance);

	/**
	* 获取blendshape solves
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"),DisplayName="Get A2F Blend Shape solves",  Category =A2FRequest)
	static USendA2FRequestGetArrayProxy* SendHttpA2FGetBlendShapesolvesRequest();

	/**
	* 获取Livelink Nodes
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"),DisplayName="Get A2F Livelink Nodes",  Category =A2FRequest)
	static USendA2FRequestGetArrayProxy* SendHttpA2FGetStreamLivelinkNodesRequest();

public:
	FOnA2FRequestGetarrayEventC OnResponseC;
	FOnA2FRequestGetarrayEventC OnErrorC;
protected:
	virtual void OnErrorInternal(UCBHTTPResponse* const Response) override;
	virtual void OnSuccessInternal(UCBHTTPResponse* const Response) override;

private:
	bool GetResponseInstance( TArray<FString>& Instance,const FString& Response);
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnA2FRequestGetTimeEvent,const float,Num);
DECLARE_DELEGATE_OneParam(FOnA2FRequestGetTimeEventC,const float);
UCLASS(meta = (NotPlaceable,ExposedAsyncProxy = RequestTask))
class CBAUDIO2FACE_API USendA2FRequestGetTimeProxy : public USendHttpRequestProxyBase
{
	GENERATED_BODY()
public:
	/*从服务器发送成功响应时调用*/
	UPROPERTY(BlueprintAssignable)
	FOnA2FRequestGetTimeEvent OnResponse;

	/*发生错误时调用*/
	UPROPERTY(BlueprintAssignable)
	FOnA2FRequestGetTimeEvent OnError;
	/**
	* 获取playerinstance当前播放时间
	* @param PlayerInstance 选择的instance
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"),DisplayName="Get A2F Player Time",  Category =A2FRequest)
	static USendA2FRequestGetTimeProxy* SendHttpA2FGetPlayerTimeRequest(const FString& PlayerInstance);


public:
	FOnA2FRequestGetTimeEventC OnResponseC;
	FOnA2FRequestGetTimeEventC OnErrorC;
protected:
	virtual void OnErrorInternal(UCBHTTPResponse* const Response) override;
	virtual void OnSuccessInternal(UCBHTTPResponse* const Response) override;

private:
	bool GetResponse(float& Result , const FString& Response);
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnA2FRequestGetNumEvent,const float,Num);
DECLARE_DELEGATE_OneParam(FOnA2FRequestGetNumEventC,const float);
UCLASS(meta = (NotPlaceable,ExposedAsyncProxy = RequestTask))
class CBAUDIO2FACE_API USendA2FRequestGetNumProxy : public USendHttpRequestProxyBase
{
	GENERATED_BODY()
public:
	/*从服务器发送成功响应时调用*/
	UPROPERTY(BlueprintAssignable)
	FOnA2FRequestGetNumEvent OnResponse;

	/*发生错误时调用*/
	UPROPERTY(BlueprintAssignable)
	FOnA2FRequestGetNumEvent OnError;

	/**
	 * 获取instance的frame
	 * @param Instance 选择的instance
	 * @param bAsTimestamp 是否作为时间戳,若false为帧数
	 */
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"),DisplayName="Get A2F Instance Frame",  Category =A2FRequest)
	static USendA2FRequestGetNumProxy* SendHttpA2FGetFrameRequest(const FString& Instance, bool bAsTimestamp =false);

public:
	FOnA2FRequestGetNumEventC OnResponseC;
	FOnA2FRequestGetNumEventC OnErrorC;
protected:
	virtual auto OnErrorInternal(UCBHTTPResponse* const Response) -> void override;
	virtual void OnSuccessInternal(UCBHTTPResponse* const Response) override;

private:
	bool GetFrameResponse(int32& Result , const FString& Response);
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnA2FRequestEmotionWeightsEvent,FA2FEmotion, OutEmotion);
DECLARE_DELEGATE_OneParam(FOnA2FRequestEmotionWeightsEventC,FA2FEmotion);

UCLASS(meta = (NotPlaceable,ExposedAsyncProxy = RequestTask))
class CBAUDIO2FACE_API USendA2FRequestEmotionWeightsProxy : public USendHttpRequestProxyBase
{
	GENERATED_BODY()
public:
	/*从服务器发送成功响应时调用*/
	UPROPERTY(BlueprintAssignable)
	FOnA2FRequestEmotionWeightsEvent OnResponse;

	/*发生错误时调用*/
	UPROPERTY(BlueprintAssignable)
	FOnA2FRequestEmotionWeightsEvent OnError;

	/**
	* 设置instance的帧数,注意：bAsVector和bAsTimestamp有bug，请先用false
	* @param Instance instance名
	* @param Frame 帧率
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"),DisplayName="Get A2F Emotion Weights By Frame",  Category =A2FRequest)
	static USendA2FRequestEmotionWeightsProxy* SendHttpA2FGetEmotionWeightsByFrameRequest(const FString& Instance,int32 Frame,bool bAsVector,bool bAsTimestamp);

private:
	FOnA2FRequestEmotionWeightsEventC OnResponseC;
	FOnA2FRequestEmotionWeightsEventC OnErrorC;
	
protected:
	virtual void OnErrorInternal(UCBHTTPResponse* const Response) override;
	virtual void OnSuccessInternal(UCBHTTPResponse* const Response) override;

private:
	bool GetResponse(FString& Result ,FA2FEmotion& Response);
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnA2FRequestEmotionSettingsEvent,FAudio2EmotionSettings, OutEmotion);
DECLARE_DELEGATE_OneParam(FOnA2FRequestEmotionSettingsEventC,FAudio2EmotionSettings);
UCLASS(meta = (NotPlaceable,ExposedAsyncProxy = RequestTask))
class CBAUDIO2FACE_API USendA2FRequestEmotionSettingsProxy : public USendHttpRequestProxyBase
{
	GENERATED_BODY()
public:
	/*从服务器发送成功响应时调用*/
	UPROPERTY(BlueprintAssignable)
	FOnA2FRequestEmotionSettingsEvent OnResponse;

	/*发生错误时调用*/
	UPROPERTY(BlueprintAssignable)
	FOnA2FRequestEmotionSettingsEvent OnError;

	/**
	* 获取Emotion设置
	* @param Instance instance名
	* @param Settings Emotion设置
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"),DisplayName="Get A2F Emotion Settings",  Category =A2FRequest)
	static USendA2FRequestEmotionSettingsProxy* SendHttpA2FGetEmotionSettingsRequest(const FString& Instance);

private:
	FOnA2FRequestEmotionSettingsEventC OnResponseC;
	FOnA2FRequestEmotionSettingsEventC OnErrorC;
protected:
	virtual void OnErrorInternal(UCBHTTPResponse* const Response) override;
	virtual void OnSuccessInternal(UCBHTTPResponse* const Response) override;

private:
	bool GetResponse(FString& Result ,FAudio2EmotionSettings& Response);
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnA2FRequestPlayerInstanceEvent,const TArray<FString>&, Regular,const TArray<FString>&, Streaming);
DECLARE_DELEGATE_TwoParams(FOnA2FRequestPlayerInstanceEventC,const TArray<FString>&,const TArray<FString>&);
UCLASS(meta = (NotPlaceable,ExposedAsyncProxy = RequestTask))
class CBAUDIO2FACE_API USendA2FRequestPlayerInstanceProxy : public USendHttpRequestProxyBase
{
	GENERATED_BODY()
public:
	/*从服务器发送成功响应时调用*/
	UPROPERTY(BlueprintAssignable)
	FOnA2FRequestPlayerInstanceEvent OnResponse;

	/*发生错误时调用*/
	UPROPERTY(BlueprintAssignable)
	FOnA2FRequestPlayerInstanceEvent OnError;

	/**
	 * 获取播放实例
	 */
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"),DisplayName="Get A2F Player Instance",  Category =A2FRequest)
	static USendA2FRequestPlayerInstanceProxy* SendHttpA2FGetPlayerInstanceRequest();

public:
	FOnA2FRequestPlayerInstanceEventC OnResponseC;
	FOnA2FRequestPlayerInstanceEventC OnErrorC;
protected:
	virtual void OnErrorInternal(UCBHTTPResponse* const Response) override;
	virtual void OnSuccessInternal(UCBHTTPResponse* const Response) override;

private:
	void GetResponse(FString& Result , TArray<FString>& OutRegularResponse,TArray<FString>& OutStreamingResponse);
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnA2FRequestGetStringEvent,const FString&, Regular);
DECLARE_DELEGATE_OneParam(FOnA2FRequestGetStringEventC,const FString&);
UCLASS(meta = (NotPlaceable,ExposedAsyncProxy = RequestTask))
class CBAUDIO2FACE_API USendA2FRequestGetStringProxy : public USendHttpRequestProxyBase
{
	GENERATED_BODY()
public:
	/*从服务器发送成功响应时调用*/
	UPROPERTY(BlueprintAssignable)
	FOnA2FRequestGetStringEvent OnResponse;

	/*发生错误时调用*/
	UPROPERTY(BlueprintAssignable)
	FOnA2FRequestGetStringEvent OnError;

	/**
	 * 获取player的根路径
	 * @param PlayerInstance player的实例
	 */
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"),DisplayName="Get A2F Player Root Path",  Category =A2FRequest)
	static USendA2FRequestGetStringProxy* SendHttpA2FGetPlayerRootPathRequest(const FString& PlayerInstance);

	/**
	* 获取player的track
	* @param PlayerInstance player的实例
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"),DisplayName="Get A2F Player Current Track",  Category =A2FRequest)
	static USendA2FRequestGetStringProxy* SendHttpA2FGetPlayerCurrentTrackRequest(const FString& PlayerInstance);

public:
	FOnA2FRequestGetStringEventC OnResponseC;
	FOnA2FRequestGetStringEventC OnErrorC;
protected:
	virtual void OnErrorInternal(UCBHTTPResponse* const Response) override;
	virtual void OnSuccessInternal(UCBHTTPResponse* const Response) override;

private:
	void GetResponse(FString& Result , FString& Response);
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnA2FRequestPlayerRangeEvent,const FA2FPlayerRange&, PlayerRange);
DECLARE_DELEGATE_OneParam(FOnA2FRequestPlayerRangeEventC,const FA2FPlayerRange&);
UCLASS(meta = (NotPlaceable,ExposedAsyncProxy = RequestTask))
class CBAUDIO2FACE_API USendA2FRequestPlayerRangeProxy : public USendHttpRequestProxyBase
{
	GENERATED_BODY()
public:
	/*从服务器发送成功响应时调用*/
	UPROPERTY(BlueprintAssignable)
	FOnA2FRequestPlayerRangeEvent OnResponse;

	/*发生错误时调用*/
	UPROPERTY(BlueprintAssignable)
	FOnA2FRequestPlayerRangeEvent OnError;

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"),DisplayName="Get A2F Player Range",  Category =A2FRequest)
	static USendA2FRequestPlayerRangeProxy* SendHttpA2FGetPlayerIRangeRequest(const FString& PlayerInstance);

public:
	FOnA2FRequestPlayerRangeEventC OnResponseC;
	FOnA2FRequestPlayerRangeEventC OnErrorC;
protected:
	virtual void OnErrorInternal(UCBHTTPResponse* const Response) override;
	virtual void OnSuccessInternal(UCBHTTPResponse* const Response) override;

private:
	void GetResponse(FString& Result , FA2FPlayerRange& PlayerRange);
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnA2FRequestStreamLivelinkNodeConnectedEvent,bool, bConnected);
DECLARE_DELEGATE_OneParam(FOnA2FRequestStreamLivelinkNodeConnectedEventC,bool);
UCLASS(meta = (NotPlaceable,ExposedAsyncProxy = RequestTask))
class CBAUDIO2FACE_API USendA2FRequestLivelinkNodeConnectedProxy : public USendHttpRequestProxyBase
{
	GENERATED_BODY()
public:
	/*从服务器发送成功响应时调用*/
	UPROPERTY(BlueprintAssignable)
	FOnA2FRequestStreamLivelinkNodeConnectedEvent OnResponse;

	/*发生错误时调用*/
	UPROPERTY(BlueprintAssignable)
	FOnA2FRequestStreamLivelinkNodeConnectedEvent OnError;

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"),DisplayName="A2F Livelink Node Connected",  Category =A2FRequest)
	static USendA2FRequestLivelinkNodeConnectedProxy* SendHttpA2FLivelinkNodeConnectedRequest(const FString& NodePath);

public:
	FOnA2FRequestStreamLivelinkNodeConnectedEventC OnResponseC;
	FOnA2FRequestStreamLivelinkNodeConnectedEventC OnErrorC;
protected:
	virtual void OnErrorInternal(UCBHTTPResponse* const Response) override;
	virtual void OnSuccessInternal(UCBHTTPResponse* const Response) override;

private:
	bool JsonStringToResultBool(const FString& InString, bool& OutResult);
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnA2FRequestStreamLivelinkSettingsEvent,FStreamLivelinkSettings, StreamLivelinkSettings);
DECLARE_DELEGATE_OneParam(FOnA2FRequestStreamLivelinkSettingsEventC,FStreamLivelinkSettings);
UCLASS(meta = (NotPlaceable,ExposedAsyncProxy = RequestTask))
class CBAUDIO2FACE_API USendA2FRequestStreamLivelinkSettingsProxy : public USendHttpRequestProxyBase
{
	GENERATED_BODY()
public:
	/*从服务器发送成功响应时调用*/
	UPROPERTY(BlueprintAssignable)
	FOnA2FRequestStreamLivelinkSettingsEvent OnResponse;

	/*发生错误时调用*/
	UPROPERTY(BlueprintAssignable)
	FOnA2FRequestStreamLivelinkSettingsEvent OnError;

	/**
	* 获取StreamLivelink设置
	* @param NodePath StreamLivelink Node路径
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"),DisplayName="Get A2F StreamLivelink Settings",  Category =A2FRequest)
	static USendA2FRequestStreamLivelinkSettingsProxy* SendHttpA2FGetStreamLivelinkSettingsRequest(const FString& NodePath);

public:
	FOnA2FRequestStreamLivelinkSettingsEventC OnResponseC;
	FOnA2FRequestStreamLivelinkSettingsEventC OnErrorC;
protected:
	virtual void OnErrorInternal(UCBHTTPResponse* const Response) override;
	virtual void OnSuccessInternal(UCBHTTPResponse* const Response) override;

private:
	bool GetResponse(FString& Result ,FStreamLivelinkSettings& Response);
};