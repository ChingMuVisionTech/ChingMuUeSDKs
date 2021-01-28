// Copyright 2015-2019 ChingMU, Inc. All Rights Reserved.

#include "AnimNode_ChingMURetargetPoseForBuild.h "
//#include "AnimNode_ChingMURetargetPose.h"
#include "MotionCapturePrivatePCH.h"
#include "FMotionCapture.h"
//#include "Features/IModularFeatures.h"
#include "Animation/AnimInstanceProxy.h"

#include "AnimationRuntime.h"




FAnimNode_ChingMURetargetPoseForBuild ::FAnimNode_ChingMURetargetPoseForBuild () : MotionCapture(&IMotionCapture::Get())
{

}


void FAnimNode_ChingMURetargetPoseForBuild ::Initialize_AnyThread(const FAnimationInitializeContext& Context)
{  

	motionCapturePlugin = (FMotionCapture*)MotionCapture;
	if (SeverAddr.Contains("MCServer@") && SeverAddr.Contains("3884"))
	{
		motionCapturePlugin->CMServerAddress = SeverAddr;
		motionCapturePlugin->IsCheckedForceFaceX = bIsCheckedForceX;
	}

}

void FAnimNode_ChingMURetargetPoseForBuild ::Update_AnyThread(const FAnimationUpdateContext & Context)
{
	GetEvaluateGraphExposedInputs().Execute(Context);
}



void FAnimNode_ChingMURetargetPoseForBuild::Evaluate_AnyThread(FPoseContext& Output)
{

	if (BonesInforFromClient_t.Num()  == 0|| RetargetCharactersID.Num()==0 || (!motionCapturePlugin->AnimNodeForPoseUpdate))
	{
		
		return;
	}
	TArray<int> keys;
	RetargetCharactersID.GetKeys(keys);
	if (!keys.Contains(HuamnId))
	{
		UE_LOG(LogLoad, Log, TEXT(" %s   %i "),TEXT("No Find The HumanID: "), HuamnId);
		return;
	}

	Output.ResetToRefPose();
	FCSPose<FCompactPose> OutputPose;
	OutputPose.InitPose(&Output.Pose.GetBoneContainer());
	const FBoneContainer Container = OutputPose.GetPose().GetBoneContainer();
	FVector pos[150];
	FQuat boneRot[150];
	FTimecode curTimeCode;


	if (!MotionCapture->CMRetargetHumanTC(TCHAR_TO_ANSI(*SeverAddr), HuamnId, curTimeCode, pos, boneRot))
		return;
	FTransform ComponentTransform = Output.AnimInstanceProxy->GetComponentTransform();

	int startIndex = RetargetCharactersID[HuamnId];


	for (int j = startIndex; j < BonesInforFromClient_t.Num(); j++)
	{
		// set target transform

		FString boneName = BonesInforFromClient_t[j].name.ToString();
		FName   boneName2 = FName(*boneName);
		int boneIndex = (BonesInforFromClient_t[j].sensor - 100) % 150;
		if (boneIndex == 0 && j != startIndex)
		{
			break;
		}
		int32 MeshIndex = Container.GetPoseBoneIndexForBoneName(boneName2);

		if (MeshIndex != INDEX_NONE)
		{
			FCompactPoseBoneIndex CPIndex = Container.MakeCompactPoseIndex(FMeshPoseBoneIndex(MeshIndex));
			//OutPose.();
			if (CPIndex != INDEX_NONE)
			{
				// source transform
				FTransform source;

				if (boneIndex > 0)
				{
					if (bUsingWorldRotation)
					{
						source.SetRotation(boneRot[boneIndex]);
						source.SetLocation(pos[boneIndex]);
						OutputPose.SetComponentSpaceTransform(CPIndex, source);
					}
					else 
					{
						source.SetRotation(boneRot[boneIndex]);
						source.SetLocation(pos[boneIndex]);

						FAnimationRuntime::ConvertBoneSpaceTransformToCS(ComponentTransform, OutputPose, source, CPIndex, EBoneControlSpace::BCS_ParentBoneSpace);
						OutputPose.SetComponentSpaceTransform(CPIndex, source);
					}
					
				}

			}
		}

	}

	FCSPose<FCompactPose>::ConvertComponentPosesToLocalPoses(OutputPose, Output.Pose);

}

/*
void FAnimNode_ChingMURetargetPoseForBuild ::Evaluate_AnyThread(FPoseContext& Output)
{
	
	
	if (BonesInforFromClient_t.Num()==0)
	{
		return;
	}

	Output.ResetToRefPose();
	FCSPose<FCompactPose> OutputPose;
	OutputPose.InitPose(&Output.Pose.GetBoneContainer());
	const FBoneContainer Container = OutputPose.GetPose().GetBoneContainer();
	FVector pos[150];
	FQuat boneRot[150];
	FTimecode curTimeCode;


	if (!MotionCapture->CMRetargetHumanTC(TCHAR_TO_ANSI(*SeverAddr), HuamnId, curTimeCode, pos, boneRot))
		return;
	FTransform ComponentTransform = Output.AnimInstanceProxy->GetComponentTransform();

	int startIndex =0;
	for (int i = 0; i <BonesInforFromClient_t.Num(); i++)
	{
		int index = (BonesInforFromClient_t[i].sensor - 100) % 150;
		if (index==0  ) 
		{
			if (BonesInforFromClient_t[i].sensor == HuamnId*150+100)
			{
				startIndex = i;
				break;
			}
			
		}
	
	}
	for (int j = startIndex; j< BonesInforFromClient_t.Num();j++)
	{
		// set target transform
	
		FString boneName =  BonesInforFromClient_t[j].name.ToString();
		FName   boneName2 = FName(*boneName);
		int boneIndex = (BonesInforFromClient_t[j].sensor - 100) % 150;
		if (boneIndex==0 &&  j !=startIndex)
		{
			break;
		}
		int32 MeshIndex = Container.GetPoseBoneIndexForBoneName(boneName2);

		if (MeshIndex != INDEX_NONE)
		{
			FCompactPoseBoneIndex CPIndex = Container.MakeCompactPoseIndex(FMeshPoseBoneIndex(MeshIndex));
			//OutPose.();
			if (CPIndex != INDEX_NONE)
			{
				// source transform
				FTransform source;

				if (boneIndex>0) 
				{
					source.SetRotation(boneRot[boneIndex]);
					source.SetLocation(pos[boneIndex]);

					FAnimationRuntime::ConvertBoneSpaceTransformToCS(ComponentTransform, OutputPose, source, CPIndex, EBoneControlSpace::BCS_ParentBoneSpace);
					OutputPose.SetComponentSpaceTransform(CPIndex, source);
				}

			}
		}

	}

	FCSPose<FCompactPose>::ConvertComponentPosesToLocalPoses(OutputPose, Output.Pose);

	
	
}
*/

