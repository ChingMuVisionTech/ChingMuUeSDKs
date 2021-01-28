// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.


#include "MotionCaptureEditorPCH.h"
#include "Interfaces/IPluginManager.h"

#include "Editor.h"

#include "Modules/ModuleManager.h"
#include "Misc/CoreDelegates.h"
#include "Widgets/Docking/SDockTab.h"
#include "WorkspaceMenuStructure.h"
#include "WorkspaceMenuStructureModule.h"
#include "EditorStyleSet.h"
#include "Styling/SlateStyle.h"
#include "Styling/SlateTypes.h"
#include "Styling/SlateStyleRegistry.h"
#include "Framework/Application/SlateApplication.h"

#include "Features/IModularFeatures.h"
#include "LevelEditor.h"

#include "FMotionCapture.h"
#include "MotionCaptureEditorPanel.h"

/**
 * Implements the Messaging module.
 */

#define LOCTEXT_NAMESPACE "MotionCaptureModule"

static const FName MotionCaptureTabName(TEXT("ChingMU Mocap"));
static const FName LevelEditorModuleName(TEXT("LevelEditor"));

#define IMAGE_PLUGIN_BRUSH( RelativePath, ... ) FSlateImageBrush( InPluginContent( RelativePath, ".png" ), __VA_ARGS__ )

FString InPluginContent(const FString& RelativePath, const ANSICHAR* Extension)
{
	static FString ContentDir = IPluginManager::Get().FindPlugin(TEXT("MotionCapture"))->GetContentDir();
	return (ContentDir / RelativePath) + Extension;
}

class FMotionCaptureEditorModule : public IModuleInterface
{
public:
	TSharedPtr<FSlateStyleSet> StyleSet;

	TSharedPtr< class ISlateStyle > GetStyleSet() { return StyleSet; }

	// IModuleInterface interface

	virtual void StartupModule() override
	{
		static FName MotionCaptureStyle(TEXT("MotionCaptureStyle"));
		StyleSet = MakeShareable(new FSlateStyleSet(MotionCaptureStyle));

		if (FModuleManager::Get().IsModuleLoaded(LevelEditorModuleName))
		{
			RegisterTab();
		}
		else
		{
			ModulesChangedHandle = FModuleManager::Get().OnModulesChanged().AddRaw(this, &FMotionCaptureEditorModule::ModulesChangesCallback);
		}

		const FVector2D Icon16x16(16.0f, 16.0f);
		const FVector2D Icon20x20(20.0f, 20.0f);
		const FVector2D Icon40x40(40.0f, 40.0f);

		StyleSet->SetContentRoot(FPaths::EngineContentDir() / TEXT("Editor/Slate"));
		StyleSet->SetCoreContentRoot(FPaths::EngineContentDir() / TEXT("Slate"));

		StyleSet->Set("MotionCapture.Common.Icon.Small", new IMAGE_PLUGIN_BRUSH(TEXT("chingmu_16x"), Icon16x16));

		FSlateStyleRegistry::RegisterSlateStyle(*StyleSet.Get());
	}

	void ModulesChangesCallback(FName ModuleName, EModuleChangeReason ReasonForChange)
	{
		if (ReasonForChange == EModuleChangeReason::ModuleLoaded && ModuleName == LevelEditorModuleName)
		{
			RegisterTab();
		}
	}

	void RegisterTab()
	{
		FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>(LevelEditorModuleName);

		TSharedPtr<FSlateStyleSet> StyleSetPtr = StyleSet;

		//Register our UI
		LevelEditorTabManagerChangedHandle = LevelEditorModule.OnTabManagerChanged().AddLambda([StyleSetPtr]()
		{
			FLevelEditorModule& LocalLevelEditorModule = FModuleManager::GetModuleChecked<FLevelEditorModule>(LevelEditorModuleName);
			LocalLevelEditorModule.GetLevelEditorTabManager()->RegisterTabSpawner(MotionCaptureTabName, FOnSpawnTab::CreateStatic(&FMotionCaptureEditorModule::SpawnMotionCaptureTab, StyleSetPtr))
				.SetGroup(WorkspaceMenu::GetMenuStructure().GetLevelEditorCategory())
				.SetDisplayName(LOCTEXT("MotionCaptureTabTitle", "ChingMU Mocap"))
				.SetTooltipText(LOCTEXT("SequenceRecorderTooltipText", "Open ChingMU Motion Capture streaming manager tab."))
				.SetIcon(FSlateIcon(StyleSetPtr->GetStyleSetName(), "MotionCapture.Common.Icon.Small"));
		});
	}

	virtual void ShutdownModule() override
	{
		if (FSlateApplication::IsInitialized())
		{
			FGlobalTabmanager::Get()->UnregisterTabSpawner(MotionCaptureTabName);
		}

		FModuleManager::Get().OnModulesChanged().Remove(ModulesChangedHandle);

		if (LevelEditorTabManagerChangedHandle.IsValid() && FModuleManager::Get().IsModuleLoaded(LevelEditorModuleName))
		{
			FLevelEditorModule& LevelEditorModule = FModuleManager::GetModuleChecked<FLevelEditorModule>(LevelEditorModuleName);
			LevelEditorModule.OnTabManagerChanged().Remove(LevelEditorTabManagerChangedHandle);
		}
	}

	virtual bool SupportsDynamicReloading() override
	{
		return false;
	}

	static TSharedRef<SDockTab> SpawnMotionCaptureTab(const FSpawnTabArgs& SpawnTabArgs, TSharedPtr<FSlateStyleSet> StyleSet)
	{
		FMotionCapture* mocap = NULL;
		bool isExist = IModularFeatures::Get().IsModularFeatureAvailable(FMotionCapture::ModularFeatureName);
		if(isExist)
			mocap = &IModularFeatures::Get().GetModularFeature<FMotionCapture>(FMotionCapture::ModularFeatureName);

		const FSlateBrush* IconBrush = StyleSet->GetBrush("MotionCapture.Common.Icon.Small");

		const TSharedRef<SDockTab> MajorTab =
			SNew(SDockTab)
			.Icon(IconBrush)
			.TabRole(ETabRole::NomadTab);

		MajorTab->SetContent(SNew(SMotionCaptureEditorPanel, mocap));

		return MajorTab;
	}

private:

	FDelegateHandle LevelEditorTabManagerChangedHandle;
	FDelegateHandle ModulesChangedHandle;
};

IMPLEMENT_MODULE(FMotionCaptureEditorModule, MotionCaptureEditor);

#undef LOCTEXT_NAMESPACE

