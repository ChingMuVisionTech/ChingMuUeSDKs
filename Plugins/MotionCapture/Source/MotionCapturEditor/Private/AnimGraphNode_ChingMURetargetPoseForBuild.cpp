// Copyright 2015-2019 ChingMU, Inc. All Rights Reserved.
#include "AnimGraphNode_ChingMURetargetPoseForBuild.h"
#include "MotionCaptureEditorPCH.h"

#define LOCTEXT_NAMESPACE "ChingMUAnimNode_OutWithRetargetPoseForBuild"

UAnimGraphNode_ChingMURetargetPoseForBuild::UAnimGraphNode_ChingMURetargetPoseForBuild(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

FText UAnimGraphNode_ChingMURetargetPoseForBuild::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return LOCTEXT("NodeTitle", "ChingMURetargetPoseForBuild");
}

FText UAnimGraphNode_ChingMURetargetPoseForBuild::GetTooltipText() const
{
	return LOCTEXT("NodeTooltip", "Output ChingMURetarget Pose for runtime");
}

FText UAnimGraphNode_ChingMURetargetPoseForBuild::GetMenuCategory() const
{
	return LOCTEXT("NodeCategory", "ChingMU");
}


FLinearColor UAnimGraphNode_ChingMURetargetPoseForBuild::GetNodeTitleColor() const
{
	return FLinearColor(0.0f, 1.0f, 1.0f);
}

#undef LOCTEXT_NAMESPACE