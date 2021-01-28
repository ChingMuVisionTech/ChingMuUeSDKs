// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "MotionCapturePrivatePCH.h"
#include "FMotionCapture.h"
#include "Sockets.h"
#include "Networking.h"
#include "Features/IModularFeatures.h"
#include "Engine.h"

//#include "AnimNode_ChingMURetargetPoseForBuild.h "

bool FMotionCapture::LoadDependency(const FString& Dir, const FString& Name, void*& Handle)
{
	FString Lib = Name + TEXT(".") + FPlatformProcess::GetModuleExtension();
	FString Path = Dir.IsEmpty() ? *Lib : FPaths::Combine(*Dir, *Lib);

	Handle = FPlatformProcess::GetDllHandle(*Path);

	if (Handle == nullptr)
	{
		GLog->Logf(ELogVerbosity::Warning, TEXT("MotionCapture: Failed to load required library %s. Plug-in will not be functional."), *Path);
		return false;
	}

	return true;
}

void FMotionCapture::FreeDependency(void*& Handle)
{
	if (Handle != nullptr)
	{
		if(CMPluginQuitExtern)
			CMPluginQuitExtern();

		FPlatformProcess::FreeDllHandle(Handle);
		Handle = nullptr;
	}
}

void FMotionCapture::Shutdown()
{
	FTicker::GetCoreTicker().RemoveTicker(TickDelegateHandle);
	FreeDependency(CMPluginHandle);
}

/// <summary>
/// 追踪日志  
/// Track log
/// </summary>
/// <param name="address">ServerIP，for example:"MCServer@192.168.3.178" or "MCServer@SH1DT010"</param>
/// <param name="channel">ID of Body</param>
/// <returns></returns>
double FMotionCapture::CMAnalog(ANSICHAR* address, int32 channel)
{
	return CMAnalogExtern(address, channel, GFrameCounter);
}

//*
bool FMotionCapture::CMButton(ANSICHAR* address, int32 channel)
{
	return CMButtonExtern(address, channel, GFrameCounter);
}

// 普通刚体追踪.............................................................................................................................................................
/// <summary>
/// 刚体追踪数据的空间位置Position
/// Get the Position of tracker Body
/// </summary>
/// <param name="address">ServerIP，for example:"MCServer@192.168.3.178" or "MCServer@SH1DT010"</param>
/// <param name="channel">ID of Body</param>
/// <return
FVector FMotionCapture::CMTrackerPos(ANSICHAR* address, int32 channel)
{
	FVector TrackerPos;

	TrackerPos.X = (float)CMTrackerExtern(address, channel, 1, GFrameCounter, false) / 10.0;
	TrackerPos.Y = (float)CMTrackerExtern(address, channel, 0, GFrameCounter, false) / 10.0;
	TrackerPos.Z = (float)CMTrackerExtern(address, channel, 2, GFrameCounter, false) / 10.0;

	return TrackerPos;
}

/// <summary>
/// 刚体追踪数据的旋转值Quaternion
/// Get the Rotation of tracker Body
/// </summary>
/// <param name="address">ServerIP，for example:"MCServer@192.168.3.178" or "MCServer@SH1DT010"</param>
/// <param name="channel">ID of Body</param>
/// <param name="lockYRotation">if value is true,the Y axis of rotation  will be lock </param>
/// <returns></returns>
FQuat FMotionCapture::CMTrackerQuat(ANSICHAR* address, int32 channel, bool lockUpRotation)
{
	return FQuat(
		(float)CMTrackerExtern(address, channel, 4, GFrameCounter, lockUpRotation),
		(float)CMTrackerExtern(address, channel, 3, GFrameCounter, lockUpRotation),
		(float)CMTrackerExtern(address, channel, 5, GFrameCounter, lockUpRotation),
		-(float)CMTrackerExtern(address, channel, 6, GFrameCounter, lockUpRotation));
}

bool FMotionCapture::CMTrackerIsDetected(ANSICHAR* address, int32 channel)
{
	return CMTrackerExternIsDetected(address, channel, GFrameCounter);
}

FTransform FMotionCapture::CMTrackerTC(ANSICHAR* address, int32 channel, FTimecode &timecode)
{
	VrpnTimeCode vrpntimecode;
	double T[3], R_Quat[4];
	bool detected = CMTrackerTCExtern(address, channel, &vrpntimecode, T, R_Quat);

	if (vrpntimecode.ttc.valid)
	{
		timecode.Hours		= vrpntimecode.ttc.hours;
		timecode.Minutes	= vrpntimecode.ttc.minutes;
		timecode.Seconds	= vrpntimecode.ttc.seconds;
		timecode.Frames		= vrpntimecode.ttc.frames;
		timecode.bDropFrameFormat = false;
	}
	else
	{
		timecode.Hours		= 0;
		timecode.Minutes	= 0;
		timecode.Seconds	= 0;
		timecode.Frames		= 0;
		timecode.bDropFrameFormat = false;
	}

	FTransform transform;
	if (detected)
	{

		transform.SetRotation(FQuat(R_Quat[1], R_Quat[0], R_Quat[2], -R_Quat[3]));
		transform.SetLocation(FVector(T[1] / 10.0, T[0] / 10.0, T[2] / 10.0));
		
	}

	return transform;
}

