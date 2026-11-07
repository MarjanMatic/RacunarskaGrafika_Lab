
// RG_LV1.h : main header file for the RG_LV1 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CRGLV1App:
// See RG_LV1.cpp for the implementation of this class
//

class CRGLV1App : public CWinApp
{
public:
	CRGLV1App() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CRGLV1App theApp;
