// Fill out your copyright notice in the Description page of Project Settings.

#include "ChingMUComponent.h"
#include "MotionCapturePrivatePCH.h"

#include "FMotionCapture.h"

#include "Engine.h"
//#include "FMotionCapture.h"
#include "HeadMountedDisplay.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "IXRTrackingSystem.h"
#include <stdio.h>
#include "../Interface/ChingmuHumanCap/ChingmuHumanCap.h"
//#include "../Interface/SpringVRHandCap/SpringVRHandCap.h"

#define SEQUENCERECORDER_API
//#include "SequenceRecorder/Public/ISequenceRecorder.h"
//#include "ISequenceRecorder.h"
#include "../Interface/ChingmuIQFace/IQFace.h"
#include "Sockets.h"
#include "Networking.h"

#pragma warning(disable:4996)
UChingMUComponent::UChingMUComponent()
{
	//APlayerController


	PrimaryComponentTick.bCanEverTick = true;

	// load ChingMU plugin
	m_motioncap = &IMotionCapture::Get();
	if (m_motioncap)
	{
		UE_LOG(LogTemp, Display, TEXT(" ChingMU_ComponnetStance get it and start using......"));
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("ChingMU_ComponnetStance get faild  and can't not using......  "));
	}
	//Init Mocap¡¢handCap¡¢FaceCap
	humanCap = new ChingmuHumanCap();
	//handCap = new SpringVRHandCap();
	faceCap = new IQFace();

	for (size_t i = 0; i < sizeof(curBtnState) / sizeof(bool); i++)
	{
		curBtnState[i] = false;
		preBtnState[i] = false;
		IsButtonDown[i] = false;
		IsButtonStay[i] = false;
		IsButtonUp[i] = false;
	}

	IsForceTpose = false;
	m_loadConfig = false;
	CharacterSkinMesh = nullptr;

}


void UChingMUComponent::BeginPlay()
{
	m_loadConfig = false;
	Super::BeginPlay();

	// read soldier config file
	FString GameConfigDir = FPaths::ProjectConfigDir();
	FString ConfigDir = GameConfigDir + "config.txt";
	FILE *pConfig = _tfopen(*ConfigDir, L"rt");
	if (!pConfig)
	{
		GLog->Logf(ELogVerbosity::Warning, TEXT("MotionCapture: Failed to read config.txt"));
		return;
	}

	fscanf(pConfig, "%s", &m_ServerAddr);
	fscanf(pConfig, "%d", &m_HeadID);
	fscanf(pConfig, "%d", &m_WeaponID);
	fscanf(pConfig, "%d", &m_BodyID);
	fscanf(pConfig, "%d", &m_LeftHandID);
	fscanf(pConfig, "%d", &m_RightHandID);
	fscanf(pConfig, "%d", &m_LeftAnkleID);
	fscanf(pConfig, "%d", &m_RightAnkleID);
	fscanf(pConfig, "%d", &m_HumanID);

	fclose(pConfig);

#if WITH_EDITOR
	// test Sequence Recorder by C++
	SequenceRecorder = &FModuleManager::LoadModuleChecked<ISequenceRecorder>("SequenceRecorder");
#endif // !1
	//´´½¨¹Ç÷ÀÓ³Éä£»
	CreateBoneIndexToBoneNameMap(humanBoneNames);

	CharacterSkinMesh = dynamic_cast<USkinnedMeshComponent*> (this->GetOwner()->GetComponentByClass(TSubclassOf<USkinnedMeshComponent>(USkinnedMeshComponent::StaticClass())));

	//SetModelArmsAndLegsInTpose();

	if (IsForceTpose)
	{
		SetModelArmsAndLegsInTpose();
	}
	if (CharacterSkinMesh)
	{
		GetTposeBonesRotation(CharacterSkinMesh);
		GetTposeParentBonesRotation(CharacterSkinMesh);

	}

	m_loadConfig = true;
}

void UChingMUComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UChingMUComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void UChingMUComponent::StartConnectServer()
{
	if (!m_motioncap)return;
	FString s2 = "ChingMU Commponet instance Start Connect And Register Event from ChingMU Commponent.....................    ";
	UE_LOG(LogTemp, Log, TEXT(" %s"), *s2);
	FMotionCapture* motionCapturePlugin =(FMotionCapture*)m_motioncap;
	FString addr;
	if (motionCapturePlugin->CnState == ConnectState::NoConnect)
	{
		addr = motionCapturePlugin->CMServerAddress;
		if (addr.Contains("MCServer@") && addr.Contains("3884"))
		{
			motionCapturePlugin->ConnectCommand = "ConnectServer";
		}
	}
	else
	{
		FString s3 = motionCapturePlugin->CMServerAddress + " it is Connected or Connecting,in ChingMU MotionCapRetargetPanel from ChingMU Commponent";
		UE_LOG(LogTemp, Warning, TEXT(" %s"), *s3);
	}
}

