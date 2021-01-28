// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "MotionCaptureEditorPanel.h"
#include "MotionCaptureEditorPCH.h"
#include "Editor.h"
#include "Engine/Selection.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Layout/SSplitter.h"
#include "Widgets/SOverlay.h"
#include "Templates/SharedPointer.h"
#include "Widgets/Views/SListView.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SCheckBox.h"
#include "Widgets/Input/SEditableText.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "Widgets/Input/SComboButton.h"
#include "Framework/Application/SlateApplication.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "Framework/SlateDelegates.h"
#include "UObject/UObjectHash.h"
#include "EditorStyleSet.h"
#include "Modules/ModuleManager.h"
#include "PropertyEditorModule.h"
#include "IStructureDetailsView.h"
#include "Editor/EditorPerformanceSettings.h"
#include "Widgets/Input/STextEntryPopup.h"
#include "CoreGlobals.h"

#define LOCTEXT_NAMESPACE "MotionCaptureEditorPanel"


// Static Subject UI FNames
namespace SubjectTreeUI
{
	static const FName NameColumnSubjectKey(TEXT("ID"));
	static const FName NameColumnSubjectName(TEXT("Name"));
	static const FName NameColumnSubjectAttach(TEXT("Attach"));
};



SMotionCaptureEditorPanel* SMotionCaptureEditorPanel::SelfRef = NULL;
SMotionCaptureEditorPanel::~SMotionCaptureEditorPanel()
{
	SelfRef = NULL;
}

