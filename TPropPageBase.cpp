// TPropPageBase.cpp : implementation file
//

#include "stdafx.h"
#include <afxwin.h>
#include "CTIToolkit.h"
#include "aspect.h"
#include "axes.h"
#include "axis.h"
#include "axislabels.h"
#include "canvas.h"
#include "series.h"
#include "titles.h"
#include "pen.h"
#include "panel.h"
#include "axistitle.h"
#include "strings.h"
#include "printer.h"
#include "legend.h"
//#include "cdxCSizingDialog.h"
#include "TPropPageBase.h"
//#include "TPropPageOne.h"
//#include "TPropPageTwo.h"
//#include "TPropPageThree.h"
#include "TeeChartDefines.h"
#include "CError.h"
#include "CTIhelp.hh"
#include <htmlhelp.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// TPropPageBase

void AFXAPI DDV_MinMaxDouble(CDataExchange* pDX, 
                             double& value, 
                             double EminVal, 
                             double EmaxVal, 
                             double MminVal, 
                             double MmaxVal,
                             int    nDigits)
{
    if (TPropPageBase::changingUnits) return;
    double minVal=EminVal;
    double maxVal=EmaxVal;
    if (TPropPageBase::metricflag)
    {
        minVal=MminVal;
        maxVal=MmaxVal;
    }

    try
    {
        DDV_MinMaxDouble(pDX,value,minVal,maxVal);
    }
    catch(...)
    {
        CString strValue;
        CString strFormat;

        if (value < minVal)
		{
            value = minVal;
		}
        else
		{
            value = maxVal;
		}
		strFormat.Format("%if", nDigits);
		strFormat = "%." + strFormat;
		strValue.Format(strFormat, value);
		// Attempted fix for VS2005:
		//HWND hWndLastControl = GetDlgItem(m_hWnd, pDX->m_idLastControl);
		//SetWindowText(hWndLastControl, strValue);
        //SetWindowText(pDX->m_hWndLastControl, strValue);
		pDX->Fail();
    }
}

bool CheckRange(double value, double min, double max, char* msg)
{
    bool returnValue = true;
    if (value < min) 
        returnValue = false;
    if (value > max)
        returnValue = false;

    if (!returnValue)
    {
        CError ce(msg);
		std::cout << ce;
    }

    return returnValue;
}

bool TPropPageBase::CheckData()
{
	/*
    CPropertyPage* page;
    bool errorflag = false;
    CError ce;

    int pageno = getApp().tps->GetActiveIndex();
    int button = 0;
    if (pageno == 0)
    {
        page = getApp().tps->GetPage(0);
        button = page->GetCheckedRadioButton( IDC_PSYCH_RADIO_ITEM1, 
                                               IDC_PSYCH_RADIO_ITEM4 );
    }

    if ( !errorflag && (dblWBT > dblDBT) && (button != IDC_PSYCH_RADIO_ITEM4 ) ) 
    {
        ce = "Wet bulb temperature cannot exceed dry bulb temperature";
        cout << ce;
        errorflag = true;
    }

    if ( !errorflag && (pageno == 1) ) 
    {
        
        page = getApp().tps->GetPage(1);
        if ( ((TPropPageTwo*)page)->m_dblMerkelT2 > ((TPropPageTwo*)page)->m_dblMerkelT1 )
        {
            ce = "T2 temperature cannot exceed T1 temperature";
            cout << ce;
            errorflag = true;
        }
    }

    berrorFlag = errorflag;
	*/

    return false;
}

IMPLEMENT_DYNAMIC(TPropPageBase, cdxCSizingPropSheet)

bool   TPropPageBase::metricflagsetting = false;
bool   TPropPageBase::metricflag = false;
bool   TPropPageBase::berrorFlag = false;
bool   TPropPageBase::changingUnits = false;

/*
double TPropPageBase::dblPressure = 0;
double TPropPageBase::dblWBT = 0;
double TPropPageBase::dblDBT = 0;
double TPropPageBase::dblT1 = 0;
double TPropPageBase::dblT2 = 0;
double TPropPageBase::dblRange = 0;
double TPropPageBase::dblAltitude = 0;
double TPropPageBase::dblC = 0;
double TPropPageBase::dblSlope = 0;
double TPropPageBase::dblLg = 0;
double TPropPageBase::dblEnthalpy = 0;
double TPropPageBase::dblRelHumidity = 0;
*/
//double TPropPageBase::*pEdit1 = NULL;