// 头显追踪....................................................................................................................................................................
/// <summary>
/// 获取刚体追踪数据和头显陀螺仪数据融合后的空间位置Position
/// Get the position  data that Merge tracker Body data with IMU of HMD 
/// </summary>
/// <param name="address">ServerIP，for example:"MCServer@192.168.3.178" or "MCServer@SH1DT010"</param>
/// <param name="channel">ID of Body</param>
/// <param name="R_oculus、T_oculus">the RT  of IMU</param>
/// <returns></returns>
FVector FMotionCapture::CMHeadPos(ANSICHAR* address, int32 channel, double* R_oculus, double* T_oculus)
{
	FVector TrackerPos;
	TrackerPos.X = (float)CMHeadExtern(address, channel, 1, GFrameCounter, R_oculus, T_oculus, ENGINE_UNREAL) / 10.0;
	TrackerPos.Y = (float)CMHeadExtern(address, channel, 0, GFrameCounter, R_oculus, T_oculus, ENGINE_UNREAL) / 10.0;
	TrackerPos.Z = (float)CMHeadExtern(address, channel, 2, GFrameCounter, R_oculus, T_oculus, ENGINE_UNREAL) / 10.0;

	return TrackerPos;
}

/// <summary>
/// 获取刚体追踪数据和头显陀螺仪数据融合后的四元素旋转值
/// Get the rotation  data that Merge tracker Body data with IMU of HMD 
/// </summary>
/// <param name="address">ServerIP，for example:"MCServer@192.168.3.178" or "MCServer@SH1DT010"</param>
/// <param name="channel">ID of Body</param>
/// <param name="R_oculus、T_oculus">the RT  of IMU</param>
/// <returns>Quaternion of that merge tracker quaternion and quaternion of IMU</returns>
FQuat FMotionCapture::CMHeadQuat(ANSICHAR* address, int32 channel, double* R_oculus, double* T_oculus)
{
	return FQuat(
		(float)CMHeadExtern(address, channel, 4, GFrameCounter, R_oculus, T_oculus, ENGINE_UNREAL),
		(float)CMHeadExtern(address, channel, 3, GFrameCounter, R_oculus, T_oculus, ENGINE_UNREAL),
		(float)CMHeadExtern(address, channel, 5, GFrameCounter, R_oculus, T_oculus, ENGINE_UNREAL),
		-(float)CMHeadExtern(address, channel, 6, GFrameCounter, R_oculus, T_oculus, ENGINE_UNREAL));
}

FVector FMotionCapture::CMTouchPos(ANSICHAR* address, int32 channel, double* R_oculus, double* T_oculus)
{
	return CMHeadPos(address, channel, R_oculus, T_oculus);
}

FQuat FMotionCapture::CMTouchQuat(ANSICHAR* address, int32 channel, double* R_oculus, double* T_oculus)
{
	return CMHeadQuat(address, channel, R_oculus, T_oculus);
}

// 全身动捕......................................................................................................................................................................................
/// <summary>
/// 获取全身动捕数据接口，包括23个关节相对旋转值，和根骨骼Hip的世界坐标
/// Get Human Fullbody Tracker data ,including of 23joints localRotation and root joint world Position;
/// </summary>
/// <param name="address">ServerIP，for example:"MCServer@192.168.3.178" or "MCServer@SH1DT010"</param>
/// <param name="channel">ID of human</param>
/// <param name="pos">root joint world Position</param>
/// <param name="boneRot">21joints localRotation</param>
/// <returns></returns>
bool FMotionCapture::CMHuman(ANSICHAR* address, int32 channel, FVector *pos, FQuat *boneRot)
{
	double boneAttitude[3+4*150] = { 0.0f };
	int isBoneDetected[150] = { 0 };
	if (!pos || !boneRot)
		return false;

	int segmentNum = strstr(address, ":3884") ? 150 : 23;
	bool isHumanDetected = CMHumanExtern(address, channel, GFrameCounter, boneAttitude, isBoneDetected);
	if (isHumanDetected)
	{
		// set pos
		/*pos->X = -boneAttitude[0] / 10.0; 
		pos->Y = boneAttitude[1] / 10.0;
		pos->Z =  boneAttitude[2] / 10.0;*/
		
		pos->X = boneAttitude[1] / 10.0;
		pos->Y = boneAttitude[0] / 10.0;
		pos->Z = boneAttitude[2] / 10.0;

		// set rotation
		for (int i = 0; i < segmentNum; ++i)
		{
			if (isBoneDetected[i] == 1)
			{
				//(UE Skeleton)
				//boneRot[i] = FQuat(-(float)boneAttitude[i * 4 + 3],(float)boneAttitude[i * 4 + 4] ,(float)boneAttitude[i * 4 + 5], -(float)boneAttitude[i * 4 + 6]);

				//Maya Skeleton
			//	boneRot[i] = FQuat(-(float)boneAttitude[i * 4 + 4], -(float)boneAttitude[i * 4 + 3], -(float)boneAttitude[i * 4 + 5], (float)boneAttitude[i * 4 + 6]);
				boneRot[i] = FQuat((float)boneAttitude[i * 4 + 4], (float)boneAttitude[i * 4 + 3], (float)boneAttitude[i * 4 + 5], -(float)boneAttitude[i * 4 + 6]);

			}
		}
	}

	return isHumanDetected;
}