void SMotionCaptureEditorPanel::Construct(const FArguments& Args, FMotionCapture* InCMPlugin)
{
	check(InCMPlugin);
	CMPlugin = InCMPlugin;
	SelfRef = static_cast<SMotionCaptureEditorPanel*>(this);

	GConfig->GetText(TEXT("/Script/ChingMU.Mocap"), TEXT("StageNotesAddress"), StageNotesAddress, GEditorIni);
	GConfig->GetText(TEXT("/Script/ChingMU.Mocap"), TEXT("CMTrackerAddress"), CMPlugin->CMTrackerAddress, GEditorIni);
	GConfig->GetText(TEXT("/Script/ChingMU.Mocap"), TEXT("AttachFilePath"), AttachFilePath, GEditorIni);


#if 0
	// add new subject to tree view
	FMotionCaptureSubjectItemPtr Source = MakeShared<FMotionCaptureSubjectItem>();
	Source->SubjectKey = 0;
	Source->SubjectName = FText::FromString("Body_0");
	AllItems.Add(Source);
	CMPlugin->SubjectData.Add(Source);
	Source = MakeShared<FMotionCaptureSubjectItem>();
	Source->SubjectKey = 100;
	Source->SubjectName = FText::FromString("Body_1");
	AllItems.Add(Source);
	CMPlugin->SubjectData.Add(Source);
	Source = MakeShared<FMotionCaptureSubjectItem>();
	Source->SubjectKey = 200;
	Source->SubjectName = FText::FromString("Body_2");
	AllItems.Add(Source);
	CMPlugin->SubjectData.Add(Source);
#endif

	SAssignNew(SubjectsTreeView, STreeView<FMotionCaptureSubjectItemPtr>)
	.TreeItemsSource(&CMPlugin->SubjectData)
	.OnGenerateRow(this, &SMotionCaptureEditorPanel::MakeTreeRowWidget)
	.OnGetChildren(this, &SMotionCaptureEditorPanel::GetChildrenForInfo)
	.OnSelectionChanged(this, &SMotionCaptureEditorPanel::OnSelectionChanged)
	.ExternalScrollbar(SNew(SScrollBar).Orientation(Orient_Vertical))
	.SelectionMode(ESelectionMode::Single)
	.AllowOverscroll(EAllowOverscroll::No)
	.HeaderRow
	(
		SNew(SHeaderRow)
		+ SHeaderRow::Column(SubjectTreeUI::NameColumnSubjectKey)
		.DefaultLabel(LOCTEXT("SubjectKey", "ID"))
		.FillWidth(0.1f)
		+ SHeaderRow::Column(SubjectTreeUI::NameColumnSubjectName)
		.DefaultLabel(LOCTEXT("SubjectName", "Name"))
		.FillWidth(0.4f)
		+ SHeaderRow::Column(SubjectTreeUI::NameColumnSubjectAttach)
		.DefaultLabel(LOCTEXT("AttachName", "Attach"))
		.FillWidth(0.5f)
	);

	ChildSlot
	[
		// version string
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(FMargin(0.0f, 4.0f, 0.0f, 0.0f))
		[
			SNew(STextBlock)
			.Text(LOCTEXT("CMPluginVersion", "CMPlugin Version:V1.3.2,  Only CMTracker_Client Versions later than 2.3.1(included) are supported. "))
			.Font(FEditorStyle::GetFontStyle("PropertyWindow.NormalFont"))
			.ShadowColorAndOpacity(FLinearColor::Black.CopyWithNewOpacity(0.3f))
			.ShadowOffset(FVector2D::UnitVector)
		]
		// stage notes region
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(FMargin(0.0f, 4.0f, 0.0f, 0.0f))
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				//----title----
				SNew(SBorder)
				.BorderImage(FEditorStyle::GetBrush("ToolPanel.GroupBorder"))
				.Padding(FMargin(4.0f, 4.0f))
				[
					SNew(STextBlock)
					.Text(LOCTEXT("StageNotesRegion", "StageNotes"))
					.Font(FEditorStyle::GetFontStyle("PropertyWindow.NormalFont"))
					.ShadowColorAndOpacity(FLinearColor::Black.CopyWithNewOpacity(0.3f))
					.ShadowOffset(FVector2D::UnitVector)
				]
			]
			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SVerticalBox)
				//----stage notes connection----
				+ SVerticalBox::Slot()
				.AutoHeight()
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.Padding(2.0f)
					.HAlign(HAlign_Left)
					[
						// Connect label
						SNew(STextBlock)
						.Text(LOCTEXT("StageNotesConnection", "Connect"))
						.Font(FEditorStyle::GetFontStyle("PropertyWindow.NormalFont"))
						.ShadowColorAndOpacity(FLinearColor::Black.CopyWithNewOpacity(0.3f))
						.ShadowOffset(FVector2D::UnitVector)
					]
					+ SHorizontalBox::Slot()
					.Padding(2.0f)
					.HAlign(HAlign_Left)
					[
						// Connect Checkbox
						SNew(SCheckBox)
						.OnCheckStateChanged(this, &SMotionCaptureEditorPanel::StageNotes_ConnectServer)
					]
				]
				//----server address----
				+ SVerticalBox::Slot()
				.AutoHeight()
				.Padding(FMargin(0.0f, 0.0f, 0.0f, 0.0f))
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.Padding(2.0f)
					.HAlign(HAlign_Left)
					[
						// server address label
						SNew(STextBlock)
						.Text(LOCTEXT("StageNotesAddress", "Address"))
						.Font(FEditorStyle::GetFontStyle("PropertyWindow.NormalFont"))
						.ShadowColorAndOpacity(FLinearColor::Black.CopyWithNewOpacity(0.3f))
						.ShadowOffset(FVector2D::UnitVector)
					]
					+ SHorizontalBox::Slot()
					.Padding(2.0f)
					.HAlign(HAlign_Fill)
					[
						// server address edit text
						SNew(SEditableTextBox)
						.HintText(LOCTEXT("StageNotesAddress_Hit", "StageNotesServer:Port"))
						.ToolTipText(LOCTEXT("StageNotesAddress_Tooltip", "Please input StageNotes server's IP address"))
						.Text(this, &SMotionCaptureEditorPanel::StageNotes_GetServerAddress)
						.OnTextCommitted(this, &SMotionCaptureEditorPanel::StageNotes_SetServerAddress)
						.OnTextChanged(this, &SMotionCaptureEditorPanel::StageNotes_SetServerAddress, ETextCommit::Default)
					]
				]
				//----Reqest FQN Button----
				+ SVerticalBox::Slot()
				.AutoHeight()
				.Padding(FMargin(0.0f, 0.0f, 0.0f, 0.0f))
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.Padding(2.0f)
					.HAlign(HAlign_Left)
					[
						// pull label
						SNew(STextBlock)
						.Text(LOCTEXT("StageNotesRequestLabel", "Request"))
						.Font(FEditorStyle::GetFontStyle("PropertyWindow.NormalFont"))
						.ShadowColorAndOpacity(FLinearColor::Black.CopyWithNewOpacity(0.3f))
						.ShadowOffset(FVector2D::UnitVector)
					]
					+ SHorizontalBox::Slot()
					.Padding(2.0f)
					.HAlign(HAlign_Fill)
					[
						// pull button
						SNew(SButton)
						.HAlign(HAlign_Center)
						.Text(LOCTEXT("StageNotesRequestButton", "Grab FQN"))
						//.ButtonStyle(FEditorStyle::Get(), "FlatButton.Success")
						//.ForegroundColor(FLinearColor::White)
						.OnClicked(this, &SMotionCaptureEditorPanel::StageNotes_GrabFQNFromServer)
					]
				]
				//--------FQN--------
				+ SVerticalBox::Slot()
				.AutoHeight()
				.Padding(FMargin(0.0f, 0.0f, 0.0f, 0.0f))
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.Padding(2.0f)
					.HAlign(HAlign_Left)
					[
						// FQN label
						SNew(STextBlock)
						.Text(LOCTEXT("StageNotesFQN", "FQN"))
						.Font(FEditorStyle::GetFontStyle("PropertyWindow.NormalFont"))
						.ShadowColorAndOpacity(FLinearColor::Black.CopyWithNewOpacity(0.3f))
						.ShadowOffset(FVector2D::UnitVector)
					]
					+ SHorizontalBox::Slot()
					.Padding(2.0f)
					.HAlign(HAlign_Fill)
					[
						// FQN text
						SNew(SEditableTextBox)
						.IsReadOnly(true)
						.Text(this, &SMotionCaptureEditorPanel::StageNotes_UpdateFQN)
					]
				]
			]
		]
		// Motion Capture region
		+ SVerticalBox::Slot()
		.Padding(FMargin(0.0f, 4.0f, 0.0f, 0.0f))
		[
			//----title----
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SBorder)
				.BorderImage(FEditorStyle::GetBrush("ToolPanel.GroupBorder"))
				.Padding(FMargin(4.0f, 4.0f))
				[
					SNew(STextBlock)
					.Text(LOCTEXT("MotionCaptureRegion", "MotionCapture"))
					.Font(FEditorStyle::GetFontStyle("PropertyWindow.NormalFont"))
					.ShadowColorAndOpacity(FLinearColor::Black.CopyWithNewOpacity(0.3f))
					.ShadowOffset(FVector2D::UnitVector)
				]
			]

		    //----character modele import setting----
			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.Padding(2.0f)
				.HAlign(HAlign_Left)
				[
					// IsForceX label
					SNew(STextBlock)
					.Text(LOCTEXT("IsForceX", "IsForceX"))
					.Font(FEditorStyle::GetFontStyle("PropertyWindow.NormalFont"))
					.ShadowColorAndOpacity(FLinearColor::Black.CopyWithNewOpacity(0.3f))
					.ShadowOffset(FVector2D::UnitVector)
				]
				+ SHorizontalBox::Slot()
				.Padding(2.0f)
				.HAlign(HAlign_Left)
				[
					// IsForceX Checkbox
					SNew(SCheckBox)
					.OnCheckStateChanged(this, &SMotionCaptureEditorPanel::IsForceFaceXaxis)
					.IsChecked(ECheckBoxState::Checked)
				]
			]

			//----motion capture connection----
			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.Padding(2.0f)
				.HAlign(HAlign_Left)
				[
					// Connect label
					SNew(STextBlock)
					.Text(LOCTEXT("MotionCaptureConnection", "Connect"))
					.Font(FEditorStyle::GetFontStyle("PropertyWindow.NormalFont"))
					.ShadowColorAndOpacity(FLinearColor::Black.CopyWithNewOpacity(0.3f))
					.ShadowOffset(FVector2D::UnitVector)
				]
			+ SHorizontalBox::Slot()
				.Padding(2.0f)
				.HAlign(HAlign_Left)
				[
					// Connect Checkbox
					SNew(SCheckBox)
					.OnCheckStateChanged(this, &SMotionCaptureEditorPanel::CMTracker_ConnectServer)
				]
			]
			//----server address----
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(FMargin(0.0f, 0.0f, 0.0f, 0.0f))
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.Padding(2.0f)
				.HAlign(HAlign_Left)
				[
					// server address label
					SNew(STextBlock)
					.Text(LOCTEXT("MotionCaptureAddress", "Address"))
					.Font(FEditorStyle::GetFontStyle("PropertyWindow.NormalFont"))
					.ShadowColorAndOpacity(FLinearColor::Black.CopyWithNewOpacity(0.3f))
					.ShadowOffset(FVector2D::UnitVector)
				]
				+ SHorizontalBox::Slot()
				.Padding(2.0f)
				.HAlign(HAlign_Fill)
				[
					// server address edit text
					SNew(SEditableTextBox)
					.HintText(LOCTEXT("CMTrackerAddress_Hit", "MCServer@192.168.3.110:3383"))
					.ToolTipText(LOCTEXT("CMTrackerAddress_Tooltip", "Please input CMTracker IP address"))
					.Text(this, &SMotionCaptureEditorPanel::CMTracker_GetServerAddress)
					.OnTextCommitted(this, &SMotionCaptureEditorPanel::CMTracker_SetServerAddress)
					.OnTextChanged(this, &SMotionCaptureEditorPanel::CMTracker_SetServerAddress, ETextCommit::Default)
				]
			]
			//----TimeCode----
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(FMargin(0.0f, 0.0f, 0.0f, 0.0f))
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.Padding(2.0f)
				.HAlign(HAlign_Left)
				[
					// TimeCode label
					SNew(STextBlock)
					.Text(LOCTEXT("TimeCodeLabel", "TimeCode"))
					.Font(FEditorStyle::GetFontStyle("PropertyWindow.NormalFont"))
					.ShadowColorAndOpacity(FLinearColor::Black.CopyWithNewOpacity(0.3f))
					.ShadowOffset(FVector2D::UnitVector)
				]
				+ SHorizontalBox::Slot()
				.Padding(2.0f)
				.HAlign(HAlign_Fill)
				[
					// TimeCode text
					SNew(SEditableTextBox)
					.IsReadOnly(true)
					.Text(this, &SMotionCaptureEditorPanel::CMTracker_UpdateTimeCode)
				]
			]
			//----Auto attach----
			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.Padding(2.0f)
				.HAlign(HAlign_Left)
				[
					// auto attach label
					SNew(STextBlock)
					.Text(LOCTEXT("AutoAttachLabel", "AutoAttach"))
					.Font(FEditorStyle::GetFontStyle("PropertyWindow.NormalFont"))
					.ShadowColorAndOpacity(FLinearColor::Black.CopyWithNewOpacity(0.3f))
					.ShadowOffset(FVector2D::UnitVector)
				]
				+ SHorizontalBox::Slot()
				.Padding(2.0f)
				.HAlign(HAlign_Left)
				[
					// auto attach Checkbox
					SNew(SCheckBox)
					.OnCheckStateChanged(this, &SMotionCaptureEditorPanel::CMTracker_AutoAttach)
				]
			]
			//----attach file----
			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.Padding(2.0f)
				.HAlign(HAlign_Left)
				[
					// attach file label
					SNew(STextBlock)
					.Text(LOCTEXT("AttachFileLabel", "AttachFile"))
					.Font(FEditorStyle::GetFontStyle("PropertyWindow.NormalFont"))
					.ShadowColorAndOpacity(FLinearColor::Black.CopyWithNewOpacity(0.3f))
					.ShadowOffset(FVector2D::UnitVector)
				]
				+ SHorizontalBox::Slot()
				.Padding(2.0f)
				.HAlign(HAlign_Fill)
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					[
						// attach file edit text
						SNew(SEditableTextBox)
						.HintText(LOCTEXT("AttachFile_Hit", "Content/Mocap/autoAttach.att"))
						.ToolTipText(LOCTEXT("AttachFile_Tooltip", "Please input attach file path"))
						.Text(this, &SMotionCaptureEditorPanel::CMTracker_GetAttachFile)
						.OnTextCommitted(this, &SMotionCaptureEditorPanel::CMTracker_SetAttachFile)
						.OnTextChanged(this, &SMotionCaptureEditorPanel::CMTracker_SetAttachFile, ETextCommit::Default)
					]
					+ SHorizontalBox::Slot()
					.Padding(FMargin(2.0f, 0.0f))
					.MaxWidth(150.f)
					[
						SNew(SHorizontalBox)
						+SHorizontalBox::Slot()
						.HAlign(HAlign_Fill)
						[
							// load attach button
							SNew(SButton)
							.HAlign(HAlign_Center)
							.Text(LOCTEXT("LoadAttachButton", "Load"))
							.OnClicked(this, &SMotionCaptureEditorPanel::CMTracker_LoadAttachFile)
						]
						+ SHorizontalBox::Slot()
						.HAlign(HAlign_Fill)
						[
							// append attach button
							SNew(SButton)
							.HAlign(HAlign_Center)
							.Text(LOCTEXT("AppendAttachButton", "Append"))
							.OnClicked(this, &SMotionCaptureEditorPanel::CMTracker_AppendAttachFile)
						]
					]
				]
			]
			//----attach button----
			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.HAlign(HAlign_Fill)
				[
					// attach button
					SNew(SButton)
					.HAlign(HAlign_Center)
					.Text(LOCTEXT("AttachButton", "Attach"))
					.OnClicked(this, &SMotionCaptureEditorPanel::CMTracker_AttachSubject)
				]
				+ SHorizontalBox::Slot()
				.HAlign(HAlign_Fill)
				[
					// detach button
					SNew(SButton)
					.HAlign(HAlign_Center)
					.Text(LOCTEXT("DetachButton", "Detach"))
					.OnClicked(this, &SMotionCaptureEditorPanel::CMTracker_DetachSubject)
				]
			]
			//----bone tree view----
			+ SVerticalBox::Slot()
			.VAlign(VAlign_Fill)
			[
				SNew(SScrollBox)
				.Orientation(Orient_Vertical)
				+ SScrollBox::Slot()
				[
					SubjectsTreeView->AsShared()
				]
			]
		]
	];
}

