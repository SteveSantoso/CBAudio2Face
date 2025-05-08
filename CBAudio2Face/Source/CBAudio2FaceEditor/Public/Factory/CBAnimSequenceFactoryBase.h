// Copyright 2025-Present Xiao Lan fei. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "Interfaces/IMainFrameModule.h"
#include "CBAnimSequenceFactoryBase.generated.h"

#define LOCTEXT_NAMESPACE "CBAnimSequenceFactoryBase"
UCLASS()
class CBAUDIO2FACEEDITOR_API UCBAnimSequenceFactoryBase : public UFactory
{
	GENERATED_BODY()
public:
	UCBAnimSequenceFactoryBase(const FObjectInitializer& ObjectInitializer);

public:
	virtual FText GetDisplayName() const override;
	virtual bool DoesSupportClass(UClass* Class) override;
	virtual bool FactoryCanImport(const FString& Filename) override;
};
template<class T1,class T2>
UObject *HandleFactoryCreateNew(UObject* InParent, FName InName)
{
	TSharedPtr<SWindow> ParentWindow;

	if (FModuleManager::Get().IsModuleLoaded("MainFrame"))
	{
		IMainFrameModule& MainFrame = FModuleManager::LoadModuleChecked<IMainFrameModule>("MainFrame");
		ParentWindow = MainFrame.GetParentWindow();
	}

	TSharedRef<SWindow> Window = SNew(SWindow)
		.Title(LOCTEXT("CBAnimSequence", "Generate specific animation sequences"))
		.SizingRule(ESizingRule::Autosized);

	T1* Settings = GetMutableDefault<T1>();
	TSharedPtr<T2> TALWindow;
	Window->SetContent
	(
		SAssignNew(TALWindow, T2)
		.AnimSequenceSettings(Settings)
		.WidgetWindow(Window)
		.Parent(InParent)
		.Name(InName)
	);

	FSlateApplication::Get().AddModalWindow(Window, ParentWindow, false);

	return TALWindow->GetAnimSequence();
}

#undef LOCTEXT_NAMESPACE