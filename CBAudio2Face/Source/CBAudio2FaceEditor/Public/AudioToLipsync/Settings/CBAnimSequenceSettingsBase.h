// Copyright 2025-Present Xiao Lan fei. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CBAnimSequenceSettingsBase.generated.h"

/**
 * 
 */
UCLASS(config = ATLAnimSequenceSettings)
class CBAUDIO2FACEEDITOR_API UCBAnimSequenceSettingsBase : public UObject
{
	GENERATED_BODY()

public:
	UCBAnimSequenceSettingsBase();
	
	UPROPERTY(config,EditAnywhere, Category = "AnimSequenceSettings")
	FString Taken;

	UPROPERTY(config,EditAnywhere, Category = "AnimSequenceSettings")
	TSoftObjectPtr<USkeleton> Skeleton;

	UPROPERTY(config,EditAnywhere, Category = "AnimSequenceSettings")
	TSoftObjectPtr<UPoseAsset> PoseAsset;

	UPROPERTY(config,EditAnywhere, Category = "AnimSequenceSettings")
	TEnumAsByte<ERichCurveInterpMode> InterpMode;

public:
	virtual void SaveObjectStorageConfig();
	virtual void LoadObjectStorageConfig();
	
};
