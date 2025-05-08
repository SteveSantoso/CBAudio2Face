// Copyright 2025-Present Xiao Lan fei. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CBAudio2FaceTypes.generated.h"


//Audio2Face的设置
USTRUCT(BlueprintType)
struct CBAUDIO2FACE_API FAudio2FaceSettings
{
    GENERATED_BODY()
    
    // 设备ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic information")
    FString DeviceId;

    // A2E网络路径
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic information")
    FString A2ENetworkPath;

    // A2E网络名称
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic information")
    FString A2ENetworkName;

    // 网络根路径
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic information")
    FString NetworkRootPath;

    // 网络名称
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic information")
    FString NetworkName;

    // A2E自动生成
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic information")
    bool bA2EAutoGenerate;

    // A2E流式直播模式
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic information")
    bool bA2EStreamingLiveMode;

    // A2E强制设置键值
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic information")
    bool bA2EForceSetKeys;

    // 数据源镜头
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic information")
    FString SourceShot;

    // 关键帧帧率
    UPROPERTY(EditAnywhere, BlueprintReadWrite,meta=(ClampMin=1.0,ClampMax=1000.0), Category = "Parameter settings")
    double KeyframerFps;

    // 下半脸平滑度
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameter settings")
    double LowerFaceSmoothing;

    // 上半脸平滑度
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameter settings")
    double UpperFaceSmoothing;

    // 下半脸强度
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameter settings")
    double LowerFaceStrength;

    // 上半脸强度
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameter settings")
    double UpperFaceStrength;

    // 眨眼强度
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameter settings")
    double BlinkStrength;

    // 面具级别
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameter settings")
    double FaceMaskLevel;

    // 面具柔软度
    UPROPERTY(EditAnywhere, BlueprintReadWrite,meta=(ClampMin=0.001,ClampMax=0.5), Category = "Parameter settings")
    double FaceMaskSoftness;

    // 输入强度
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameter settings")
    double InputStrength;

    // 预测延迟
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameter settings")
    double PredictionDelay;

