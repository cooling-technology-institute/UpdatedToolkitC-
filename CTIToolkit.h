// CTI BlueBook.h : main header file for the CTI DEMO application
//

#if !defined(AFX_CTIDEMO_H__4060AB1E_1038_11D2_80CD_00A0C9D7D02F__INCLUDED_)
#define AFX_CTIDEMO_H__4060AB1E_1038_11D2_80CD_00A0C9D7D02F__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "TPropPageBase.h"

#ifdef _PERF_CURVE_ONLY
#define SZ_APP_NAME			_T("CTI PerfCurve")
#define SZ_APP_NAME_SHORT	_T("PerfCurve")
#define SZ_APP_COPYRIGHT	_T("CTI PerfCurve© ")
#define SZ_HELP_FILE_NAME	_T("CTIPerfCurve.chm")
#elif _DEMO_VERSION
#define SZ_APP_NAME			_T("CTI Toolkit Demo")
#define SZ_APP_NAME_SHORT	_T("Toolkit Demo")
#define SZ_APP_COPYRIGHT	_T("CTI Toolkit© Demo")
#define SZ_HELP_FILE_NAME	_T("CTIToolkit.chm")
#else
#define SZ_APP_NAME			_T("CTI Toolkit")
#define SZ_APP_NAME_SHORT	_T("Toolkit")
#define SZ_APP_COPYRIGHT	_T("CTI Toolkit©")
#define SZ_HELP_FILE_NAME	_T("CTIToolkit.chm")
#endif

/////////////////////////////////////////////////////////////////////////////
// CCTIApp:
// See CTI Demo.cpp for the implementation of this class
//

class CCTIApp : public CWinApp
{
public:
    TPropPageBase* tps;
    HICON m_hIcon;
    TCHAR serialnumber[15];

	CCTIApp();
    TCHAR* getSerialNumber();
    bool   checkSerialNumber();

	CString GetVersion() const;
	CString GetShortVersion() const;	
	CString GetAppName() const;
	CString GetAppCopyright() const;

	bool IsTeeChartRegistered() const;
	bool RegisterTeeChart() const;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCTIApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CCTIApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

extern CCTIApp& getApp();
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CTIDEMO_H__4060AB1E_1038_11D2_80CD_00A0C9D7D02F__INCLUDED_)
