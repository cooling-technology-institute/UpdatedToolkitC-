// CTI Demo.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "CTIToolkit.h" 
#include "winver.h"
#include "MainFrm.h"
#include "Splash.h"
#include "StaticHyperLink.h"  
#include "PrintView.h"
#include "PrintFrame.h"
#include "PrintList.h"
#include "PrintPerfReport.h"
#include "CTIhelp.hh"
#ifdef _MFC_PREVIEW
#include "ToolkitDoc.h"
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CPrintFrame wndPrintFrame;
CPrintView	wndPrintView;

CPrintFrame wndPrintListFrame;
CPrintList  wndPrintList;

CPrintFrame			wndPrintPerfDataFrame;
CPrintPerfReport	wndPrintPerfDataView;

#ifdef _MFC_PREVIEW

class CMyMainFrame : public CFrameWnd
{
	DECLARE_DYNCREATE(CMyMainFrame)
public:

};

IMPLEMENT_DYNCREATE(CMyMainFrame, CFrameWnd)

#endif
/////////////////////////////////////////////////////////////////////////////
// CCTIApp

BEGIN_MESSAGE_MAP(CCTIApp, CWinApp)
	//{{AFX_MSG_MAP(CCTIApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCTIApp construction

CCTIApp::CCTIApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CCTIApp object

CCTIApp  theApp;

CCTIApp& getApp()
{
    return theApp;
}

TCHAR* CCTIApp::getSerialNumber()
{
    return serialnumber;
}

bool CCTIApp::checkSerialNumber()
{
    // check to see if software is registered
    DWORD   dwSize = 50;
    DWORD   dwType = REG_SZ;
    HKEY 	m_hKey;
    TCHAR   string[50];
    bool    error_flag = false;
	LONG result = 0;

    // open the key
	CString szKey("SOFTWARE\\Cooling Technology Institute\\");
	szKey += GetAppName() + _T("\\");
	szKey += GetShortVersion();
	//szKey += "\\Serial";

	if ((result = RegOpenKeyEx(HKEY_LOCAL_MACHINE,
                         szKey, 
                         0L, 
                         KEY_QUERY_VALUE | KEY_WOW64_64KEY,
                         &m_hKey))
         == ERROR_SUCCESS )
    {
         // read the value
         if (ERROR_SUCCESS == RegQueryValueEx(m_hKey, _T("Serial"), NULL, &dwType, (BYTE*)string, &dwSize))
         {
              // check the serial number 
              for (int x = 0; x < ((int)dwSize - 1); x++)
              {
                  switch (x)
                  {
                  // alphabetic
                  case 0 :
                  case 1 :
                  case 8 :
                  case 9 :
                  case 10:
                      if ( !isalpha( string[x] ) ) error_flag = true;
                      break;
                  case 2 :
                  case 4 :
                  case 5 :
                  case 6 :
                      if (!isdigit( string[x] ) ) error_flag = true;
                      break;
                  case 3 :
                  case 7 :
                      if ( string[x] != '-' ) error_flag = true;
                      break;
                  default:
                      error_flag = true;
                  }
              }
              if (!error_flag)
                  _tcscpy(serialnumber,string);
              RegCloseKey (m_hKey);
         }
         else
         {
             error_flag = true;
         }
    }
    else
    {
        error_flag = true;
    }

    return !error_flag;
}

/////////////////////////////////////////////////////////////////////////////
// CCTIApp initialization

BOOL CCTIApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

#ifndef DEBUG
#ifndef _DEMO_VERSION
    if (!checkSerialNumber())
    {
		CString szMsg("Please use the install program to install ");
		szMsg += SZ_APP_NAME;
        AfxMessageBox(szMsg,MB_ICONSTOP);
        exit(1);
    }