void UChingMUComponent::DisConnectServer()
{
	if (!m_motioncap)return;
	FString s2 = "ChingMU Commponet instance Start DisConnect And UnRegister Event from ChingMU Commponent.....................    ";
	UE_LOG(LogTemp, Log, TEXT(" %s"), *s2);
	FMotionCapture* motionCapturePlugin = (FMotionCapture*)m_motioncap;
	FString addr;
	if (motionCapturePlugin->CnState == ConnectState::Connected)
	{
		addr = motionCapturePlugin->CMServerAddress;
		if (addr.Contains("MCServer@") && addr.Contains("3884"))
		{
			motionCapturePlugin->ConnectCommand = "DisConnect";
		}
	}
	else
	{
		motionCapturePlugin->CnState = ConnectState::NoConnect;
		addr = motionCapturePlugin->CMServerAddress;
		if (addr.Contains("MCServer@") && addr.Contains("3884"))
		{
			motionCapturePlugin->ConnectCommand = "DisConnect";
		}
		FString s3 = motionCapturePlugin->CMServerAddress + " it is not Connected from ChingMU Commponent ";
		UE_LOG(LogTemp, Warning, TEXT(" %s"), *s3);
	}
}

UChingMUComponent::~UChingMUComponent()
{
	if (humanCap != NULL)
		delete humanCap;
	if (handCap != NULL)
		delete handCap;
	if (faceCap != NULL)
		delete faceCap;


	humanCap = NULL;
	handCap = NULL;
	faceCap = NULL;
}

void UChingMUComponent::GetHeadPose(FString ServerAddr, int32 HeadID, FTransform &pose)
{
	// Get ChingMU Soldier state
	if (!m_motioncap)return;

	if (GEngine->XRSystem.IsValid() && GEngine->XRSystem->GetHMDDevice() && GEngine->XRSystem->GetHMDDevice()->IsHMDConnected())
	{
		// get HeadDevice IMU(  rotation and position)
		//GEngine->HMDDevice->GetCurrentOrientationAndPosition(DeviceQuat, DevicePos);  //for 4.17

		GEngine->XRSystem->GetCurrentPose(IXRTrackingSystem::HMDDeviceId, DeviceQuat, DevicePos); //for 4.19
		double R_oculus[4] = { DeviceQuat.X, DeviceQuat.Y, DeviceQuat.Z, DeviceQuat.W };
		double P_oculus[3] = { DevicePos.X, DevicePos.Y, DevicePos.Z };

		
		// get ChingMU Head rotation and position
		pose.SetLocation(m_motioncap->CMHeadPos(TCHAR_TO_ANSI(*ServerAddr), HeadID, R_oculus, P_oculus));
		pose.SetRotation(m_motioncap->CMHeadQuat(TCHAR_TO_ANSI(*ServerAddr), HeadID, R_oculus, P_oculus));
	}
	else
	{
		pose.SetLocation(m_motioncap->CMTrackerPos(TCHAR_TO_ANSI(*ServerAddr), HeadID));
		pose.SetRotation(m_motioncap->CMTrackerQuat(TCHAR_TO_ANSI(*ServerAddr), HeadID));
	}

	pose.SetScale3D(FVector(0.0001, 0.0001, 0.0001));
}

void UChingMUComponent::GetHumanHeadPose(FString ServerAddr, int32 HumanID, FTransform &pose)
{
	check(!ServerAddr.Contains(":3884"));

	GetHeadPose(ServerAddr, 100 + 24 * (HumanID + 1) - 1, pose);
}

void UChingMUComponent::GetTrackerPose(FString ServerAddr, int32 TrackerID, FTransform &pose)
{
	// Get ChingMU Soldier state
	if (!m_motioncap)	return;
	if (TrackerID < 0)	return;
	pose.SetLocation(m_motioncap->CMTrackerPos(TCHAR_TO_ANSI(*ServerAddr), TrackerID));
	pose.SetRotation(m_motioncap->CMTrackerQuat(TCHAR_TO_ANSI(*ServerAddr), TrackerID));
}

void UChingMUComponent::GetTrackerPoseTC(FString ServerAddr, int32 TrackerID, FTransform &pose, FTimecode &time)
{
	if (!m_motioncap)	return;
	if (TrackerID < 0)	return;

	FTransform transform = m_motioncap->CMTrackerTC(TCHAR_TO_ANSI(*ServerAddr), TrackerID, time);
	pose.SetLocation(transform.GetLocation());
	pose.SetRotation(transform.GetRotation());
}

void UChingMUComponent::GetTouchPose(FString ServerAddr, FTransform &pose, int32 TrackerID, FTransform RT, FRotator TouchRotator)
{
	if (!m_motioncap)	return;
	if (TrackerID < 0)	return;

	FQuat quatOf_RT = FQuat(RT.GetRotation());
	FVector LocOf_RT = RT.GetLocation();
	double touchQuat[4] = { quatOf_RT.X, quatOf_RT.Y, quatOf_RT.Z, quatOf_RT.W };
	double touchPos[3] = { LocOf_RT.X , LocOf_RT.Y, LocOf_RT.Z };

	pose.SetLocation(m_motioncap->CMHeadPos(TCHAR_TO_ANSI(*ServerAddr), TrackerID, touchQuat, touchPos));
	FQuat actualQuat = m_motioncap->CMHeadQuat(TCHAR_TO_ANSI(*ServerAddr), TrackerID, touchQuat, touchPos)*TouchRotator.Quaternion();
	pose.SetRotation(actualQuat);
}

bool UChingMUComponent::CMTrackerIsDetected(FString ServerAddr, int32 TrackerID)
{
	return m_motioncap->CMTrackerIsDetected(TCHAR_TO_ANSI(*ServerAddr), TrackerID);
}

FString UChingMUComponent::GetConfigServerAddr()
{
	if (!m_loadConfig)
		return FString();

	return FString(m_ServerAddr);
}