TPropPageBase::TPropPageBase(UINT /* nIDCaption */, CWnd* pParentWnd, UINT /* iSelectPage */)
	: cdxCSizingPropSheet(IDS_PROPSHEET_CAPTION, pParentWnd)
{
    metricflag = SetMetricFlag();
    berrorFlag = false;
	psychopage.SetDefaultValues();
	merkelpage.SetDefaultValues();
	curvespage.SetDefaultValues();
	performancepage.SetDefaultValues();
#ifdef _DEMO_VERSION
	limitspage.SetDefaultValues();
	AddPage( &limitspage );
#endif

#ifndef _PERF_CURVE_ONLY // test out perf page only version
    AddPage( &psychopage );
    AddPage( &merkelpage );
    AddPage( &curvespage );
#endif
    AddPage( &performancepage );

    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
//    m_edit.Create( ES_LEFT, psychopage.m_wndPsychResults.GetRect(), 
}

TPropPageBase::TPropPageBase(LPCTSTR /* pszCaption */, CWnd* pParentWnd, UINT /* iSelectPage */)
	: cdxCSizingPropSheet(IDS_PROPSHEET_CAPTION, pParentWnd)
{
    metricflag = SetMetricFlag();
    berrorFlag = false;
	psychopage.SetDefaultValues();
	merkelpage.SetDefaultValues();
	curvespage.SetDefaultValues();
	performancepage.SetDefaultValues();

#ifdef _DEMO_VERSION
	limitspage.SetDefaultValues();
	AddPage( &limitspage );
#endif

#ifndef _PERF_CURVE_ONLY // test out perf page only version
    AddPage( &psychopage );
    AddPage( &merkelpage );
    AddPage( &curvespage );
#endif
    AddPage( &performancepage );
    
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

TPropPageBase::~TPropPageBase()
{

/*
    HKEY  m_hKey;
    DWORD dw;

    // Create/Open the key
    RegCreateKeyEx( HKEY_LOCAL_MACHINE, 
                    "SOFTWARE\\Cooling Technology Institute\\CTIToolkit\\Units",
                    0L,
                    NULL,
                    REG_OPTION_NON_VOLATILE,
                    KEY_ALL_ACCESS,
                    NULL,
                    &m_hKey,
                    &dw );

    // Set the values
    char buffer[25];
    sprintf(buffer,"%.2f",dblPressure);
    RegSetValueEx( m_hKey, "Pressure", 0L, REG_SZ, (unsigned char*)buffer, _tcslen(buffer)+1 );
    sprintf(buffer,"%.2f",dblWBT);
    RegSetValueEx( m_hKey, "WBT", 0L, REG_SZ, (unsigned char*)buffer, _tcslen(buffer)+1 );
    sprintf(buffer,"%.2f",dblDBT);
    RegSetValueEx( m_hKey, "DBT", 0L, REG_SZ, (unsigned char*)buffer, _tcslen(buffer)+1 );
    sprintf(buffer,"%.2f",dblAltitude);
    RegSetValueEx( m_hKey, "Altitude", 0L, REG_SZ, (unsigned char*)buffer, _tcslen(buffer)+1 );
    sprintf(buffer,"%.2f",dblRelHumidity);
    RegSetValueEx( m_hKey, "Humidity", 0L, REG_SZ, (unsigned char*)buffer, _tcslen(buffer)+1 );
    sprintf(buffer,"%.4f",dblEnthalpy);
    RegSetValueEx( m_hKey, "Enthalpy", 0L, REG_SZ, (unsigned char*)buffer, _tcslen(buffer)+1 );

*/
    
}

BEGIN_MESSAGE_MAP(TPropPageBase, cdxCSizingPropSheet)
	//{{AFX_MSG_MAP(TPropPageBase)
	ON_COMMAND(ID_APP_EXIT, OnAppExit)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_ACTIVATE()
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
    ON_COMMAND(ID_DEMAND_CURVES, OnMultiEdit)
	ON_COMMAND(ID_HELPHELP, OnHelphelp)
	ON_MESSAGE(WM_HELP, OnHelp)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT, OnUpdateFilePrint)
	//}}AFX_MSG_MAP
    ON_COMMAND(IDOK,OnOK)
    ON_COMMAND(IDCANCEL,OnCancel)
