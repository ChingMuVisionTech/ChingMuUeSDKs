#include "ChingmuFBXAnimation.h"
#include"MotionCapturePrivatePCH.h"
#include "Engine.h"
//#include"../../MotionCapture/Public/ChingMUComponent.h"

void AChingmuFBXAnimation::BeginPlay()
{
	Super::BeginPlay();

}

void AChingmuFBXAnimation::EndPlay(const EEndPlayReason::Type EndPlayReason)
{		

}

void AChingmuFBXAnimation::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);	

}

AChingmuFBXAnimation::AChingmuFBXAnimation()
{
	PrimaryActorTick.bCanEverTick = true;
	offSetRot = FRotator(0,90,0);
	Chingmu = CreateDefaultSubobject<UChingMUComponent>(TEXT("Chingmu"));
	SketalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SketalMesh"));
	HumanPoseMesh = CreateDefaultSubobject<UPoseableMeshComponent>(TEXT("HumanPoseMesh"));
}

void AChingmuFBXAnimation::GetOrinAnimationData()
{
	orinAnimationData.positions.Hips = SketalMesh->GetSocketLocation("skeleton0_Hips");
	orinAnimationData.quats.Hips = SketalMesh->GetSocketQuaternion("skeleton0_Hips");
	orinAnimationData.quats.Spine = SketalMesh->GetSocketQuaternion("skeleton0_Spine");
	orinAnimationData.quats.Spine1 = SketalMesh->GetSocketQuaternion("skeleton0_Spine1");
	orinAnimationData.quats.Spine2 = SketalMesh->GetSocketQuaternion("skeleton0_Spine2");
	orinAnimationData.quats.Spine3 = SketalMesh->GetSocketQuaternion("skeleton0_Spine3");
	orinAnimationData.quats.Neck = SketalMesh->GetSocketQuaternion("skeleton0_Neck");
	orinAnimationData.quats.Head = SketalMesh->GetSocketQuaternion("skeleton0_Head");

	orinAnimationData.quats.LeftShoulder = SketalMesh->GetSocketQuaternion("skeleton0_LeftShoulder");
	orinAnimationData.quats.LeftArm = SketalMesh->GetSocketQuaternion("skeleton0_LeftArm");
	orinAnimationData.quats.LeftForeArm = SketalMesh->GetSocketQuaternion("skeleton0_LeftForeArm");
	orinAnimationData.quats.LeftHand = SketalMesh->GetSocketQuaternion("skeleton0_LeftHand");
	orinAnimationData.quats.RightShoulder = SketalMesh->GetSocketQuaternion("skeleton0_RightShoulder");
	orinAnimationData.quats.RightArm = SketalMesh->GetSocketQuaternion("skeleton0_RightArm");
	orinAnimationData.quats.RightforeArm = SketalMesh->GetSocketQuaternion("skeleton0_RightForeArm");
	orinAnimationData.quats.RightHand = SketalMesh->GetSocketQuaternion("skeleton0_RightHand");

	orinAnimationData.quats.LeftUpLeg = SketalMesh->GetSocketQuaternion("skeleton0_LeftUpLeg");
	orinAnimationData.quats.LeftLeg = SketalMesh->GetSocketQuaternion("skeleton0_LeftLeg");
	orinAnimationData.quats.LeftFoot = SketalMesh->GetSocketQuaternion("skeleton0_LeftFoot");
	orinAnimationData.quats.LeftToeBase = SketalMesh->GetSocketQuaternion ("skeleton0_LeftToeBase");

	orinAnimationData.quats.RightUpLeg = SketalMesh->GetSocketQuaternion("skeleton0_RightUpLeg");
	orinAnimationData.quats.RightLeg = SketalMesh->GetSocketQuaternion("skeleton0_RightLeg");
	orinAnimationData.quats.RightFoot = SketalMesh->GetSocketQuaternion("skeleton0_RightFoot");
	orinAnimationData.quats.RightToeBase = SketalMesh->GetSocketQuaternion("skeleton0_RightToeBase");
}