bool UChingMUComponent::GetConfigIKTrackerID(int32 &headID, int32 &gunID, int32 &bodyID, int32 &lhandID, int32 &rhandID, int32 &lfootID, int32 &rfootID)
{
	if (!m_loadConfig)
		return false;

	headID = m_HeadID;
	gunID = m_WeaponID;
	bodyID = m_BodyID;
	lhandID = m_LeftHandID;
	rhandID = m_RightHandID;
	lfootID = m_LeftAnkleID;
	rfootID = m_RightAnkleID;

	return true;
}

int32 UChingMUComponent::GetConfigHumanID()
{
	if (!m_loadConfig)
		return 0;

	return m_HumanID;
}

void UChingMUComponent::GetHumanState(FString ServerAddr, int32 ID, FWorldSpaceHumanState & state)
{
	if (!humanCap)return;
	check(!ServerAddr.Contains(":3884"));

	state = *(humanCap->GetHumanState(TCHAR_TO_ANSI(*ServerAddr), &ID));

}


void UChingMUComponent::SetHumanState(UPoseableMeshComponent *human, FHumanBonesName names, FWorldSpaceHumanState state)
{
	human->SetBoneLocationByName(names.Hips, state.positions.Hips, EBoneSpaces::ComponentSpace);

	human->SetBoneRotationByName(names.Hips, state.Rotators.Hips, EBoneSpaces::ComponentSpace);
	human->SetBoneRotationByName(names.Spine, state.Rotators.Spine, EBoneSpaces::ComponentSpace);
	human->SetBoneRotationByName(names.Spine1, state.Rotators.Spine1, EBoneSpaces::ComponentSpace);
	human->SetBoneRotationByName(names.Spine2, state.Rotators.Spine2, EBoneSpaces::ComponentSpace);
	human->SetBoneRotationByName(names.Spine3, state.Rotators.Spine3, EBoneSpaces::ComponentSpace);
	human->SetBoneRotationByName(names.Neck, state.Rotators.Neck, EBoneSpaces::ComponentSpace);
	human->SetBoneRotationByName(names.Head, state.Rotators.Head, EBoneSpaces::ComponentSpace);

	human->SetBoneRotationByName(names.LeftShoulder, state.Rotators.LeftShoulder, EBoneSpaces::ComponentSpace);
	human->SetBoneRotationByName(names.LeftArm, state.Rotators.LeftArm, EBoneSpaces::ComponentSpace);
	human->SetBoneRotationByName(names.LeftForeArm, state.Rotators.LeftForeArm, EBoneSpaces::ComponentSpace);
	human->SetBoneRotationByName(names.LeftHand, state.Rotators.LeftHand, EBoneSpaces::ComponentSpace);

	human->SetBoneRotationByName(names.RightShoulder, state.Rotators.RightShoulder, EBoneSpaces::ComponentSpace);
	human->SetBoneRotationByName(names.RightArm, state.Rotators.RightArm, EBoneSpaces::ComponentSpace);
	human->SetBoneRotationByName(names.RightForeArm, state.Rotators.RightforeArm, EBoneSpaces::ComponentSpace);
	human->SetBoneRotationByName(names.RightHand, state.Rotators.RightHand, EBoneSpaces::ComponentSpace);

	human->SetBoneRotationByName(names.LeftUpLeg, state.Rotators.LeftUpLeg, EBoneSpaces::ComponentSpace);
	human->SetBoneRotationByName(names.LeftLeg, state.Rotators.LeftLeg, EBoneSpaces::ComponentSpace);
	human->SetBoneRotationByName(names.LeftFoot, state.Rotators.LeftFoot, EBoneSpaces::ComponentSpace);
	human->SetBoneRotationByName(names.LeftToeBase, state.Rotators.LeftToeBase, EBoneSpaces::ComponentSpace);

	human->SetBoneRotationByName(names.RightUpLeg, state.Rotators.RightUpLeg, EBoneSpaces::ComponentSpace);
	human->SetBoneRotationByName(names.RightLeg, state.Rotators.RightLeg, EBoneSpaces::ComponentSpace);
	human->SetBoneRotationByName(names.RightFoot, state.Rotators.RightFoot, EBoneSpaces::ComponentSpace);
	human->SetBoneRotationByName(names.RightToeBase, state.Rotators.RightToeBase, EBoneSpaces::ComponentSpace);
}

void SetBoneStateRecusive(UPoseableMeshComponent *human, FMotionCaptureSubjectItemPtr subject, FVector pos[150], FQuat rot[150])
{
	FRotator offSetRot = FRotator(0, 90, 0);
	FString boneName = subject->SubjectName.ToString();
	FName   boneName2 = FName(*boneName);
	int boneIndex = (subject->SubjectKey - 100) % 150;
	if (boneName == "Hips")
		human->SetBoneLocationByName(boneName2, pos[boneIndex], EBoneSpaces::ComponentSpace);

	human->SetBoneRotationByName(boneName2, (offSetRot.Quaternion()*rot[boneIndex]).Rotator(), EBoneSpaces::ComponentSpace);

	for (auto child : subject->Children)
	{
		SetBoneStateRecusive(human, child, pos, rot);
	}
}

