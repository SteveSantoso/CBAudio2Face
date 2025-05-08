// Copyright 2025-Present Xiao Lan fei. All Rights Reserved.


#include "AudioToLipsync/CBAudioToLipsyncFunctionLibrary.h"


#include "CBAudio2Face.h"
#include "AudioToLipsync/CBAudioToLipsyncTypes.h"
#include "AudioToLipsync/CBAudioToLipsyncSequence.h"
#include "AudioToLipsync/CBHanleAnimation.h"

#if WITH_EDITOR
#include "AssetToolsModule.h"
#endif

UAnimSequence* UCBAudioToLipsyncFunctionLibrary::Audio2FaceBlendShapeToAnimaiton(const FString& InJson,
                                                                                 const FATLAnimInfo& InATLAnimInfo, double Amplitude)
{
	
	FAudio2FaceBlendShapeInfo Audio2FaceBlendShapeInfo;
	GetAudio2FaceBlendShapeInfo(InJson, Audio2FaceBlendShapeInfo);

	FATLInfo TLInfo;
	Audio2FaceBlendShapeToATLRInfo(Audio2FaceBlendShapeInfo,TLInfo);
	
	return ATLAnimInfoToAnimaiton( TLInfo, InATLAnimInfo);
}

UAnimSequence* UCBAudioToLipsyncFunctionLibrary::ATLAnimInfoToAnimaiton( const FATLInfo& ATLInfo,const FATLAnimInfo &InATLAnimInfo)
{
	UCBAudioToLipsyncSequence* AnimSequence = nullptr;
	
	if (IsValid(InATLAnimInfo.Skeleton))
	{
		TMap<int32, CBHanleAnimation::FAffectedPoseCurve> InAffectedPose;
			if (!ResolveToAffectedPoseCurves(InATLAnimInfo.PoseAsset, ATLInfo.BlendShapesNames, InAffectedPose))
			{
				if (!InATLAnimInfo.PoseAsset)
				{
					UE_LOG(LogCBAudioToLipsync,
						Error,
						TEXT("PoseAsset needs to be loaded. If it is already loaded but still empty,\
first open the PoseAsset resource in the editor to ensure it is loaded into memory, \
and then you can happily generate the mouth shape."));

					ensure(0);
				}

				return NULL;
			}

			TMap<FString, FString> BoneNamesMapping =
			{
				{TEXT("head01_bind"),TEXT("neck_01")},
				{TEXT("r_eye_bind"),TEXT("FACIAL_R_Eye")},
				{TEXT("l_eye_bind"),TEXT("FACIAL_L_Eye")}
			};

			//每帧需要的时间是多少
			double FramesPerSecondStep = 1.f / ATLInfo.FPS;

			FRawCurveTracks RawCurveData;
			TMap<FName, FRawAnimSequenceTrack> BoneTracks;

			TArray<FName> CurveMetaDataNames;
			InATLAnimInfo.Skeleton->GetCurveMetaDataNames(CurveMetaDataNames);

		
			for (int32 i = 0; i <ATLInfo.Frames.Num(); i++)
			{
				//计算口型权重
				TMap<FName, float> OutCWBS;
				ResolveToCurveWeightedBlendShape(
					ATLInfo.Frames[i],
					CurveMetaDataNames,
					ATLInfo.BlendShapesNames,
					InAffectedPose, OutCWBS);

				//构建曲线
			CBHanleAnimation::BuildCurveTracks(
					OutCWBS,
					CurveMetaDataNames,
					FramesPerSecondStep,
					i,
					InATLAnimInfo.InterpMode,
					RawCurveData);

				//处理骨骼 用户可能开启了眼睛和脖子
				CBHanleAnimation::HandleBones(
					ATLInfo, 
					InATLAnimInfo,
					BoneNamesMapping,BoneTracks, 
					ATLInfo.Frames[i]);
			}

			//添加骨骼追踪 最终眼睛和脖子
		CBHanleAnimation::AddBoneTracks(BoneTracks,InATLAnimInfo);

			//合成阶段
			const float SequenceLength = FramesPerSecondStep * ATLInfo.Frames.Num();

			UPackage* AnimSequencePackage = Cast<UPackage>(InATLAnimInfo.Parent);
#if WITH_EDITOR
			// we have to check InParent is not valid as this will tell us whether we import asset via editor asset factory or not
			if (!IsValid(InATLAnimInfo.Parent))
			{
				FAssetToolsModule& AssetToolsModule = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools");

				FString DefaultSuffix;
				FString AssetName;
				FString PackageName;
				const FString BasePackageName(FString::Printf(TEXT("/MetahumanSDK/DebugAssets/ATL/AnimSequence"), *InATLAnimInfo.Name));
				AssetToolsModule.Get().CreateUniqueAssetName(BasePackageName, DefaultSuffix, /*out*/ PackageName, /*out*/ AssetName);

				// Create a unique package name and asset name
				AnimSequencePackage = CreatePackage(*PackageName);
				AnimSequencePackage->SetFlags(RF_Standalone | RF_Public);
				AnimSequencePackage->FullyLoad();
			}
#endif
			AnimSequence = NewObject<UCBAudioToLipsyncSequence>(
				IsValid(AnimSequencePackage) ? AnimSequencePackage : GetTransientPackage(),
				*InATLAnimInfo.Name,
				RF_Public | RF_Standalone);
			{
				AnimSequence->SetSkeleton(InATLAnimInfo.Skeleton);
			AnimSequence->SetSequenceLength(SequenceLength);
			
			}
#if WITH_EDITOR
			CBHanleAnimation::AnimationDataRegistrationOnEditor(AnimSequence, ATLInfo, ATLInfo.Frames.Num(), RawCurveData, BoneTracks);
#else
		//todo
			//AnimationDataRegistrationOnRunTime(AnimSequence, InResponse, SequenceLength, RawCurveData, InATLAnimInfo,BoneTracks);
#endif
	}

	return AnimSequence;
}

