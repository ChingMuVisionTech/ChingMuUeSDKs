#pragma once
#include"../Interface/IHumanCap.h"



//class IHumanCap;
class ChingmuHumanCap:public IHumanCap
{
public:
	ChingmuHumanCap();
	~ChingmuHumanCap();

	 virtual FWorldSpaceHumanState* GetHumanState(ANSICHAR  *ServerAddr, int32 *humanID) override;


private:
	IMotionCapture	 *m_motioncap;
	ANSICHAR  *mServerAddr;

	FQuat localSpaceBonesQuat[40];
	FVector worldSpaceBonesPos[40];
	FRotator offSetRot;
};

