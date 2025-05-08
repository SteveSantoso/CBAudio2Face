#pragma once
#include "CoreMinimal.h"
#include "CBA2FAnimSequenceWidgetBase.h"

class SCBA2FAnimSequenceWidgetByJson:public SCBA2FAnimSequenceWidgetBase
{
public:
	SCBA2FAnimSequenceWidgetByJson();

public:
	virtual void Construct(const FArguments& InArgs);

protected:
	virtual FReply OnSure();
};
