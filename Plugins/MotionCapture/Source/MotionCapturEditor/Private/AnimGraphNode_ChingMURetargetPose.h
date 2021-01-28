// Copyright 2015-2019 ChingMU, Inc. All Rights Reserved.

#pragma once

#include "AnimGraphNode_Base.h"
#include "AnimNode_ChingMURetargetPose.h"

#include "AnimGraphNode_ChingMURetargetPose.generated.h"
UCLASS()
class UAnimGraphNode_ChingMURetargetPose : public UAnimGraphNode_Base
{
	GENERATED_UCLASS_BODY()

		UPROPERTY(EditAnywhere, Category = Settings)
		FAnimNode_ChingMURetargetPose Node;

	// UEdGraphNode interface
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FText GetTooltipText() const override;
	virtual FText GetMenuCategory() const override;
	virtual FLinearColor GetNodeTitleColor() const override;
	// End of UEdGraphNode
};