#endif
#endif

    SetRegistryKey(_T("Cooling Tower Institute"));
    
	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Change help file name if this is the Toolkit demo version
	char drive_orig[_MAX_DRIVE];
	char dir_orig[_MAX_DIR];
	char fname_orig[_MAX_FNAME], fname_new[_MAX_FNAME];
	char ext_orig[_MAX_EXT], ext_new[_MAX_EXT];

	_tsplitpath(m_pszHelpFilePath, drive_orig, dir_orig, 
		fname_orig, ext_orig);
	LPTSTR pszTemp = (LPTSTR) malloc(_tcslen(m_pszHelpFilePath)+
		_tcslen(SZ_HELP_FILE_NAME));
	_tsplitpath(SZ_HELP_FILE_NAME, NULL, NULL, fname_new, ext_new);

	if ((_tcsicmp(fname_new, fname_orig) != 0)  || 
		(_tcsicmp(ext_orig, ext_new) != 0))
	{
		_makepath(pszTemp, drive_orig, dir_orig, fname_new, ext_new);
		free((void*)m_pszHelpFilePath);
		m_pszHelpFilePath = pszTemp;
	}
	else
		free((void*)pszTemp);

	// Check to make sure TeeChart is registered
	if (IsTeeChartRegistered() == false) {
		RegisterTeeChart(); }

#ifdef _MFC_PREVIEW
	CSingleDocTemplate * pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CToolkitDoc),
		RUNTIME_CLASS(CMyMainFrame),
		RUNTIME_CLASS(CPrintPerfReport));
	AddDocTemplate(pDocTemplate);
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);
#endif

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
    CSplashWnd* splash = new CSplashWnd;
    splash->EnableSplashScreen(cmdInfo.m_bShowSplash);
    splash->ShowSplashScreen();
    delete splash;

	// Print Window

#ifndef _MFC_PREVIEW
	CRect		rectClient;
	wndPrintFrame.Create(NULL, _T("CTI Print"));
	wndPrintListFrame.Create(NULL, _T("CTI List"));
	wndPrintPerfDataFrame.Create(NULL, _T("CTI Performance Data"));

	wndPrintFrame.GetClientRect(rectClient);
	CRect		rectWnd(0,0,rectClient.bottom*2,rectClient.right*2);
	CRect		rectLst(0,0,rectClient.bottom*2,rectClient.right*2);
	CRect		rectReport(0,0,rectClient.bottom*2,rectClient.right*2);
	wndPrintView.Create(NULL, _T("CTI Print"), WS_CHILD, 
		rectWnd, &wndPrintFrame, 999);
	wndPrintList.Create(NULL, _T("CTI List"), WS_CHILD, 
		rectLst, &wndPrintListFrame, 999);
	wndPrintPerfDataView.Create(NULL, _T("CTI Performance Data"), WS_CHILD, 
		rectReport, &wndPrintPerfDataFrame, 999);

	// The one and only window has been initialized, so show and update it.
    tps = new TPropPageBase( "Cooling Technology Institute", NULL, IDD_PROPPAGE_ONE );
	m_pMainWnd = tps;
    tps->DoModal(); 
    delete tps;
	return FALSE;
#else
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	m_pMainWnd->ShowWindow(SW_HIDE);
	m_pMainWnd->UpdateWindow();
    tps = new TPropPageBase( "Cooling Technology Institute", NULL, IDD_PROPPAGE_ONE );
    tps->Create(m_pMainWnd);
//	m_pMainWnd->ShowWindow(SW_HIDE);
	return TRUE;
#endif
}