bool FMotionCapture::CMHumanTC(ANSICHAR* address, int32 channel, FTimecode &timecode, FVector *pos, FQuat *boneRot)
{
	VrpnTimeCode vrpntimecode;
	double boneAttitude[3+4*150] = { 0.0f };
	int isBoneDetected[150] = { 0 };
	if (!pos || !boneRot)
		return false;

	int segmentNum = strstr(address, ":3884") ? 150 : 23;
	bool isHumanDetected = CMHumanTCExtern(address, channel, GFrameCounter, &vrpntimecode, boneAttitude, isBoneDetected);
	if (isHumanDetected)
	{
		// set pos
		pos->X = boneAttitude[1] / 10.0;
		pos->Y = boneAttitude[0] / 10.0;
		pos->Z = boneAttitude[2] / 10.0;


		// set rotation
		for (int i = 0; i < segmentNum; ++i)
		{
			if (isBoneDetected[i] == 1)
			{
				//(UE Skeleton)
				//boneRot[i] = FQuat(-(float)boneAttitude[i * 4 + 3],(float)boneAttitude[i * 4 + 4] ,(float)boneAttitude[i * 4 + 5], -(float)boneAttitude[i * 4 + 6]);

				//Maya Skeleton
				boneRot[i] = FQuat((float)boneAttitude[i * 4 + 4], (float)boneAttitude[i * 4 + 3], (float)boneAttitude[i * 4 + 5], -(float)boneAttitude[i * 4 + 6]);
			}
		}
	}
	
	// set timecode
	if (isHumanDetected && vrpntimecode.ttc.valid)
	{
		timecode.Hours = vrpntimecode.ttc.hours;
		timecode.Minutes = vrpntimecode.ttc.minutes;
		timecode.Seconds = vrpntimecode.ttc.seconds;
		timecode.Frames = vrpntimecode.ttc.frames;
		timecode.bDropFrameFormat = false;
	}
	else
	{
		timecode.Hours = 0;
		timecode.Minutes = 0;
		timecode.Seconds = 0;
		timecode.Frames = 0;
		timecode.bDropFrameFormat = false;
	}

	return isHumanDetected;
}

