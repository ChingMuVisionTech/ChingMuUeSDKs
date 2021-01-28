#pragma once


class IFaceCap
{

protected:

public:
	IFaceCap() {}

	virtual void GetFaceState()=0;

	virtual ~IFaceCap() {}
};