//    ON_COMMAND(ID_APPLY_NOW, OnApplyNow)
END_MESSAGE_MAP()

void TPropPageBase::DoDataExchange(CDataExchange* pDX) 
{
	cdxCSizingPropSheet::DoDataExchange(pDX);
    return;
}

BOOL TPropPageBase::OnInitDialog() 
{ 
	HICON hIcon = (HICON)LoadImage(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDR_MAINFRAME), IMAGE_ICON, 16, 16, 0);
	SetIcon(hIcon, false);
	SetIcon(m_hIcon, TRUE);

    // insert menu bar
    m_menuBar.LoadMenu( IDR_MAINFRAME );
    SetMenu(&m_menuBar);

	m_menuBar.ModifyMenu(ID_APP_ABOUT, MF_BYCOMMAND | MF_STRING, ID_APP_ABOUT,
		CString("&About ") + SZ_APP_NAME);

    m_menuBar.EnableMenuItem( ID_FILE_NEW, MF_GRAYED );
    m_menuBar.EnableMenuItem( ID_FILE_OPEN, MF_GRAYED );
    m_menuBar.EnableMenuItem( ID_FILE_CLOSE, MF_GRAYED );
    m_menuBar.EnableMenuItem( ID_FILE_SAVE, MF_GRAYED );
    m_menuBar.EnableMenuItem( ID_FILE_SAVE_AS, MF_GRAYED );
    m_menuBar.EnableMenuItem( ID_FILE_PRINT_PREVIEW, MF_GRAYED );
    m_menuBar.EnableMenuItem( ID_FILE_PRINT, MF_ENABLED );
    m_menuBar.EnableMenuItem( ID_EDIT_COPY, MF_ENABLED);
    m_menuBar.EnableMenuItem( ID_EDIT_CUT, MF_ENABLED );
    m_menuBar.EnableMenuItem( ID_EDIT_PASTE, MF_ENABLED );
    m_menuBar.EnableMenuItem( ID_EDIT_UNDO, MF_GRAYED );
//    m_menuBar.EnableMenuItem( IDM_COEF, MF_GRAYED );
//    m_menuBar.EnableMenuItem( IDM_LG, MF_GRAYED );

    CMenu* Epopup = m_menuBar.GetSubMenu( 1 ); //->GetSubMenu( 3 );
    if (Epopup)
    {
        Epopup->EnableMenuItem( 1, MF_GRAYED | MF_BYPOSITION );
    }

//    Epopup->Enable(false);
//    if (Epopup) Cpopup->
//    get coordinates of menu
//    CDC* pDC = GetDC();
//    CRect menuRect;
//    int menuHeight = GetSystemMetrics(SM_CYMENU);
//    CPen Pen(PS_SOLID, 1, COLORREF(0x000000));
//    pDC->SelectObject(&Pen);
//    pDC->MoveTo(0,menuHeight+2);
//    pDC->LineTo(200,menuHeight+2);

    // set style of box
    long wStyle = GetWindowLong( m_hWnd, GWL_STYLE );
    SetWindowLong(m_hWnd,GWL_STYLE, wStyle | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_CLIPSIBLINGS);

    // center the window
    CenterWindow();

    // hide buttons at bottom
    CWnd* bcp = GetDlgItem( IDOK );
    bcp->ShowWindow( SW_HIDE );
    bcp = GetDlgItem( IDCANCEL );
    bcp->ShowWindow( SW_HIDE );
    bcp = GetDlgItem( ID_APPLY_NOW );
    bcp->ShowWindow( SW_HIDE );
    bcp = GetDlgItem( IDHELP );
    bcp->ShowWindow( SW_HIDE );
    BOOL bResult = cdxCSizingPropSheet::OnInitDialog();
    // get units from registry
    metricflag = SetMetricFlag();
    metricflagsetting = metricflag;
    psychopage.SetDefaultValues();
    if (metricflag)
    {
        m_menuBar.CheckMenuItem( ID_EDIT_COPY, MF_CHECKED  );
        m_menuBar.CheckMenuItem( ID_EDIT_CUT,  MF_UNCHECKED );
    }
    else
    {
        m_menuBar.CheckMenuItem( ID_EDIT_COPY, MF_UNCHECKED  );
        m_menuBar.CheckMenuItem( ID_EDIT_CUT,  MF_CHECKED   );
    }


	//m_wndToolBar.Create(this);
	//m_wndToolBar.LoadToolBar(IDR_MAINFRAME);
	//m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() | CBRS_TOOLTIPS | CBRS_SIZE_DYNAMIC);

	//m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	//EnableDocking(CBRS_ALIGN_ANY);
	//DockControlBar(&m_wndToolBar);

	return bResult;
}

