// Copyright 2025-Present Xiao Lan fei. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

struct FATLAnimInfo;
class UCBAnimSequenceSettingsBase;

class SCBA2FAnimSequenceWidgetBase : public SCompoundWidget
{
public:
	SCBA2FAnimSequenceWidgetBase();

public:
	SLATE_BEGIN_ARGS(SCBA2FAnimSequenceWidgetBase)
		:_AnimSequenceSettings(NULL)
	{}
	SLATE_ARGUMENT(UCBAnimSequenceSettingsBase*, AnimSequenceSettings)
	SLATE_ARGUMENT(TSharedPtr<SWindow>, WidgetWindow)
	SLATE_ARGUMENT(UObject*, Parent)
	SLATE_ARGUMENT(FName, Name)

	SLATE_END_ARGS()


public:
	virtual void Construct(const FArguments& InArgs);

public:
	void SetSequenceSettings(UCBAnimSequenceSettingsBase *InBase);
	void BuildATLAnimInfo(FATLAnimInfo &InOutATLRequest);

public:
	UAnimSequence* GetAnimSequence() const { return AnimSequence; }

protected:
	bool OnIsPropertyEditingEnabled();
	void OnFinishedChangingProperties(const FPropertyChangedEvent& InPropertyChangedEvent);

	virtual FReply OnSure();
	virtual FReply OnCancel();

	
protected:
	UCBAnimSequenceSettingsBase* AnimSequenceSettings;
	TSharedPtr<SBox> AnimSequenceViewBox;
	TWeakPtr<SWindow> Window;
	UAnimSequence* AnimSequence;
	UObject* Parent;
	FName Name;
	
};
