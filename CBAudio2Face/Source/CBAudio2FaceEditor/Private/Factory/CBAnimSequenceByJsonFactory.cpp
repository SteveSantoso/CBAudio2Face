// Copyright 2025-Present Xiao Lan fei. All Rights Reserved.


#include "Factory/CBAnimSequenceByJsonFactory.h"

#include "AudioToLipsync/Settings/CBAnimSequenceByJsonSettings.h"
#include "AudioToLipsync/Widget/CBA2FAnimSequenceWidgetByJson.h"

UObject* UCBAnimSequenceByJsonFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName,
                                                        EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	return HandleFactoryCreateNew<UCBAnimSequenceByJsonSettings, SCBA2FAnimSequenceWidgetByJson>(InParent, InName);
}
