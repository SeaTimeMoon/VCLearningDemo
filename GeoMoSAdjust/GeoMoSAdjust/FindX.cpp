#include "StdAfx.h"
#include "FindX.h"

CFindX::CFindX(void)
{
}

CFindX::~CFindX(void)
{
}

bool CFindX::operator()( GeoOrientation& ori )
{
	if(ori.strStationPoint==nodeType)
		return true;
	else
		return false;
}


