#include "ChingmuHumanCap.h"
#include"MotionCapturePrivatePCH.h"
#include "IMotionCapture.h"


ChingmuHumanCap::ChingmuHumanCap()
{
	m_motioncap = &IMotionCapture::Get();
	offSetRot = FRotator(0, 90, 0);

	for (int i = 0; i < 40; ++i)
	{
		localSpaceBonesQuat[i] = FQuat(0.0, 0.0, 0.0, 1.0);
		worldSpaceBonesPos[i] = FVector(0.0, 0.0, 0.0);
	}


}

ChingmuHumanCap::~ChingmuHumanCap()
{
}

FWorldSpaceHumanState* ChingmuHumanCap::GetHumanState(ANSICHAR  *ServerAddr, int32 *humanID)
{
	if (!m_motioncap)
		return NULL;

	FQuat copy;
	FTransform pose;
#if 0 // get human poistion
	FTimecode time;
	for (size_t i = 0; i < 23; i++)
	{
		int id = (100 + i + 24 * *humanID);
#if 1
		pose = m_motioncap->CMTrackerTC(ServerAddr, id, time);
		worldSpaceBonesPos[i] = pose.GetLocation();
		copy = pose.GetRotation();
#else
		worldSpaceBonesPos[i] = m_motioncap->CMTrackerPos(ServerAddr, id);
		copy = m_motioncap->CMTrackerQuat(ServerAddr, id, false);
		//boneRot[i] = FQuat(-(float)boneAttitude[i * 4 + 4], -(float)boneAttitude[i * 4 + 3], -(float)boneAttitude[i * 4 + 5], (float)boneAttitude[i * 4 + 6]);
#endif
		localSpaceBonesQuat[i].X = copy.Y;
		localSpaceBonesQuat[i].Y = -copy.X;
		localSpaceBonesQuat[i].Z = copy.Z;
		localSpaceBonesQuat[i].W = copy.W;

		if (!i)
		{
			worldSpaceHumanState.timecode = time;
			//m_motioncap->CMTrackerTC(ServerAddr, id, worldSpaceHumanState.timecode);
		}
	}
#else
	m_motioncap->CMHumanTC(ServerAddr, *humanID, worldSpaceHumanState.timecode, worldSpaceBonesPos, localSpaceBonesQuat);
#endif

	worldSpaceHumanState.positions.Hips = worldSpaceBonesPos[0];
	worldSpaceHumanState.quats.Hips = offSetRot.Quaternion()*localSpaceBonesQuat[0];
	worldSpaceHumanState.Rotators.Hips = worldSpaceHumanState.quats.Hips.Rotator();
	worldSpaceHumanState.Rotators.HipsPosition = worldSpaceBonesPos[0];

	worldSpaceHumanState.positions.Spine = worldSpaceBonesPos[1];
	worldSpaceHumanState.quats.Spine = worldSpaceHumanState.quats.Hips*localSpaceBonesQuat[1];
	worldSpaceHumanState.Rotators.Spine = worldSpaceHumanState.quats.Spine.Rotator();

	worldSpaceHumanState.positions.Spine1 = worldSpaceBonesPos[2];
	worldSpaceHumanState.quats.Spine1 = worldSpaceHumanState.quats.Spine*localSpaceBonesQuat[2];
	worldSpaceHumanState.Rotators.Spine1 = worldSpaceHumanState.quats.Spine1.Rotator();

	worldSpaceHumanState.positions.Spine2 = worldSpaceBonesPos[3];
	worldSpaceHumanState.quats.Spine2 = worldSpaceHumanState.quats.Spine1*localSpaceBonesQuat[3];
	worldSpaceHumanState.Rotators.Spine2 = worldSpaceHumanState.quats.Spine2.Rotator();

	worldSpaceHumanState.positions.Spine3 = worldSpaceBonesPos[4];
	worldSpaceHumanState.quats.Spine3 = worldSpaceHumanState.quats.Spine2*localSpaceBonesQuat[4];
	worldSpaceHumanState.Rotators.Spine3 = worldSpaceHumanState.quats.Spine3.Rotator();

	worldSpaceHumanState.positions.Neck = worldSpaceBonesPos[5];
	worldSpaceHumanState.quats.Neck = worldSpaceHumanState.quats.Spine3*localSpaceBonesQuat[5];
	worldSpaceHumanState.Rotators.Neck = worldSpaceHumanState.quats.Neck.Rotator();

	worldSpaceHumanState.positions.Head = worldSpaceBonesPos[6];
	worldSpaceHumanState.quats.Head = worldSpaceHumanState.quats.Neck*localSpaceBonesQuat[6];
	worldSpaceHumanState.Rotators.Head = worldSpaceHumanState.quats.Head.Rotator();

	worldSpaceHumanState.positions.LeftShoulder = worldSpaceBonesPos[7];
	worldSpaceHumanState.quats.LeftShoulder = worldSpaceHumanState.quats.Spine3*localSpaceBonesQuat[7];
	worldSpaceHumanState.Rotators.LeftShoulder = worldSpaceHumanState.quats.LeftShoulder.Rotator();

	worldSpaceHumanState.positions.LeftArm = worldSpaceBonesPos[8];
	worldSpaceHumanState.quats.LeftArm = worldSpaceHumanState.quats.LeftShoulder*localSpaceBonesQuat[8];
	worldSpaceHumanState.Rotators.LeftArm = worldSpaceHumanState.quats.LeftArm.Rotator();

	worldSpaceHumanState.positions.LeftForeArm = worldSpaceBonesPos[9];
	worldSpaceHumanState.quats.LeftForeArm = worldSpaceHumanState.quats.LeftArm*localSpaceBonesQuat[9];
	worldSpaceHumanState.Rotators.LeftForeArm = worldSpaceHumanState.quats.LeftForeArm.Rotator();

	worldSpaceHumanState.positions.LeftHand = worldSpaceBonesPos[10];
	worldSpaceHumanState.quats.LeftHand = worldSpaceHumanState.quats.LeftForeArm*localSpaceBonesQuat[10];
	worldSpaceHumanState.Rotators.LeftHand = worldSpaceHumanState.quats.LeftHand.Rotator();

	worldSpaceHumanState.positions.RightShoulder = worldSpaceBonesPos[11];
	worldSpaceHumanState.quats.RightShoulder = worldSpaceHumanState.quats.Spine3*localSpaceBonesQuat[11];
	worldSpaceHumanState.Rotators.RightShoulder = worldSpaceHumanState.quats.RightShoulder.Rotator();

	worldSpaceHumanState.positions.RightArm = worldSpaceBonesPos[12];
	worldSpaceHumanState.quats.RightArm = worldSpaceHumanState.quats.RightShoulder*localSpaceBonesQuat[12];
	worldSpaceHumanState.Rotators.RightArm = worldSpaceHumanState.quats.RightArm.Rotator();

	worldSpaceHumanState.positions.RightforeArm = worldSpaceBonesPos[13];
	worldSpaceHumanState.quats.RightforeArm = worldSpaceHumanState.quats.RightArm*localSpaceBonesQuat[13];
	worldSpaceHumanState.Rotators.RightforeArm = worldSpaceHumanState.quats.RightforeArm.Rotator();

	worldSpaceHumanState.positions.RightHand = worldSpaceBonesPos[14];
	worldSpaceHumanState.quats.RightHand = worldSpaceHumanState.quats.RightforeArm*localSpaceBonesQuat[14];
	worldSpaceHumanState.Rotators.RightHand = worldSpaceHumanState.quats.RightHand.Rotator();

	worldSpaceHumanState.positions.LeftUpLeg = worldSpaceBonesPos[15];
	worldSpaceHumanState.quats.LeftUpLeg = worldSpaceHumanState.quats.Hips*localSpaceBonesQuat[15];
	worldSpaceHumanState.Rotators.LeftUpLeg = worldSpaceHumanState.quats.LeftUpLeg.Rotator();

	worldSpaceHumanState.positions.LeftLeg = worldSpaceBonesPos[16];
	worldSpaceHumanState.quats.LeftLeg = worldSpaceHumanState.quats.LeftUpLeg*localSpaceBonesQuat[16];
	worldSpaceHumanState.Rotators.LeftLeg = worldSpaceHumanState.quats.LeftLeg.Rotator();

	worldSpaceHumanState.positions.LeftFoot = worldSpaceBonesPos[17];
	worldSpaceHumanState.quats.LeftFoot = worldSpaceHumanState.quats.LeftLeg*localSpaceBonesQuat[17];
	worldSpaceHumanState.Rotators.LeftFoot = worldSpaceHumanState.quats.LeftFoot.Rotator();

	worldSpaceHumanState.positions.LeftToeBase = worldSpaceBonesPos[18];
	worldSpaceHumanState.quats.LeftToeBase = worldSpaceHumanState.quats.LeftFoot *localSpaceBonesQuat[18];
	worldSpaceHumanState.Rotators.LeftToeBase = worldSpaceHumanState.quats.LeftToeBase.Rotator();

	worldSpaceHumanState.positions.RightUpLeg = worldSpaceBonesPos[19];
	worldSpaceHumanState.quats.RightUpLeg = worldSpaceHumanState.quats.Hips*localSpaceBonesQuat[19];
	worldSpaceHumanState.Rotators.RightUpLeg = worldSpaceHumanState.quats.RightUpLeg.Rotator();

	worldSpaceHumanState.positions.RightLeg = worldSpaceBonesPos[20];
	worldSpaceHumanState.quats.RightLeg = worldSpaceHumanState.quats.RightUpLeg*localSpaceBonesQuat[20];
	worldSpaceHumanState.Rotators.RightLeg = worldSpaceHumanState.quats.RightLeg.Rotator();

	worldSpaceHumanState.positions.RightFoot = worldSpaceBonesPos[21];
	worldSpaceHumanState.quats.RightFoot = worldSpaceHumanState.quats.RightLeg*localSpaceBonesQuat[21];
	worldSpaceHumanState.Rotators.RightFoot = worldSpaceHumanState.quats.RightFoot.Rotator();

	worldSpaceHumanState.positions.RightToeBase = worldSpaceBonesPos[22];
	worldSpaceHumanState.quats.RightToeBase = worldSpaceHumanState.quats.RightFoot *localSpaceBonesQuat[22];
	worldSpaceHumanState.Rotators.RightToeBase = worldSpaceHumanState.quats.RightToeBase.Rotator();

	return &worldSpaceHumanState;
}