void SMotionCaptureEditorPanel::IsForceFaceXaxis(ECheckBoxState IsExpandedState)
{
	if (IsExpandedState == ECheckBoxState::Checked)
	{
		CMPlugin->IsCheckedForceFaceX = true;
	}
	else 
	{
		CMPlugin->IsCheckedForceFaceX = false;
	}
	/*FString s2 = "DebugPrint   DebugPrint  DebugPrint  DebugPrint  DebugPrint    ";
	UE_LOG(LogLoad, Log, TEXT(" %s  %b"), *s2, IsExpandedState);*/
}

void SMotionCaptureEditorPanel::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	/*if (CMPlugin->IsCheckedForceFaceX) 
	{
		FString s2 = "Tick  Tick  Tick  Tick   true ";
		int d =BonesInforFromClient_t.Num();
		UE_LOG(LogLoad, Log, TEXT(" %s   %d"), *s2,d);
	}
	else 
	{
		FString s2 = "Tick  Tick  Tick  Tick   fasle  fasle ";
		UE_LOG(LogLoad, Log, TEXT(" %s  "), *s2);
	}



	*/

	//if (FMotionCapture::IsResetHierarchy) 
	//{
	//	FString s2 = "Tick  Tick  Tick  Tick   true ";
	//	int d = BonesInforFromClient_t.Num();
	//	int d2 =FMotionCapture::BonesInforsFromClient.Num();
	//	UE_LOG(LogLoad, Log, TEXT(" %s   %d   %d"), *s2, d,d2);
	//}
	//else 
	//{
	//	FString s2 = "Tick  Tick  Tick  Tick   fasle  fasle ";
	//	int d2 = FMotionCapture::BonesInforsFromClient.Num();
	//	UE_LOG(LogLoad, Log, TEXT(" %s   %d "), *s2,d2);
	//}

	if (CMPlugin->IsResetPanelList && ConnectServer)
	{
		if (CMPlugin->BonesInforsFromClient.Num() > 0)
		{
			ResetPanelHierarchy();
			for (int i=0;i< CMPlugin->BonesInforsFromClient.Num();i++)
			{
				UpdatePanelHierarchy(CMPlugin->BonesInforsFromClient[i]);
			}
			/*int x = CMPlugin->BonesInforsFromClient.Num();
			UE_LOG(LogLoad, Log, TEXT("  %d"), x);*/
		}
		CMPlugin->IsResetPanelList = false;
	}


	if (RefreshSubjectsTree)
	{
		RefreshSubjectsTree = false;
		SubjectsTreeView->RequestTreeRefresh();
	}
}

