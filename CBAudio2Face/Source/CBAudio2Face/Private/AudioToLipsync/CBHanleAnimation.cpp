#include "AudioToLipsync/CBHanleAnimation.h"

#include "Animation/PoseAsset.h"
#include "AudioToLipsync/CBAudioToLipsyncTypes.h"

#define LOCTEXT_NAMESPACE "CBHanleAnimation"
namespace CBHanleAnimation
{
	//获取到受影响的曲线
	bool ResolveToAffectedPoseCurves(
		UPoseAsset* InPoseAsset,
		const TArray<FString>& InBlendShapesNames,
		TMap<int32,FAffectedPoseCurve> &OutAffectedPose)
	{
		if (IsValid(InPoseAsset) && InBlendShapesNames.Num() > 0)
		{
			const TArray<FName> CurveNames = InPoseAsset->GetCurveFNames();
			const TArray<FName> PoseNames = InPoseAsset->GetPoseFNames();

			//构建映射关系
			TMap<FName, int32> PoseMapping;
			for (int32 i = 0; i < PoseNames.Num(); i++)
			{
				PoseMapping.Add(PoseNames[i], i);
			}

			//收集影响的曲线和曲线名字
			for (int32 i = 0; i < InBlendShapesNames.Num(); i++)
			{
				OutAffectedPose.Add(i, FAffectedPoseCurve());
				FAffectedPoseCurve& InAffectedPoseCurve = OutAffectedPose[i];
				InAffectedPoseCurve.Index = i;//记录一下

				const FName BlendName = *InBlendShapesNames[i];

				InAffectedPoseCurve.PoseName = BlendName;
				if (PoseMapping.Contains(BlendName))
				{
					const TArray<float> CurveValues = InPoseAsset->GetCurveValues(PoseMapping[BlendName]);
					for (int32 j = 0; j < CurveValues.Num(); j++)
					{
						if (CurveValues[j] > 0)
						{
							InAffectedPoseCurve.AffectedCurveNames.Add(CurveNames[j]);
							InAffectedPoseCurve.AffectedCurveWeights.Add(CurveValues[j]);
						}
					}
				}
				else
				{
					InAffectedPoseCurve.PoseName = BlendName;
					InAffectedPoseCurve.AffectedCurveNames.Add(BlendName);
					InAffectedPoseCurve.AffectedCurveWeights.Add(1.0f);
				}

				check(InAffectedPoseCurve.AffectedCurveNames.Num() == InAffectedPoseCurve.AffectedCurveWeights.Num());
			}

			return OutAffectedPose.Num() > 0;
		}

		return false;
	}

	void ResolveToCurveWeightedBlendShape(
		const FFaceFrames& InFaceFrames,
		const TArray<FName>& SkeletonCurveNames,
		const TArray<FString>& InBlendShapesNames,
		const TMap<int32, FAffectedPoseCurve>& InAffectedPose,
		TMap<FName, float>& OutCWBS)
	{
		for (int32 i = 0; i < InBlendShapesNames.Num(); i++)
		{
			//I 获取当前的PoseCurve
			FAffectedPoseCurve InPoseCurve;
			if (const FAffectedPoseCurve *InPoseCurvePtr = InAffectedPose.Find(i))
			{
				InPoseCurve = *InPoseCurvePtr;
			}
			else
			{
				InPoseCurve.PoseName = *InBlendShapesNames[i];
				InPoseCurve.Index = i;
				InPoseCurve.AffectedCurveNames.Add(InPoseCurve.PoseName);
				InPoseCurve.AffectedCurveWeights.Add(1.f);
			}

			//II 是否包含
			if (!SkeletonCurveNames.Contains(InPoseCurve.PoseName))
			{
				continue;
			}

			//III 计算权重
			const float PoseWeight = InFaceFrames.Blendshapes[i] / 100.0f;
			for (int32 j = 0; j < InPoseCurve.AffectedCurveNames.Num(); j++)
			{
				const FName CurveName = InPoseCurve.AffectedCurveNames[j];
				float CurrentBlendShapeValue = OutCWBS.FindRef(CurveName) + InPoseCurve.AffectedCurveWeights[j] * PoseWeight;
				
				OutCWBS.Add(CurveName, FMath::Clamp(CurrentBlendShapeValue, 0.0f, 1.0f));
			}
		}
	}

	void BuildCurveTracks(
		const TMap<FName, float> &InCWBS,
		const TArray<FName>& InBlendShapesNames,
		double InFramesPerSecondStep,
		int32 FrameIndex,
		ERichCurveInterpMode InterpMode,
		FRawCurveTracks &OutCurveData)
	{
		for (auto &Tmp : InCWBS)
		{
			if (!InBlendShapesNames.Contains(Tmp.Key))
			{
				continue;
			}

			FFloatCurve* InCurve = static_cast<FFloatCurve*>(OutCurveData.GetCurveData(Tmp.Key, ERawCurveTrackTypes::RCT_Float));
			if (InCurve == nullptr)
			{
				ensure(OutCurveData.AddCurveData(Tmp.Key));
				InCurve = static_cast<FFloatCurve*>(OutCurveData.GetCurveData(Tmp.Key, ERawCurveTrackTypes::RCT_Float));
				
				InCurve->SetName(Tmp.Key);
			}

			//开始填充关键帧
			FRichCurve& RichCurve = InCurve->FloatCurve;
			FKeyHandle NewKeyHandle = RichCurve.AddKey(FrameIndex * InFramesPerSecondStep, Tmp.Value, false);
		
			RichCurve.SetKeyTangentMode(NewKeyHandle,RCTM_Auto,true);
			RichCurve.SetKeyTangentWeightMode(NewKeyHandle, RCTWM_WeightedNone, true);
			RichCurve.SetKeyInterpMode(NewKeyHandle,InterpMode, true);

			FRichCurveKey& NewKey = RichCurve.GetKey(NewKeyHandle);
			NewKey.ArriveTangent = 0.f;
			NewKey.LeaveTangent = 0.f;
			NewKey.ArriveTangentWeight = 0.f;
			NewKey.LeaveTangentWeight = 0.f;
		}
	}