void UChingMUComponent::SetRetargetHumanState(UPoseableMeshComponent *human, FString name)
{
	if (!human || !m_motioncap)
		return;

	// get human instance name
	/*int len;
	FString name = UKismetSystemLibrary::GetDisplayName(human);
	if (name.FindChar('.', len))
		name = name.Left(len);*/

		// driven bind mesh
	for (auto sklt : ((FMotionCapture*)m_motioncap)->BindSkeletons)
	{
		if (name == sklt.Value)
		{
			FVector temp_pos[150];
			FQuat boneRot[150];
			FTimecode curTimeCode;
			if (!m_motioncap->CMRetargetHumanTC(TCHAR_TO_ANSI(*((FMotionCapture*)m_motioncap)->CMTrackerAddress.ToString()), sklt.Key, curTimeCode, temp_pos, boneRot))
				return;

			// find subject
			int32 trackerID = sklt.Key * 150 + 100;
			for (auto itr : ((FMotionCapture*)m_motioncap)->SubjectData)
			{
				if (itr->SubjectKey == trackerID)
				{
					SetBoneStateRecusive(human, itr, temp_pos, boneRot);
					break;
				}
			}

			break;
		}
	}
}

#if WITH_EDITOR
#include "SequenceRecorder/Public/ISequenceRecorder.h"
void UChingMUComponent::StartRecord(USkeletalMeshComponent *human, FString SequenceName)
{
	//TArrayView<AActor* const> ActorsToRecord;// = GetActorsToRecord();
	if (SequenceRecorder)
		SequenceRecorder->StartRecording(human->GetOwner(), FString(), SequenceName);
}

void UChingMUComponent::StopRecord()
{
	if (SequenceRecorder)
		SequenceRecorder->StopRecording();
}
#endif // !1
void UChingMUComponent::GetHandsState(FWorldSpaceHandState & leftHand, FWorldSpaceHandState & rightHand)
{
	if (!handCap)return;
	handCap->GetHandsState(leftHand, rightHand);
}

void UChingMUComponent::SetHandsState(FFingersName lefthandFingersName, FFingersName righthandFingersName,
	FWorldSpaceHandState leftHand, FWorldSpaceHandState rightHand,
	UPoseableMeshComponent * humanPoseableMesh)
{
	if (!handCap || humanPoseableMesh == nullptr)return;
	handCap->SetHandsState(&lefthandFingersName, &righthandFingersName, &leftHand, &rightHand, humanPoseableMesh);
}

FString UChingMUComponent::GetStageNotesFQN()
{
	if (!m_motioncap)
		return FString();

	return ((FMotionCapture*)m_motioncap)->StageNotesFQN;
}

FTimecode UChingMUComponent::GetTimeCode()
{
	if (!m_motioncap)
		return FTimecode();

	return ((FMotionCapture*)m_motioncap)->Timecode;
}

void UChingMUComponent::CreateBoneIndexToBoneNameMap(FHumanBonesName names)
{
	BoneIndexToBoneNameMap.Emplace(0, names.Hips);
	BoneIndexToBoneNameMap.Emplace(1, names.Spine);
	BoneIndexToBoneNameMap.Emplace(2, names.Spine1);
	BoneIndexToBoneNameMap.Emplace(3, names.Spine2);
	BoneIndexToBoneNameMap.Emplace(4, names.Spine3);
	BoneIndexToBoneNameMap.Emplace(5, names.Neck);
	BoneIndexToBoneNameMap.Emplace(6, names.Head);
	BoneIndexToBoneNameMap.Emplace(7, names.LeftShoulder);
	BoneIndexToBoneNameMap.Emplace(8, names.LeftArm);
	BoneIndexToBoneNameMap.Emplace(9, names.LeftForeArm);
	BoneIndexToBoneNameMap.Emplace(10, names.LeftHand);
	BoneIndexToBoneNameMap.Emplace(11, names.RightShoulder);
	BoneIndexToBoneNameMap.Emplace(12, names.RightArm);
	BoneIndexToBoneNameMap.Emplace(13, names.RightForeArm);
	BoneIndexToBoneNameMap.Emplace(14, names.RightHand);
	BoneIndexToBoneNameMap.Emplace(15, names.LeftUpLeg);
	BoneIndexToBoneNameMap.Emplace(16, names.LeftLeg);
	BoneIndexToBoneNameMap.Emplace(17, names.LeftFoot);
	BoneIndexToBoneNameMap.Emplace(18, names.LeftToeBase);
	BoneIndexToBoneNameMap.Emplace(19, names.RightUpLeg);
	BoneIndexToBoneNameMap.Emplace(20, names.RightLeg);
	BoneIndexToBoneNameMap.Emplace(21, names.RightFoot);
	BoneIndexToBoneNameMap.Emplace(22, names.RightToeBase);
}

void UChingMUComponent::GetTposeBonesRotation(USkinnedMeshComponent *SkinMesh)
{
	for (int i = 0; i < 23; i++)
	{
		if (BoneIndexToBoneNameMap[i] == NAME_None)
		{

			continue;
		}
		TposeBonesComponentSpaceRotation[i] = SkinMesh->GetBoneQuaternion(BoneIndexToBoneNameMap[i], EBoneSpaces::ComponentSpace);
		TposeBonesWorldSpaceRotation[i] = SkinMesh->GetBoneQuaternion(BoneIndexToBoneNameMap[i], EBoneSpaces::WorldSpace);

	}
}

void UChingMUComponent::GetTposeParentBonesRotation(USkinnedMeshComponent *SkinMesh)
{
	for (int i = 0; i < 23; i++)
	{
		if (BoneIndexToBoneNameMap[i] == NAME_None)
		{
			continue;
		}
		TposeParentBonesComponentSpaceRotation[i] = SkinMesh->GetBoneQuaternion(SkinMesh->GetParentBone(BoneIndexToBoneNameMap[i]), EBoneSpaces::ComponentSpace);
		TposeParentBonesWorldSpaceRotation[i] = SkinMesh->GetBoneQuaternion(SkinMesh->GetParentBone(BoneIndexToBoneNameMap[i]), EBoneSpaces::WorldSpace);
	}
}


