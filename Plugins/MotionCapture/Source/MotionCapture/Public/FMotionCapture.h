#pragma once
#include "MotionCapturePrivatePCH.h"
#include "Features/IModularFeature.h"
#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"
#include "Interfaces/IPluginManager.h"



DEFINE_LOG_CATEGORY(LogMotionCapture);

typedef union VrpnTimeCode
{
	struct tTC
	{
		unsigned int subframes : 5;	// 0-31
		unsigned int frames : 7;	// 0-127
		unsigned int seconds : 6;	// 0-59
		unsigned int minutes : 6;	// 0-59
		unsigned int hours : 5;		// 0-23
		unsigned int standard : 2;	// SMPTETCBASERATE
		unsigned int valid : 1;		// time code valid
	}ttc;
	unsigned int	utc;
}VrpnTimeCode;

typedef struct timeval {
	long    tv_sec;         /* seconds */
	long    tv_usec;        /* and microseconds */
}timeval;

typedef struct VrpnResetHier {
	timeval			msg_time;	// Time of the report
}VrpnResetHier;

typedef struct VrpnHier {
	timeval			msg_time;	// Time of the report
	int				sensor;		// Which sensor is reporting
	int				parent;		// parent sensor id
	char			name[127];	// current sensor name
}VrpnHier;

