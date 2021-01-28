#pragma once
#include "../../MotionCapture/Public/ChingMUComponent.h"
#include "Runtime/Engine/Classes/Components/PoseableMeshComponent.h"


class UChingMUComponent;

class IHandCap
{

protected:

	FWorldSpaceHandState m_LeftHandState;
	FWorldSpaceHandState m_RightHandState;

public:
	IHandCap() 
	{
		m_LeftHandState = FWorldSpaceHandState();
		m_RightHandState = FWorldSpaceHandState();
	}

	
	virtual void GetHandsState(FWorldSpaceHandState &leftHand, FWorldSpaceHandState &rightHand)=0;
	void SetHandsState(FFingersName *lefthandFingersName, FFingersName *righthandFingersName,
		FWorldSpaceHandState *leftHand, FWorldSpaceHandState *rightHand,
		UPoseableMeshComponent *humanPoseableMesh)
	{
		//Set LeftHand Rotator
		if (leftHand != nullptr)
		{
			humanPoseableMesh->SetBoneRotationByName(lefthandFingersName->palm, leftHand->palm.Rotator(), EBoneSpaces::ComponentSpace);
			humanPoseableMesh->SetBoneRotationByName(lefthandFingersName->thumb1, leftHand->thumb1.Rotator(), EBoneSpaces::ComponentSpace);
			humanPoseableMesh->SetBoneRotationByName(lefthandFingersName->thumb2, leftHand->thumb2.Rotator(), EBoneSpaces::ComponentSpace);
			humanPoseableMesh->SetBoneRotationByName(lefthandFingersName->thumb3, leftHand->thumb3.Rotator(), EBoneSpaces::ComponentSpace);
			humanPoseableMesh->SetBoneRotationByName(lefthandFingersName->index1, leftHand->index1.Rotator(), EBoneSpaces::ComponentSpace);
			humanPoseableMesh->SetBoneRotationByName(lefthandFingersName->index2, leftHand->index2.Rotator(), EBoneSpaces::ComponentSpace);
			humanPoseableMesh->SetBoneRotationByName(lefthandFingersName->index3, leftHand->index3.Rotator(), EBoneSpaces::ComponentSpace);
			humanPoseableMesh->SetBoneRotationByName(lefthandFingersName->middle1, leftHand->middle1.Rotator(), EBoneSpaces::ComponentSpace);
			humanPoseableMesh->SetBoneRotationByName(lefthandFingersName->middle2, leftHand->middle2.Rotator(), EBoneSpaces::ComponentSpace);
			humanPoseableMesh->SetBoneRotationByName(lefthandFingersName->middle3, leftHand->middle3.Rotator(), EBoneSpaces::ComponentSpace);
			humanPoseableMesh->SetBoneRotationByName(lefthandFingersName->ring1, leftHand->ring1.Rotator(), EBoneSpaces::ComponentSpace);
			humanPoseableMesh->SetBoneRotationByName(lefthandFingersName->ring2, leftHand->ring2.Rotator(), EBoneSpaces::ComponentSpace);
			humanPoseableMesh->SetBoneRotationByName(lefthandFingersName->ring3, leftHand->ring3.Rotator(), EBoneSpaces::ComponentSpace);
			humanPoseableMesh->SetBoneRotationByName(lefthandFingersName->pinky1, leftHand->pinky1.Rotator(), EBoneSpaces::ComponentSpace);
			humanPoseableMesh->SetBoneRotationByName(lefthandFingersName->pinky2, leftHand->pinky2.Rotator(), EBoneSpaces::ComponentSpace);
			humanPoseableMesh->SetBoneRotationByName(lefthandFingersName->pinky3, leftHand->pinky3.Rotator(), EBoneSpaces::ComponentSpace);
		}
		

		//Set RightHand Rotator
		if (rightHand != nullptr)
		{
			humanPoseableMesh->SetBoneRotationByName(righthandFingersName->palm, rightHand->palm.Rotator(), EBoneSpaces::ComponentSpace);
			humanPoseableMesh->SetBoneRotationByName(righthandFingersName->thumb1, rightHand->thumb1.Rotator(), EBoneSpaces::ComponentSpace);
			humanPoseableMesh->SetBoneRotationByName(righthandFingersName->thumb2, rightHand->thumb2.Rotator(), EBoneSpaces::ComponentSpace);
			humanPoseableMesh->SetBoneRotationByName(righthandFingersName->thumb3, rightHand->thumb3.Rotator(), EBoneSpaces::ComponentSpace);
			humanPoseableMesh->SetBoneRotationByName(righthandFingersName->index1, rightHand->index1.Rotator(), EBoneSpaces::ComponentSpace);
			humanPoseableMesh->SetBoneRotationByName(righthandFingersName->index2, rightHand->index2.Rotator(), EBoneSpaces::ComponentSpace);
			humanPoseableMesh->SetBoneRotationByName(righthandFingersName->index3, rightHand->index3.Rotator(), EBoneSpaces::ComponentSpace);
			humanPoseableMesh->SetBoneRotationByName(righthandFingersName->middle1, rightHand->middle1.Rotator(), EBoneSpaces::ComponentSpace);
			humanPoseableMesh->SetBoneRotationByName(righthandFingersName->middle2, rightHand->middle2.Rotator(), EBoneSpaces::ComponentSpace);
			humanPoseableMesh->SetBoneRotationByName(righthandFingersName->middle3, rightHand->middle3.Rotator(), EBoneSpaces::ComponentSpace);
			humanPoseableMesh->SetBoneRotationByName(righthandFingersName->ring1, rightHand->ring1.Rotator(), EBoneSpaces::ComponentSpace);
			humanPoseableMesh->SetBoneRotationByName(righthandFingersName->ring2, rightHand->ring2.Rotator(), EBoneSpaces::ComponentSpace);
			humanPoseableMesh->SetBoneRotationByName(righthandFingersName->ring3, rightHand->ring3.Rotator(), EBoneSpaces::ComponentSpace);
			humanPoseableMesh->SetBoneRotationByName(righthandFingersName->pinky1, rightHand->pinky1.Rotator(), EBoneSpaces::ComponentSpace);
			humanPoseableMesh->SetBoneRotationByName(righthandFingersName->pinky2, rightHand->pinky2.Rotator(), EBoneSpaces::ComponentSpace);
			humanPoseableMesh->SetBoneRotationByName(righthandFingersName->pinky3, rightHand->pinky3.Rotator(), EBoneSpaces::ComponentSpace);
		}
	
	}


	virtual ~IHandCap() {}
};

