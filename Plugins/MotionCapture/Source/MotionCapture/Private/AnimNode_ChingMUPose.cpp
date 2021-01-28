#include "AnimNode_ChingMUPose.h"
#include "MotionCapturePrivatePCH.h"
//#include "UChingMUComponent.h"
//#include "FMotionCapture.h"
//#include "SubclassOf.h"
#include "Components/PoseableMeshComponent.h"
#include "Features/IModularFeatures.h"
#include "Animation/AnimInstanceProxy.h"


FAnimNode_ChingMUPose::FAnimNode_ChingMUPose()
{

}

//bool CreateCurve(USkeleton *Skeleton, FSmartName CurveName)
//{
//	FCurveMetaData *CurveMetaData = Skeleton->GetCurveMetaData(CurveName);
//	if (CurveMetaData && !CurveMetaData->LinkedBones.Num())
//	{
//		Skeleton->Modify();
//		TArray<FBoneNode> BoneArray = Skeleton->GetBoneTree();
//		for (int i = 0; i < BoneArray.Num(); ++i)
//		{
//			FName boneName = Skeleton->GetReferenceSkeleton().GetBoneName(i);
//			if (boneName.ToString().Contains("Hips"))
//			{
//				FBoneReference Hips(boneName);
//				Hips.Initialize(Skeleton);
//				CurveMetaData->LinkedBones.Add(Hips);
//				CurveMetaData->MaxLOD = 255;
//
//				return true;
//			}
//		}
//	}
//
//	return false;
//}

void FAnimNode_ChingMUPose::Initialize_AnyThread(const FAnimationInitializeContext& Context)
{

	
	ChingMUComponent = dynamic_cast<UChingMUComponent*> (Context.AnimInstanceProxy->GetSkelMeshComponent()->GetAnimInstance()->GetOwningActor()->GetComponentByClass(TSubclassOf<UChingMUComponent>(UChingMUComponent::StaticClass())));
	UE_LOG(LogTemp, Display, TEXT("From ChingMUPose Initialize_AnyThread   "));

}

void FAnimNode_ChingMUPose::Update_AnyThread(const FAnimationUpdateContext & Context)
{
	//EvaluateGraphExposedInputs.Execute(Context);
	//API±ä¶¯
	GetEvaluateGraphExposedInputs().Execute(Context);
	if (ChingMUComponent == nullptr)
	{
		return;
	}
	if (!(ChingMUComponent->m_loadConfig))
	{
		return;
	}

	ChingMUComponent->CalculateBoneCSRotation(SeverAddr, HuamnId, RootBonePosFromTrackWorldSpace, BonesComponentSpaceRotation);

	//// Accumulate Delta time from update
	//CachedDeltaTime += Context.GetDeltaTime();
}

void FAnimNode_ChingMUPose::Evaluate_AnyThread(FPoseContext& Output) //FPoseContext£¬µ±Ç°¹Ç÷À¿Õ¼äÏÂµÄ£¨ÉÏÏÂÎÄ£©poses£¨×´Ì¬£©;
{
	Output.ResetToRefPose();
	if (ChingMUComponent == nullptr)
	{
		return;
	}
	if (!(ChingMUComponent->m_loadConfig))
	{
		return;
	}

	FCSPose<FCompactPose> OutPutCsPose; //SkeletaMesh×é¼þÏÂµÄpose
	OutPutCsPose.InitPose(&Output.Pose.GetBoneContainer());//¹Ç÷ÀÈÝÆ÷£¬°üº¬ÁË´Ë¹Ç÷ÀµÄË÷Òý£¬ÒÔ¼°´Ë¹Ç÷ÀËùÔÚµÄ¹Ç¼Ü£¬µÈµÈÐÅÏ¢£»
	const FBoneContainer Container = OutPutCsPose.GetPose().GetBoneContainer();

	for (int i = 0; i < 23; i++)
	{
		int32 MeshBoneIndex = Container.GetPoseBoneIndexForBoneName(ChingMUComponent->BoneIndexToBoneNameMap[i]);
		if (MeshBoneIndex != INDEX_NONE)
		{

			FCompactPoseBoneIndex CPIndex = Container.MakeCompactPoseIndex(FMeshPoseBoneIndex(MeshBoneIndex));
			FTransform CSTransformFromSource = OutPutCsPose.GetComponentSpaceTransform(CPIndex);//»ñÈ¡´Ë¹Ç÷ÀÔÚ×é¼þ¿Õ¼äÏÂµÄtransform;

			if (i == 0)
			{
				CSTransformFromSource.SetLocation(RootBonePosFromTrackWorldSpace);
			}
			CSTransformFromSource.SetRotation(BonesComponentSpaceRotation[i]);

			OutPutCsPose.SetComponentSpaceTransform(CPIndex, CSTransformFromSource);//ÉèÖÃ´Ë¹Ç÷ÀÔÚ×é¼þ¿Õ¼äÏÂµÄtransform;È·±£¸ÃtransformÖÐµÄÐý×ªÓëÎ»ÖÃ¶¼ÊÇ(SkinMesh)SkeletaMesh×é¼þÏÂµÄÐý×ªÓëÎ»ÖÃ£»

			//OutPutCsPose.ConvertToLocalPoses(Output.Pose);//½«SkeletaMesh×é¼þÏÂµÄpose×ª»»Îª¹Ç÷À¿Õ¼äÏÂµÄpose£¬ÒòÎª×îÖÕÊä³ö±ØÐëÊÇ¹Ç÷À¿Õ¼äÏÂµÄpose ;

			//API±ä¶¯
			FCSPose<FCompactPose>::ConvertComponentPosesToLocalPoses(OutPutCsPose, Output.Pose);//½«SkeletaMesh×é¼þÏÂµÄpose×ª»»Îª¹Ç÷À¿Õ¼äÏÂµÄpose£¬ÒòÎª×îÖÕÊä³ö±ØÐëÊÇ¹Ç÷À¿Õ¼äÏÂµÄpose ;
		}
	}

}