struct CallbackReturnHierarchyResult
{
	//timeval			msg_time;	// Time of the report
public:
	int				sensor;		// Which sensor is reporting
	int				parent;		// parent sensor id
	FText			name;	// current sensor name

};
bool operator ==(const CallbackReturnHierarchyResult &x, const CallbackReturnHierarchyResult &y)
{
	if (x.sensor == y.sensor)
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool operator ==(CallbackReturnHierarchyResult x, const int y)
{
	if (x.sensor == y)
	{
		return true;
	}
	else
	{
		return false;
	}
}
TArray<CallbackReturnHierarchyResult> BonesInforFromClient_t;
TMap<int, int>RetargetCharactersID;
long LastMaxTv_sec;
long CurrentMaxTv_sec;
long LastT;
bool IsResetHierarchyCore;

class FSocket;
struct FMotionCaptureSubjectItem;
typedef int32 FMotionCaptureSubjectKey;
typedef TSharedPtr<FMotionCaptureSubjectItem> FMotionCaptureSubjectItemPtr;
// Structure that defines a bone entry in the subject UI
struct FMotionCaptureSubjectItem
{
	// Id of the subject
	FMotionCaptureSubjectKey SubjectKey;

	// If SubjectKey.SubjectName is None then we are a source, this should be set instead
	FText SubjectName;
	FText AttachSubjectName;

	TWeakPtr<FMotionCaptureSubjectItem> Parent;

	// Children (if this entry represents a source instead of a specific subject
	TArray<FMotionCaptureSubjectItemPtr> Children;
};

typedef void(*pCMUnityStartExtern)();
typedef void(*pCMUnityQuitExtern)();
typedef void(*pCMUnityEnableLog)(bool enable);
typedef bool(*pCMPluginConnect)(ANSICHAR* address);
typedef VrpnTimeCode(*pCMPluginTimeCode)(char* _address);
typedef double(*pCMAnalogExtern)(ANSICHAR* address, int channel, int frameCount);
typedef bool(*pCMButtonExtern)(ANSICHAR* address, int channel, int frameCount);
typedef double(*pCMTrackerExtern)(ANSICHAR* address, int channel, int component, int frameCount, bool lockUpRotation);
typedef double(*pCMHeadExtern)(ANSICHAR* address, int channel, int component, int frameCount, double* R_oculus, double* T_oculus, unsigned int platform);
typedef bool(*pCMHumanExtern)(ANSICHAR* address, int32 channel, int frameCount, double* attitude, int* segmentIsDetected);
typedef bool(*pCMTrackerExternIsDetected)(ANSICHAR* address, int32 channel, int frameCount);
typedef bool(*pCMTrackerExternTC)(ANSICHAR* address, int channel, VrpnTimeCode *timecode, double T[3], double R_Quat[4]);
typedef bool(*pCMHumanExternTC)(ANSICHAR* address, int channel, int frameCount, VrpnTimeCode *timecode, double* attitude, int* segmentIsDetected);
typedef bool(*pCMRetargetHumanExternTC)(char* _address, int channel, int frameCount, VrpnTimeCode *timecode, double *position, double *quaternion, int* segmentIsDetected);

typedef void __stdcall ResetHierarchyCallback(void*, VrpnResetHier b);
typedef void __stdcall UpdateHierarchyCallback(void*, VrpnHier b);
typedef bool(*pCMPluginRegisterResetHierarchy)(ANSICHAR* address, void* userdata, ResetHierarchyCallback func);
typedef bool(*pCMPluginRegisterUpdateHierarchy)(ANSICHAR* address, void* userdata, UpdateHierarchyCallback func);
typedef bool(*pCMPluginUnRegisterResetHierarchy)(ANSICHAR* address, void* userdata, ResetHierarchyCallback func);
typedef bool(*pCMPluginUnRegisterUpdateHierarchy)(ANSICHAR* address, void* userdata, UpdateHierarchyCallback func);

// platform: engine platform type
enum ENGINE_PLATFORM
{
	ENGINE_DEFAULT,		// x->right, y->front, z->up 
	ENGINE_UNITY,		// x->right, y->up, z->front 
	ENGINE_UNREAL,		// x->left, y->front, z->up	 
	ENGINE_COUNT,
};

enum ConnectState 
{
    Connecting,
	Connected,
	NoConnect,
};


FName IMotionCapture::ModularFeatureName(TEXT("ModularFeature_MotionCapture"));
class MOTIONCAPTURE_API FMotionCapture : public IMotionCapture, public IModularFeature
{
public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	bool LoadDependency(const FString& Dir, const FString& Name, void*& Handle);
	void FreeDependency(void*& Handle);
	void Shutdown();

	bool Tick(float DeltaSeconds);

	void CMPluginStart();
	void CMPluginQuit();
	bool CMPluginConnect(ANSICHAR* address);
	VrpnTimeCode CMPluginTimeCode(ANSICHAR* address);

	
	
private:
    //bool IsRegistered;
	float CachedDeltaTime_x;
	float timePoint_x;
	
	void ConnectAndRegister(float DeltaSeconds);
	void DisConnectAndUnRegister();
public:
	//bool IsConnect;
	//bool IsDisConnect;
	FString ConnectCommand;
	ConnectState CnState;
	FString CMServerAddress;
	bool IsCheckedForceFaceX;
	bool IsResetPanelList;
	bool AnimNodeForPoseUpdate;
	static void __stdcall UpdateCharacterHierarchy(void *ref, VrpnHier hierarchy);
	static void __stdcall ResetCharacterHierarchy(void*, VrpnResetHier b);
    TArray<CallbackReturnHierarchyResult> BonesInforsFromClient;
   

public:
	bool CMPluginRegisterResetHier(ANSICHAR*, void*, ResetHierarchyCallback);
	bool CMPluginRegisterUpdateHier(ANSICHAR*, void*, UpdateHierarchyCallback);
	bool CMPluginUnRegisterResetHier(ANSICHAR*, void*, ResetHierarchyCallback);
	bool CMPluginUnRegisterUpdateHier(ANSICHAR*, void*, UpdateHierarchyCallback);

	double CMAnalog(ANSICHAR* address, int32 channel);
	bool CMButton(ANSICHAR* address, int32 channel);
	virtual FVector CMTrackerPos(ANSICHAR* address, int32 channel) override;
	virtual FQuat CMTrackerQuat(ANSICHAR* address, int32 channel, bool lockUpRotation = false) override;
	virtual bool CMTrackerIsDetected(ANSICHAR* address, int32 channel) override;
	virtual FTransform CMTrackerTC(ANSICHAR* address, int32 channel, FTimecode &timecode) override;

	virtual FVector CMHeadPos(ANSICHAR* address, int32 channel, double* R_oculus, double* T_oculus) override;
	virtual FQuat CMHeadQuat(ANSICHAR* address, int32 channel, double* R_oculus, double* T_oculus) override;
	virtual FVector CMTouchPos(ANSICHAR* address, int32 channel, double* R_oculus, double* T_oculus) override;
	virtual FQuat CMTouchQuat(ANSICHAR* address, int32 channel, double* R_oculus, double* T_oculus) override;

	virtual bool CMHuman(ANSICHAR* address, int32 channel, FVector *pos, FQuat *boneRot) override;
	virtual bool CMHumanTC(ANSICHAR* address, int32 channel, FTimecode &timecode, FVector *pos, FQuat *boneRot) override;
	virtual bool CMRetargetHumanTC(ANSICHAR* address, int32 channel, FTimecode &timecode, FVector *pos, FQuat *boneRot) override;


private:
	FSocket	*m_stageNotesSocket;
public:
	bool ConnectStageNotes(FString address, bool connect);
	bool ReceiveStageNotesTakeName(FString &takeName);


public:
	/** Delegate for callbacks to Tick */
	FTickerDelegate TickDelegate;

	/** Handle to various registered delegates */
	FDelegateHandle TickDelegateHandle;

	void* CMPluginHandle;
	pCMUnityStartExtern CMPluginStartExtern;
	pCMUnityQuitExtern CMPluginQuitExtern;
	pCMUnityEnableLog CMPluginLogExtern;
	pCMPluginConnect CMPluginConnectExtern;
	pCMPluginTimeCode CMPluginTimeCodeExtern;
	pCMAnalogExtern CMAnalogExtern;
	pCMButtonExtern CMButtonExtern;
	pCMTrackerExtern CMTrackerExtern;
	pCMHeadExtern CMHeadExtern;

	pCMPluginRegisterResetHierarchy CMRegisterResetHierExtern;
	pCMPluginRegisterUpdateHierarchy CMRegisterUpdateHierExtern;
	pCMPluginUnRegisterResetHierarchy CMUnRegisterResetHierExtern;
	pCMPluginUnRegisterUpdateHierarchy CMUnRegisterUpdateHierExtern;


	pCMHumanExtern CMHumanExtern;
	pCMRetargetHumanExternTC CMRetargetHumanExternTC;
	pCMTrackerExternIsDetected CMTrackerExternIsDetected;

	// for realtime timecode
	pCMTrackerExternTC	CMTrackerTCExtern;
	pCMHumanExternTC	CMHumanTCExtern;

	// for MotionCaptureEditor
	FText CMTrackerAddress;
	//Subject tree items
	TArray<FMotionCaptureSubjectItemPtr> SubjectData;
	//Bind subjects
	TMap<FMotionCaptureSubjectKey, FString> BindActors;
	TMap<FMotionCaptureSubjectKey, FString> BindSkeletons;

	
	FTimecode Timecode;
	FString   StageNotesFQN;

};