void  SMotionCaptureEditorPanel::StageNotes_ConnectServer(ECheckBoxState IsExpandedState)
{
	ConnectStageNotes = (IsExpandedState == ECheckBoxState::Checked);

	CMPlugin->ConnectStageNotes(StageNotesAddress.ToString(), ConnectStageNotes);

	if (ConnectStageNotes)
	{
		GConfig->SetText(TEXT("/Script/ChingMU.Mocap"), TEXT("StageNotesAddress"), StageNotesAddress, GEditorIni);
		GConfig->Flush(false, GEditorIni);
	}		
}

FText SMotionCaptureEditorPanel::StageNotes_GetServerAddress() const
{
	return StageNotesAddress;
}

void SMotionCaptureEditorPanel::StageNotes_SetServerAddress(const FText& InText, ETextCommit::Type InCommitType)
{
	if(!ConnectStageNotes)
		StageNotesAddress = InText;
}

FReply SMotionCaptureEditorPanel::StageNotes_GrabFQNFromServer()
{
	FString TakeName;
	if (CMPlugin->ReceiveStageNotesTakeName(TakeName))
		StageNotesFQN = FText::FromString(TakeName);

	return FReply::Handled();
}

FText SMotionCaptureEditorPanel::StageNotes_UpdateFQN() const
{
	return StageNotesFQN;
}

