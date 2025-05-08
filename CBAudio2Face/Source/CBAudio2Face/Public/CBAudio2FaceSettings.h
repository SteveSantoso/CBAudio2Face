// Copyright 2025-Present Xiao Lan fei. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "CBAudio2FaceSettings.generated.h"

/**
 * 
 */
UCLASS(config = CBAudio2Face, defaultconfig, meta = (DisplayName = "Audio 2 Face Settings"))
class CBAUDIO2FACE_API UCBAudio2FaceSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	// UDeveloperSettings Interface
	virtual FName GetCategoryName() const override;
	
	/**是否激活a2f*/
	UPROPERTY(EditAnywhere, config, Category = "A2F Server")
	bool bActive=false;

	/**服务器端口*/
	UPROPERTY(EditAnywhere, config, Category = "A2F Server")
	FString ServerIP=TEXT("8011");

	/**服务器IP*/
	UPROPERTY(EditAnywhere, config, Category = "A2F Server")
	FString ServerPort=TEXT("127.0.0.1");
	
	/**audio 2 face的ip和端口，是服务启动的配置项，也是http请求*/
	UPROPERTY(EditAnywhere, config, Category = "A2F Server")
	FString ServerURL=TEXT("http://127.0.0.1:8011");

	/**audio 2 face 程序路径*/
	UPROPERTY(EditAnywhere, config, Category = "A2F Server")
	FString Audio2FaceExePath;

	/**audio 2 face加载的USD名字*/
	UPROPERTY(EditAnywhere, config, DisplayName="USD File",Category = "A2F Server")
	FString USDFile=TEXT("Claire.usd");
	
	/**audio 2 face wav路径*/
	UPROPERTY(EditAnywhere, config, Category = "A2F Server")
	FString Audio2FaceRootPath=TEXT("LocalA2FWav://");
	
public:
	static  UCBAudio2FaceSettings* Get();
	
};
