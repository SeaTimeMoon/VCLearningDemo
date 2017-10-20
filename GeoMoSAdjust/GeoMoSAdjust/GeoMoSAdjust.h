// GeoMoSAdjust.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CGeoMoSAdjustApp:
// See GeoMoSAdjust.cpp for the implementation of this class
//

class CGeoMoSAdjustApp : public CWinApp
{
public:
	CGeoMoSAdjustApp();

// Overrides
	public:
	virtual BOOL InitInstance();
	BOOL CheckDongleValidity();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CGeoMoSAdjustApp theApp;