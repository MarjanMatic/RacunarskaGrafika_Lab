
// RG_LV2.h : main header file for the RG_LV2 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CRGLV2App:
// See RG_LV2.cpp for the implementation of this class
//

class CRGLV2App : public CWinAppEx
{
public:
	CRGLV2App() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CRGLV2App theApp;