    // 皮肤强度
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ClampMin=0.0,ClampMax=2.0f),Category = "Parameter settings")
    double SkinStrength;

    // 下颚牙齿强度
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameter settings")
    double LowerTeethStrength;

    // 下颚牙齿高度偏移
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameter settings")
    double LowerTeethHeightOffset;

    // 下颚牙齿深度偏移
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameter settings")
    double LowerTeethDepthOffset;

    // 嘴唇张开偏移
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameter settings")
    double LipOpenOffset;

    // 舌头强度
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameter settings")
    double TongueStrength;

    // 舌头高度偏移
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameter settings")
    double TongueHeightOffset;

    // 舌头深度偏移
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameter settings")
    double TongueDepthOffset;

    // 眼球强度
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameter settings")
    double EyeballsStrength;

    // 瞬目强度
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameter settings")
    double SaccadeStrength;

    // 右眼旋转X偏移
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameter settings")
    double RightEyeRotXOffset;

    // 右眼旋转Y偏移
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameter settings")
    double RightEyeRotYOffset;

    // 左眼旋转X偏移
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameter settings")
    double LeftEyeRotXOffset;

    // 左眼旋转Y偏移
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameter settings")
    double LeftEyeRotYOffset;

    // 眼睑张开偏移
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameter settings")
    double EyelidOpenOffset;

    // 眨眼间隔
    UPROPERTY(EditAnywhere, BlueprintReadWrite,meta=(ClampMin=0.5,ClampMax=10.0), Category = "Parameter settings")
    double BlinkInterval;

    // 眼球瞬动种子
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameter settings")
    int32 EyeSaccadeSeed;

    // A2E窗口大小
    UPROPERTY(EditAnywhere, BlueprintReadWrite,meta=(ClampMin=0.06,ClampMax=2.0), Category = "Parameter settings")
    double A2EWindowSize;

    // A2E步长
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameter settings")
    double A2EStride;

    // A2E情绪强度
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameter settings")
    double A2EEmotionStrength;

    // A2E平滑核半径
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameter settings")
    int32 A2ESmoothingKernelRadius;

    // A2E平滑指数
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameter settings")
    double A2ESmoothingExp;

    // A2E最大情绪数
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ClampMin=1,ClampMax=6),Category = "Parameter settings")
    int32 A2EMaxEmotions;

    // A2E对比度
    UPROPERTY(EditAnywhere, BlueprintReadWrite,meta=(ClampMin=0.1,ClampMax=3.0), Category = "Parameter settings")
    double A2EContrast;

    // A2E流式更新周期
    UPROPERTY(EditAnywhere, BlueprintReadWrite,meta=(ClampMin=1,ClampMax=10), Category = "Parameter settings")
    int32 A2EStreamingUpdatePeriod;

    // A2E流式平滑度
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameter settings")
    double A2EStreamingSmoothing;

    // A2E流式过渡时间
    UPROPERTY(EditAnywhere, BlueprintReadWrite,meta=(ClampMin=0.1,ClampMax=1.0),Category = "Parameter settings")
    double A2EStreamingTransitionTime;

    // A2E是否设置首选情绪
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic information")
    bool bA2EPreferredEmotionIsSet;

    // A2E首选情绪强度
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameter settings")
    double A2EPreferredEmotionStrength;

    // 数据源帧数
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameter settings")
    double SourceFrame;

    // 情绪
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameter settings")
    TArray<float> Emotion;

    // A2E首选情绪
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameter settings")
    TArray<float> A2EPreferredEmotion;

    FAudio2FaceSettings()
        :bA2EAutoGenerate(false)
        ,bA2EStreamingLiveMode(false)
        ,bA2EForceSetKeys(false)
		,KeyframerFps(25.0)
		,LowerFaceSmoothing(0.0)
		,UpperFaceSmoothing(0.0)
		,LowerFaceStrength(0.0)
		,UpperFaceStrength(0.0)
		,BlinkStrength(0.0)
		,FaceMaskLevel(0.0)
		,FaceMaskSoftness(0.4)
		,InputStrength(0.0)
		,PredictionDelay(0.0)
		,SkinStrength(0.0)
		,LowerTeethStrength(0.0)
		,LowerTeethHeightOffset(0.0)
		,LowerTeethDepthOffset(0.0)
		,LipOpenOffset(0.0)
		,TongueStrength(0.0)
		,TongueHeightOffset(0.0)
		,TongueDepthOffset(0.0)
		,EyeballsStrength(0.0)
		,SaccadeStrength(0.0)
		,RightEyeRotXOffset(0.0)
		,RightEyeRotYOffset(0.0)
		,LeftEyeRotXOffset(0.0)
		,LeftEyeRotYOffset(0.0)
		,EyelidOpenOffset(0.0)
		,BlinkInterval(5.0)
		,EyeSaccadeSeed(0.0)
		,A2EWindowSize(1.0)
		,A2EStride(0.0)
		,A2EEmotionStrength(0.0)
		,A2ESmoothingKernelRadius(0)
		,A2ESmoothingExp(0.0)
		,A2EMaxEmotions(5.0)
		,A2EContrast(2.0)
		,A2EStreamingUpdatePeriod(5)
		,A2EStreamingSmoothing(0.0)
		,A2EStreamingTransitionTime(0.5)
		,bA2EPreferredEmotionIsSet(false)
		,A2EPreferredEmotionStrength(0.0)
		,SourceFrame(0.0)
    {
        
    }
};


USTRUCT(BlueprintType)
struct CBAUDIO2FACE_API FA2FEmotion
{
	GENERATED_BODY()
	
	//惊奇
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ClampMin=0.0f,ClampMax=1.0f),Category = "Emotion")
	float Amazement; 

	// 愤怒
	UPROPERTY(EditAnywhere, BlueprintReadWrite,meta=(ClampMin=0.0f,ClampMax=1.0f), Category = "Emotion")
	float Anger; 

	// 狡猾
	UPROPERTY(EditAnywhere, BlueprintReadWrite,meta=(ClampMin=0.0f,ClampMax=1.0f), Category = "Emotion")
	float Cheekiness; 

	// 厌恶
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ClampMin=0.0f,ClampMax=1.0f),Category = "Emotion")
	float Disgust; 

	// 害怕
	UPROPERTY(EditAnywhere, BlueprintReadWrite,meta=(ClampMin=0.0f,ClampMax=1.0f), Category = "Emotion")
	float Fear; 

	// 悲伤
	UPROPERTY(EditAnywhere, BlueprintReadWrite,meta=(ClampMin=0.0f,ClampMax=1.0f), Category = "Emotion")
	float Grief; 

	// 快乐
	UPROPERTY(EditAnywhere, BlueprintReadWrite,meta=(ClampMin=0.0f,ClampMax=1.0f), Category = "Emotion")
	float Joy;

	// 喘不过气来
	UPROPERTY(EditAnywhere, BlueprintReadWrite,meta=(ClampMin=0.0f,ClampMax=1.0f), Category = "Emotion")
	float OutOfBreath; 

	// 疼痛
	UPROPERTY(EditAnywhere, BlueprintReadWrite,meta=(ClampMin=0.0f,ClampMax=1.0f), Category = "Emotion")
	float Pain; 

	// 伤心
	UPROPERTY(EditAnywhere, BlueprintReadWrite,meta=(ClampMin=0.0f,ClampMax=1.0f), Category = "Emotion")
	float Sadness;

	FA2FEmotion()
		:Amazement(0.0f)
		,Anger(0.0f)
		,Cheekiness(0.0f)
		,Disgust(0.0f)
		,Fear(0.0f)
		,Grief(0.0f)
		,Joy(0.0f)
		,OutOfBreath(0.0f)
		,Pain(0.0f)
		,Sadness(0.0f)
	{
	}
};


