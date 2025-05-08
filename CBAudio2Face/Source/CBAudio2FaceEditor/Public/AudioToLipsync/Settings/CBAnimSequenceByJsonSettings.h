#pragma once
#include "CBAnimSequenceSettingsBase.h"
#include "CBAnimSequenceByJsonSettings.generated.h"
UCLASS()
class UCBAnimSequenceByJsonSettings:public UCBAnimSequenceSettingsBase
{
public:
	GENERATED_BODY()

public:
	UCBAnimSequenceByJsonSettings();

	UPROPERTY(config, EditAnywhere, Category = "AnimSequenceJsonSettings|Audio2Face")
	double Amplitude;

	UPROPERTY(config,EditAnywhere, Category = "AnimSequenceJsonSettings|Audio2Face")
	FFilePath BlendShapeJson;
};


