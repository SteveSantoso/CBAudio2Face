#include "AudioToLipsync/Widget/CBA2FAnimSequenceWidgetByJson.h"

#include "AudioToLipsync/CBAudioToLipsyncFunctionLibrary.h"
#include "AudioToLipsync/CBAudioToLipsyncTypes.h"
#include "AudioToLipsync/Settings/CBAnimSequenceByJsonSettings.h"


#define LOCTEXT_NAMESPACE "CBA2FAnimSequenceWidgetByJson"

SCBA2FAnimSequenceWidgetByJson::SCBA2FAnimSequenceWidgetByJson()
{
}

void SCBA2FAnimSequenceWidgetByJson::Construct(const FArguments& InArgs)
{
	SCBA2FAnimSequenceWidgetBase::Construct(InArgs);
}

FReply SCBA2FAnimSequenceWidgetByJson::OnSure()
{
	if (UCBAnimSequenceByJsonSettings * InAnimSequenceByJsonSettings =  Cast<UCBAnimSequenceByJsonSettings>(AnimSequenceSettings))
	{
		if (InAnimSequenceByJsonSettings->BlendShapeJson.FilePath.IsEmpty())
		{
			return FReply::Handled();
		}

		InAnimSequenceByJsonSettings->SaveObjectStorageConfig();

		FATLAnimInfo ATLAnimInfo;
		BuildATLAnimInfo(ATLAnimInfo);

		FString JsonString;
		FFileHelper::LoadFileToString(JsonString,*InAnimSequenceByJsonSettings->BlendShapeJson.FilePath);

		if (JsonString.Len()> 0)
		{
			UCBAudioToLipsyncFunctionLibrary::Audio2FaceBlendShapeToAnimaiton(JsonString,ATLAnimInfo,InAnimSequenceByJsonSettings->Amplitude);

			if (!AnimSequence)
			{
				FMessageDialog::Open(EAppMsgType::YesNo, LOCTEXT("OnAudioToLipsyncComplete_Error",
					"Animation generation failed.The following reasons may have caused the failure.\n\
1.The plugin is not the latest version.\n\
2.PoseAsset needs to be opened once in advance to load it into memory.\n\
3.If neither of the above methods works, please contact the plugin author to update the plugin in a timely manner."));

				//AnimSequence = NewObject<UAnimSequence>(GetTransientPackage(),ATLAnimInfo.Name,RF_Public | RF_Standalone);

				if (Window.IsValid())
				{
					Window.Pin()->RequestDestroyWindow();
				}
			}
			else
			{
				if (Window.IsValid())
				{
					Window.Pin()->RequestDestroyWindow();
				}
			}
		}	
	}

	return FReply::Handled();
}
#undef LOCTEXT_NAMESPACE