void UChingMUComponent::FullBodyMotionCapBaseBonesComponentSpaceRotation(UPoseableMeshComponent *SkinMesh, FString address, int32 HuamnID)
{
	FVector RootBonePos = FVector::ZeroVector;
	if (!address.IsEmpty() && HuamnID >= 0)
	{
		m_motioncap->CMHuman(TCHAR_TO_ANSI(*address), HuamnID, &RootBonePos, BonesRealTimeMotionCapturLocalRotation);
	}
	else
	{
		m_motioncap->CMHuman(m_ServerAddr, m_HumanID, &RootBonePos, BonesRealTimeMotionCapturLocalRotation);
	}

	SkinMesh->SetBoneLocationByName(BoneIndexToBoneNameMap[0], RootBonePos, EBoneSpaces::ComponentSpace);
	for (int i = 0; i < 23; i++)
	{

		if (TposeBonesComponentSpaceRotation[i] == FQuat::Identity)
		{
			continue;
		}
		//UE_LOG(LogTemp, Display, TEXT("BoneName: %s"), *BoneIndexToBoneNameMap[i].ToString());

		FQuat BoneFinalComponentSpaceRotation = FQuat::Identity;
		BoneFinalComponentSpaceRotation = SkinMesh->GetBoneQuaternion(SkinMesh->GetParentBone(BoneIndexToBoneNameMap[i]), EBoneSpaces::ComponentSpace) * TposeParentBonesComponentSpaceRotation[i].Inverse() * BonesRealTimeMotionCapturLocalRotation[i] * TposeBonesComponentSpaceRotation[i];

		//BoneFinalWorldRotation = SkinMesh->GetBoneQuaternion(SkinMesh->GetParentBone(BoneIndexToBoneNameMap[i]), EBoneSpaces::WorldSpace) *BonesRealTimeMotionCapturLocalRotation[i];
		SkinMesh->SetBoneRotationByName(BoneIndexToBoneNameMap[i], BoneFinalComponentSpaceRotation.Rotator(), EBoneSpaces::ComponentSpace);
	}
}
void UChingMUComponent::FullBodyMotionCapBaseBonesWorldSpaceRotation(UPoseableMeshComponent *SkinMesh, FString address, int32 HuamnID)
{

	FVector RootBonePos = FVector::ZeroVector;
	if (!address.IsEmpty() && HuamnID >= 0)
	{
		m_motioncap->CMHuman(TCHAR_TO_ANSI(*address), HuamnID, &RootBonePos, BonesRealTimeMotionCapturLocalRotation);
	}
	else
	{
		m_motioncap->CMHuman(m_ServerAddr, m_HumanID, &RootBonePos, BonesRealTimeMotionCapturLocalRotation);
	}

	SkinMesh->SetBoneLocationByName(BoneIndexToBoneNameMap[0], RootBonePos, EBoneSpaces::WorldSpace);
	for (int i = 0; i < 23; i++)
	{

		if (TposeBonesWorldSpaceRotation[i] == FQuat::Identity)
		{
			continue;
		}
		//UE_LOG(LogTemp, Display, TEXT("BoneName: %s"), *BoneIndexToBoneNameMap[i].ToString());

		FQuat BoneFinalWorldRotation = FQuat::Identity;
		BoneFinalWorldRotation = SkinMesh->GetBoneQuaternion(SkinMesh->GetParentBone(BoneIndexToBoneNameMap[i]), EBoneSpaces::WorldSpace) * TposeParentBonesWorldSpaceRotation[i].Inverse() * BonesRealTimeMotionCapturLocalRotation[i] * TposeBonesWorldSpaceRotation[i];

		//BoneFinalWorldRotation = SkinMesh->GetBoneQuaternion(SkinMesh->GetParentBone(BoneIndexToBoneNameMap[i]), EBoneSpaces::WorldSpace) *BonesRealTimeMotionCapturLocalRotation[i];
		SkinMesh->SetBoneRotationByName(BoneIndexToBoneNameMap[i], BoneFinalWorldRotation.Rotator(), EBoneSpaces::WorldSpace);
	}
}

void UChingMUComponent::CalculateBoneCSRotation(FString address, int32 HuamnID, FVector& CharacterRootBonePos, FQuat* BonesComponentSpaceRotation)
{
	//FVector RootBonePos = FVector::ZeroVector;
   // RootBonePos = FVector::ZeroVector;
	if (!address.IsEmpty() && HuamnID >= 0)
	{
		m_motioncap->CMHuman(TCHAR_TO_ANSI(*address), HuamnID, &CharacterRootBonePos, BonesRealTimeMotionCapturLocalRotation);
	}
	else
	{
		m_motioncap->CMHuman(m_ServerAddr, m_HumanID, &CharacterRootBonePos, BonesRealTimeMotionCapturLocalRotation);
	}

	//SkinMesh->SetBoneLocationByName(BoneIndexToBoneNameMap[0], RootBonePos, EBoneSpaces::ComponentSpace);
	for (int i = 0; i < 23; i++)
	{

		if (TposeBonesComponentSpaceRotation[i] == FQuat::Identity)
		{
			continue;
		}

		//FQuat BoneFinalWorldRotation = FQuat::Identity;


		BonesComponentSpaceRotation[i] = CharacterSkinMesh->GetBoneQuaternion(CharacterSkinMesh->GetParentBone(BoneIndexToBoneNameMap[i]), EBoneSpaces::ComponentSpace) * TposeParentBonesComponentSpaceRotation[i].Inverse() * BonesRealTimeMotionCapturLocalRotation[i] * TposeBonesComponentSpaceRotation[i];

	}
}


