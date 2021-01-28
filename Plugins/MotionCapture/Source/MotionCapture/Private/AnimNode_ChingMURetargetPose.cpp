// Copyright 2015-2019 ChingMU, Inc. All Rights Reserved.

#include "AnimNode_ChingMURetargetPose.h"
#include "MotionCapturePrivatePCH.h"
#include "FMotionCapture.h"
#include "Features/IModularFeatures.h"
#include "Animation/AnimInstanceProxy.h"


#include "AnimationRuntime.h"

#if WITH_EDITOR

FAnimNode_ChingMURetargetPose::FAnimNode_ChingMURetargetPose()
	: MotionCapture(&IMotionCapture::Get())
{
}

bool CreateCurve(USkeleton *Skeleton, FSmartName CurveName)
{
	FCurveMetaData *CurveMetaData = Skeleton->GetCurveMetaData(CurveName);
	if (CurveMetaData && !CurveMetaData->LinkedBones.Num())
	{
		Skeleton->Modify();
		TArray<FBoneNode> BoneArray = Skeleton->GetBoneTree();
		for (int i = 0; i < BoneArray.Num(); ++i)
		{
			FName boneName = Skeleton->GetReferenceSkeleton().GetBoneName(i);
			if (boneName.ToString().Contains("Hips"))
			{
				FBoneReference Hips(boneName);
				Hips.Initialize(Skeleton);
				CurveMetaData->LinkedBones.Add(Hips);
				CurveMetaData->MaxLOD = 255;

				return true;
			}
		}
	}

	return false;
}

void FAnimNode_ChingMURetargetPose::Initialize_AnyThread(const FAnimationInitializeContext& Context)
{

	//SubjectName = Context.AnimInstanceProxy->GetActorName();
	SubjectName = Context.AnimInstanceProxy->GetSkelMeshComponent()->GetAnimInstance()->GetOwningActor()->GetActorLabel();

	// Create TimeCode Curve
	FSmartName CurveName;
	USkeleton* Skeleton = Context.AnimInstanceProxy->GetSkeleton();

	// TCHour
	Skeleton->AddSmartNameAndModify(USkeleton::AnimCurveMappingName, "TCHour", CurveName);
	CreateCurve(Skeleton, CurveName);

	// TCMinute
	Skeleton->AddSmartNameAndModify(USkeleton::AnimCurveMappingName, "TCMinute", CurveName);
	CreateCurve(Skeleton, CurveName);

	// TCSecond
	Skeleton->AddSmartNameAndModify(USkeleton::AnimCurveMappingName, "TCSecond", CurveName);
	CreateCurve(Skeleton, CurveName);

	// TCFrame
	Skeleton->AddSmartNameAndModify(USkeleton::AnimCurveMappingName, "TCFrame", CurveName);
	CreateCurve(Skeleton, CurveName);

}

void FAnimNode_ChingMURetargetPose::Update_AnyThread(const FAnimationUpdateContext & Context)
{
	GetEvaluateGraphExposedInputs().Execute(Context);
	//EvaluateGraphExposedInputs.Execute(Context);

	// Accumulate Delta time from update
	CachedDeltaTime += Context.GetDeltaTime();
}