bool FMotionCapture::CMRetargetHumanTC(ANSICHAR* address, int32 channel, FTimecode &timecode, FVector *pos, FQuat *boneRot)
{
	VrpnTimeCode vrpntimecode;
	double bonePosition[3 * 150] = { 0.0f };
	double boneAttitude[4 * 150] = { 0.0f };
	int isBoneDetected[150] = { 0 };



	FVector Mb_pos[150];
	FQuat Mb_boneRot[150];

	if (!pos || !boneRot)
		return false;

	int segmentNum = strstr(address, ":3884") ? 150 : 23;
	bool isHumanDetected = CMRetargetHumanExternTC(address, channel, GFrameCounter, &vrpntimecode, bonePosition, boneAttitude, isBoneDetected);
	if (isHumanDetected)
	{
		// set rotation
		for (int i = 0; i < segmentNum; ++i)
		{
			if (isBoneDetected[i] == 1)
			{
				if (IsCheckedForceFaceX)
				{
					//Client to  UE4 世界坐标系转换
					// set pos
					pos[i].X = bonePosition[3 * i + 1] / 10.0;
					pos[i].Y = bonePosition[3 * i + 0] / 10.0;
					pos[i].Z = bonePosition[3 * i + 2] / 10.0;

					// set rotation
					boneRot[i] = FQuat((float)boneAttitude[i * 4 + 1], (float)boneAttitude[i * 4 + 0], (float)boneAttitude[i * 4 + 2], -(float)boneAttitude[i * 4 + 3]);
				}
				else 
				{
#if 1
					//Client to  MB 世界坐标系转换
					// set pos
					Mb_pos[i].X = -bonePosition[3 * i + 0] / 10.0; //tracker client的单位是mm ,mb ,maya的单位是cm
					Mb_pos[i].Y = bonePosition[3 * i + 2] / 10.0;
					Mb_pos[i].Z = bonePosition[3 * i + 1] / 10.0;
					// set rotation
					Mb_boneRot[i] = FQuat(-(float)boneAttitude[i * 4 + 0], (float)boneAttitude[i * 4 + 2], (float)boneAttitude[i * 4 + 1], (float)boneAttitude[i * 4 + 3]);


					//MB to UE4 世界坐标系转换（这个地方事实上没有按照MB与UE4坐标轴方向的差异来转换,这个地方实际上是测试出来的，目前只测试了MB或MAYA到UE4的转换，这种转换是对的，MB与MAYA的世界坐标系是一样的）
					// set pos
					pos[i].X = Mb_pos[i].X;//  ue4单位是cm
					pos[i].Y = -Mb_pos[i].Y;// 
					pos[i].Z = Mb_pos[i].Z;// 
					// set rotation
					boneRot[i] = FQuat((float)Mb_boneRot[i].X, -(float)Mb_boneRot[i].Y, (float)Mb_boneRot[i].Z, -(float)Mb_boneRot[i].W);
#else
					// set pos //发给虎牙直播的，对应胡勇修改过专门发给虎牙的CMTrack版本，一个应急版本，必须搭配修改动画蓝图节点，在驱动刚开始的时候旋转一下角色，把角色从地面搬起来
					pos[i].X = bonePosition[3 * i + 0];// / 10.0;
					pos[i].Y = -bonePosition[3 * i + 1];// / 10.0;
					pos[i].Z = bonePosition[3 * i + 2];// / 10.0;

					boneRot[i] = FQuat((float)boneAttitude[i * 4 + 0], -(float)boneAttitude[i * 4 + 1], (float)boneAttitude[i * 4 + 2], -(float)boneAttitude[i * 4 + 3]);
#endif
				}

			}
		}
	}

	// set timecode
	if (isHumanDetected && vrpntimecode.ttc.valid)
	{
		timecode.Hours = vrpntimecode.ttc.hours;
		timecode.Minutes = vrpntimecode.ttc.minutes;
		timecode.Seconds = vrpntimecode.ttc.seconds;
		timecode.Frames = vrpntimecode.ttc.frames;
		timecode.bDropFrameFormat = false;
	}
	else
	{
		timecode.Hours = 0;
		timecode.Minutes = 0;
		timecode.Seconds = 0;
		timecode.Frames = 0;
		timecode.bDropFrameFormat = false;
	}

	return isHumanDetected;
}
/*
bool FMotionCapture::CMRetargetHumanTC(ANSICHAR* address, int32 channel, FTimecode &timecode, FVector *pos, FQuat *boneRot)
{
	VrpnTimeCode vrpntimecode;
	double bonePosition[3 * 150] = { 0.0f };
	double boneAttitude[4 * 150] = { 0.0f };
	int isBoneDetected[150] = { 0 };
	if (!pos || !boneRot)
		return false;

	int segmentNum = strstr(address, ":3884") ? 150 : 23;
	bool isHumanDetected = CMRetargetHumanExternTC(address, channel, GFrameCounter, &vrpntimecode, bonePosition, boneAttitude, isBoneDetected);
	
	if (isHumanDetected)
	{

		// set rotation
		for (int i = 0; i < segmentNum; ++i)
		{
			//if (isBoneDetected[i] == 1)
			{
				// set pos
				pos[i].X = bonePosition[3*i + 1] / 10.0;
				pos[i].Y = bonePosition[3*i + 0] / 10.0;
				pos[i].Z = bonePosition[3*i + 2] / 10.0;

				//Maya Skeleton
				boneRot[i] = FQuat((float)boneAttitude[i * 4 + 1], (float)boneAttitude[i * 4 + 0], (float)boneAttitude[i * 4 + 2], -(float)boneAttitude[i * 4 + 3]);
			}
		}
	}

	// set timecode
	if (isHumanDetected && vrpntimecode.ttc.valid)
	{
		timecode.Hours = vrpntimecode.ttc.hours;
		timecode.Minutes = vrpntimecode.ttc.minutes;
		timecode.Seconds = vrpntimecode.ttc.seconds;
		timecode.Frames = vrpntimecode.ttc.frames;
		timecode.bDropFrameFormat = false;
	}
	else
	{
		timecode.Hours = 0;
		timecode.Minutes = 0;
		timecode.Seconds = 0;
		timecode.Frames = 0;
		timecode.bDropFrameFormat = false;
	}

	return isHumanDetected;
}
*/