void AChingmuFBXAnimation::GetAnimationData(FWorldSpaceHumanState & Final)
{
	GetOrinAnimationData();
	OrinAnimationDataModify();
	Final = finalAnimationdata;	
}

void AChingmuFBXAnimation::OrinAnimationDataModify()
{
	finalAnimationdata.positions.Hips.X = -orinAnimationData.positions.Hips.Y;
	finalAnimationdata.positions.Hips.Y = orinAnimationData.positions.Hips.X;
	finalAnimationdata.positions.Hips.Z = orinAnimationData.positions.Hips.Z;

	AniWorldSpaceBonesQuat[0] = *AxisConvert(orinAnimationData.quats.Hips);
	AniWorldSpaceBonesQuat[1] = *AxisConvert(orinAnimationData.quats.Spine);
	AniWorldSpaceBonesQuat[2] = *AxisConvert(orinAnimationData.quats.Spine1);
	AniWorldSpaceBonesQuat[3] = *AxisConvert(orinAnimationData.quats.Spine2);
	AniWorldSpaceBonesQuat[4] = *AxisConvert(orinAnimationData.quats.Spine3);
	AniWorldSpaceBonesQuat[5] = *AxisConvert(orinAnimationData.quats.Neck);
	AniWorldSpaceBonesQuat[6] = *AxisConvert(orinAnimationData.quats.Head);

	AniWorldSpaceBonesQuat[7] = *AxisConvert(orinAnimationData.quats.LeftShoulder);
	AniWorldSpaceBonesQuat[8] = *AxisConvert(orinAnimationData.quats.LeftArm);
	AniWorldSpaceBonesQuat[9] = *AxisConvert(orinAnimationData.quats.LeftForeArm);
	AniWorldSpaceBonesQuat[10] = *AxisConvert(orinAnimationData.quats.LeftHand);

	AniWorldSpaceBonesQuat[11] = *AxisConvert(orinAnimationData.quats.RightShoulder);
	AniWorldSpaceBonesQuat[12] = *AxisConvert(orinAnimationData.quats.RightArm);
	AniWorldSpaceBonesQuat[13] = *AxisConvert(orinAnimationData.quats.RightforeArm);
	AniWorldSpaceBonesQuat[14] = *AxisConvert(orinAnimationData.quats.RightHand);

	AniWorldSpaceBonesQuat[15] = *AxisConvert(orinAnimationData.quats.LeftUpLeg);
	AniWorldSpaceBonesQuat[16] = *AxisConvert(orinAnimationData.quats.LeftLeg);
	AniWorldSpaceBonesQuat[17] = *AxisConvert(orinAnimationData.quats.LeftFoot);
	AniWorldSpaceBonesQuat[18] = *AxisConvert(orinAnimationData.quats.LeftToeBase);

	AniWorldSpaceBonesQuat[19] = *AxisConvert(orinAnimationData.quats.RightUpLeg);
	AniWorldSpaceBonesQuat[20] = *AxisConvert(orinAnimationData.quats.RightLeg);
	AniWorldSpaceBonesQuat[21] = *AxisConvert(orinAnimationData.quats.RightFoot);
	AniWorldSpaceBonesQuat[22] = *AxisConvert(orinAnimationData.quats.RightToeBase);


	for (size_t i = 0; i < sizeof(AniWorldSpaceBonesQuat) / sizeof(AniWorldSpaceBonesQuat[0]); i++)
	{
		AniWorldSpaceBonesQuat[i] = offSetRot.Quaternion()*AniWorldSpaceBonesQuat[i];
	}

	//get Ani WorldRotator

	finalAnimationdata.Rotators.Hips = AniWorldSpaceBonesQuat[0].Rotator();
	finalAnimationdata.Rotators.Spine = AniWorldSpaceBonesQuat[1].Rotator();
	finalAnimationdata.Rotators.Spine1 = AniWorldSpaceBonesQuat[2].Rotator();
	finalAnimationdata.Rotators.Spine2 = AniWorldSpaceBonesQuat[3].Rotator();
	finalAnimationdata.Rotators.Spine3 = AniWorldSpaceBonesQuat[4].Rotator();
	finalAnimationdata.Rotators.Neck = AniWorldSpaceBonesQuat[5].Rotator();
	finalAnimationdata.Rotators.Head = AniWorldSpaceBonesQuat[6].Rotator();

	finalAnimationdata.Rotators.LeftShoulder = AniWorldSpaceBonesQuat[7].Rotator();
	finalAnimationdata.Rotators.LeftArm = AniWorldSpaceBonesQuat[8].Rotator();
	finalAnimationdata.Rotators.LeftForeArm = AniWorldSpaceBonesQuat[9].Rotator();
	finalAnimationdata.Rotators.LeftHand = AniWorldSpaceBonesQuat[10].Rotator();

	finalAnimationdata.Rotators.RightShoulder = AniWorldSpaceBonesQuat[11].Rotator();
	finalAnimationdata.Rotators.RightArm = AniWorldSpaceBonesQuat[12].Rotator();
	finalAnimationdata.Rotators.RightforeArm = AniWorldSpaceBonesQuat[13].Rotator();
	finalAnimationdata.Rotators.RightHand = AniWorldSpaceBonesQuat[14].Rotator();

	finalAnimationdata.Rotators.LeftUpLeg = AniWorldSpaceBonesQuat[15].Rotator();
	finalAnimationdata.Rotators.LeftLeg = AniWorldSpaceBonesQuat[16].Rotator();
	finalAnimationdata.Rotators.LeftFoot = AniWorldSpaceBonesQuat[17].Rotator();
	finalAnimationdata.Rotators.LeftToeBase = AniWorldSpaceBonesQuat[18].Rotator();

	finalAnimationdata.Rotators.RightUpLeg = AniWorldSpaceBonesQuat[19].Rotator();
	finalAnimationdata.Rotators.RightLeg = AniWorldSpaceBonesQuat[20].Rotator();
	finalAnimationdata.Rotators.RightFoot = AniWorldSpaceBonesQuat[21].Rotator();
	finalAnimationdata.Rotators.RightToeBase = AniWorldSpaceBonesQuat[22].Rotator();
}

