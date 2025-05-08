// Copyright 2025-Present Xiao Lan fei. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CBAudioToLipsyncTypes.h"

struct FFaceFrames;

namespace CBHanleAnimation
{
	struct FAffectedPoseCurve
	{
		FAffectedPoseCurve()
		{
			Index = INDEX_NONE;
		}

		int32 Index;
		FName PoseName;
		TArray<FName> AffectedCurveNames;
		TArray<float> AffectedCurveWeights;
	};

	//FCompressedRichCurve
	struct FMirrorsInPartCompressedRichCurve
	{
		/** Compression format used by CompressedKeys */
		TEnumAsByte<ERichCurveCompressionFormat> CompressionFormat;

		/** Compression format used to pack the key time */
		TEnumAsByte<ERichCurveKeyTimeCompressionFormat> KeyTimeCompressionFormat;

		/** Pre-infinity extrapolation state */
		TEnumAsByte<ERichCurveExtrapolation> PreInfinityExtrap;

		/** Post-infinity extrapolation state */
		TEnumAsByte<ERichCurveExtrapolation> PostInfinityExtrap;

		/**
		* If the compression format is constant, this is the value returned
		* Inline here to reduce the likelihood of accessing the compressed keys data for the common case of constant/zero/empty curves
		* When a curve is linear/cubic/mixed, the constant float value isn't used and instead we use the number of keys
		*/
		FCompressedRichCurve::TConstantValueNumKeys ConstantValueNumKeys;

		/** KeyDataOffset*/
		int32 KeyDataOffset;
	};

	//获取到受影响的曲线
	bool ResolveToAffectedPoseCurves(UPoseAsset* InPoseAsset,const TArray<FString>& InBlendShapesNames,TMap<int32,FAffectedPoseCurve> &OutAffectedPose);

	void ResolveToCurveWeightedBlendShape(const FFaceFrames& InFaceFrames,const TArray<FName>& SkeletonCurveNames,const TArray<FString>& InBlendShapesNames,
		const TMap<int32, FAffectedPoseCurve>& InAffectedPose,
		TMap<FName, float>& OutCWBS);

	void BuildCurveTracks(const TMap<FName, float> &InCWBS,const TArray<FName>& InBlendShapesNames,double InFramesPerSecondStep,int32 FrameIndex,
		ERichCurveInterpMode InterpMode,
		FRawCurveTracks &OutCurveData);

	void HandleBones(const FATLInfo& InATLInfo,const FATLAnimInfo& InATLAnimInfo,const TMap<FString, FString>& InBoneNamesMapping,
		TMap<FName, FRawAnimSequenceTrack> &OutBoneTracks,
		const FFaceFrames &InFaceFrame);

	void AddBoneTracks(TMap<FName, FRawAnimSequenceTrack>& InAddBoneTracks, const FATLAnimInfo& InATLAnimInfo);
	
	void AnimationDataRegistrationOnEditor(UAnimSequence* AnimSequence, const FATLInfo& InResponse,int32 NumberFrames,const FRawCurveTracks &RawCurveData,
	const TMap<FName, FRawAnimSequenceTrack> &InBoneTracks);
}