void FMotionCapture::ConnectAndRegister(float DeltaSeconds)
{
	FString s2 = "Start Connect And Register  .....................    ";
	UE_LOG(LogTemp, Log, TEXT(" %s"), *s2);
	CachedDeltaTime_x += DeltaSeconds;
	if (CachedDeltaTime_x > (timePoint_x + 0.2f)&& (CnState == ConnectState::Connecting))
	{
		bool c = CMPluginConnect(TCHAR_TO_ANSI(*CMServerAddress));
		timePoint_x = CachedDeltaTime_x;
		if (c) 
		{
			CnState = ConnectState::Connected;
			BonesInforFromClient_t.Reset();
			RetargetCharactersID.Reset();

			bool r = CMPluginRegisterResetHier(TCHAR_TO_ANSI(*CMServerAddress), &FMotionCapture::ResetCharacterHierarchy, &FMotionCapture::ResetCharacterHierarchy);
			r &= CMPluginRegisterUpdateHier(TCHAR_TO_ANSI(*CMServerAddress), &FMotionCapture::UpdateCharacterHierarchy, &FMotionCapture::UpdateCharacterHierarchy);

			if (r)
			{
				//RetargetCharactersSkletName.Add(Context.AnimInstanceProxy->GetSkeleton()->GetFName());
				CachedDeltaTime_x = 0;
				timePoint_x = 0;
				ConnectCommand.Empty();
				GLog->Log(ELogVerbosity::Display, "MotionCapture core Tick register callback succeed.");
			}
			else
			{
				GLog->Log(ELogVerbosity::Display, "MotionCapture core Tick register callback failed.");
			}
		}
		else
		{
			CnState = ConnectState::Connecting;
		}
	}

}

void FMotionCapture::DisConnectAndUnRegister()
{
	FString s2 = "Start DisConnect And UnRegister from MotionCapture core .....................    ";
	UE_LOG(LogTemp, Log, TEXT(" %s"), *s2);
	CMPluginUnRegisterResetHier(TCHAR_TO_ANSI(*CMServerAddress), &FMotionCapture::ResetCharacterHierarchy, &FMotionCapture::ResetCharacterHierarchy);
	CMPluginUnRegisterUpdateHier(TCHAR_TO_ANSI(*CMServerAddress), &FMotionCapture::UpdateCharacterHierarchy, &FMotionCapture::UpdateCharacterHierarchy);
	BonesInforFromClient_t.Reset();
	RetargetCharactersID.Reset();
	AnimNodeForPoseUpdate = false;
	BonesInforsFromClient.Reset();
	CMPluginQuit();
	ConnectCommand.Empty();
	//CnState = ConnectState::NoConnect;
}
bool FMotionCapture::Tick(float DeltaSeconds)
{
	/*
	if(CnState == ConnectState::NoConnect)
	{
		if (IsConnect)
		{
			CnState == ConnectState::Connecting;
			CMPluginStart();
		}
	}

	if ( CnState == ConnectState::Connecting)
	{
		ConnectAndRegister(DeltaSeconds);
	}

	if (CnState == ConnectState::Connected)
	{
		if (IsDisConnect)
		{
			CnState == ConnectState::NoConnect;
			DisnnectAndUnRegister();
		}
	}
	*/
	switch (CnState)
	{
	case Connecting:
		ConnectAndRegister(DeltaSeconds);
		break;
	case Connected:
		if (ConnectCommand =="DisConnect")
		{
			CnState = ConnectState::NoConnect;
			DisConnectAndUnRegister();
		}
		break;
	case NoConnect:
		if (ConnectCommand=="ConnectServer")
		{
			CnState = ConnectState::Connecting;
			CMPluginStart();
		}
		break;
	}





	/*
	if (CMServerAddress.IsEmpty())
	{
		return true;
	}
	if (!IsRegistered)
	{
		CachedDeltaTime_x += DeltaSeconds;
	}
	if (CachedDeltaTime_x > (timePoint_x + 0.2f) && (!IsConnect))
	{

		IsConnect = CMPluginConnect(TCHAR_TO_ANSI(*CMServerAddress));
		timePoint_x = CachedDeltaTime_x;
	}

	if (IsConnect && (!IsRegistered))
	{
		if (CachedDeltaTime_x > (timePoint_x + 0.2f))
		{
			timePoint_x = CachedDeltaTime_x;
			BonesInforFromClient_t.Reset();
			RetargetCharactersID.Reset();

		    IsRegistered = CMPluginRegisterResetHier(TCHAR_TO_ANSI(*CMServerAddress), &FMotionCapture::ResetCharacterHierarchy, &FMotionCapture::ResetCharacterHierarchy);
			IsRegistered &= CMPluginRegisterUpdateHier(TCHAR_TO_ANSI(*CMServerAddress), &FMotionCapture::UpdateCharacterHierarchy, &FMotionCapture::UpdateCharacterHierarchy);
			
			if (IsRegistered)
			{
				//RetargetCharactersSkletName.Add(Context.AnimInstanceProxy->GetSkeleton()->GetFName());
				CachedDeltaTime_x = 0;
				timePoint_x = 0;

				GLog->Log(ELogVerbosity::Display, "MotionCapture core Tick register callback succeed.");
			}
			else
			{
				GLog->Log(ELogVerbosity::Display, "MotionCapture core Tick register callback failed.");
			}
		}
		
	}
	*/


	if (IsResetHierarchyCore)
	{
		int min = FDateTime::Now().GetMinute();
		int second = FDateTime::Now().GetSecond();
		long ct = min * 60 + second;
		int dt =  ct-LastT;
		FString f = " IsResetHierarchyCore  true ";
		UE_LOG(LogTemp, Warning, TEXT("时间:%s   %d   %d   %d "), *f, ct, LastT, dt);
		if (dt>=2) 
		{
			BonesInforsFromClient.Reset();
			for (int i = 0; i < BonesInforFromClient_t.Num(); i++)
			{
				BonesInforsFromClient.Add(BonesInforFromClient_t[i]);
			}
			IsResetHierarchyCore = false;
			AnimNodeForPoseUpdate = true;
			IsResetPanelList = true;
		}
	}

	if (IsRunningDedicatedServer() == true)
	{
		return true;
	}
	return true;
}

