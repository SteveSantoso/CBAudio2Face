// Copyright 2025-Present Xiao Lan fei. All Rights Reserved.


#include "Factory/CBAnimSequenceFactoryBase.h"

#define LOCTEXT_NAMESPACE "CBAnimSequenceFactoryBase"
UCBAnimSequenceFactoryBase::UCBAnimSequenceFactoryBase(const FObjectInitializer& ObjectInitializer)
{
	SupportedClass = UAnimSequence::StaticClass();
}

FText UCBAnimSequenceFactoryBase::GetDisplayName() const
{
	return LOCTEXT("CBAnimSequenceFactoryDescription", "CB Audio To Lipsync");
}

bool UCBAnimSequenceFactoryBase::DoesSupportClass(UClass* Class)
{
	return SupportedClass == Class;
}

bool UCBAnimSequenceFactoryBase::FactoryCanImport(const FString& Filename)
{
	return false;
}

#undef LOCTEXT_NAMESPACE