void  SMotionCaptureEditorPanel::CMTracker_ConnectServer(ECheckBoxState IsExpandedState)
{
	if (IsExpandedState == ECheckBoxState::Checked)
	{
		/*
		CMPlugin->CMPluginStart();
		bool succeed = false;
		for (int i = 0; !succeed && i < 3; ++i)
		{
			FPlatformProcess::Sleep(0.2f);
			succeed = CMPlugin->CMPluginConnect(TCHAR_TO_ANSI(*CMPlugin->CMTrackerAddress.ToString()));
		}

		succeed = CMPlugin->CMPluginRegisterResetHier(TCHAR_TO_ANSI(*CMPlugin->CMTrackerAddress.ToString()), &SMotionCaptureEditorPanel::ResetHierarchy, &SMotionCaptureEditorPanel::ResetHierarchy);
		succeed &= CMPlugin->CMPluginRegisterUpdateHier(TCHAR_TO_ANSI(*CMPlugin->CMTrackerAddress.ToString()), &SMotionCaptureEditorPanel::UpdateHierarchy, &SMotionCaptureEditorPanel::UpdateHierarchy);
		if (succeed)
			GLog->Log(ELogVerbosity::Display, "ChingMU Retarget editor plane register callback succeed.");
		else
			GLog->Log(ELogVerbosity::Display, "ChingMU Retarget editor plane register callback failed.");

		GConfig->SetText(TEXT("/Script/ChingMU.Mocap"), TEXT("CMTrackerAddress"), CMPlugin->CMTrackerAddress, GEditorIni);
		GConfig->Flush(false, GEditorIni);
		ConnectServer = true;
		*/

		/*
		if (!CMPlugin->IsConnect)
		{
			CMPlugin->CMServerAddress = *CMPlugin->CMTrackerAddress.ToString();

		}
		else 
		{
			if (CMPlugin->BonesInforsFromClient.Num()>0)
			{
				CMPlugin->IsResetPanelList = true;
			}
		}
		GConfig->SetText(TEXT("/Script/ChingMU.Mocap"), TEXT("CMTrackerAddress"), CMPlugin->CMTrackerAddress, GEditorIni);
		GConfig->Flush(false, GEditorIni);
		ConnectServer = true;
		*/
		FString s2 = "Retarget Editor Pannel Start Connect And Register Event from ChingMU MotionCapRetargetPanel.....................    ";
		UE_LOG(LogTemp, Log, TEXT(" %s"), *s2);
		FString addr; 
		if (CMPlugin->CnState== ConnectState::NoConnect)
		{
			addr = *CMPlugin->CMTrackerAddress.ToString();
			if (addr.Contains("MCServer@") && addr.Contains("3884"))
			{
				CMPlugin->CMServerAddress = addr;
				CMPlugin->ConnectCommand = "ConnectServer";
			}
		}
		else 
		{
			FString s3 = CMPlugin->CMServerAddress +" it is Connected or Connecting,in ChingMU Commponnect from ChingMU MotionCapRetargetPanel";
			UE_LOG(LogTemp, Warning, TEXT(" %s"), *s3);
		}
		
		GConfig->SetText(TEXT("/Script/ChingMU.Mocap"), TEXT("CMTrackerAddress"), CMPlugin->CMTrackerAddress, GEditorIni);
		GConfig->Flush(false, GEditorIni);
		ConnectServer = true;

	}
	else
	{
		/*
		CMPlugin->CMPluginQuit();
		ConnectServer = false;

		CMPlugin->BindActors.Empty();
		CMPlugin->BindSkeletons.Empty();
		SelfRef->CMPlugin->SubjectData.Reset();
		SelfRef->AllItems.Reset();
		SelfRef->SubjectsTreeView->RequestTreeRefresh();
		*/

		/*
		ConnectServer = false;
		CMPlugin->BindActors.Empty();
		CMPlugin->BindSkeletons.Empty();
		SelfRef->CMPlugin->SubjectData.Reset();
		SelfRef->AllItems.Reset();
		SelfRef->SubjectsTreeView->RequestTreeRefresh();
		*/

		FString s2 = "Retarget Editor Pannel Start DisConnect And UnRegister Event from ChingMU MotionCapRetargetPanel.....................    ";
		UE_LOG(LogTemp, Log, TEXT(" %s"), *s2);
		FString addr;
		if (CMPlugin->CnState == ConnectState::Connected)
		{
			addr=CMPlugin->CMServerAddress;
			if (addr.Contains("MCServer@") && addr.Contains("3884"))
			{
				CMPlugin->ConnectCommand = "DisConnect";
			}	
		}
		else 
		{
			FString s3 = CMPlugin->CMServerAddress + " it is not Connected from ChingMU MotionCapRetargetPanel";
			UE_LOG(LogTemp, Warning, TEXT(" %s"), *s3);
		}
	
		ConnectServer = false;
		CMPlugin->BindActors.Empty();
		CMPlugin->BindSkeletons.Empty();
		SelfRef->CMPlugin->SubjectData.Reset();
		SelfRef->AllItems.Reset();
		SelfRef->SubjectsTreeView->RequestTreeRefresh();
	}
}

FText SMotionCaptureEditorPanel::CMTracker_GetServerAddress() const
{
	return CMPlugin->CMTrackerAddress;
}

void  SMotionCaptureEditorPanel::CMTracker_SetServerAddress(const FText& InText, ETextCommit::Type InCommitType)
{
	if (!ConnectServer)
		CMPlugin->CMTrackerAddress = InText;
}

FText SMotionCaptureEditorPanel::CMTracker_UpdateTimeCode() const
{
	if (!ConnectServer)
		return FText::FromString("00:00:00:00");
	
	VrpnTimeCode tc = CMPlugin->CMPluginTimeCode(TCHAR_TO_ANSI(*CMPlugin->CMTrackerAddress.ToString()));
	if(!tc.ttc.valid)
		return FText::FromString("00:00:00:00");

	char tcStr[100];
	sprintf_s(tcStr, "%02d:%02d:%02d:%02d", tc.ttc.hours, tc.ttc.minutes, tc.ttc.seconds, tc.ttc.frames);
	return FText::FromString(tcStr);
}

void  SMotionCaptureEditorPanel::CMTracker_AutoAttach(ECheckBoxState IsExpandedState)
{
	if (IsExpandedState == ECheckBoxState::Checked)
	{
		AutoAttach = true;
		
		for (const FMotionCaptureSubjectItemPtr& Item : CMPlugin->SubjectData)
		{
			if (!Item->AttachSubjectName.IsEmpty())
				continue;
			
			AActor *AttachActor = FindActorByName(Item->SubjectName);
			if (!AttachActor)
				continue;

			FString ActorLabel = AttachActor->GetActorLabel();
			FMotionCaptureSubjectKey trackerID = Item->SubjectKey;
			if (trackerID < 100)
			{
				// bind tracker
				CMPlugin->BindActors.Add(trackerID) = ActorLabel;
			}
			else
			{
				// bind human skeleton
				FMotionCaptureSubjectKey humanID = (trackerID - 100) / 150;
				CMPlugin->BindSkeletons.Add(humanID) = ActorLabel;
			}

			// new subject item for change AttachSubjectName
			FMotionCaptureSubjectItemPtr Source = MakeShared<FMotionCaptureSubjectItem>();
			Source->AttachSubjectName = FText::FromString(ActorLabel);
			Source->SubjectKey = Item->SubjectKey;
			Source->SubjectName = Item->SubjectName;
			Source->Parent = Item->Parent;
			Source->Children = Item->Children;
			// add the subject item to the array
			int Index = CMPlugin->SubjectData.Find(Item);
			if (INDEX_NONE != Index)
				CMPlugin->SubjectData[Index] = Source;
			SubjectsTreeView->RequestListRefresh();
		}
	}
	else
	{
		AutoAttach = false;
	}
}

FText SMotionCaptureEditorPanel::CMTracker_GetAttachFile() const
{
	return AttachFilePath;
}

void  SMotionCaptureEditorPanel::CMTracker_SetAttachFile(const FText& InText, ETextCommit::Type InCommitType)
{
	AttachFilePath = InText;
}