void TPropPageBase::OnOK() 
{
    int pageno = GetActiveIndex();
    switch (pageno)
    {
    case 0 :
        psychopage.OnPsychButtonRecalc();
        break;
    case 1 :
        merkelpage.OnMerkelButtonRecalc();
        break;
    case 2 :
        curvespage.OnCurveButtonRecalc();
        break;
    case 3 :
		performancepage.OnPerformanceButtonRecalc();
        break;
    }
}

void TPropPageBase::OnCancel()
{
    int pageno = GetActiveIndex();
    switch (pageno)
    {
    case 0 :
        psychopage.SetDefaultValues();
        psychopage.UpdateData(false);
        break;
    case 1 :
        merkelpage.SetDefaultValues();
        merkelpage.UpdateData(false);
        break;
    case 2 :
        curvespage.SetDefaultValues();
        curvespage.UpdateData(false);
        break;
    case 3 :
        performancepage.SetDefaultValues();
        performancepage.UpdateData(false);
        break;
    }
}

void TPropPageBase::OnAppExit() 
{
    EndDialog(IDOK);
}

void TPropPageBase::OnSize(UINT nType, int cx, int cy) 
{
	cdxCSizingPropSheet::OnSize(nType, cx, cy);	
}

void TPropPageBase::OnPaint() 
{
    if (getApp().tps->IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CPaintDC dc(this); // device context for painting
        
        //
        // draws a line under the menubar
        //
        
        // get colors
        DWORD hicolor = GetSysColor(COLOR_ACTIVEBORDER);
        DWORD locolor = GetSysColor(COLOR_3DSHADOW);
        // get current width
        CRect crect;
        GetClientRect(&crect);
        // get coordinates of menu
        CDC* pDC = GetDC();
        // setup pens
        CPen BPen(PS_SOLID, 1, COLORREF(locolor));
        CPen WPen(PS_SOLID, 1, COLORREF(hicolor));
        // draw black line
        pDC->SelectObject(&BPen);
        pDC->MoveTo(5,0);
        pDC->LineTo(crect.right-5,0);
        // draw white line
        pDC->SelectObject(&WPen);
        pDC->MoveTo(5,1);
        pDC->LineTo(crect.right-5,1);
    }
    
    int pageno = GetActiveIndex();
    switch (pageno)
    {
    case 0 :
#ifdef _DEMO_VERSION
        m_menuBar.EnableMenuItem( ID_FILE_PRINT_DIRECT,  MF_GRAYED );
        m_menuBar.EnableMenuItem( ID_FILE_PRINT_PREVIEW, MF_GRAYED );
#else
        m_menuBar.EnableMenuItem( ID_FILE_PRINT_DIRECT,  MF_ENABLED );
        m_menuBar.EnableMenuItem( ID_FILE_PRINT_PREVIEW, MF_ENABLED );
#endif
        m_menuBar.EnableMenuItem( ID_EDIT_COPY,          MF_ENABLED);
        m_menuBar.EnableMenuItem( ID_EDIT_CUT,           MF_ENABLED );
        m_menuBar.EnableMenuItem( ID_DEMAND_CURVES,      MF_GRAYED );
        break;

    case 1 :
        m_menuBar.EnableMenuItem( ID_FILE_PRINT_DIRECT,  MF_GRAYED );
        m_menuBar.EnableMenuItem( ID_FILE_PRINT_PREVIEW, MF_GRAYED );
        m_menuBar.EnableMenuItem( ID_EDIT_COPY,          MF_ENABLED);
        m_menuBar.EnableMenuItem( ID_EDIT_CUT,           MF_ENABLED);
        m_menuBar.EnableMenuItem( ID_DEMAND_CURVES,      MF_GRAYED );
        break;
    
	case 2 :
#ifdef _DEMO_VERSION
        m_menuBar.EnableMenuItem( ID_FILE_PRINT_DIRECT,  MF_GRAYED );
        m_menuBar.EnableMenuItem( ID_FILE_PRINT_PREVIEW, MF_GRAYED );		
#else
        m_menuBar.EnableMenuItem( ID_FILE_PRINT_DIRECT,  MF_ENABLED );
        m_menuBar.EnableMenuItem( ID_FILE_PRINT_PREVIEW, MF_ENABLED );
#endif
        m_menuBar.EnableMenuItem( ID_EDIT_COPY,          MF_ENABLED );
        m_menuBar.EnableMenuItem( ID_EDIT_CUT,           MF_ENABLED );
        m_menuBar.EnableMenuItem( ID_DEMAND_CURVES,      MF_ENABLED );
        break;
    
	case 3 :
#ifdef _DEMO_VERSION
        m_menuBar.EnableMenuItem( ID_FILE_PRINT_DIRECT,  MF_GRAYED );
        m_menuBar.EnableMenuItem( ID_FILE_PRINT_PREVIEW, MF_GRAYED );		
#else
        m_menuBar.EnableMenuItem( ID_FILE_PRINT_DIRECT,  MF_ENABLED );
        m_menuBar.EnableMenuItem( ID_FILE_PRINT_PREVIEW, MF_ENABLED );
#endif
        m_menuBar.EnableMenuItem( ID_EDIT_COPY,          MF_ENABLED );
        m_menuBar.EnableMenuItem( ID_EDIT_CUT,           MF_ENABLED );
        m_menuBar.EnableMenuItem( ID_DEMAND_CURVES,      MF_GRAYED );
        break;
	
	default:  // will catch -1 on first tab if demo version
        m_menuBar.EnableMenuItem( ID_FILE_PRINT_DIRECT,  MF_GRAYED );
        m_menuBar.EnableMenuItem( ID_FILE_PRINT_PREVIEW, MF_GRAYED );
        m_menuBar.EnableMenuItem( ID_EDIT_COPY,          MF_ENABLED);
        m_menuBar.EnableMenuItem( ID_EDIT_CUT,           MF_ENABLED);
        m_menuBar.EnableMenuItem( ID_DEMAND_CURVES,      MF_GRAYED );
    }
}

