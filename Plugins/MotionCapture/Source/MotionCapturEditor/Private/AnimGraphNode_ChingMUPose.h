#pragma once
#include "AnimGraphNode_Base.h"
#include "AnimNode_ChingMUPose.h"

#include "AnimGraphNode_ChingMUPose.generated.h"

UCLASS()
class UAnimGraphNode_ChingMUPose : public UAnimGraphNode_Base
{
	GENERATED_UCLASS_BODY()

		/*
		ÿ��������ͼ������������ɣ���һ���ֳ�֮Ϊ����ʱ�����ڵ㣬���������ǣ�������ʱ�������ɫÿ֡��pose; �����ⲿ�ִ����������༭�����У�������������е�FAnimNode_ChingMUPose
		���е�����ʱ�����ڵ㶼����̳���FAnimNode_Base��
		�ڶ����ֳ�Ϊ��ʾ�����ڵ㣬���������Ƕ��嶯���ڵ���ζ�����ͼ����ʾ��ֻ������������ڶ�����ͼ��ʹ������������������е�UAnimGraphNode_ChingMUPose
		���е���ʾ�����ڵ㶼����̳���UAnimGraphNode_Base��
		��������һЩ�������������Զ��壨��ʾ�������ڵ�̳в���д��ʵ���Լ������ݣ���������һЩ������

*/
	//ע�⣺��x��ʾ�����ڵ��б������һ��x����ʱ�����ڵ��ʵ����
	UPROPERTY(EditAnywhere, Category = Settings)
	FAnimNode_ChingMUPose Node;


	// UEdGraphNode interface
	//���壺�˶����ڵ������
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	//���壺�������ͣ�ڴ˶����ڵ����棬��ʾ���ڴ˽ڵ��һЩ��Ϣ������˽ڵ�����ã�
	virtual FText GetTooltipText() const override;
	//���壺�˶����ڵ������ĸ����࣬ͬһ����Ķ����ڵ㣬һ���ڱ༭������ʾ��ͬһ�����£�
	virtual FText GetMenuCategory() const override;
	//���壺�˶����ڵ����ɫ��
	virtual FLinearColor GetNodeTitleColor() const override;
	// End of UEdGraphNode
};
