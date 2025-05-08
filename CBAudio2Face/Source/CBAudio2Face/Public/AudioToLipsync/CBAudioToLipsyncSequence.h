// Copyright 2025-Present Xiao Lan fei. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimSequence.h"
#include "CBAudioToLipsyncSequence.generated.h"

/**
 * Anim Sequence with opened interface for runtime manipulations
 */
UCLASS()
class CBAUDIO2FACE_API UCBAudioToLipsyncSequence : public UAnimSequence
{
	GENERATED_BODY()

public:

	void SetSequenceLength(const float& Length);
};