FReply SMotionCaptureEditorPanel::CMTracker_LoadAttachFile()
{
	FString AttachFileFullPath = FPaths::ProjectDir();
	AttachFileFullPath += AttachFilePath.ToString();

	FILE *AttachFile;
	if (fopen_s(&AttachFile, TCHAR_TO_ANSI(*AttachFileFullPath), "rt"))
		return FReply::Handled();

	CMPlugin->BindActors.Reset();
	CMPlugin->BindSkeletons.Reset();
	GConfig->SetText(TEXT("/Script/ChingMU.Mocap"), TEXT("AttachFilePath"), AttachFilePath, GEditorIni);
	GConfig->Flush(false, GEditorIni);

	char Line[1000];
	char TrackerSkeleton[1000];
	char MocapName[1000];
	char ModelName[1000];
	while (!feof(AttachFile))
	{
		if (NULL == fgets(Line, sizeof(Line), AttachFile))
			break;
		if (sscanf_s(Line, "%s %s %s",  TrackerSkeleton, 1000,  MocapName, 1000, ModelName, 1000) <= 0)
			break;

		FText MocapTextName = FText::FromString(MocapName);
		FText ModelTextName = FText::FromString(ModelName);
		if (!strcmp(TrackerSkeleton, "Tracker"))
		{
			for (auto &item : CMPlugin->SubjectData)
			{
				if (item->SubjectKey < 100 && item->SubjectName.EqualTo(MocapTextName))
				{
					if (!CMPlugin->BindActors.Find(item->SubjectKey))
						CMPlugin->BindActors.Add(item->SubjectKey) = ModelName;
					else
						CMPlugin->BindActors[item->SubjectKey] = ModelName;

					// new subject item for change AttachSubjectName
					FMotionCaptureSubjectItemPtr Source = MakeShared<FMotionCaptureSubjectItem>();
					Source->AttachSubjectName = ModelTextName;
					Source->SubjectKey = item->SubjectKey;
					Source->SubjectName = item->SubjectName;
					Source->Parent = item->Parent;
					Source->Children = item->Children;
					// add the subject item to the array
					int Index = CMPlugin->SubjectData.Find(item);
					if (INDEX_NONE != Index)
						CMPlugin->SubjectData[Index] = Source;
					SubjectsTreeView->RequestListRefresh();
					break;
				}
			}
		}
		else if (!strcmp(TrackerSkeleton, "Skeleton"))
		{
			for (auto &item : CMPlugin->SubjectData)
			{
				if (item->SubjectKey >= 100 && item->SubjectName.EqualTo(MocapTextName))
				{
					FMotionCaptureSubjectKey humanID = (item->SubjectKey - 100) / 150;
					if (!CMPlugin->BindSkeletons.Find(humanID))
						CMPlugin->BindSkeletons.Add(humanID) = ModelName;
					else
						CMPlugin->BindSkeletons.Add(humanID) = ModelName;
					
					// new subject item for change AttachSubjectName
					FMotionCaptureSubjectItemPtr Source = MakeShared<FMotionCaptureSubjectItem>();
					Source->AttachSubjectName = ModelTextName;
					Source->SubjectKey = item->SubjectKey;
					Source->SubjectName = item->SubjectName;
					Source->Parent = item->Parent;
					Source->Children = item->Children;
					// add the subject item to the array
					int Index = CMPlugin->SubjectData.Find(item);
					if (INDEX_NONE != Index)
						CMPlugin->SubjectData[Index] = Source;
					SubjectsTreeView->RequestListRefresh();
					break;
				}
			}
		}
	}

	fclose(AttachFile);
	return FReply::Handled();
}

FReply SMotionCaptureEditorPanel::CMTracker_AppendAttachFile()
{
	FString AttachFileFullPath = FPaths::ProjectDir();
	AttachFileFullPath += AttachFilePath.ToString();

	FILE *AttachFile;
	if (fopen_s(&AttachFile, TCHAR_TO_ANSI(*AttachFileFullPath), "at"))
		return FReply::Handled();

	GConfig->SetText(TEXT("/Script/ChingMU.Mocap"), TEXT("AttachFilePath"), AttachFilePath, GEditorIni);
	GConfig->Flush(false, GEditorIni);
	// Get selected subjects
	TArray<FMotionCaptureSubjectItemPtr> SelectedItems = SubjectsTreeView->GetSelectedItems();
	for (const FMotionCaptureSubjectItemPtr& SelectedItem : SelectedItems)
	{
		FMotionCaptureSubjectKey trackerID = SelectedItem->SubjectKey;
		if (SelectedItem->AttachSubjectName.IsEmpty())
			continue;

		if (trackerID < 100)
		{
			fprintf_s(AttachFile, "Tracker %s %s\n", 
				TCHAR_TO_ANSI(*SelectedItem->SubjectName.ToString()),
				TCHAR_TO_ANSI(*SelectedItem->AttachSubjectName.ToString()));
		}
		else
		{
			FMotionCaptureSubjectKey humanID = (trackerID - 100) / 150;
			fprintf_s(AttachFile, "Skeleton %s %s\n", 
				TCHAR_TO_ANSI(*SelectedItem->SubjectName.ToString()),
				TCHAR_TO_ANSI(*SelectedItem->AttachSubjectName.ToString()));
		}
		break;
	}

	fclose(AttachFile);
	return FReply::Handled();
}

FReply SMotionCaptureEditorPanel::CMTracker_AttachSubject()
{
	// Get Editor selected actors
	AActor *SelectedActor = NULL;
	USelection* SelectedActors = GEditor->GetSelectedActors();
	for (FSelectionIterator Iter(*SelectedActors); Iter; ++Iter)
	{
		SelectedActor = Cast<AActor>(*Iter);
		break;
	}
	if (!SelectedActor)
		return FReply::Handled();

	// Get selected subjects
	FString ActorName = SelectedActor->GetName();
	FString ActorLabel = SelectedActor->GetActorLabel();
	TArray<FMotionCaptureSubjectItemPtr> SelectedItems = SubjectsTreeView->GetSelectedItems();
	for (const FMotionCaptureSubjectItemPtr& SelectedItem : SelectedItems)
	{
		FMotionCaptureSubjectKey trackerID = SelectedItem->SubjectKey;
		int SelectIndex = CMPlugin->SubjectData.Find(SelectedItem);
		if (INDEX_NONE != SelectIndex)
		{
			if (trackerID < 100)
			{
				// bind tracker
				CMPlugin->BindActors.Add(trackerID) = ActorLabel;
			}
			else
			{
				// bind human skeleton
				FMotionCaptureSubjectKey humanID = (trackerID - 100) / 150;
				CMPlugin->BindSkeletons.Add(humanID) = ActorLabel;
			}

			FMotionCaptureSubjectItemPtr Source = MakeShared<FMotionCaptureSubjectItem>();
			Source->AttachSubjectName = FText::FromString(ActorLabel);
			Source->SubjectKey = CMPlugin->SubjectData[SelectIndex]->SubjectKey;
			Source->SubjectName = CMPlugin->SubjectData[SelectIndex]->SubjectName;
			Source->Parent = CMPlugin->SubjectData[SelectIndex]->Parent;
			Source->Children = CMPlugin->SubjectData[SelectIndex]->Children;
			CMPlugin->SubjectData[SelectIndex] = Source;

			SubjectsTreeView->SetItemSelection(Source, true);
			SubjectsTreeView->RequestListRefresh();
		}

		break;
	}

	return FReply::Handled();
}

