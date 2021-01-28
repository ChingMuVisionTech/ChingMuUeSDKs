// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"
#include "Containers/Array.h"

/**
 * The public interface to this module.  In most cases, this interface is only public to sibling modules 
 * within this plugin.
 */
class IMotionCapture : public IModuleInterface
{
public:

	/**
	 * Singleton-like access to this module's interface.  This is just for convenience!
	 * Beware of calling this during the shutdown phase, though.  Your module might have been unloaded already.
	 *
	 * @return Returns singleton instance, loading the module on demand if needed
	 */
	static inline IMotionCapture& Get()
	{
		return FModuleManager::LoadModuleChecked< IMotionCapture >( "MotionCapture" );
	}

	/**
	 * Checks to see if this module is loaded and ready.  It is only valid to call Get() if IsAvailable() returns true.
	 *
	 * @return True if the module is loaded and ready to use
	 */
	static inline bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded( "MotionCapture" );
	}

	static FName ModularFeatureName;


	virtual double CMAnalog(ANSICHAR* address, int32 channel) = 0;
	virtual	bool CMButton(ANSICHAR* address, int32 channel) = 0;


	/**
	 * for CMTracker common body tracker info, which do not own IMU Gyro data.
	 * address: CMTracker vrpn interface name and CMTracker computer name
	 * channel: CMTracker tracking body ID
	 * lockUpRotation: For body which should rotate by z-axis
	 */
	virtual FVector CMTrackerPos(ANSICHAR* address, int32 channel) = 0;
	virtual FQuat CMTrackerQuat(ANSICHAR* address, int32 channel, bool lockUpRotation = false) = 0;	
	virtual bool CMTrackerIsDetected(ANSICHAR* address, int32 channel) = 0;
	virtual FTransform CMTrackerTC(ANSICHAR* address, int32 channel, FTimecode &timecode) = 0;

	/**
	 * for CMTracker head tracker(oculus or htc vive) info, which own IMU Gyro data.
	 * address: CMTracker vrpn interface name and CMTracker computer name
	 * channel: CMTracker tracking body ID
	 * R_oculus: Oculus rotation Quaternion
	 * T_oculus: Oculus current transform, can be set FVector(0.0f, 0.0f, 0.0f)
	 */
	virtual FVector CMHeadPos(ANSICHAR* address, int32 channel, double* R_oculus, double* T_oculus) = 0;
	virtual FQuat CMHeadQuat(ANSICHAR* address, int32 channel, double* R_oculus, double* T_oculus) = 0;


	virtual FVector CMTouchPos(ANSICHAR* address, int32 channel, double* R_oculus, double* T_oculus) = 0;
	virtual FQuat CMTouchQuat(ANSICHAR* address, int32 channel, double* R_oculus, double* T_oculus) = 0;

	virtual bool CMHuman(ANSICHAR* address, int32 channel, FVector *pos, FQuat *boneRot) = 0;
	virtual bool CMHumanTC(ANSICHAR* address, int32 channel, FTimecode &timecode, FVector *pos, FQuat *boneRot) = 0;
	virtual bool CMRetargetHumanTC(ANSICHAR* address, int32 channel, FTimecode &timecode, FVector *pos, FQuat *boneRot) = 0;
};