void AChingmuFBXAnimation::GetAnimationDataMiXMocap(FString ServerAddr, int humanID, bool useMocapPos, FWorldSpaceHumanRotator AniDataIn, FWorldSpaceHumanState & MIX)
{
	//»ñÈ¡RealTime ¶¯²¶Êý¾Ý´æÓÚworldSpaceHumanState
	Chingmu->GetHumanState(TCHAR_TO_ANSI(*ServerAddr), humanID, worldSpaceHumanState);

	//Ñ¡ÔñÊ¹ÓÃrealtime ¶¯²¶Î»ÒÆ£¬»¹ÊÇFBX ¶¯»­Î»ÒÆ
	if (useMocapPos)
	{
		MIXState.positions.Hips = worldSpaceHumanState.positions.Hips;
	}
	else
	{
		MIXState.positions.Hips.X = -AniDataIn.HipsPosition.Y;
		MIXState.positions.Hips.Y = AniDataIn.HipsPosition.X;
		MIXState.positions.Hips.Z = AniDataIn.HipsPosition.Z;
	}


	// ÏÂ°ëÉí ANiÊý¾Ý
	AniWorldSpaceBonesQuat[0] = *AxisConvert(AniDataIn.Hips.Quaternion());

	AniWorldSpaceBonesQuat[15] = *AxisConvert(AniDataIn.LeftUpLeg.Quaternion());
	AniWorldSpaceBonesQuat[16] = *AxisConvert(AniDataIn.LeftLeg.Quaternion());
	AniWorldSpaceBonesQuat[17] = *AxisConvert(AniDataIn.LeftFoot.Quaternion());
	AniWorldSpaceBonesQuat[18] = *AxisConvert(AniDataIn.LeftToeBase.Quaternion());
																									   
	AniWorldSpaceBonesQuat[19] = *AxisConvert(AniDataIn.RightUpLeg.Quaternion());
	AniWorldSpaceBonesQuat[20] = *AxisConvert(AniDataIn.RightLeg.Quaternion());
	AniWorldSpaceBonesQuat[21] = *AxisConvert(AniDataIn.RightFoot.Quaternion());
	AniWorldSpaceBonesQuat[22] = *AxisConvert(AniDataIn.RightToeBase.Quaternion());

	//ÏÂ°ëÉí¸úËæÐý×ª¸úËæRealCapÐý×ª
	worldSpaceHumanState.Rotators.Hips.Roll = 0;
	worldSpaceHumanState.Rotators.Hips.Pitch = 0;
	AniWorldSpaceBonesQuat[0] = worldSpaceHumanState.Rotators.Hips.Quaternion()*AniWorldSpaceBonesQuat[0];
	for (size_t i = 15; i < sizeof(AniWorldSpaceBonesQuat) / sizeof(AniWorldSpaceBonesQuat[0]); i++)
	{
		AniWorldSpaceBonesQuat[i] =  worldSpaceHumanState.Rotators.Hips.Quaternion()*AniWorldSpaceBonesQuat[i];
	}

	//ÉÏ°ëÉíÓÃÊµÊ±¶¯²¶Êý¾Ý
	MIXState.Rotators.Hips = AniWorldSpaceBonesQuat[0].Rotator();

	MIXState.Rotators.Spine = worldSpaceHumanState.Rotators.Spine;
	MIXState.Rotators.Spine1 = worldSpaceHumanState.Rotators.Spine1;
	MIXState.Rotators.Spine2 = worldSpaceHumanState.Rotators.Spine2;
	MIXState.Rotators.Spine3 = worldSpaceHumanState.Rotators.Spine3;
	MIXState.Rotators.Neck = worldSpaceHumanState.Rotators.Neck;
	MIXState.Rotators.Head = worldSpaceHumanState.Rotators.Head;

	MIXState.Rotators.LeftShoulder = worldSpaceHumanState.Rotators.LeftShoulder;
	MIXState.Rotators.LeftArm = worldSpaceHumanState.Rotators.LeftArm;
	MIXState.Rotators.LeftForeArm = worldSpaceHumanState.Rotators.LeftForeArm;
	MIXState.Rotators.LeftHand = worldSpaceHumanState.Rotators.LeftHand;

	MIXState.Rotators.RightShoulder = worldSpaceHumanState.Rotators.RightShoulder;
	MIXState.Rotators.RightArm = worldSpaceHumanState.Rotators.RightArm;
	MIXState.Rotators.RightforeArm = worldSpaceHumanState.Rotators.RightforeArm;
	MIXState.Rotators.RightHand = worldSpaceHumanState.Rotators.RightHand;

	//ÏÂ°ëÉí¶¯»­Êý¾Ý
	MIXState.Rotators.LeftUpLeg = AniWorldSpaceBonesQuat[15].Rotator();
	MIXState.Rotators.LeftLeg = AniWorldSpaceBonesQuat[16].Rotator();
	MIXState.Rotators.LeftFoot = AniWorldSpaceBonesQuat[17].Rotator();
	MIXState.Rotators.LeftToeBase = AniWorldSpaceBonesQuat[18].Rotator();

	MIXState.Rotators.RightUpLeg = AniWorldSpaceBonesQuat[19].Rotator();
	MIXState.Rotators.RightLeg = AniWorldSpaceBonesQuat[20].Rotator();
	MIXState.Rotators.RightFoot = AniWorldSpaceBonesQuat[21].Rotator();
	MIXState.Rotators.RightToeBase = AniWorldSpaceBonesQuat[22].Rotator();

	MIX = MIXState;
}


FQuat* AChingmuFBXAnimation::AxisConvert(FQuat quat)
{
	quat = quat*FQuat(0, -1.0 / sqrtf(2.0), 0, 1.0 / sqrtf(2.0))*FQuat(1.0 / sqrtf(2.0), 0, 0, 1.0 / sqrtf(2.0));
	return &quat;
}

AChingmuFBXAnimation::~AChingmuFBXAnimation()
{
}
