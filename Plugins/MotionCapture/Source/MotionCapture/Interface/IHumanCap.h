#pragma once
#include "../Model/MocapData.h"
#include "UObject/Object.h"


class IHumanCap
{
public:
	IHumanCap() {}

	virtual FWorldSpaceHumanState* GetHumanState(ANSICHAR  *ServerAddr, int32 *humanID)=0;
	virtual ~IHumanCap() {}

protected:
	FWorldSpaceHumanState worldSpaceHumanState;
};

