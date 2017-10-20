#pragma once
#include "GeoAdjustStruct.h"

class CFindX
{
public:
	CFindX(void);
	~CFindX(void);

	CFindX(const CString str){nodeType=str;}

	CString GetTest() {return nodeType;}

	bool operator()(GeoOrientation& ori); 

private: 

	CString nodeType; 
};
