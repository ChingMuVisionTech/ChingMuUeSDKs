#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNodeBase.h"
#include "ChingMUComponent.h"
class  UChingMUComponent;
#include "AnimNode_ChingMUPose.generated.h"


USTRUCT(BlueprintInternalUseOnly)
struct MOTIONCAPTURE_API FAnimNode_ChingMUPose : public FAnimNode_Base
{
	GENERATED_USTRUCT_BODY()

		/*
每个动画蓝图都由两部分组成，
第一部分称为显示动画节点，它的作用是定义动画节点如何动画蓝图中显示，只有这样你才能在动画蓝图中使用它。比如这个例子中的UAnimGraphNode_ChingMUPose
所有的显示动画节点都必须继承于UAnimGraphNode_Base；
第二部分称之为运行时动画节点，它的作用是，在运行时，计算角色每帧的pose; 所以这部分代码可以脱离编辑器运行；比如这个例子中的FAnimNode_ChingMUPose
所有的运行时动画节点都必须继承于FAnimNode_Base；
它有以下一些函数，可以让自定义运行时动画节点继承并重写，实现自己的内容，例如以下一些函数：

*/

	//公开此节点的相关属性，这里所说的属性是指该节点的输入参数，同时也是此节点工作时要引用的东西，Category，该参数的所属分类，PinShownByDefault，此属性会默认显示；
    /** Set server ip */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ChingMU_SeverAddr, meta = (PinShownByDefault))
	FString  SeverAddr;

	//公开此节点的相关属性
	/** Set human id */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ChingMU_HumanId, meta = (PinShownByDefault))
    int32 HuamnId;

	FAnimNode_ChingMUPose();//当此节点实例化的时候（也就是将此节点拖入到动画蓝图中，或者打开此节点所在的动画蓝图，或者指定/开始引用此节点所在的动画蓝图时，就会调用此构造函数）

	// FAnimNode_Base interface 在此节点被构造之后（也就是此节点的构造函数运行之后），会首先调用此函数，用来初始化一些此动画节点所需的东西；
	virtual void Initialize_AnyThread(const FAnimationInitializeContext& Context) override;

	// virtual void CacheBones_AnyThread(const FAnimationCacheBonesContext & Context) {};

	//此节点在Initialize_AnyThread之后被每帧调用，用来更新一些东西，这些东西可能是，要计算角色pose时要用的东西；
	virtual void Update_AnyThread(const FAnimationUpdateContext & Context) override;

	//此节点运行在Update_AnyThread之后被每帧调用，用来计算每帧骨架上所有骨骼的pose (这个函数输出的Output pose,是最终的骨骼空间下的pose，动画蓝图中最终输出的pse为骨骼空间下的pose);
	virtual void Evaluate_AnyThread(FPoseContext& Output) override;
	// End of FAnimNode_Base interface

private:

	UChingMUComponent* ChingMUComponent;
	FVector RootBonePosFromTrackWorldSpace;
	FQuat BonesComponentSpaceRotation[23];

	//IMotionCapture* MotionCapture;

	//FString SubjectName;

	// Delta time from update so that it can be passed to retargeter
	//float CachedDeltaTime;

};
