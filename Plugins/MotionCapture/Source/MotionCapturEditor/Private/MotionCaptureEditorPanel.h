// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Widgets/SCompoundWidget.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/Views/SListView.h"
#include "Widgets/Views/STreeView.h"
#include "Components/PoseableMeshComponent.h"
#include "EditorUndoClient.h"
#include "FMotionCapture.h"

class ITableRow;
class STableViewBase;
class FMenuBuilder;
class FUICommandList;



class SMotionCaptureEditorPanel : public SCompoundWidget
{
	SLATE_BEGIN_ARGS(SMotionCaptureEditorPanel)
	{
	}

	SLATE_END_ARGS()

	~SMotionCaptureEditorPanel();

	void Construct(const FArguments& Args, FMotionCapture* InClient);

	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime);

private:
	/// StageNotes
	void  StageNotes_ConnectServer(ECheckBoxState IsExpandedState);
	FText StageNotes_GetServerAddress() const;
	void  StageNotes_SetServerAddress(const FText& InText, ETextCommit::Type InCommitType);
	FReply StageNotes_GrabFQNFromServer();
	FText StageNotes_UpdateFQN() const;

	/// CMTracker
	void  CMTracker_ConnectServer(ECheckBoxState IsExpandedState);
	FText CMTracker_GetServerAddress() const;
	void  CMTracker_SetServerAddress(const FText& InText, ETextCommit::Type InCommitType);
	FText CMTracker_UpdateTimeCode() const;
	void  CMTracker_AutoAttach(ECheckBoxState IsExpandedState);
	FText CMTracker_GetAttachFile() const;
	void  CMTracker_SetAttachFile(const FText& InText, ETextCommit::Type InCommitType);
	FReply CMTracker_LoadAttachFile();
	FReply CMTracker_AppendAttachFile();
	FReply CMTracker_AttachSubject();
	FReply CMTracker_DetachSubject();


	void IsForceFaceXaxis(ECheckBoxState IsExpandedState);
	void ResetPanelHierarchy();
	void UpdatePanelHierarchy(CallbackReturnHierarchyResult hierarchy);


	/// Subject tree view
	static void __stdcall ResetHierarchy(void*, VrpnResetHier b);
	static void __stdcall UpdateHierarchy(void*, VrpnHier tp);

	

	AActor* FindActorByName(const FText& InText);
	void RebuildSubjectList();
	TSharedRef<ITableRow> MakeTreeRowWidget(FMotionCaptureSubjectItemPtr InInfo, const TSharedRef<STableViewBase>& OwnerTable);
	void GetChildrenForInfo(FMotionCaptureSubjectItemPtr InInfo, TArray< FMotionCaptureSubjectItemPtr >& OutChildren);
	void OnSelectionChanged(FMotionCaptureSubjectItemPtr BoneInfo, ESelectInfo::Type SelectInfo);


private:
	static SMotionCaptureEditorPanel* SelfRef;
	FMotionCapture* CMPlugin;

	/// StageNotes
	bool  ConnectStageNotes;
	FText StageNotesAddress;
	FText StageNotesFQN;
	/// CMTracker
	bool ConnectServer;
	bool AutoAttach;
	FText AttachFilePath;
	


	//Subject tree widget
	volatile bool RefreshSubjectsTree;
	TSharedPtr<STreeView<FMotionCaptureSubjectItemPtr>> SubjectsTreeView;
	//Subject tree items
	TArray<FMotionCaptureSubjectItemPtr> AllItems;
	FMotionCaptureSubjectKey SelectedSubject;

};