	void HandleBones(const FATLInfo& InATLInfo, const FATLAnimInfo& InATLAnimInfo,
		const TMap<FString, FString>& InBoneNamesMapping, TMap<FName, FRawAnimSequenceTrack>& OutBoneTracks,
		const FFaceFrames& InFaceFrame)
	{
		// Set bone keys for current frame
		const TArray<FTransform> BonesPoses = InATLAnimInfo.Skeleton->GetReferenceSkeleton().GetRefBonePose();
		for (int BoneIndex = 0; BoneIndex < InATLInfo.BonesNames.Num(); ++BoneIndex)
		{
			FString ATLBoneName = InATLInfo.BonesNames[BoneIndex];
			bool bHasBoneNameInMap = InBoneNamesMapping.Contains(ATLBoneName) && InBoneNamesMapping[ATLBoneName].Len();
			FString BoneNameString = bHasBoneNameInMap ? InBoneNamesMapping[ATLBoneName] : ATLBoneName;
			FName BoneName = FName(BoneNameString);

			int32 BoneSkeletonIndex = InATLAnimInfo.Skeleton->GetReferenceSkeleton().FindBoneIndex(BoneName);
			if (BoneSkeletonIndex == INDEX_NONE)
			{
				continue;
			}
			if (!OutBoneTracks.Contains(BoneName))
			{
				OutBoneTracks.Add(BoneName, FRawAnimSequenceTrack());
			}

			const FVector& BoneTransform = InFaceFrame.Bones[BoneIndex].BoneValue;
			FRawAnimSequenceTrack& Track = OutBoneTracks[BoneName];

			auto RotationDelta = FQuat::MakeFromEuler(BoneTransform);
			auto OriginalRotation = BonesPoses[BoneSkeletonIndex].GetRotation();
			auto Rotation = OriginalRotation * RotationDelta;

			Track.RotKeys.Add(FQuat4f(Rotation));
			Track.ScaleKeys.Add(FVector3f(BonesPoses[BoneSkeletonIndex].GetScale3D()));
			Track.PosKeys.Add(FVector3f(BonesPoses[BoneSkeletonIndex].GetLocation()));
		}
	}

	void AddBoneTracks(TMap<FName, FRawAnimSequenceTrack>& InAddBoneTracks, const FATLAnimInfo& InATLAnimInfo)
	{
		const TArray<FTransform> RefBonePose = InATLAnimInfo.Skeleton->GetReferenceSkeleton().GetRefBonePose();

		for (auto& Track : InAddBoneTracks)
		{
			int32 i = InATLAnimInfo.Skeleton->GetReferenceSkeleton().FindBoneIndex(Track.Key);
			if (i != INDEX_NONE)
			{
				Track.Value.RotKeys.Add(FQuat4f(RefBonePose[i].GetRotation()));
				Track.Value.ScaleKeys.Add(FVector3f(RefBonePose[i].GetScale3D()));
				Track.Value.PosKeys.Add(FVector3f(RefBonePose[i].GetLocation()));
			}
		}
	}

	void AnimationDataRegistrationOnEditor(UAnimSequence* AnimSequence, const FATLInfo& InResponse, int32 NumberFrames,
		const FRawCurveTracks& RawCurveData, const TMap<FName, FRawAnimSequenceTrack>& InBoneTracks)
	{
#if WITH_EDITOR
		if (AnimSequence)
		{
			IAnimationDataController& InAnimationDataController = AnimSequence->GetController();
			InAnimationDataController.SetModel(AnimSequence->GetDataModelInterface());

			AnimSequence->GetController().OpenBracket(LOCTEXT("AudioToLipsyncSubsystem", "Adding float curve."), false);

			InAnimationDataController.InitializeModel();
			InAnimationDataController.SetNumberOfFrames(NumberFrames);
			InAnimationDataController.SetFrameRate(FFrameRate(InResponse.FPS, 1.f));

			//主要是脸部的曲线
			for (auto& Tmp : RawCurveData.FloatCurves)
			{
				const FAnimationCurveIdentifier AnimationCurveIdentifier(Tmp.GetName(), ERawCurveTrackTypes::RCT_Float);
				AnimSequence->GetController().AddCurve(AnimationCurveIdentifier, Tmp.GetCurveTypeFlags());
				AnimSequence->GetController().SetCurveKeys(AnimationCurveIdentifier, Tmp.FloatCurve.Keys);
			}

			//主要添加脖子和眼睛
			for (auto& Track : InBoneTracks)
			{
				AnimSequence->GetController().AddBoneCurve(Track.Key);
				AnimSequence->GetController().SetBoneTrackKeys(Track.Key, Track.Value.PosKeys, Track.Value.RotKeys, Track.Value.ScaleKeys);
			}

			InAnimationDataController.NotifyPopulated();
			InAnimationDataController.CloseBracket(false);
		}	
#endif
	}
};

#undef LOCTEXT_NAMESPACE