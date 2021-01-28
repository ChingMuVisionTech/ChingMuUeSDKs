// Copyright 2015-2019 ChingMU, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNodeBase.h"

//#include "FMotionCapture.h"

#include "AnimNode_ChingMURetargetPoseForBuild.generated.h"

/*
 struct CallbackReturnResult 
 {
	//timeval			msg_time;	// Time of the report
 public:
	int				sensor;		// Which sensor is reporting
	int				parent;		// parent sensor id
	FText			name;	// current sensor name


};
*/
class FMotionCapture;
class IMotionCapture;

USTRUCT(BlueprintInternalUseOnly)
struct MOTIONCAPTURE_API FAnimNode_ChingMURetargetPoseForBuild : public FAnimNode_Base
{
	GENERATED_USTRUCT_BODY()

	
	FAnimNode_ChingMURetargetPoseForBuild();

	// FAnimNode_Base interface
	virtual void Initialize_AnyThread(const FAnimationInitializeContext& Context) override;

	virtual void CacheBones_AnyThread(const FAnimationCacheBonesContext & Context) override {}

	virtual void Update_AnyThread(const FAnimationUpdateContext & Context) override;

	virtual void Evaluate_AnyThread(FPoseContext& Output) override;
	// End of FAnimNode_Base interface
//	static void __stdcall UpdateHierarchy(void*, VrpnHier tp);

   /** Set server ip */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ChingMU_SeverAddr, meta = (PinShownByDefault))
		FString  SeverAddr;

	/** Set human id */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ChingMU_HumanId, meta = (PinShownByDefault))
		int32 HuamnId;

	/** Check this option if world rotation is used in ChingMU Retarget animation data,If local rotation is used,do not check this option*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Alpha, meta = (PinShownByDefault, DisplayName = "IsUsingWorldRotation"))
		bool bUsingWorldRotation = false;

	/** Check this option if forceX CheckBox is checked when import character to ue4,If CheckBox is not cheked,so do not check this option*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Alpha, meta = (PinShownByDefault, DisplayName = "IsCheckedForceX"))
		bool bIsCheckedForceX = true;

private:
	/*FMotionCapture* MotionCapturePlugin;*/
	IMotionCapture* MotionCapture;
	FMotionCapture* motionCapturePlugin;
	/*float CachedDeltaTime;
	float timePoint;*/
	/*bool IsConnect ;
	bool IsRegistered;*/
	//TArray<CallbackReturnResult> BoneInforsFromClient;
};