FReply SMotionCaptureEditorPanel::CMTracker_DetachSubject()
{
	// Get selected subjects
	TArray<FMotionCaptureSubjectItemPtr> SelectedItems = SubjectsTreeView->GetSelectedItems();
	for (const FMotionCaptureSubjectItemPtr& SelectedItem : SelectedItems)
	{
		FMotionCaptureSubjectKey trackerID = SelectedItem->SubjectKey;
		int SelectIndex = CMPlugin->SubjectData.Find(SelectedItem);
		if (INDEX_NONE != SelectIndex)
		{
			if (trackerID < 100)
			{
				// bind tracker
				CMPlugin->BindActors.Remove(trackerID);
			}
			else
			{
				// bind human skeleton
				FMotionCaptureSubjectKey humanID = (trackerID - 100) / 150;
				CMPlugin->BindSkeletons.Remove(humanID);
			}

			FMotionCaptureSubjectItemPtr Source = MakeShared<FMotionCaptureSubjectItem>();
			Source->AttachSubjectName = FText();
			Source->SubjectKey = CMPlugin->SubjectData[SelectIndex]->SubjectKey;
			Source->SubjectName = CMPlugin->SubjectData[SelectIndex]->SubjectName;
			Source->Parent = CMPlugin->SubjectData[SelectIndex]->Parent;
			Source->Children = CMPlugin->SubjectData[SelectIndex]->Children;
			CMPlugin->SubjectData[SelectIndex] = Source;

			SubjectsTreeView->SetItemSelection(Source, true);
			SubjectsTreeView->RequestListRefresh();
		}

		break;
	}

	return FReply::Handled();
}

void  SMotionCaptureEditorPanel::ResetPanelHierarchy()
{
	if (!SelfRef)
		return;

	TArray<FMotionCaptureSubjectItemPtr> SelectedItems = SelfRef->SubjectsTreeView->GetSelectedItems();
	for (const FMotionCaptureSubjectItemPtr& SelectedItem : SelectedItems)
	{
		SelfRef->SelectedSubject = SelectedItem->SubjectKey;
		break;
	}

	SelfRef->CMPlugin->SubjectData.Reset();
	SelfRef->AllItems.Reset();
	SelfRef->RefreshSubjectsTree = true;
}



void __stdcall SMotionCaptureEditorPanel::ResetHierarchy(void *ref, VrpnResetHier b)
{
	if (!SelfRef)
		return;

	TArray<FMotionCaptureSubjectItemPtr> SelectedItems = SelfRef->SubjectsTreeView->GetSelectedItems();
	for (const FMotionCaptureSubjectItemPtr& SelectedItem : SelectedItems)
	{
		SelfRef->SelectedSubject = SelectedItem->SubjectKey;
		break;
	}

	SelfRef->CMPlugin->SubjectData.Reset();
	SelfRef->AllItems.Reset();
	SelfRef->RefreshSubjectsTree = true;
}

void  SMotionCaptureEditorPanel::UpdatePanelHierarchy(CallbackReturnHierarchyResult hierarchy)
{
	if (!SelfRef)
		return;

	// add new subject to tree view
	FMotionCaptureSubjectItemPtr Source = MakeShared<FMotionCaptureSubjectItem>();
	Source->SubjectKey = hierarchy.sensor;
	Source->SubjectName = hierarchy.name;
	SelfRef->AllItems.Add(Source);
	if (hierarchy.parent != -1)
	{
		for (const FMotionCaptureSubjectItemPtr& Item : SelfRef->AllItems)
		{
			if (Item->SubjectKey == hierarchy.parent)
			{
				Item->Children.Add(Source);
				break;
			}
		}
	}
	else
	{
		// restore bind subject name
		if (hierarchy.sensor < 100)
		{
			for (auto Actor : SelfRef->CMPlugin->BindActors)
			{
				if (Actor.Key == hierarchy.sensor)
				{
					Source->AttachSubjectName = FText::FromString(Actor.Value);
					break;
				}
			}
		}
		else
		{
			FMotionCaptureSubjectKey humanID = (hierarchy.sensor - 100) / 150;
			for (auto Skeleton : SelfRef->CMPlugin->BindSkeletons)
			{
				if (Skeleton.Key == humanID)
				{
					Source->AttachSubjectName = FText::FromString(Skeleton.Value);
					break;
				}
			}
		}

		// auto attach
		if (SelfRef->AutoAttach)
		{
			AActor *AttachActor = SelfRef->FindActorByName(Source->SubjectName);
			if (AttachActor)
			{
				FString ActorLabel = AttachActor->GetActorLabel();
				Source->AttachSubjectName = FText::FromString(ActorLabel);
				if (hierarchy.sensor < 100)
				{
					SelfRef->CMPlugin->BindActors.Add(hierarchy.sensor) = ActorLabel;
				}
				else
				{
					FMotionCaptureSubjectKey humanID = (hierarchy.sensor - 100) / 150;
					SelfRef->CMPlugin->BindSkeletons.Add(humanID) = ActorLabel;
				}
			}
		}
		SelfRef->CMPlugin->SubjectData.Add(Source);
	}

	// recovery the old selection
	for (const FMotionCaptureSubjectItemPtr& Item : SelfRef->AllItems)
	{
		if (Item->SubjectKey == SelfRef->SelectedSubject)
		{
			SelfRef->SubjectsTreeView->SetItemSelection(Item, true);
			break;
		}
	}

	//SelfRef->SubjectsTreeView->RequestTreeRefresh();
	SelfRef->RefreshSubjectsTree = true;
}


