// Copyright 2015-2019 ChingMU, Inc. All Rights Reserved.
#include "MotionCaptureEditorPCH.h"
#include "AnimGraphNode_ChingMURetargetPose.h"

#define LOCTEXT_NAMESPACE "ChingMUAnimNode_OutWithRetargetPose"

UAnimGraphNode_ChingMURetargetPose::UAnimGraphNode_ChingMURetargetPose(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

FText UAnimGraphNode_ChingMURetargetPose::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return LOCTEXT("NodeTitle", "ChingMURetarget Pose");
}

FText UAnimGraphNode_ChingMURetargetPose::GetTooltipText() const
{
	return LOCTEXT("NodeTooltip", "Retrieves the current pose associated with the supplied subject");
}

FText UAnimGraphNode_ChingMURetargetPose::GetMenuCategory() const
{
	return LOCTEXT("NodeCategory", "ChingMU");
}


FLinearColor UAnimGraphNode_ChingMURetargetPose::GetNodeTitleColor() const
{
	return FLinearColor(0.0f, 1.0f, 1.0f);
}

#undef LOCTEXT_NAMESPACE