void FMotionCapture::CMPluginStart()
{
	if (CMPluginStartExtern)
		CMPluginStartExtern();
}

void FMotionCapture::CMPluginQuit()
{
	if (CMPluginQuitExtern)
		CMPluginQuitExtern();
}

bool FMotionCapture::CMPluginConnect(ANSICHAR* address)
{
	if (CMPluginConnectExtern)
		return CMPluginConnectExtern(address);
	else
		return false;
}

//bool FMotionCapture::ConnectServer(FString serverAddr)
//{
//    IsConnect = CMPluginConnect(TCHAR_TO_ANSI(*serverAddr));
//	return IsConnect;
//}


VrpnTimeCode FMotionCapture::CMPluginTimeCode(ANSICHAR* address)
{
	VrpnTimeCode tc;
	if (CMPluginTimeCodeExtern)
	{
		tc = CMPluginTimeCodeExtern(address);
		if (tc.ttc.valid)
		{
			Timecode.Hours = tc.ttc.hours;
			Timecode.Minutes = tc.ttc.minutes;
			Timecode.Seconds = tc.ttc.seconds;
			Timecode.Frames = tc.ttc.frames;
			Timecode.bDropFrameFormat = false;
		}
		else
		{
			Timecode = FTimecode();
		}
	}
	
	return tc;
}

bool FMotionCapture::CMPluginRegisterResetHier(ANSICHAR* address, void* userdata, ResetHierarchyCallback func)
{
	if (CMRegisterResetHierExtern)
		return CMRegisterResetHierExtern(address, userdata, func);
	else
		return false;


}
bool FMotionCapture::CMPluginRegisterUpdateHier(ANSICHAR* address, void* userdata, UpdateHierarchyCallback func)
{
	if (CMRegisterUpdateHierExtern)
		return CMRegisterUpdateHierExtern(address, userdata, func);
	else
		return false;

}
//bool FMotionCapture::RegisterUpdateAndResetHierCallBack(FString SeverAddr)
//{
//    IsRegistered = CMPluginRegisterResetHier(TCHAR_TO_ANSI(*SeverAddr), &FMotionCapture::ResetCharacterHierarchy, &FMotionCapture::ResetCharacterHierarchy);
//	IsRegistered &=CMPluginRegisterUpdateHier(TCHAR_TO_ANSI(*SeverAddr), &FMotionCapture::UpdateCharacterHierarchy, &FMotionCapture::UpdateCharacterHierarchy);
//	return IsRegistered;
//}

void __stdcall  FMotionCapture::ResetCharacterHierarchy(void*, VrpnResetHier b)
{
	long seconds = b.msg_time.tv_sec;
	long micseconds = b.msg_time.tv_usec;
    CurrentMaxTv_sec = seconds;

	FString s2 = "ResetHierarchy    ";
	UE_LOG(LogLoad, Log, TEXT(" %s    %i    %i"), *s2, seconds, micseconds);
	IsResetHierarchyCore = true;
	int min =FDateTime::Now().GetMinute();
	LastT= min*60 + FDateTime::Now().GetSecond();

	UE_LOG(LogTemp, Warning, TEXT("时间:%s,   %d"), *s2, LastT);
	//if (LastT==59) 
	//{
	//	LastT
	//	LastT = 0;

	//}

}