void SetBoneStateRecusive(FCSPose<FCompactPose>& OutPose, FMotionCaptureSubjectItemPtr subject, FVector pos[150], FQuat rot[150] , FTransform ComponentTransform)
{
	// set target transform
	FString boneName = subject->SubjectName.ToString();
	FName   boneName2 = FName(*boneName);
	const FBoneContainer Container = OutPose.GetPose().GetBoneContainer();
	int32 MeshIndex = Container.GetPoseBoneIndexForBoneName(boneName2);

	
	
	if (MeshIndex != INDEX_NONE)
	{
		FCompactPoseBoneIndex CPIndex = Container.MakeCompactPoseIndex(FMeshPoseBoneIndex(MeshIndex));
		
		//OutPose.();
		int boneIndex = (subject->SubjectKey - 100) % 150;
		if (CPIndex != INDEX_NONE)
		{
			if (boneIndex>0)
			{
				// source transform
				FTransform source;

				source.SetRotation(rot[boneIndex]);
				source.SetLocation(pos[boneIndex]);

				FAnimationRuntime::ConvertBoneSpaceTransformToCS(ComponentTransform, OutPose, source, CPIndex, EBoneControlSpace::BCS_ParentBoneSpace);
				OutPose.SetComponentSpaceTransform(CPIndex, source);
			}

		}
	}

	// loop the children bones
	for (auto child : subject->Children)
	{
		SetBoneStateRecusive(OutPose, child, pos, rot, ComponentTransform);
	}

}
	
	
void SetBoneStateRecusive(FCSPose<FCompactPose>& OutPose, FMotionCaptureSubjectItemPtr subject, FVector pos[150], FQuat rot[150])
{
	// set target transform
	FString boneName = subject->SubjectName.ToString();
	FName   boneName2 = FName(*boneName);
	const FBoneContainer Container = OutPose.GetPose().GetBoneContainer();
	int32 MeshIndex = Container.GetPoseBoneIndexForBoneName(boneName2);



	if (MeshIndex != INDEX_NONE)
	{
		FCompactPoseBoneIndex CPIndex = Container.MakeCompactPoseIndex(FMeshPoseBoneIndex(MeshIndex));

		//OutPose.();
		if (CPIndex != INDEX_NONE)
		{
			// source transform
			FTransform source;
			int boneIndex = (subject->SubjectKey - 100) % 150;
			source.SetRotation(rot[boneIndex]);
			source.SetLocation(pos[boneIndex]);

			//FAnimationRuntime::ConvertBoneSpaceTransformToCS(ComponentTransform, OutPose, source, CPIndex, EBoneControlSpace::BCS_ParentBoneSpace);
			OutPose.SetComponentSpaceTransform(CPIndex, source);

		}
	}

	// loop the children bones
	for (auto child : subject->Children)
	{
		SetBoneStateRecusive(OutPose, child, pos, rot);
	}


}

void SetTimeCodeCurve(USkeleton* Skeleton, const FTimecode time, FBlendedCurve& OutCurve)
{
	// TCHour
	SmartName::UID_Type UID = Skeleton->GetUIDByName(USkeleton::AnimCurveMappingName, "TCHour");
	if (UID != SmartName::MaxUID)
		OutCurve.Set(UID, time.Hours);

	// TCMinute
	UID = Skeleton->GetUIDByName(USkeleton::AnimCurveMappingName, "TCMinute");
	if (UID != SmartName::MaxUID)
		OutCurve.Set(UID, time.Minutes);

	// TCSecond
	UID = Skeleton->GetUIDByName(USkeleton::AnimCurveMappingName, "TCSecond");
	if (UID != SmartName::MaxUID)
		OutCurve.Set(UID, time.Seconds);

	// TCFrame
	UID = Skeleton->GetUIDByName(USkeleton::AnimCurveMappingName, "TCFrame");
	if (UID != SmartName::MaxUID)
		OutCurve.Set(UID, time.Frames);
}

