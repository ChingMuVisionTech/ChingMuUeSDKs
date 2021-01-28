#pragma once
#include "../IFaceCap.h"

class IQFace:public IFaceCap
{
public:
	IQFace();
	virtual void GetFaceState()override;


};

