#include "MotionCaptureEditorPCH.h"
#include "AnimGraphNode_ChingMUPose.h"

#define LOCTEXT_NAMESPACE "ChingMUAnimNode_OutWithoutRetargetPose"

UAnimGraphNode_ChingMUPose::UAnimGraphNode_ChingMUPose(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

FText UAnimGraphNode_ChingMUPose::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return LOCTEXT("NodeTitle", "ChingMU Pose");
}

FText UAnimGraphNode_ChingMUPose::GetTooltipText() const
{
	return LOCTEXT("NodeTooltip", "this node use to return the current Without Retarget pose from ChingMUTrackSystem ");
}

FText UAnimGraphNode_ChingMUPose::GetMenuCategory() const
{
	return LOCTEXT("NodeCategory", "ChingMU");
}

FLinearColor UAnimGraphNode_ChingMUPose::GetNodeTitleColor() const
{
	return FLinearColor(1.0f, 1.0f, 0.0f);
}

#undef LOCTEXT_NAMESPACE