CString CCTIApp::GetVersion() const
{
    CString strVersion = "0.90";
    CString strFileName;
    DWORD   dwFileVerLen;
    DWORD   dwTemp;
    LPBYTE  lpBuffer = 0;
    LPVOID  lpData;
    UINT    uDataLen;

	GetModuleFileName(NULL, strFileName.GetBuffer(MAX_PATH+1), MAX_PATH);
    strFileName.ReleaseBuffer();

    // Get the size of the Version info
    dwFileVerLen = GetFileVersionInfoSize((LPSTR)((LPCTSTR)strFileName), &dwTemp);

    // If we got something then continue
    if (dwFileVerLen)
    {
        // Allocate a buffer big enough to hold the Version info and check to 
        // see if we successful
        lpBuffer = new BYTE[dwFileVerLen + (2 * 1024)];
        if (lpBuffer)
        {
            // Get the Version info and continue if successful
            if (GetFileVersionInfo(strFileName.GetBuffer(24), 0, dwFileVerLen, lpBuffer))
            {
                // get the VS_FIXEDFILEINFO and continue if successful
                if (VerQueryValue(lpBuffer, "\\", &lpData, &uDataLen) && uDataLen)
                {
                    // now build the file version of the DLL
                    VS_FIXEDFILEINFO * structFileInfo = (VS_FIXEDFILEINFO *)lpData;

                    WORD wVerMajorHi = HIWORD(structFileInfo->dwFileVersionMS);
                    WORD wVerMajorLow = LOWORD(structFileInfo->dwFileVersionMS);
                    WORD wVerMinorHi = HIWORD(structFileInfo->dwFileVersionLS);
#ifdef _DEMO_VERSION
                    strVersion.Format("%u.%u.%02u.99",wVerMajorHi,wVerMajorLow,wVerMinorHi);
#else
                    WORD wVerMinorLow = LOWORD(structFileInfo->dwFileVersionLS);
                    strVersion.Format("%u.%u.%02u.%02u",wVerMajorHi,wVerMajorLow,wVerMinorHi,wVerMinorLow);
#endif
                }
            }

            // do cleanup
            strFileName.ReleaseBuffer();
            delete lpBuffer;
        }
    }

    return strVersion;
}


CString CCTIApp::GetShortVersion() const
{
	CString strVersion(GetVersion());
	int nPos = strVersion.Find(_T('.'));
	if (nPos == -1)
		return strVersion;

	nPos = strVersion.Find(_T('.'), nPos+1);
	if (nPos != -1)
		strVersion.Delete(nPos, strVersion.GetLength()-nPos);
	strVersion.TrimRight(_T('0'));

	// Check for too much trimming
	if (strVersion[strVersion.GetLength()-1] == _T('.'))
		strVersion += '0'; 
	
	return strVersion;
}


CString CCTIApp::GetAppName() const
{
	CString strAppName(SZ_APP_NAME);
	strAppName.Replace(_T("CTIToolkit"), _T("CTI Toolkit"));
	return strAppName;
}


CString CCTIApp::GetAppCopyright() const
{	
	CString strAppName(SZ_APP_COPYRIGHT);
	strAppName.Replace(_T("CTIToolkit"), _T("CTI Toolkit"));
	return strAppName;
}


/*  The RegisterTeeChart() function is provided to re-register
	the TeeChart object.  If the user has multiple installs
	of Toolkit/PerfCurve/Demo and uninstalls one of them,
	the uninstall can unregister TeeChart, which 
	will then stop other installations from working.

    If a TeeChart load fails, or a dialog with a TeeChart
	object fails to open, it may be worth calling this 
	function to see if re-registering will fix the problem.
*/
bool CCTIApp::IsTeeChartRegistered() const
{
	HKEY hKeyClsid, hKeyControl;
	LONG regResult = ::RegOpenKeyEx(HKEY_CLASSES_ROOT, _T("CLSID"), 0,
		KEY_READ, &hKeyClsid);
	if (regResult != ERROR_SUCCESS) 
		return false;
	
	// We only use one version of teechart.ocx, with a known GUID.
	// Change this to enumerate CLSID keys if using more than one
	// version, or a different version, of teechart.
	regResult = ::RegOpenKeyEx(hKeyClsid, 
		_T("{008BBE7E-C096-11D0-B4E3-00A0C901D681}"), 
		0, KEY_READ, &hKeyControl);

	::RegCloseKey(hKeyClsid);
	if (regResult == ERROR_SUCCESS) 
		::RegCloseKey(hKeyControl);

	return (regResult == ERROR_SUCCESS);
}