void UCBAudioToLipsyncFunctionLibrary::GetAudio2FaceBlendShapeInfo(const FString& InJson,
                                                                   FAudio2FaceBlendShapeInfo& BlendShapeInfo)
{
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(InJson);
	TSharedPtr<FJsonValue> ReadRoot;

	if (FJsonSerializer::Deserialize(JsonReader, ReadRoot))
	{
		if (TSharedPtr<FJsonObject> InJsonObject = ReadRoot->AsObject())
		{
			BlendShapeInfo.ExportFps = InJsonObject->GetIntegerField(TEXT("exportFps"));
			BlendShapeInfo.TrackPath = InJsonObject->GetStringField(TEXT("trackPath"));
			BlendShapeInfo.NumPoses = InJsonObject->GetIntegerField(TEXT("numPoses"));
			BlendShapeInfo.NumFrames = InJsonObject->GetIntegerField(TEXT("numFrames"));

			//解析facsNames
			if (InJsonObject->HasField(TEXT("facsNames")))
			{
				const TArray<TSharedPtr<FJsonValue>>& InBlendArray = InJsonObject->GetArrayField(TEXT("facsNames"));
				for (auto& SubTmp : InBlendArray)
				{
					BlendShapeInfo.FacsNames.Add(SubTmp->AsString());
				}
			}

			if (InJsonObject->HasField(TEXT("weightMat")))
			{
				const TArray<TSharedPtr<FJsonValue>>& InBlendArray = InJsonObject->GetArrayField(TEXT("weightMat"));
				for (auto& SubTmp : InBlendArray)
				{
					FAudio2FaceBlendShapeWeightMat &InWeight = BlendShapeInfo.WeightMat.AddDefaulted_GetRef();
					
					const TArray<TSharedPtr<FJsonValue>>& InSubBlendArray = SubTmp->AsArray();
					for (auto &Tmp : InSubBlendArray)
					{
						InWeight.Weight.Add(Tmp->AsNumber());
					}	
				}
			}
		}
	}
}

void UCBAudioToLipsyncFunctionLibrary::Audio2FaceBlendShapeToATLRInfo(const FAudio2FaceBlendShapeInfo& InShapeInfo,
	FATLInfo& ATLInfo, double InAmplitude)
{
	ATLInfo.BlendShapesNames = InShapeInfo.FacsNames;
	ATLInfo.Frames.SetNum(InShapeInfo.WeightMat.Num());
	for (int32 i = 0; i < ATLInfo.Frames.Num(); i++)
	{
		ATLInfo.Frames[i].Blendshapes.SetNum(InShapeInfo.WeightMat[i].Weight.Num());
		for (int32 j = 0; j < InShapeInfo.WeightMat[i].Weight.Num(); j++)
		{
			ATLInfo.Frames[i].Blendshapes[j] = InShapeInfo.WeightMat[i].Weight[j] * InAmplitude;
		}
	}

	ATLInfo.FPS = InShapeInfo.ExportFps;
}