void TPropPageBase::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	cdxCSizingPropSheet::OnActivate(nState, pWndOther, bMinimized);
	// TODO: Add your message handler code here
	
}

void TPropPageBase::OnEditCopy() // metric 
{
    TPropPageBase::changingUnits = true;
    metricflag = true;
    if ( metricflag != metricflagsetting )
    {
        metricflagsetting = metricflag;
        SetRegKey( metricflag );
        m_menuBar.CheckMenuItem( ID_EDIT_COPY, MF_CHECKED  );  // metric
        m_menuBar.CheckMenuItem( ID_EDIT_CUT,  MF_UNCHECKED ); // english

        if (::IsWindow(psychopage.m_hWnd))		psychopage.UpDateUnits();
        if (::IsWindow(curvespage.m_hWnd))		curvespage.ChangeUnits();
        if (::IsWindow(merkelpage.m_hWnd))		merkelpage.ChangeUnits();
		if (::IsWindow(performancepage.m_hWnd)) performancepage.ChangeUnits();
#ifdef _DEMO_VERSION
		if (::IsWindow(limitspage.m_hWnd))		limitspage.ChangeUnits();
#endif
    }
    TPropPageBase::changingUnits = false;
}

void TPropPageBase::OnEditCut() // english
{
    TPropPageBase::changingUnits = true;
    metricflag = false;
    if ( metricflag != metricflagsetting )
    {
        metricflagsetting = metricflag;    
        SetRegKey( metricflag );
        m_menuBar.CheckMenuItem( ID_EDIT_COPY, MF_UNCHECKED  ); // metric
        m_menuBar.CheckMenuItem( ID_EDIT_CUT,  MF_CHECKED ); // english

        if (::IsWindow(psychopage.m_hWnd))		psychopage.UpDateUnits();
        if (::IsWindow(curvespage.m_hWnd))		curvespage.ChangeUnits();
        if (::IsWindow(merkelpage.m_hWnd))		merkelpage.ChangeUnits();
		if (::IsWindow(performancepage.m_hWnd)) performancepage.ChangeUnits();
#ifdef _DEMO_VERSION
		if (::IsWindow(limitspage.m_hWnd))		limitspage.ChangeUnits();
#endif
    }
    TPropPageBase::changingUnits = false;
}