void __stdcall SMotionCaptureEditorPanel::UpdateHierarchy(void *ref, VrpnHier hierarchy)
{
	if (!SelfRef)
		return;

	// add new subject to tree view
	FMotionCaptureSubjectItemPtr Source = MakeShared<FMotionCaptureSubjectItem>();
	Source->SubjectKey = hierarchy.sensor;
	Source->SubjectName = FText::FromString(hierarchy.name);
	SelfRef->AllItems.Add(Source);
	if (hierarchy.parent != -1)
	{
		for (const FMotionCaptureSubjectItemPtr& Item : SelfRef->AllItems)
		{
			if (Item->SubjectKey == hierarchy.parent)
			{
				Item->Children.Add(Source);
				break;
			}
		}
	}
	else
	{
		// restore bind subject name
		if (hierarchy.sensor < 100)
		{
			for (auto Actor : SelfRef->CMPlugin->BindActors)
			{
				if (Actor.Key == hierarchy.sensor)
				{
					Source->AttachSubjectName = FText::FromString(Actor.Value);
					break;
				}
			}
		}
		else
		{
			FMotionCaptureSubjectKey humanID = (hierarchy.sensor - 100) / 150;
			for (auto Skeleton : SelfRef->CMPlugin->BindSkeletons)
			{
				if (Skeleton.Key == humanID)
				{
					Source->AttachSubjectName = FText::FromString(Skeleton.Value);
					break;
				}
			}
		}

		// auto attach
		if (SelfRef->AutoAttach)
		{
			AActor *AttachActor = SelfRef->FindActorByName(Source->SubjectName);
			if (AttachActor)
			{
				FString ActorLabel = AttachActor->GetActorLabel();
				Source->AttachSubjectName = FText::FromString(ActorLabel);
				if (hierarchy.sensor < 100)
				{
					SelfRef->CMPlugin->BindActors.Add(hierarchy.sensor) = ActorLabel;
				}
				else
				{
					FMotionCaptureSubjectKey humanID = (hierarchy.sensor - 100) / 150;
					SelfRef->CMPlugin->BindSkeletons.Add(humanID) = ActorLabel;
				}
			}
		}
		SelfRef->CMPlugin->SubjectData.Add(Source);
	}

	// recovery the old selection
	for (const FMotionCaptureSubjectItemPtr& Item : SelfRef->AllItems)
	{
		if (Item->SubjectKey == SelfRef->SelectedSubject)
		{
			SelfRef->SubjectsTreeView->SetItemSelection(Item, true);
			break;
		}
	}

	//SelfRef->SubjectsTreeView->RequestTreeRefresh();
	SelfRef->RefreshSubjectsTree = true;
}

AActor* SMotionCaptureEditorPanel::FindActorByName(const FText& InText)
{
	UWorld *World = GEditor->GetEditorWorldContext().World();
	for (ULevel const* Level : World->GetLevels())
	{
		if (!Level)
			continue;

		for (AActor* Actor : Level->Actors)
		{
			if (!Actor)
				continue;

			if (Actor->GetActorLabel().ToLower().Contains(InText.ToString().ToLower())
				|| InText.ToString().ToLower().Contains(Actor->GetActorLabel().ToLower()))
			{
				return Actor;
			}
		}
	}

	return NULL;
}

void SMotionCaptureEditorPanel::RebuildSubjectList()
{
	TArray<FMotionCaptureSubjectKey> SavedSelection;
	TArray<FMotionCaptureSubjectItemPtr> SelectedItems = SubjectsTreeView->GetSelectedItems();
	for (const FMotionCaptureSubjectItemPtr& SelectedItem : SelectedItems)
	{
		SavedSelection.Add(SelectedItem->SubjectKey);
	}

	CMPlugin->SubjectData.Reset();
	for (const FMotionCaptureSubjectItemPtr& Item : AllItems)
	{
		if (Item->Parent == NULL)
		{
			CMPlugin->SubjectData.Add(Item);
		}
	}

	for (const FMotionCaptureSubjectItemPtr& Item : AllItems)
	{
		for (FMotionCaptureSubjectKey& Selection : SavedSelection)
		{
			if (Item->SubjectKey == Selection)
			{
				SubjectsTreeView->SetItemSelection(Item, true);
				break;
			}
		}
	}

	SubjectsTreeView->RequestTreeRefresh();
}

TSharedRef<ITableRow> SMotionCaptureEditorPanel::MakeTreeRowWidget(FMotionCaptureSubjectItemPtr InInfo, const TSharedRef<STableViewBase>& OwnerTable)
{
	return SNew(STableRow<FMotionCaptureSubjectItemPtr>, OwnerTable)
		.Content()
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.HAlign(HAlign_Left)
			.FillWidth(0.1f)
			[
				SNew(STextBlock)
				.Text(FText::FromString(FString::FromInt(InInfo->SubjectKey)))
			]
			+ SHorizontalBox::Slot()
			.HAlign(HAlign_Left)
			.FillWidth(0.4f)
			[
				SNew(STextBlock)
				.Text(InInfo->SubjectName)
			]
			+ SHorizontalBox::Slot()
			.HAlign(HAlign_Left)
			.FillWidth(0.5f)
			[
				SNew(STextBlock)
				.Text(InInfo->AttachSubjectName)
			]
		];
}

void SMotionCaptureEditorPanel::GetChildrenForInfo(FMotionCaptureSubjectItemPtr InInfo, TArray< FMotionCaptureSubjectItemPtr >& OutChildren)
{
	OutChildren = InInfo->Children;
}

void SMotionCaptureEditorPanel::OnSelectionChanged(FMotionCaptureSubjectItemPtr SubjectEntry, ESelectInfo::Type SelectInfo)
{
	if (SubjectEntry.IsValid())
	{
		SelectedSubject = SubjectEntry->SubjectKey;
	}
}

#undef LOCTEXT_NAMESPACE

