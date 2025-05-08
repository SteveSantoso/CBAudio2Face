// Copyright 2025-Present Xiao Lan fei. All Rights Reserved.


#include "AudioToLipsync/Widget/CBA2FAnimSequenceWidgetBase.h"

#include "PropertyEditorModule.h"
#include "Animation/PoseAsset.h"
#include "AudioToLipsync/Settings/CBAnimSequenceSettingsBase.h"
#include "CBAudio2Face/Public/AudioToLipsync/CBAudioToLipsyncTypes.h"

#define LOCTEXT_NAMESPACE "CBA2FAnimSequenceWidgetBase"
SCBA2FAnimSequenceWidgetBase::SCBA2FAnimSequenceWidgetBase()
{
	AnimSequence=nullptr;
}

void SCBA2FAnimSequenceWidgetBase::Construct(const FArguments& InArgs)
{
	AnimSequenceSettings = InArgs._AnimSequenceSettings;
	Window = InArgs._WidgetWindow;
	Parent = InArgs._Parent;
	Name = InArgs._Name;

	ChildSlot
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(2)
		[
			SAssignNew(AnimSequenceViewBox, SBox)
		]
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(2)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(2)
			[
				SNew(SButton)
				.HAlign(HAlign_Left)
				.Text(LOCTEXT("AnimSequence_Cancel", "Cancel"))
				.OnClicked(this, &SCBA2FAnimSequenceWidgetBase::OnCancel)
			]
			+ SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(2)
			[
				SNew(SButton)
				.HAlign(HAlign_Right)
				.Text(LOCTEXT("AnimSequence_Sure", "Sure"))
				.OnClicked(this, &SCBA2FAnimSequenceWidgetBase::OnSure)
			]
		]
	];

	SetSequenceSettings(AnimSequenceSettings);
}

void SCBA2FAnimSequenceWidgetBase::SetSequenceSettings(UCBAnimSequenceSettingsBase* InBase)
{
	if (AnimSequenceViewBox.IsValid() && InBase)
	{
		FPropertyEditorModule& PropertyEditorModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");

		FDetailsViewArgs DetailsViewArgs;
		DetailsViewArgs.bAllowSearch = false;
		DetailsViewArgs.NameAreaSettings = FDetailsViewArgs::HideNameArea;

		if (TSharedPtr<IDetailsView> DetailsView = PropertyEditorModule.CreateDetailView(DetailsViewArgs))
		{
			DetailsView->SetObject(InBase);
			DetailsView->SetIsPropertyEditingEnabledDelegate(FIsPropertyEditingEnabled::CreateSP(this, &SCBA2FAnimSequenceWidgetBase::OnIsPropertyEditingEnabled));
			DetailsView->OnFinishedChangingProperties().Add(FOnFinishedChangingProperties::FDelegate::CreateSP(this, &SCBA2FAnimSequenceWidgetBase::OnFinishedChangingProperties));

			AnimSequenceViewBox->SetContent(DetailsView.ToSharedRef());
		}
	}
}

void SCBA2FAnimSequenceWidgetBase::BuildATLAnimInfo(FATLAnimInfo& InOutATLRequest)
{
	if (AnimSequenceSettings)
	{
		InOutATLRequest.InterpMode = AnimSequenceSettings->InterpMode;
		InOutATLRequest.Skeleton = AnimSequenceSettings->Skeleton.Get();
		if (!InOutATLRequest.Skeleton)
		{
			const FString PackageName = AnimSequenceSettings->Skeleton.GetLongPackageName();
			InOutATLRequest.Skeleton = LoadObject<USkeleton>(nullptr, *PackageName);
		}

		InOutATLRequest.PoseAsset = AnimSequenceSettings->PoseAsset.Get();
		if (!InOutATLRequest.PoseAsset)
		{
			const FString PackageName = AnimSequenceSettings->PoseAsset.GetLongPackageName();
			InOutATLRequest.PoseAsset = LoadObject<UPoseAsset>(nullptr, *PackageName);
		}

		InOutATLRequest.Parent = Parent;
		InOutATLRequest.Name = Name.ToString();
	}
}

bool SCBA2FAnimSequenceWidgetBase::OnIsPropertyEditingEnabled()
{
	return true;
}

void SCBA2FAnimSequenceWidgetBase::OnFinishedChangingProperties(const FPropertyChangedEvent& InPropertyChangedEvent)
{
}

FReply SCBA2FAnimSequenceWidgetBase::OnSure()
{
	return FReply::Handled();
}

FReply SCBA2FAnimSequenceWidgetBase::OnCancel()
{
	if (Window.IsValid())
	{
		Window.Pin()->RequestDestroyWindow();
	}

	return FReply::Handled();
}
#undef LOCTEXT_NAMESPACE