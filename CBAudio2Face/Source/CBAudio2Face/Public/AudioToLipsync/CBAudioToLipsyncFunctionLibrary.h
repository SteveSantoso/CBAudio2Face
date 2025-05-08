// Copyright 2025-Present Xiao Lan fei. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CBAudioToLipsyncTypes.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CBAudioToLipsyncFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class CBAUDIO2FACE_API UCBAudioToLipsyncFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	static  UAnimSequence* Audio2FaceBlendShapeToAnimaiton(const FString& InJson, const FATLAnimInfo& InATLAnimInfo,double Amplitude = 40.0);
	static  UAnimSequence *ATLAnimInfoToAnimaiton(const FATLInfo& ATLInfo,const FATLAnimInfo &InATLAnimInfo);
private:
	static void GetAudio2FaceBlendShapeInfo(const FString& InJson, FAudio2FaceBlendShapeInfo& BlendShapeInfo);
	static void Audio2FaceBlendShapeToATLRInfo(const FAudio2FaceBlendShapeInfo &InShapeInfo, FATLInfo& ATLInfo,double InAmplitude = 40.0);
};
