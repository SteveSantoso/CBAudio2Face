// Copyright 2025-Present Xiao Lan fei. All Rights Reserved.


#include "AudioToLipsync/Settings/CBAnimSequenceSettingsBase.h"

#include "CBAudio2FaceEditor.h"

UCBAnimSequenceSettingsBase::UCBAnimSequenceSettingsBase()
{
}

void UCBAnimSequenceSettingsBase::SaveObjectStorageConfig()
{
	FString SettingsEditorConfigPath = FPaths::ProjectConfigDir() / TEXT("Default") + GetClass()->ClassConfigName.ToString() + TEXT(".ini");

	SaveConfig(CPF_Config, *SettingsEditorConfigPath);

	UE_LOG(LogCBAudioToLipsyncEditor, Display, TEXT("SettingsEditorConfigPath=%s"), *SettingsEditorConfigPath);
}

void UCBAnimSequenceSettingsBase::LoadObjectStorageConfig()
{
	FString SettingsEditorConfigPath = FPaths::ProjectConfigDir() / TEXT("Default") + GetClass()->ClassConfigName.ToString() + TEXT(".ini");

	LoadConfig(GetClass(), *SettingsEditorConfigPath);

	UE_LOG(LogCBAudioToLipsyncEditor, Display, TEXT("SettingsEditorConfigPath=%s"), *SettingsEditorConfigPath);
}