void TPropPageBase::SetRegKey(bool flag) 
{
    HKEY  m_hKey;
    DWORD dw;

    // Create/Open the key
	CString szKey(_T("SOFTWARE\\Cooling Technology Institute\\"));
	szKey += getApp().GetAppName() + _T("\\");
	szKey += getApp().GetShortVersion();
//	szKey += "Units";
    RegCreateKeyEx( HKEY_LOCAL_MACHINE, 
                    szKey,
                    0L,
                    NULL,
                    REG_OPTION_NON_VOLATILE,
                    KEY_ALL_ACCESS,
                    NULL,
                    &m_hKey,
                    &dw );

    // Set the units
    LPCTSTR Units;
    if (flag)
    {
        Units = _T("Metric");
    }  
    else
    {
        Units = _T("English");
    }
    RegSetValueEx( m_hKey, _T("Units"), 0L, REG_SZ, (const unsigned char *)Units, _tcslen(Units)+1 );
}

void TPropPageBase::GetRegistryValues()     
{    
	/*
    HKEY m_hKey;
    DWORD dw;

    // Create/Open the key
    RegCreateKeyEx( HKEY_LOCAL_MACHINE, 
                    "SOFTWARE\\Cooling Technology Institute\\CTIToolkit\\Units",
                    0L,
                    NULL,
                    REG_OPTION_NON_VOLATILE,
                    KEY_ALL_ACCESS,
                    NULL,
                    &m_hKey,
                    &dw );

    char  buffer[20];
    DWORD dwType = REG_SZ;
    DWORD dwSize = 20;
    
     // Set the values
    long error;
    error = RegQueryValueEx( m_hKey,
                     "Pressure",
                     NULL,
                     &dwType,
                     reinterpret_cast<BYTE*>(buffer),
                     &dwSize );
    if ( error == ERROR_SUCCESS )
    {
        dblPressure = atof(buffer);
    }
 
    error = RegQueryValueEx( m_hKey,
                     "WBT",
                     NULL,
                     &dwType,
                     reinterpret_cast<BYTE*>(buffer),
                     &dwSize );
    if ( error == ERROR_SUCCESS )
    {
        dblWBT = atof(buffer);
    }

    error = RegQueryValueEx( m_hKey,
                     "DBT",
                     NULL,
                     &dwType,
                     reinterpret_cast<BYTE*>(buffer),
                     &dwSize );
    if ( error == ERROR_SUCCESS )
    {
        dblDBT = atof(buffer);
    }

    error = RegQueryValueEx( m_hKey,
                     "Altitude",
                     NULL,
                     &dwType,
                     reinterpret_cast<BYTE*>(buffer),
                     &dwSize );
    if ( error == ERROR_SUCCESS )
    {
        dblAltitude = atof(buffer);
    }

    error = RegQueryValueEx( m_hKey,
                     "Humidity",
                     NULL,
                     &dwType,
                     reinterpret_cast<BYTE*>(buffer),
                     &dwSize );
    if ( error == ERROR_SUCCESS )
    {
        dblRelHumidity = atof(buffer);
    }

    error = RegQueryValueEx( m_hKey,
                     "Enthalpy",
                     NULL,
                     &dwType,
                     reinterpret_cast<BYTE*>(buffer),
                     &dwSize );
    if ( error == ERROR_SUCCESS )
    {
        dblEnthalpy = atof(buffer);
    }
	*/
}

