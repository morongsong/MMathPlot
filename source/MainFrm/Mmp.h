

#pragma once

#include "resource.h"

class CMmpApp : public CWinAppEx
{
public:
	CMmpApp();
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;
public:
	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();
public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()

};

extern CMmpApp theApp;
