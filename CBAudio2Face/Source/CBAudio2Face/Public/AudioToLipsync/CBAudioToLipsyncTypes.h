// Copyright 2025-Present Xiao Lan fei. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CBAudioToLipsyncTypes.generated.h"

USTRUCT(BlueprintType)
struct CBAUDIO2FACE_API FAudio2FaceBlendShapeWeightMat
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weight")
	TArray<double> Weight;
};

USTRUCT(BlueprintType)
struct CBAUDIO2FACE_API FAudio2FaceBlendShapeInfo
{
	GENERATED_BODY()
	
	//导出的FPS
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio2FaceBlendShape")
	int32 ExportFps;

	//路径
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio2FaceBlendShape")
	FString TrackPath;

	//多少姿势
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio2FaceBlendShape")
	int32 NumPoses;

	//总帧数
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio2FaceBlendShape")
	int32 NumFrames;

	//受影响的表情
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio2FaceBlendShape")
	TArray<FString> FacsNames;

	//对于的权重
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio2FaceBlendShape")
	TArray<FAudio2FaceBlendShapeWeightMat> WeightMat;

	FAudio2FaceBlendShapeInfo()
	:ExportFps(INDEX_NONE)
	,NumPoses(INDEX_NONE)
	,NumFrames(INDEX_NONE)
	{
		
	}
};


DECLARE_DYNAMIC_DELEGATE_ThreeParams(FAudioToLipsyncBPDelegate,UAnimSequence*, InAnimSequence,int32 ,Index, const FString&, InMsg);
DECLARE_DYNAMIC_DELEGATE_ThreeParams(FAudioToLipsyncLiveLinkStreamBPDelegate,int32,InMaindex, int32, Index, const FString&, InMsg);
DECLARE_DELEGATE_ThreeParams(FAudioToLipsyncDelegate, UAnimSequence*,int32,const FString&)

USTRUCT(BlueprintType)
struct CBAUDIO2FACE_API FATLAnimInfo
{
	GENERATED_BODY()
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ATLAnimInfo")
	USkeleton* Skeleton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ATLAnimInfo")
	UPoseAsset* PoseAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ATLAnimInfo")
	TEnumAsByte<ERichCurveInterpMode> InterpMode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "Audio2Face")
	double Amplitude;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio2Face")
	bool bStream;

	//是否是编辑器内通过鼠标右键生成
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "Audio2Face")
	bool bQueueGeneratedByStream;

	//用于返回后在池子内定位音频
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ATLAnimInfo")
	int32 Index;

	//这个字段主要是队列延迟准备
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ATLAnimInfo")
	float DelayTime;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ATLAnimInfo|Delegate")
	FAudioToLipsyncBPDelegate OnAudioToLipsyncCompleteBPDelegate;

	//流式livelink 结束后通知这个接口
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ATLAnimInfo|Delegate")
	FAudioToLipsyncLiveLinkStreamBPDelegate OnLivLinkStreamEndPlayAudioToLipsyncDelegate;

	UPROPERTY()
	UObject* Parent;

	UPROPERTY()
	FString Name;

	FAudioToLipsyncDelegate OnAudioToLipsyncCompleteDelegate;

public:
	FATLAnimInfo()
		:Skeleton(nullptr)
		,PoseAsset(nullptr)
		,InterpMode(ERichCurveInterpMode::RCIM_Linear)
		,Amplitude(50.0f)
		,bStream(false)
		,bQueueGeneratedByStream(false)
		,Index(INDEX_NONE)
		,DelayTime(0.0f)
		,Parent(nullptr)
	{}
		
};

//主要用作眼睛或者脖子的动作
USTRUCT(BlueprintType)
struct CBAUDIO2FACE_API FATLBones
{
	GENERATED_BODY()

	//骨骼名字
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Frames")
	FString BoneName;

	//对应值
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Frames")
	FVector BoneValue;

	FATLBones()
		:BoneValue(FVector())
	{
	}
};

USTRUCT(BlueprintType)
struct CBAUDIO2FACE_API FFaceFrames
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Frames")
	TArray<double> Blendshapes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Frames")
	TArray<FATLBones> Bones;
};

USTRUCT(BlueprintType)
struct CBAUDIO2FACE_API FATLInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ATLInfo")
	int32 FPS;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ATLInfo")
	TArray<FFaceFrames> Frames;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ATLInfo")
	TArray<FString> BlendShapesNames;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ATLInfo")
	TArray<FString> BonesNames;

public:
	FATLInfo()
		:FPS(INDEX_NONE)
	{}
};