USTRUCT(BlueprintType)
struct CBAUDIO2FACE_API FAudio2EmotionSettings
{
	GENERATED_BODY()

	// 流媒体平滑设置
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="A2E")
	float StreamingSmoothing;

	// 流媒体更新周期
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="A2E")
	int32 StreamingUpdatePeriod;

	// 流媒体过渡时间
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="A2E")
	float StreamingTransitionTime;

	// 窗口大小
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="A2E")
	float WindowSize;

	// 步长
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="A2E")
	int32 Stride;

	// 情感强度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="A2E")
	float EmotionStrength;

	//平滑内核半径
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "A2E")
	int32 SmoothingKernelRadius;

	//平滑Exp
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "A2E")
	int32 SmoothingExp;

	//最大情绪
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "A2E")
	int32 MaxEmotions;

	//对比
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "A2E")
	int32 Contrast;

	//首选情绪强度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "A2E")
	float PreferredEmotionStrength;

	FAudio2EmotionSettings()
		:StreamingSmoothing(0.7f)
		,StreamingUpdatePeriod(2)
		,StreamingTransitionTime(0.5f)
		,WindowSize(1.4f)
		,Stride(1)
		,EmotionStrength(0.6f)
		,SmoothingKernelRadius(0)
		,SmoothingExp(0)
		,MaxEmotions(6)
		,Contrast(1)
		,PreferredEmotionStrength(0.5f)
	{}
};

USTRUCT(BlueprintType)
struct CBAUDIO2FACE_API FA2FPlayerTrack
{
	GENERATED_BODY()

	//文件名
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerTrack")
	FString FileName;

	//选择播放最小时间
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerTrack")
	float TimeRangeMin;

	//选择播放最大时间，若为-1为根据文件时长，目前目前不管用
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerTrack")
	float TimeRangeMax;

	FA2FPlayerTrack()
		:FileName(TEXT(""))
		,TimeRangeMin(0.0f)
		,TimeRangeMax(-1.0)
	{
		
	}
};


//播放器的范围
USTRUCT(BlueprintType)
struct CBAUDIO2FACE_API FA2FPlayerRange
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Range|Default")
	float DefaultMin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Range|Default")
	float DefaultMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Range|Work")
	float WorkMin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Range|Work")
	float WorkMax;
	
	FA2FPlayerRange()
		:DefaultMin(INDEX_NONE)
		,DefaultMax(INDEX_NONE)
		,WorkMin(INDEX_NONE)
		,WorkMax(INDEX_NONE)
	{}
		
};

USTRUCT(BlueprintType)
struct CBAUDIO2FACE_API FStreamLivelinkSettings
{
	GENERATED_BODY()

	// 音频端口
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	int32 AudioPort;

	// 启用音频流
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	bool bEnableAudioStream;

	// 启用凝视
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	bool bEnableGaze;

	// 启用空闲表情
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	bool bEnableIdleFace;

	// 启用空闲头部动作
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	bool bEnableIdleHead;

	// 空闲表情乘数
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	float IdleFaceMultiplier;

	// 空闲头部动作乘数
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	float IdleHeadMultiplier;

	// 空闲动作帧率
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	int32 IdleMotionFps;

	// LiveLink 主机
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	FString LivelinkHost;

	// LiveLink 端口
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	int32 LivelinkPort;

	// LiveLink 主题
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	FString LivelinkSubject;

	FStreamLivelinkSettings()
		: AudioPort(12031)
		,bEnableAudioStream(false)
		,bEnableGaze(true)
		,bEnableIdleFace(false)
		,bEnableIdleHead(true)
		,IdleFaceMultiplier(1.0f)
		,IdleHeadMultiplier(1.0f)
		,IdleMotionFps(30)
		,LivelinkHost(TEXT("localhost"))
		,LivelinkPort(12030)
		,LivelinkSubject(TEXT("Audio2Face"))
		{}
};

//导出BS参数
USTRUCT(BlueprintType)
struct CBAUDIO2FACE_API FA2FBlendshapeParam
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlendshapeSolveParam")
	FString ExportDirectory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlendshapeSolveParam")
	FString Filename;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlendshapeSolveParam")
	FString Format;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlendshapeSolveParam")
	bool bBatch;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlendshapeSolveParam")
	int32 FPS;

	FA2FBlendshapeParam()
		:Format(TEXT("json"))
		,bBatch(false)
		,FPS(25)
	{}
};