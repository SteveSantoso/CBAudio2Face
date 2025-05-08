// Copyright 2025-Present Xiao Lan fei. All Rights Reserved.


#include "CBAudio2FaceSettings.h"

FName UCBAudio2FaceSettings::GetCategoryName() const
{
	return TEXT("CBPlugins");
}

UCBAudio2FaceSettings* UCBAudio2FaceSettings::Get()
{
	static  UCBAudio2FaceSettings*  Instance = GetMutableDefault<UCBAudio2FaceSettings>();
	
	return Instance;
}