void UChingMUComponent::SetModelArmsAndLegsInTpose()
{
	FVector vTposeRightDir = this->GetOwner()->GetActorRightVector();
	//FVector vTposeRightDir = this->GetOwner()->GetActorTransform() .TransformVector(FVector(0, 1, 0));
	FVector vTposeLeftDir = vTposeRightDir * -1;
	//FVector vTposeLeftDir = this->GetOwner()->GetActorTransform().TransformVector(FVector(0, -1, 0));
	FVector vTposeUpDir = this->GetOwner()->GetActorUpVector();
	//FVector vTposeDownDir = this->GetOwner()->GetActorTransform().TransformVector(FVector(0, 0, -1));
	FVector vTposeDownDir = vTposeUpDir * -1;

	UPoseableMeshComponent*  CharacterPoseableMesh = dynamic_cast<UPoseableMeshComponent*>(CharacterSkinMesh);
	if (CharacterPoseableMesh == nullptr)
	{
		//UE_LOG(LogTemp, Display, TEXT("not be UPoseableMeshComponent,can not force Tpose "));
		return;
	}
	//BoneIndexToBoneNameMap[8].ToString().IsEmpty();
	if (BoneIndexToBoneNameMap[8] != NAME_None && BoneIndexToBoneNameMap[9] != NAME_None)
	{
		FVector vLeftUparmDir = CharacterPoseableMesh->GetBoneLocation(BoneIndexToBoneNameMap[9]) - CharacterPoseableMesh->GetBoneLocation(BoneIndexToBoneNameMap[8]);
		float vLeftUpArmDir_vTposeLeftDir_dot = FVector::DotProduct(vLeftUparmDir, vTposeLeftDir);
		float vLeftUpArmDir_vTposeLeftDir_radian = acos(vLeftUpArmDir_vTposeLeftDir_dot / (vLeftUparmDir.Size()*vTposeLeftDir.Size()));
		float vLeftUpArmDir_vTposeLeftDir_degree = vLeftUpArmDir_vTposeLeftDir_radian * (180 / PI);
		//UE_LOG(LogTemp, Display, TEXT("debug:: ......  "));
		//UE_LOG(LogTemp, Log, TEXT("%f"), vLeftUpArmDir_vTposeLeftDir_degree);
		if (abs(vLeftUpArmDir_vTposeLeftDir_degree) >= 5.0f)
		{
			FVector rotationAxis = FVector::CrossProduct(vLeftUparmDir, vTposeLeftDir);
			rotationAxis.Normalize();

			FQuat vFixRotation = FQuat(rotationAxis, vLeftUpArmDir_vTposeLeftDir_radian);
			FQuat finalUpArmRotation = vFixRotation * CharacterPoseableMesh->GetBoneQuaternion(BoneIndexToBoneNameMap[8], EBoneSpaces::WorldSpace);
			CharacterPoseableMesh->SetBoneRotationByName(BoneIndexToBoneNameMap[8], finalUpArmRotation.Rotator(), EBoneSpaces::WorldSpace);
		}
		if (BoneIndexToBoneNameMap[10] != NAME_None)
		{
			FVector vLeftLowerArmDir = CharacterPoseableMesh->GetBoneLocation(BoneIndexToBoneNameMap[10]) - CharacterPoseableMesh->GetBoneLocation(BoneIndexToBoneNameMap[9]);
			float vLeftLowerArmDir_vTposeLeftDir_dot = FVector::DotProduct(vLeftLowerArmDir, vTposeLeftDir);
			float vLeftLowerArmDir_vTposeLeftDir_radian = acos(vLeftLowerArmDir_vTposeLeftDir_dot / (vLeftLowerArmDir.Size()*vTposeLeftDir.Size()));
			float vLeftLowerArmDir_vTposeLeftDir_degree = vLeftLowerArmDir_vTposeLeftDir_radian * (180 / PI);

			//UE_LOG(LogTemp, Display, TEXT("debug:: ......  "));
			//UE_LOG(LogTemp, Log, TEXT("%f"), vLeftLowerArmDir_vTposeLeftDir_degree);
			if (abs(vLeftLowerArmDir_vTposeLeftDir_degree) >= 5.0f)
			{
				FVector rotationAxis = FVector::CrossProduct(vLeftLowerArmDir, vTposeLeftDir);
				rotationAxis.Normalize();
				FQuat vFixRotation = FQuat(rotationAxis, vLeftLowerArmDir_vTposeLeftDir_radian);
				//FQuat vFixRotation = FQuat(rotationAxis.X*sin(vLeftLowerArmDir_vTposeLeftDir_degree / 2), rotationAxis.Y*sin(vLeftLowerArmDir_vTposeLeftDir_degree / 2), rotationAxis.Z*sin(vLeftLowerArmDir_vTposeLeftDir_degree / 2), cos(vLeftLowerArmDir_vTposeLeftDir_degree / 2));
				FQuat finalLowerArmRotation = vFixRotation * CharacterPoseableMesh->GetBoneQuaternion(BoneIndexToBoneNameMap[9], EBoneSpaces::WorldSpace);

				//UPoseableMeshComponent*  CharacterPoseableMesh = dynamic_cast<UPoseableMeshComponent*>(CharacterSkinMesh);
				CharacterPoseableMesh->SetBoneRotationByName(BoneIndexToBoneNameMap[9], finalLowerArmRotation.Rotator(), EBoneSpaces::WorldSpace);

			}
		}

	}

	if (BoneIndexToBoneNameMap[12] != NAME_None && BoneIndexToBoneNameMap[13] != NAME_None)
	{

		FVector vRightUpArmDir = CharacterPoseableMesh->GetBoneLocation(BoneIndexToBoneNameMap[13]) - CharacterPoseableMesh->GetBoneLocation(BoneIndexToBoneNameMap[12]);
		float vRightUpArmDir_vTposeRightDir_dot = FVector::DotProduct(vRightUpArmDir, vTposeRightDir);
		float vRightUpArmDir_vTposeRightDir_radian = acos(vRightUpArmDir_vTposeRightDir_dot / (vRightUpArmDir.Size()*vTposeRightDir.Size()));
		float vRightUpArmDir_vTposeRightDir_degree = vRightUpArmDir_vTposeRightDir_radian * (180 / PI);

		if (abs(vRightUpArmDir_vTposeRightDir_degree) >= 5.0f)
		{
			FVector rotationAxis = FVector::CrossProduct(vRightUpArmDir, vTposeRightDir);
			rotationAxis.Normalize();
			FQuat vFixRotation = FQuat(rotationAxis, vRightUpArmDir_vTposeRightDir_radian);
			FQuat finalUpArmRotation = vFixRotation * CharacterPoseableMesh->GetBoneQuaternion(BoneIndexToBoneNameMap[12], EBoneSpaces::WorldSpace);

			//UPoseableMeshComponent*  CharacterPoseableMesh =dynamic_cast<UPoseableMeshComponent*>(CharacterSkinMesh);
			CharacterPoseableMesh->SetBoneRotationByName(BoneIndexToBoneNameMap[12], finalUpArmRotation.Rotator(), EBoneSpaces::WorldSpace);
		}
		if (BoneIndexToBoneNameMap[14] != NAME_None)
		{
			FVector vRightLowerArmDir = CharacterPoseableMesh->GetBoneLocation(BoneIndexToBoneNameMap[14]) - CharacterPoseableMesh->GetBoneLocation(BoneIndexToBoneNameMap[13]);
			float vRightLowerArmDir_vTposeRightDir_dot = FVector::DotProduct(vRightLowerArmDir, vTposeRightDir);
			float vRightLowerArmDir_vTposeRightDir_radian = acos(vRightLowerArmDir_vTposeRightDir_dot / (vRightLowerArmDir.Size()*vTposeRightDir.Size()));
			float vRightLowerArmDir_vTposeRightDir_degree = vRightLowerArmDir_vTposeRightDir_radian * (180 / PI);

			if (abs(vRightLowerArmDir_vTposeRightDir_degree) >= 5.0f)
			{
				FVector rotationAxis = FVector::CrossProduct(vRightLowerArmDir, vTposeRightDir);
				rotationAxis.Normalize();
				FQuat vFixRotation = FQuat(rotationAxis, vRightLowerArmDir_vTposeRightDir_radian);
				FQuat finalLowerArmRotation = vFixRotation * CharacterPoseableMesh->GetBoneQuaternion(BoneIndexToBoneNameMap[13], EBoneSpaces::WorldSpace);

				//UPoseableMeshComponent*  CharacterPoseableMesh = dynamic_cast<UPoseableMeshComponent*>(CharacterSkinMesh);
				CharacterPoseableMesh->SetBoneRotationByName(BoneIndexToBoneNameMap[13], finalLowerArmRotation.Rotator(), EBoneSpaces::WorldSpace);

			}
		}

	}


	if (BoneIndexToBoneNameMap[15] != NAME_None && BoneIndexToBoneNameMap[16] != NAME_None)
	{
		FVector vLeftUpLegDir = CharacterPoseableMesh->GetBoneLocation(BoneIndexToBoneNameMap[16]) - CharacterPoseableMesh->GetBoneLocation(BoneIndexToBoneNameMap[15]);
		float vLeftUpLegDir_vTposeDownDir_dot = FVector::DotProduct(vLeftUpLegDir, vTposeDownDir);
		float vLeftUpLegDir_vTposeDownDir_radian = acos(vLeftUpLegDir_vTposeDownDir_dot / (vLeftUpLegDir.Size()*vTposeDownDir.Size()));
		float vLeftUpLegDir_vTposeDownDir_degree = vLeftUpLegDir_vTposeDownDir_radian * (180 / PI);

		if (abs(vLeftUpLegDir_vTposeDownDir_degree) >= 5.0f)
		{
			FVector rotationAxis = FVector::CrossProduct(vLeftUpLegDir, vTposeDownDir);
			rotationAxis.Normalize();
			FQuat vFixRotation = FQuat(rotationAxis, vLeftUpLegDir_vTposeDownDir_radian);
			FQuat finalUpLegRotation = vFixRotation * CharacterPoseableMesh->GetBoneQuaternion(BoneIndexToBoneNameMap[15], EBoneSpaces::WorldSpace);

			//UPoseableMeshComponent*  CharacterPoseableMesh =dynamic_cast<UPoseableMeshComponent*>(CharacterSkinMesh);
			CharacterPoseableMesh->SetBoneRotationByName(BoneIndexToBoneNameMap[15], finalUpLegRotation.Rotator(), EBoneSpaces::WorldSpace);
		}
		if (BoneIndexToBoneNameMap[17] != NAME_None)
		{
			FVector vLeftLowerLegDir = CharacterPoseableMesh->GetBoneLocation(BoneIndexToBoneNameMap[17]) - CharacterPoseableMesh->GetBoneLocation(BoneIndexToBoneNameMap[16]);
			float vLeftLowerLegDir_vTposeDownDir_dot = FVector::DotProduct(vLeftLowerLegDir, vTposeDownDir);
			float vLeftLowerLegDir_vTposeDownDir_radian = acos(vLeftLowerLegDir_vTposeDownDir_dot / (vLeftLowerLegDir.Size()*vTposeDownDir.Size()));
			float vLeftLowerLegDir_vTposeDownDir_degree = vLeftLowerLegDir_vTposeDownDir_radian * (180 / PI);

			if (abs(vLeftLowerLegDir_vTposeDownDir_degree) >= 5.0f)
			{
				FVector rotationAxis = FVector::CrossProduct(vLeftLowerLegDir, vTposeDownDir);
				rotationAxis.Normalize();
				FQuat vFixRotation = FQuat(rotationAxis, vLeftLowerLegDir_vTposeDownDir_radian);
				FQuat finalLowerLegRotation = vFixRotation * CharacterPoseableMesh->GetBoneQuaternion(BoneIndexToBoneNameMap[16], EBoneSpaces::WorldSpace);

				//UPoseableMeshComponent*  CharacterPoseableMesh = dynamic_cast<UPoseableMeshComponent*>(CharacterSkinMesh);
				CharacterPoseableMesh->SetBoneRotationByName(BoneIndexToBoneNameMap[16], finalLowerLegRotation.Rotator(), EBoneSpaces::WorldSpace);

			}
		}

	}

	if (BoneIndexToBoneNameMap[19] != NAME_None && BoneIndexToBoneNameMap[20] != NAME_None)
	{
		FVector vRightUpLegDir = CharacterPoseableMesh->GetBoneLocation(BoneIndexToBoneNameMap[20]) - CharacterPoseableMesh->GetBoneLocation(BoneIndexToBoneNameMap[19]);
		float vRightUpLegDir_vTposeDownDir_dot = FVector::DotProduct(vRightUpLegDir, vTposeDownDir);
		float vRightUpLegDir_vTposeDownDir_radian = acos(vRightUpLegDir_vTposeDownDir_dot / (vRightUpLegDir.Size()*vTposeDownDir.Size()));
		float vRightUpLegDir_vTposeDownDir_degree = vRightUpLegDir_vTposeDownDir_radian * (180 / PI);

		if (abs(vRightUpLegDir_vTposeDownDir_degree) >= 5.0f)
		{
			FVector rotationAxis = FVector::CrossProduct(vRightUpLegDir, vTposeDownDir);
			rotationAxis.Normalize();
			FQuat vFixRotation = FQuat(rotationAxis, vRightUpLegDir_vTposeDownDir_radian);
			FQuat finalUpLegRotation = vFixRotation * CharacterPoseableMesh->GetBoneQuaternion(BoneIndexToBoneNameMap[19], EBoneSpaces::WorldSpace);

			//UPoseableMeshComponent*  CharacterPoseableMesh =dynamic_cast<UPoseableMeshComponent*>(CharacterSkinMesh);
			CharacterPoseableMesh->SetBoneRotationByName(BoneIndexToBoneNameMap[19], finalUpLegRotation.Rotator(), EBoneSpaces::WorldSpace);
		}
		if (BoneIndexToBoneNameMap[21] != NAME_None)
		{
			FVector vRightLowerLegDir = CharacterPoseableMesh->GetBoneLocation(BoneIndexToBoneNameMap[21]) - CharacterPoseableMesh->GetBoneLocation(BoneIndexToBoneNameMap[20]);
			float vRightLowerLegDir_vTposeDownDir_dot = FVector::DotProduct(vRightLowerLegDir, vTposeDownDir);
			float vRightLowerLegDir_vTposeDownDir_radian = acos(vRightLowerLegDir_vTposeDownDir_dot / (vRightLowerLegDir.Size()*vTposeDownDir.Size()));
			float vRightLowerLegDir_vTposeDownDir_degree = vRightLowerLegDir_vTposeDownDir_radian * (180 / PI);

			if (abs(vRightLowerLegDir_vTposeDownDir_degree) >= 5.0f)
			{
				FVector rotationAxis = FVector::CrossProduct(vRightLowerLegDir, vTposeDownDir);
				rotationAxis.Normalize();
				FQuat vFixRotation = FQuat(rotationAxis, vRightLowerLegDir_vTposeDownDir_radian);
				FQuat finalLowerLegRotation = vFixRotation * CharacterPoseableMesh->GetBoneQuaternion(BoneIndexToBoneNameMap[20], EBoneSpaces::WorldSpace);

				//UPoseableMeshComponent*  CharacterPoseableMesh = dynamic_cast<UPoseableMeshComponent*>(CharacterSkinMesh);
				CharacterPoseableMesh->SetBoneRotationByName(BoneIndexToBoneNameMap[20], finalLowerLegRotation.Rotator(), EBoneSpaces::WorldSpace);

			}
		}

	}
}