typedef HRESULT (STDAPICALLTYPE *CTLREGPROC)();

bool CCTIApp::RegisterTeeChart() const
{
	bool bResult(false);
	HMODULE hModule = ::LoadLibrary(_T("teechart.ocx"));

#ifdef _WIN32
	if (hModule == 0)
		return false;
#else
	if (hModule <= HINSTANCE_ERROR)
		return false;
#endif

	CTLREGPROC DLLRegisterServer =
		(CTLREGPROC)::GetProcAddress(hModule,"DllRegisterServer" ) ;
	if (DLLRegisterServer != NULL)
	{
		HRESULT regResult = DLLRegisterServer() ;
		bResult = (regResult == NOERROR) ; 
	}
	::FreeLibrary(hModule);
	return bResult;
}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CStaticHyperLink m_wndCTILink;
	CString m_strProgramName;
	CString	m_strVersion;
    CString m_strSerialNumber;
	CString m_strShortCopyright;
	CString m_strShortCopyright2;
	CString	m_strCopyright;
	//}}AFX_DATA   

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    CString SerialNumber;
	CString m_fnGetVersion();
	//{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()          
public:
	afx_msg void OnStnClickedStaticVersion();
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	m_strProgramName = SZ_APP_COPYRIGHT;
	m_strVersion = _T("Version 0.4");
//	m_strShortCopyright = CString(SZ_APP_NAME) + _T(", Copyright © 1999 - 2017, Cooling Technology Institute. All rights reserved."); 
//	m_strShortCopyright2 = _T("All rights reserved."); 
	m_strShortCopyright = _T("Copyright © 1999 - 2017, Cooling Technology Institute. All rights reserved."); 
	m_strCopyright = _T("Warning: this computer program is protected by copyright law and international treaties.  Unauthorized reproduction or distribution of this program, or any portion of it, may result in severe civil and criminal penalties and will be prosecuted to the maximum extent possible under the law.");
	//}}AFX_DATA_INIT
	m_strVersion = "Version " + m_fnGetVersion();
#ifdef _DEMO_VERSION
	m_strVersion += " Limited Demo";
#endif
    SerialNumber = getApp().getSerialNumber();
    m_strSerialNumber = "Serial Number: " + SerialNumber;
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Control(pDX, IDC_STATIC_CTI, m_wndCTILink);
	DDX_Text(pDX, IDC_ABOUT_PROGRAM_NAME, m_strProgramName);
	DDX_Text(pDX, IDC_STATIC_VERSION, m_strVersion);
	DDX_Text(pDX, IDC_ABOUT_COPYRIGHT, m_strShortCopyright);
	//DDX_Text(pDX, IDC_ABOUT_COPYRIGHT2, m_strShortCopyright2);
	DDX_Text(pDX, IDC_EDIT_COPYRIGHT, m_strCopyright);
    DDX_Text(pDX, IDC_SERIAL_NUMBER, m_strSerialNumber);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP() 

// App command to run the dialog
void CCTIApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}
/////////////////////////////////////////////////////////////////////////////
// CCTIApp commands

BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	SetWindowText(CString("About ") + SZ_APP_NAME_SHORT);

	m_wndCTILink.m_fnSetLink("http://www.cti.org");
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


HBRUSH CAboutDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if (pWnd->m_hWnd == m_wndCTILink.m_hWnd)
	{
		pDC->SetTextColor(0xFF0000);
	}
	
	return hbr;
}


CString CAboutDlg::m_fnGetVersion()
{
	CCTIApp * pApp = static_cast<CCTIApp *>(AfxGetApp());
	return pApp->GetVersion();
}