void __stdcall  FMotionCapture::UpdateCharacterHierarchy(void *ref, VrpnHier hierarchy)
{
	int min = FDateTime::Now().GetMinute();
	int t = FDateTime::Now().GetSecond();
	long x =min * 60 + t;
	LastT = x;
   
	/*int year = Time.GetYear();
	int month = Time.GetMonth();
	int day = Time.GetDay();
	int hour = Time.GetHour();
	int minute = Time.GetMinute();
	int second = Time.GetSecond();
	UE_LOG(LogTemp, Warning, TEXT("时间:%d, %d, %d, %d, %d, %d  %f  %f"), year, month, day, hour, minute, second, MCt, MCTickDeltaTime);*/
	if (CurrentMaxTv_sec > LastMaxTv_sec)
	{
		BonesInforFromClient_t.Reset();
		RetargetCharactersID.Reset();

		LastMaxTv_sec = CurrentMaxTv_sec;

		long seconds = hierarchy.msg_time.tv_sec;
		long micseconds = hierarchy.msg_time.tv_usec;
		FString s2 = "UpdateCharacterHierarchy    ";
		UE_LOG(LogLoad, Log, TEXT(" %s    %i    %i"), *s2, seconds, micseconds);
	}

	CallbackReturnHierarchyResult result;
	
	result.name = FText::FromString(hierarchy.name);
	result.sensor = hierarchy.sensor;
	result.parent = hierarchy.parent;
	FString s1 = hierarchy.name;

	BonesInforFromClient_t.AddUnique(result);

	int boneIndex = (hierarchy.sensor - 100) % 150;
	if (boneIndex == 0)
	{
		int RetargetCharacterID = (hierarchy.sensor - 100) / 150;
		//int RetargetCharacterID = RetargetCharactersID.Num();
		RetargetCharactersID.Add(RetargetCharacterID, BonesInforFromClient_t.Num() - 1);
		UE_LOG(LogLoad, Log, TEXT(" %s   %i   %i    %i   %i "), *s1, result.sensor, result.parent, RetargetCharacterID, BonesInforFromClient_t.Num() - 1);
	}
}


bool FMotionCapture::CMPluginUnRegisterResetHier(ANSICHAR* address, void* userdata, ResetHierarchyCallback func)
{
	if (CMUnRegisterResetHierExtern)
		return CMUnRegisterResetHierExtern(address, userdata, func);
	else
		return false;
}

bool FMotionCapture::CMPluginUnRegisterUpdateHier(ANSICHAR* address, void* userdata, UpdateHierarchyCallback func)
{
	if (CMUnRegisterUpdateHierExtern)
		return CMUnRegisterUpdateHierExtern(address, userdata, func);
	else
		return false;
}

void FMotionCapture::StartupModule()
{
	// This code will execute after your module is loaded into memory (but after global variables are initialized, of course.)

	//const ANSICHAR BaseDir = FPaths::GamePluginsDir();
	const FString BaseDir = FPaths::ProjectPluginsDir();
	const FString MCDir = FPaths::Combine(*BaseDir, TEXT("MotionCapture"), TEXT("ThirdParty"));
#if PLATFORM_WINDOWS
#if PLATFORM_64BITS
	const FString LibDir = FPaths::Combine(*MCDir, TEXT("Win64"));
#else
	const ANSICHAR LibDir = FPaths::Combine(*MCDir, TEXT("Win32"));
#endif
#endif	//PLATFORM_WINDOWS

	if (!LoadDependency(LibDir, TEXT("CMVrpn"), CMPluginHandle))
	{
		Shutdown();
		return;
	}

	CMPluginStartExtern = (pCMUnityStartExtern)FPlatformProcess::GetDllExport(CMPluginHandle, TEXT("CMUnityStartExtern"));
	CMPluginQuitExtern = (pCMUnityQuitExtern)FPlatformProcess::GetDllExport(CMPluginHandle, TEXT("CMUnityQuitExtern"));
	CMPluginLogExtern = (pCMUnityEnableLog)FPlatformProcess::GetDllExport(CMPluginHandle, TEXT("CMUnityEnableLog"));
	CMPluginConnectExtern = (pCMPluginConnect)FPlatformProcess::GetDllExport(CMPluginHandle, TEXT("CMPluginConnectServer"));
	CMPluginTimeCodeExtern = (pCMPluginTimeCode)FPlatformProcess::GetDllExport(CMPluginHandle, TEXT("CMPluginTimeCode"));
	CMRegisterResetHierExtern = (pCMPluginRegisterResetHierarchy)FPlatformProcess::GetDllExport(CMPluginHandle, TEXT("CMPluginRegisterResetHierarchy"));
	CMRegisterUpdateHierExtern = (pCMPluginRegisterUpdateHierarchy)FPlatformProcess::GetDllExport(CMPluginHandle, TEXT("CMPluginRegisterUpdateHierarchy"));
	CMUnRegisterResetHierExtern = (pCMPluginUnRegisterResetHierarchy)FPlatformProcess::GetDllExport(CMPluginHandle, TEXT("CMPluginUnRegisterResetHierarchy"));
	CMUnRegisterUpdateHierExtern = (pCMPluginUnRegisterUpdateHierarchy)FPlatformProcess::GetDllExport(CMPluginHandle, TEXT("CMPluginUnRegisterUpdateHierarchy"));
	CMAnalogExtern = (pCMAnalogExtern)FPlatformProcess::GetDllExport(CMPluginHandle, TEXT("CMAnalogExtern"));
	CMButtonExtern = (pCMButtonExtern)FPlatformProcess::GetDllExport(CMPluginHandle, TEXT("CMButtonExtern"));
	CMTrackerExtern = (pCMTrackerExtern)FPlatformProcess::GetDllExport(CMPluginHandle, TEXT("CMTrackerExtern"));
	CMHeadExtern = (pCMHeadExtern)FPlatformProcess::GetDllExport(CMPluginHandle, TEXT("CMHeadExtern"));
	CMHumanExtern = (pCMHumanExtern)FPlatformProcess::GetDllExport(CMPluginHandle, TEXT("CMHumanExtern"));
	CMTrackerExternIsDetected = (pCMTrackerExternIsDetected)FPlatformProcess::GetDllExport(CMPluginHandle, TEXT("CMTrackerExternIsDetected"));
	CMTrackerTCExtern = (pCMTrackerExternTC)FPlatformProcess::GetDllExport(CMPluginHandle, TEXT("CMTrackerExternTC"));
	CMHumanTCExtern = (pCMHumanExternTC)FPlatformProcess::GetDllExport(CMPluginHandle, TEXT("CMHumanExternTC"));
	CMRetargetHumanExternTC = (pCMRetargetHumanExternTC)FPlatformProcess::GetDllExport(CMPluginHandle, TEXT("CMRetargetHumanExternTC"));

	if (CMPluginLogExtern)
	{
		CMPluginLogExtern(false);
	}

	TickDelegate = FTickerDelegate::CreateRaw(this, &FMotionCapture::Tick);
	TickDelegateHandle = FTicker::GetCoreTicker().AddTicker( TickDelegate);
	//IMotionCapture::ModularFeatureName = TEXT("ModularFeature_MotionCapture

	IModularFeatures::Get().RegisterModularFeature(IMotionCapture::ModularFeatureName, this);

	//CMPluginStart();
	//IsConnect = false;
	//IsRegistered = false;

	IsCheckedForceFaceX = true;
	IsResetHierarchyCore = false;
	IsResetPanelList = false;
	AnimNodeForPoseUpdate = false;
	CnState = ConnectState::NoConnect;
}

