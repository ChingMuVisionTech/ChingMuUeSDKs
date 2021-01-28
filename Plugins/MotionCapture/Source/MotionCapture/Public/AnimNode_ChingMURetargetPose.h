// Copyright 2015-2019 ChingMU, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNodeBase.h"

class IMotionCapture;

#include "AnimNode_ChingMURetargetPose.generated.h"

USTRUCT(BlueprintInternalUseOnly)
struct MOTIONCAPTURE_API FAnimNode_ChingMURetargetPose : public FAnimNode_Base
{
	GENERATED_USTRUCT_BODY()

		/** Check this option if world rotation is used in ChingMU Retarget animation data,If local rotation is used, do not check this option*/
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Alpha, meta = (PinShownByDefault, DisplayName = "UsingWorldRotation"))
		bool bUsingWorldRotation = false;
#if WITH_EDITOR
		FAnimNode_ChingMURetargetPose();

	// FAnimNode_Base interface
	virtual void Initialize_AnyThread(const FAnimationInitializeContext& Context) override;

	virtual void CacheBones_AnyThread(const FAnimationCacheBonesContext & Context) override {}

	virtual void Update_AnyThread(const FAnimationUpdateContext & Context) override;

	virtual void Evaluate_AnyThread(FPoseContext& Output) override;
	// End of FAnimNode_Base interface


private:
	IMotionCapture* MotionCapture;

	FString SubjectName;

	// Delta time from update so that it can be passed to retargeter
	float CachedDeltaTime;
#endif 
};
