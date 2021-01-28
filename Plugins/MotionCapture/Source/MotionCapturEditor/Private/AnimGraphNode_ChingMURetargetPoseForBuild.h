#pragma once


#include "AnimGraphNode_Base.h"
//#include "AnimNode_ChingMURetargetPose.h"
#include "AnimNode_ChingMURetargetPoseForBuild.h "
#include "AnimGraphNode_ChingMURetargetPoseForBuild.generated.h"
UCLASS()
class UAnimGraphNode_ChingMURetargetPoseForBuild : public UAnimGraphNode_Base
{
	GENERATED_UCLASS_BODY()

		UPROPERTY(EditAnywhere, Category = Settings)
		FAnimNode_ChingMURetargetPoseForBuild Node;
	
	// UEdGraphNode interface
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FText GetTooltipText() const override;
	virtual FText GetMenuCategory() const override;
	virtual FLinearColor GetNodeTitleColor() const override;
	// End of UEdGraphNode
};