bool TPropPageBase::SetMetricFlag()
{
    HKEY m_hKey;
    DWORD dw;
    bool rval = false;

    // Create/Open the key
	CString szKey("SOFTWARE\\Cooling Technology Institute\\");
	szKey += getApp().GetAppName() + _T("\\");
	szKey += getApp().GetShortVersion();
    RegCreateKeyEx( HKEY_LOCAL_MACHINE, 
                    szKey,
                    0L,
                    NULL,
                    REG_OPTION_NON_VOLATILE,
                    KEY_ALL_ACCESS,
                    NULL,
                    &m_hKey,
                    &dw );

    TCHAR units[20];
    DWORD dwType;
    DWORD dwSize = 20;
    RegQueryValueEx( m_hKey,
                     _T("Units"),
                     NULL,
                     &dwType,
                     reinterpret_cast<BYTE*>(units),
                     &dwSize );
    
    if ( !_tcscmp( units, _T("Metric") ) )
        rval = true;

    return rval;
}

/*
 Welcome	    1
 Psychrometric	2
 Merkel	        3
 Demand_Curve	4
 Performance	5
*/

DWORD TPropPageBase::m_fnGetPageHelpID(int welcomeFlag )
{
    DWORD helpID = Welcome_Help;
    if (welcomeFlag == 5) return helpID;

    int pageno = GetActiveIndex();
    switch (pageno)
    {
    case 0: 
        helpID = Psychrometric_Help;
        break;
    case 1:
        helpID = Merkel_Help;
        break;
    case 2:
        helpID = Demand_Curve_Help;
        break;
    case 3:
        helpID = Performance_Help;
        break;
    default:
        helpID = Welcome_Help;
    }
    return helpID;
}


LONG TPropPageBase::OnHelp(WPARAM, LPARAM)
{
	::HtmlHelp(AfxGetApp()->m_pMainWnd->m_hWnd, 
        AfxGetApp()->m_pszHelpFilePath, 
        HH_HELP_CONTEXT, m_fnGetPageHelpID(0));
	return 0;
}

void TPropPageBase::OnHelphelp() 
{
	::HtmlHelp(AfxGetApp()->m_pMainWnd->m_hWnd, 
        AfxGetApp()->m_pszHelpFilePath, 
        HH_HELP_CONTEXT, 1);
}

void TPropPageBase::OnMultiEdit()
{
    curvespage.OnMultiEdit();
}


void TPropPageBase::OnUpdateFilePrint(CCmdUI* /*pCmdUI*/) 
{
    int pageno = GetActiveIndex();
    switch (pageno)
    {
    case 0 :
        break;
    case 1 :
        break;
    case 2 :
        break;
    case 3 :
        break;
    }
}

BOOL TPropPageBase::PreTranslateMessage(MSG *pMsg)
{
	if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_RETURN)) {
		CWnd * pControl = GetFocus();
		int pageno = getApp().tps->GetActiveIndex();
		CPropertyPage * page = getApp().tps->GetPage(pageno);
		
		if ((pControl != NULL) && (page != NULL) && (page->m_hWnd != NULL)) {
			HWND controlHandle = ::GetDlgItem(page->m_hWnd, pControl->GetDlgCtrlID());
			TCHAR className[128];
			if ((controlHandle != NULL) && (::GetClassName(controlHandle, className, 128) > 0)) {
				if (_tcsnicmp(className, "edit", 4) == 0) {
					pMsg->wParam = VK_TAB;
				}
			}
		}
	}
	return cdxCSizingPropSheet::PreTranslateMessage(pMsg);
}

int TPropPageBase::GetActiveIndex() const
{
#ifdef _PERF_CURVE_ONLY
	return 3;
#elif _DEMO_VERSION
	return CPropertySheet::GetActiveIndex()-1;
#else
	return CPropertySheet::GetActiveIndex();
#endif
}
