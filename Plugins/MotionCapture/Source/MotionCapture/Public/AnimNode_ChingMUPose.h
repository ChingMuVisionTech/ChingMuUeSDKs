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
ÿ��������ͼ������������ɣ�
��һ���ֳ�Ϊ��ʾ�����ڵ㣬���������Ƕ��嶯���ڵ���ζ�����ͼ����ʾ��ֻ������������ڶ�����ͼ��ʹ������������������е�UAnimGraphNode_ChingMUPose
���е���ʾ�����ڵ㶼����̳���UAnimGraphNode_Base��
�ڶ����ֳ�֮Ϊ����ʱ�����ڵ㣬���������ǣ�������ʱ�������ɫÿ֡��pose; �����ⲿ�ִ����������༭�����У�������������е�FAnimNode_ChingMUPose
���е�����ʱ�����ڵ㶼����̳���FAnimNode_Base��
��������һЩ�������������Զ�������ʱ�����ڵ�̳в���д��ʵ���Լ������ݣ���������һЩ������

*/

	//�����˽ڵ��������ԣ�������˵��������ָ�ýڵ�����������ͬʱҲ�Ǵ˽ڵ㹤��ʱҪ���õĶ�����Category���ò������������࣬PinShownByDefault�������Ի�Ĭ����ʾ��
    /** Set server ip */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ChingMU_SeverAddr, meta = (PinShownByDefault))
	FString  SeverAddr;

	//�����˽ڵ���������
	/** Set human id */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ChingMU_HumanId, meta = (PinShownByDefault))
    int32 HuamnId;

	FAnimNode_ChingMUPose();//���˽ڵ�ʵ������ʱ��Ҳ���ǽ��˽ڵ����뵽������ͼ�У����ߴ򿪴˽ڵ����ڵĶ�����ͼ������ָ��/��ʼ���ô˽ڵ����ڵĶ�����ͼʱ���ͻ���ô˹��캯����

	// FAnimNode_Base interface �ڴ˽ڵ㱻����֮��Ҳ���Ǵ˽ڵ�Ĺ��캯������֮�󣩣������ȵ��ô˺�����������ʼ��һЩ�˶����ڵ�����Ķ�����
	virtual void Initialize_AnyThread(const FAnimationInitializeContext& Context) override;

	// virtual void CacheBones_AnyThread(const FAnimationCacheBonesContext & Context) {};

	//�˽ڵ���Initialize_AnyThread֮��ÿ֡���ã���������һЩ��������Щ���������ǣ�Ҫ�����ɫposeʱҪ�õĶ�����
	virtual void Update_AnyThread(const FAnimationUpdateContext & Context) override;

	//�˽ڵ�������Update_AnyThread֮��ÿ֡���ã���������ÿ֡�Ǽ������й�����pose (������������Output pose,�����յĹ����ռ��µ�pose��������ͼ�����������pseΪ�����ռ��µ�pose);
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