void FMotionCapture::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
//	Shutdown();

	//CMPluginUnRegisterResetHier(TCHAR_TO_ANSI(*CMServerAddress), &FMotionCapture::ResetCharacterHierarchy, &FMotionCapture::ResetCharacterHierarchy);
	//CMPluginUnRegisterUpdateHier(TCHAR_TO_ANSI(*CMServerAddress), &FMotionCapture::UpdateCharacterHierarchy, &FMotionCapture::UpdateCharacterHierarchy);
	//CMPluginQuit();
	IModularFeatures::Get().UnregisterModularFeature(IMotionCapture::ModularFeatureName, this);

}

bool FMotionCapture::ConnectStageNotes(FString address, bool connect)
{
	if (connect)
	{
		FString IP, Port;
		FIPv4Address Addr;
		if (!(address.Split(":", &IP, &Port) && FIPv4Address::Parse(IP, Addr) && Port.IsNumeric()))
		{
			char message[1024];
			sprintf_s(message, "Invalid StageNotes Server Addr:%s.", TCHAR_TO_ANSI(*address));
			GLog->Log(ELogVerbosity::Error, message);
			return false;
		}

		FIPv4Endpoint IPv4Endpoint(Addr, atoi(TCHAR_TO_ANSI(*Port)));
		m_stageNotesSocket = FTcpSocketBuilder(TEXT("StageNotesSocket"));
		if (!m_stageNotesSocket->Connect(*IPv4Endpoint.ToInternetAddr())
			|| m_stageNotesSocket->GetConnectionState() != SCS_Connected)
		{
			GLog->Log(ELogVerbosity::Error, "Connect to StageNotes Server failed.");
			return false;
		}
		else
		{
			GLog->Log(ELogVerbosity::Display, "Connect to StageNotes Server Succeed.");
			return true;
		}
	}
	else
	{
		if (m_stageNotesSocket)
		{
			m_stageNotesSocket->Close();
			delete m_stageNotesSocket;
			m_stageNotesSocket = NULL;
		}

		return true;
	}
}

bool FMotionCapture::ReceiveStageNotesTakeName(FString &takeName)
{
	if (m_stageNotesSocket == NULL || m_stageNotesSocket->GetConnectionState() != SCS_Connected)
	{
		GLog->Log(ELogVerbosity::Error, "StageNotes do not connect yet!");
		return false;
	}

	int32 byteRead = 0;
	char recBuffer[1024], tmp[1024], *recvStr;
	if (!m_stageNotesSocket->Recv((uint8*)recBuffer, sizeof(recBuffer), byteRead))
	{
		GLog->Log(ELogVerbosity::Error, "StageNotes Recv failed!");
		return false;
	}

	recvStr = recBuffer + 4;
	if (strstr(recvStr, "tc_out") && strstr(recvStr, "fqn"))
	{
		strcpy(tmp, strstr(recvStr, "fqn") + strlen("fqn\\\": \\\""));
		if (strstr(tmp, "\\\","))
		{
			*strstr(tmp, "\\\",") = 0;
			takeName = ANSI_TO_TCHAR(tmp);
			StageNotesFQN = takeName;

			return true;
		}
	}

	return false;
}



IMPLEMENT_MODULE(FMotionCapture, MotionCapture)