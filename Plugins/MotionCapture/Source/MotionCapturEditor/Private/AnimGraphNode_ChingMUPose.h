#pragma once
#include "AnimGraphNode_Base.h"
#include "AnimNode_ChingMUPose.h"

#include "AnimGraphNode_ChingMUPose.generated.h"

UCLASS()
class UAnimGraphNode_ChingMUPose : public UAnimGraphNode_Base
{
	GENERATED_UCLASS_BODY()

		/*
		每个动画蓝图都由两部分组成，第一部分称之为运行时动画节点，它的作用是，在运行时，计算角色每帧的pose; 所以这部分代码可以脱离编辑器运行；比如这个例子中的FAnimNode_ChingMUPose
		所有的运行时动画节点都必须继承于FAnimNode_Base；
		第二部分称为显示动画节点，它的作用是定义动画节点如何动画蓝图中显示，只有这样你才能在动画蓝图中使用它。比如这个例子中的UAnimGraphNode_ChingMUPose
		所有的显示动画节点都必须继承于UAnimGraphNode_Base；
		它有以下一些函数，可以让自定义（显示）动画节点继承并重写，实现自己的内容，例如以下一些函数：

*/
	//注意：在x显示动画节点中必须持有一个x运行时动画节点的实例；
	UPROPERTY(EditAnywhere, Category = Settings)
	FAnimNode_ChingMUPose Node;


	// UEdGraphNode interface
	//定义：此动画节点的名字
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	//定义：当鼠标悬停在此动画节点上面，显示关于此节点的一些信息，比如此节点的作用；
	virtual FText GetTooltipText() const override;
	//定义：此动画节点属于哪个种类，同一种类的动画节点，一般在编辑器中显示在同一分组下；
	virtual FText GetMenuCategory() const override;
	//定义：此动画节点的颜色；
	virtual FLinearColor GetNodeTitleColor() const override;
	// End of UEdGraphNode
};