void FAnimNode_ChingMURetargetPose::Evaluate_AnyThread(FPoseContext& Output)
{

	Output.ResetToRefPose();
	if (!MotionCapture)
		return;

	// driven bind tracker
	for (auto tracker : ((FMotionCapture*)MotionCapture)->BindActors)
	{
		if (SubjectName == tracker.Value)
		{
			// retrieve mocap data
			FTimecode curTimeCode;
			FTransform trans = MotionCapture->CMTrackerTC(TCHAR_TO_ANSI(*((FMotionCapture*)MotionCapture)->CMTrackerAddress.ToString()), tracker.Key, curTimeCode);
			
			//if (!MotionCapture->CMTrackerIsDetected(TCHAR_TO_ANSI(*((FMotionCapture*)MotionCapture)->CMTrackerAddress.ToString()), tracker.Key))
			//	return;
	
			// find subject
			for (auto itr : ((FMotionCapture*)MotionCapture)->SubjectData)
			{
				if (itr->SubjectKey == tracker.Key)
				{
					// set pose
					FCSPose<FCompactPose> OutputPose;
					OutputPose.InitPose(&Output.Pose.GetBoneContainer());

					// set target transform
					FString boneName = itr->SubjectName.ToString();
					USkeleton* Skeleton = Output.Pose.GetBoneContainer().GetSkeletonAsset();
					TArray<FBoneNode> BoneArray = Skeleton->GetBoneTree();
					for (int i = 0; i < BoneArray.Num(); ++i)
					{
						FName srcBoneName = Skeleton->GetReferenceSkeleton().GetBoneName(i);
						if (srcBoneName.ToString().Contains(boneName) || boneName.Contains(srcBoneName.ToString()))
						{
							const FBoneContainer Container = OutputPose.GetPose().GetBoneContainer();
							int32 MeshIndex = Container.GetPoseBoneIndexForBoneName(srcBoneName);
							if (MeshIndex != INDEX_NONE)
							{
								FCompactPoseBoneIndex CPIndex = Container.MakeCompactPoseIndex(FMeshPoseBoneIndex(MeshIndex));
								if (CPIndex != INDEX_NONE)
								{
									// source transform
									FTransform source;
									source.SetRotation(trans.Rotator().Quaternion());
									source.SetLocation(trans.GetLocation());

									OutputPose.SetComponentSpaceTransform(CPIndex, source);
									//OutputPose.ConvertToLocalPoses(Output.Pose);
									//API±ä¶¯
									FCSPose<FCompactPose>::ConvertComponentPosesToLocalPoses(OutputPose, Output.Pose);
								}
							}

							break;
						}
					}

					// set TimeCode Curve
					SetTimeCodeCurve(Skeleton, curTimeCode, Output.Curve);
					CachedDeltaTime = 0.f;

					break;
				}
			}

			break;
		}
	}

	// driven bind mesh
	for (auto sklt : ((FMotionCapture*)MotionCapture)->BindSkeletons)
	{
		if (SubjectName == sklt.Value)
		{
			// retrieve mocap data
			FVector pos[150];
			FQuat boneRot[150];
			FTimecode curTimeCode;
			if (!MotionCapture->CMRetargetHumanTC(TCHAR_TO_ANSI(*((FMotionCapture*)MotionCapture)->CMTrackerAddress.ToString()), sklt.Key, curTimeCode, pos, boneRot))
				return;

			// find subject
			int32 trackerID = sklt.Key * 150 + 100;
			for (auto itr : ((FMotionCapture*)MotionCapture)->SubjectData)
			{
				if (itr->SubjectKey == trackerID)
				{
					// set pose
					FCSPose<FCompactPose> OutputPose;
					OutputPose.InitPose(&Output.Pose.GetBoneContainer());


					if(bUsingWorldRotation)
					{
						//青瞳动画重定向数据中使用世界旋转；
					    SetBoneStateRecusive(OutputPose, itr, pos, boneRot);
					}
					else 
					{
						//青瞳动画重定向数据中使用局部旋转；
						FTransform ComponentTransform = Output.AnimInstanceProxy->GetComponentTransform();
						SetBoneStateRecusive(OutputPose, itr, pos, boneRot, ComponentTransform);
					}
					FCSPose<FCompactPose>::ConvertComponentPosesToLocalPoses(OutputPose, Output.Pose);
					// set TimeCode Curve
					USkeleton* Skeleton = Output.Pose.GetBoneContainer().GetSkeletonAsset();
					SetTimeCodeCurve(Skeleton, curTimeCode, Output.Curve);
					CachedDeltaTime = 0.f;

					
					break;
				}
			}

			break;
		}
	}
}

#endif // !1