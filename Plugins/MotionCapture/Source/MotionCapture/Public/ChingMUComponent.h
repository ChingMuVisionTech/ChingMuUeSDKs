// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "../model/MocapData.h"
#include "../model/HandData.h"
#include "../Interface/IHumanCap.h"
#include "../Interface/IHandCap.h"
#include "../Interface/IFaceCap.h"
#include "Components/PoseableMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Actor.h"
#include "Containers/Ticker.h"
#include "ChingMUComponent.generated.h"


class IMotionCapture;
class IHumanCap;
class IHandCap;
class IFaceCap;
class FSocket;
class ISequenceRecorder;

UCLASS(ClassGroup = ChingMU, meta = (BlueprintSpawnableComponent))
class MOTIONCAPTURE_API UChingMUComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UChingMUComponent();
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);
	~UChingMUComponent();

#pragma region  Chingmu Base Tracker///////////////////////////////////

	UFUNCTION(BlueprintPure, Category = "ChingMU")
		void GetHeadPose(FString ServerAddr, int32 HeadID, FTransform &pose);
	UFUNCTION(BlueprintPure, Category = "ChingMU")
		void GetHumanHeadPose(FString ServerAddr, int32 HumanID, FTransform &pose);

	UFUNCTION(BlueprintPure, Category = "ChingMU")
		void GetTrackerPose(FString ServerAddr, int32 TrackerID, FTransform &pose);
	UFUNCTION(BlueprintPure, Category = "ChingMU")
		void GetTrackerPoseTC(FString ServerAddr, int32 TrackerID, FTransform &pose, FTimecode &Timecode);
	UFUNCTION(BlueprintPure, Category = "ChingMU")
		void GetTouchPose(FString ServerAddr, FTransform &pose, int32 TrackerID, FTransform RT, FRotator TouchRotator);

	UFUNCTION(BlueprintPure, Category = "ChingMU")
		bool CMTrackerIsDetected(FString ServerAddr, int32 TrackerID);

	UFUNCTION(BlueprintPure, Category = "ChingMU")
		FString GetConfigServerAddr();
	UFUNCTION(BlueprintPure, Category = "ChingMU")
		bool GetConfigIKTrackerID(int32 &headID, int32 &gunID, int32 &bodyID, int32 &lhandID, int32 &rhandID, int32 &lfootID, int32 &rfootID);
	UFUNCTION(BlueprintPure, Category = "ChingMU")
		int32 GetConfigHumanID();




	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		bool IsForceTpose;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ChingMU")
		TMap<int, FName> BoneIndexToBoneNameMap;
     
	/** Start connect server and register. register action is only for retarget animation can be build */
	UFUNCTION(BlueprintCallable, Category = "ChingMU", meta = (PinShownByDefault))
		void StartConnectServer();
	/** DisConnect server and Unregister. Unregister action is only for retarget animation can be build */
	UFUNCTION(BlueprintCallable, Category = "ChingMU", meta = (PinShownByDefault))
		void DisConnectServer();

	UFUNCTION(BlueprintCallable, Category = "ChingMU")
		void SetModelArmsAndLegsInTpose();
	UFUNCTION(BlueprintCallable, Category = "ChingMU")
		void FullBodyMotionCapBaseBonesComponentSpaceRotation(UPoseableMeshComponent *SkinMesh, FString address, int32 HuamnID);
	UFUNCTION(BlueprintCallable, Category = "ChingMU")
		void FullBodyMotionCapBaseBonesWorldSpaceRotation(UPoseableMeshComponent *SkinMesh, FString address, int32 HuamnID);

	void CalculateBoneCSRotation(FString address, int32 HuamnID, FVector& CharacterRootBonePos, FQuat* BonesComponentSpaceRotation);

	bool m_loadConfig;
private:

	void CreateBoneIndexToBoneNameMap(FHumanBonesName names);

	FQuat TposeBonesComponentSpaceRotation[23];
	FQuat TposeParentBonesComponentSpaceRotation[23];

	FQuat TposeBonesWorldSpaceRotation[23];
	FQuat TposeParentBonesWorldSpaceRotation[23];
	//FVector TposeBonesWorldSpacePos[23];

	void GetTposeBonesRotation(USkinnedMeshComponent *SkinMesh);
	void GetTposeParentBonesRotation(USkinnedMeshComponent *SkinMesh);

	FQuat BonesRealTimeMotionCapturLocalRotation[23];
	USkinnedMeshComponent* CharacterSkinMesh;




private:
	IMotionCapture	 *m_motioncap;
	FChingMUSoldier SoldierState;
	FQuat DeviceQuat;
	FVector DevicePos;
	FQuat quat;
	FVector pos;
	bool preBtnState[6];
	bool curBtnState[6];
	bool IsButtonDown[6];
	bool IsButtonStay[6];
	bool IsButtonUp[6];

	// for config context

	char m_ServerAddr[215];
	int  m_HeadID;
	int	 m_WeaponID;
	int  m_BodyID;
	int  m_LeftHandID;
	int  m_RightHandID;
	int  m_LeftAnkleID;
	int  m_RightAnkleID;
	int  m_HumanID;


	bool IsCMTrackerDetected = false;

#pragma endregion



#pragma region  Mocap Part ////////////////////////////////////////////
private:
	IHumanCap *humanCap;
	ISequenceRecorder *SequenceRecorder;
public:

	UFUNCTION(BlueprintCallable, Category = "ChingMU")
		FString GetStageNotesFQN();
	UFUNCTION(BlueprintCallable, Category = "ChingMU")
		FTimecode GetTimeCode();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChingMU")
		FHumanBonesName humanBoneNames;
	UFUNCTION(BlueprintPure, Category = "ChingMU")
		void GetHumanState(FString ServerAddr, int32 ID, FWorldSpaceHumanState &state);
	//UFUNCTION(BlueprintPure, Category = "ChingMU")
	//	void GetHumanState_2(FString ServerAddr, int32 ID, FWorldSpaceHumanState &state, UPoseableMeshComponent *SkinMesh, UChingMUComponent* chingMuRef);

	UFUNCTION(BlueprintCallable, Category = "ChingMU")
		void SetHumanState(UPoseableMeshComponent *human, FHumanBonesName names, FWorldSpaceHumanState state);

	// set retargeted human state
	UFUNCTION(BlueprintCallable, Category = "ChingMU")
		void SetRetargetHumanState(UPoseableMeshComponent *poseableMesh, FString actorLabel);

#if WITH_EDITOR
	// for blueprint record skeleton
	UFUNCTION(BlueprintCallable, Category = "ChingMU")
		void StartRecord(USkeletalMeshComponent *human, FString SequenceName);
	UFUNCTION(BlueprintCallable, Category = "ChingMU")
		void StopRecord();
#endif // !1

#pragma endregion 



#pragma region Handcap part /////////////////////////////////////////////
private:
	IHandCap *handCap;

public:
	UFUNCTION(BlueprintCallable, Category = "ChingMU")
		void GetHandsState(FWorldSpaceHandState &leftHand, FWorldSpaceHandState &rightHand);
	UFUNCTION(BlueprintCallable, Category = "ChingMU")
		void SetHandsState(FFingersName lefthandFingersName, FFingersName righthandFingersName,
			FWorldSpaceHandState leftHand, FWorldSpaceHandState rightHand,
			UPoseableMeshComponent *humanPoseableMesh);
#pragma endregion 


#pragma region Facecap part /////////////////////////////////////////////
private:
	IFaceCap *faceCap;
public:
#pragma endregion
};
