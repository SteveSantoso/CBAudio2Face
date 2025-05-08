// Copyright 2025-Present Xiao Lan fei. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Factory/CBAnimSequenceFactoryBase.h"
#include "CBAnimSequenceByJsonFactory.generated.h"

/**
 * 
 */
UCLASS()
class CBAUDIO2FACEEDITOR_API UCBAnimSequenceByJsonFactory : public UCBAnimSequenceFactoryBase
{
	GENERATED_BODY()

public:
	virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
};
