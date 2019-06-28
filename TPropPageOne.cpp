//---------------------------------------------------------------------
//
// TPropPageOne.cpp : 
//
// Copyright c 1997-1998 Cosmo Engineering  -  All Rights Reserved.
//
// $Archive: /Toolkit/TPropPageOne.cpp $
// $Revision: 3 $
// $Modtime: 6/18/01 5:53a $
//
//---------------------------------------------------------------------
//
// $Log: /Toolkit/TPropPageOne.cpp $
// 
// 3     7/04/01 4:10a Davidl
// Changes for demo version; also, fix unit conversion bug upon window
// initialization.
// 
// 2     5/03/01 5:22a Davidl
// Changed minmax values to constants.
// 
// 1     11/06/00 9:46a Deanp
// Initial 2.0 source
// 
// 54    6/15/99 7:52a Deanp
// New version of the source from Rich Harrison.
// 
// 53    4/29/99 11:42a Administrator
// fixed overflowing enthalpy problem
// 
// 52    3/01/99 9:34a Deanp
// fixed bug in readio select.
// 
// 51    2/26/99 8:39a Deanp
// % off by 100
// 
// 50    2/25/99 8:34a Deanp
// removed menu enables from dodataexchange
// 
// 49    2/25/99 8:22a Deanp
// added  || (HumidRatio < 0) back in because Rich says so.
// 
// 48    2/25/99 8:00a Deanp
// fixed error messages.
// 
// 47    2/25/99 7:57a Deanp
// removed  (HumidRatio < 0) again
// 
// 46    2/25/99 7:56a Deanp
// removed  (HumidRatio < 0)
// 
// 45    2/25/99 7:53a Deanp
// added new validation of calcs  (SpVolume < 0) || (HumidRatio < 0)
// 
// 44    2/25/99 7:38a Deanp
// fixed unit conversion 
// 
// 43    2/25/99 7:29a Deanp
// added calc verification for Thermodynamically impossible combination
// 
// 42    2/25/99 6:50a Deanp
// change so all altitude inputs are toggled between altitude and
// barometric pressure
// 
//---------------------------------------------------------------------
#include "ListCtrl.h"
#include "afxwin.h"
#include "afxext.h"
#include "stdafx.h"
#include "CError.h"
#include "CTIToolkit.h"
#include "ctiutils.h"
#include "PrintList.h"
#include "TPrint.h"
#include "PrintFrame.h"
#include "TPropPageBase.h"
#include "TPropPageOne.h"
#include "ctihelp.hh"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//---------------------------------------------------------------------
// Print Window
//---------------------------------------------------------------------
extern CPrintList   wndPrintList;
extern CPrintFrame	wndPrintListFrame;

//---------------------------------------------------------------------
// TPropPageOne property page
//---------------------------------------------------------------------
IMPLEMENT_DYNCREATE(TPropPageOne, cdxCSizingPropPage)
BEGIN_MESSAGE_MAP(TPropPageOne, cdxCSizingPropPage)
	//{{AFX_MSG_MAP(TPropPageOne)
	ON_BN_CLICKED(IDC_PSYCH_RADIO_ITEM1, OnPsychRadioItem1)
	ON_BN_CLICKED(IDC_PSYCH_RADIO_ITEM3, OnPsychRadioItem3)
	ON_BN_CLICKED(IDC_PSYCH_RADIO_ITEM4, OnPsychRadioItem4)
	ON_WM_SIZE()
	ON_CONTROL(EN_CHANGE, IDC_PSYCH_EDIT_PRESSURE, OnDataChanged)
    ON_BN_CLICKED(IDC_PSYCH_BUTTON, OnPsychButtonRecalc)
	ON_CONTROL(EN_CHANGE, IDC_PSYCH_EDIT_WB_TEMP, OnDataChanged)
	ON_CONTROL(EN_CHANGE, IDC_PSYCH_EDIT_DB_TEMP, OnDataChanged)
	ON_BN_CLICKED(IDC_BUTTON_ALTITUDE, OnButtonAltitude)
	//}}AFX_MSG_MAP
    ON_COMMAND(ID_FILE_PRINT, TPropPageOne::OnFilePrint)
    ON_COMMAND(ID_FILE_PRINT_DIRECT, TPropPageOne::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, TPropPageOne::OnFilePrintPreview)
END_MESSAGE_MAP()


//---------------------------------------------------------------------
// FUNCTION    : TPropPageOne::TPropPageOne
//
// DESCRIPTION : None.
//
// INPUT       : None.
//
// RETURN (S)  : None.
//---------------------------------------------------------------------
// REMARKS     : None.
//---------------------------------------------------------------------
TPropPageOne::TPropPageOne() : cdxCSizingPropPage(TPropPageOne::IDD)
{
	//{{AFX_DATA_INIT(TPropPageOne)
	//}}AFX_DATA_INIT
	m_bAltitude = true;
    SetDefaultValues();
}
//---------------------------------------------------------------------
// END: TPropPageOne::TPropPageOne
//---------------------------------------------------------------------



//---------------------------------------------------------------------
// FUNCTION    : TPropPageOne::~TPropPageOne
//
// DESCRIPTION : None.
//
// INPUT       : None.
//
// RETURN (S)  : None.
//---------------------------------------------------------------------
// REMARKS     : None.
//---------------------------------------------------------------------
TPropPageOne::~TPropPageOne()
{
}
//---------------------------------------------------------------------
// END: TPropPageOne::~TPropPageOne
//---------------------------------------------------------------------



//---------------------------------------------------------------------
// FUNCTION    : TPropPageOne::OnInitDialog
//
// DESCRIPTION : None.
//
// INPUT       : None.
//
// RETURN (S)  : BOOL 
//---------------------------------------------------------------------
// REMARKS     : None.
//---------------------------------------------------------------------
BOOL TPropPageOne::OnInitDialog() 
{
	CheckRadioButton( IDC_PSYCH_RADIO_ITEM1, IDC_PSYCH_RADIO_ITEM4, IDC_PSYCH_RADIO_ITEM1);
#ifdef _DEMO_VERSION    
	GetDlgItem(IDC_PSYCH_RADIO_ITEM3)->EnableWindow(false);
	GetDlgItem(IDC_PSYCH_RADIO_ITEM4)->EnableWindow(false);
	if (m_bAltitude)
		GetDlgItem(IDC_PSYCH_EDIT_PRESSURE)->EnableWindow(false);
#endif
	cdxCSizingPropPage::OnInitDialog();
    SetHelpID(Psychrometric_Help);
    m_wndPsychResults.AddColumn(0, "Name", 80);
	m_wndPsychResults.AddColumn(1, "Value", 60);
	m_wndPsychResults.AddColumn(2, "Units", 60);
	AddSzControl(m_wndPsychResults,  cdxCSizingDialog::mdResize,cdxCSizingDialog::mdResize); 
    UpDateUnits();
	OnPsychButtonRecalc();
	return TRUE;
}
//---------------------------------------------------------------------
// END: TPropPageOne::OnInitDialog
//---------------------------------------------------------------------


//---------------------------------------------------------------------
// FUNCTION    : TPropPageOne::OnDataChanged
//
// DESCRIPTION : None.
//
// INPUT       : None.
//
// RETURN (S)  : None.
//---------------------------------------------------------------------
// REMARKS     : None.
//---------------------------------------------------------------------
void TPropPageOne::OnDataChanged()
{
	m_wndPsychResults.DeleteAllItems();
}
//---------------------------------------------------------------------
// END: TPropPageOne::OnDataChanged
//---------------------------------------------------------------------



//---------------------------------------------------------------------
// FUNCTION    : TPropPageOne::OnButtonAltitude
//
// DESCRIPTION : None.
//
// INPUT       : None.
//
// RETURN (S)  : None.
//---------------------------------------------------------------------
// REMARKS     : None.
//---------------------------------------------------------------------
void TPropPageOne::OnButtonAltitude() 
{
	UpdateData(true);

	m_bAltitude = !m_bAltitude;
	if (m_bAltitude)
	{
	    if (TPropPageBase::metricflag)
		{
	        m_strPsychPressureUnits= L_METERS;
		}
		else
		{
			m_strPsychPressureUnits= L_FEET;
		}
		m_strAltitudeLable.LoadString(IDS_ALTITUDE); 
#ifdef _DEMO_VERSION    
		GetDlgItem(IDC_PSYCH_EDIT_PRESSURE)->EnableWindow(false);
#endif
	}
	else
	{
	    if (TPropPageBase::metricflag)
		{
	        m_strPsychPressureUnits= L_KPA;
		}
		else
		{
			m_strPsychPressureUnits= L_HG;
		}
		m_strAltitudeLable.LoadString(IDS_BAROMETRIC_PRESSURE); 
#ifdef _DEMO_VERSION    
		GetDlgItem(IDC_PSYCH_EDIT_PRESSURE)->EnableWindow(true);
#endif
	}

	UpdateData(false);
}
//---------------------------------------------------------------------
// END: TPropPageOne::OnButtonAltitude
//---------------------------------------------------------------------


//---------------------------------------------------------------------
// FUNCTION    : TPropPageOne::DoDataExchange
//
// DESCRIPTION : None.
//
// INPUT       : CDataExchange* pDX
//
// RETURN (S)  : None.
//---------------------------------------------------------------------
// REMARKS     : None.
//---------------------------------------------------------------------
void TPropPageOne::DoDataExchange(CDataExchange* pDX)
{
	cdxCSizingPropPage::DoDataExchange(pDX);
    int button = GetCheckedRadioButton( IDC_PSYCH_RADIO_ITEM1, IDC_PSYCH_RADIO_ITEM4 );

    //{{AFX_DATA_MAP(TPropPageOne)
    DDX_Control(pDX, IDC_PSYCH_LIST_RESULTS,          m_wndPsychResults);
    DDX_Text(   pDX, IDC_PSYCH_STATIC_DBTEMP_UNITS,   m_strPsychDBTUnits);
    DDX_Text(   pDX, IDC_PSYCH_STATIC_PRESSURE_UNITS, m_strPsychPressureUnits);
    DDX_Text(   pDX, IDC_PSYCH_STATIC_WBTEMP_UNITS,   m_strPsychWBTUnits);
    DDX_Text(   pDX, IDC_PSYCH_STATIC_WBTEMP_LABEL,   m_strWBTempLabel);
    DDX_Text(   pDX, IDC_PSYCH_STATIC_DBTEMP_LABEL,   m_strDBTempLabel);
    //}}AFX_DATA_MAP
    
	switch (button)
    {
    case IDC_PSYCH_RADIO_ITEM1:
		//---------------------------------------------------------------------
		// Get and Validate Altitude or Barometric Pressure.
		//---------------------------------------------------------------------
		if (m_bAltitude)
		{
			DDX_Text(pDX, IDC_PSYCH_EDIT_PRESSURE, m_dblAltitude);
//			DDV_MinMaxDouble(pDX,m_dblAltitude,-500,40000,-150,12200,1);
			DDV_MinMaxDouble(pDX,m_dblAltitude, ALT_MIN_IP, ALT_MAX_IP,
                ALT_MIN_SI, ALT_MAX_SI, 1);
		}
		else
		{
			double dblPressure;
			if (TPropPageBase::metricflag)
			{
				dblPressure = Altitude2KPA(m_dblAltitude);
			}
			else
			{
				dblPressure = calcPressureF(Altitude2PSI(m_dblAltitude));
			}
			dblPressure = truncit(dblPressure, 5);
			DDX_Text(pDX, IDC_PSYCH_EDIT_PRESSURE, dblPressure);
			DDV_MinMaxDouble(pDX, dblPressure, BAROMETRIC_PRESSURE_MIN_IP,
				BAROMETRIC_PRESSURE_MAX_IP,BAROMETRIC_PRESSURE_MIN_SI,
				BAROMETRIC_PRESSURE_MAX_SI,3);
			if (TPropPageBase::metricflag)
			{
				m_dblAltitude = KPA2Altitude(dblPressure);
			}
			else
			{
				m_dblAltitude = PSI2Altitude(calcPressureC(dblPressure));
			}
			m_dblAltitude = truncit(m_dblAltitude, 4);
		}
        DDX_Text(   pDX, IDC_PSYCH_EDIT_WB_TEMP, m_dblPsychWBT);
        DDV_MinMaxDouble(pDX,m_dblPsychWBT,WBT_MIN_IP,WBT_MAX_IP,
			WBT_MIN_SI,WBT_MAX_SI,2);
        DDX_Text(   pDX, IDC_PSYCH_EDIT_DB_TEMP, m_dblPsychDBT);
        DDV_MinMaxDouble(pDX,m_dblPsychDBT,DBT_MIN_IP,DBT_MAX_IP,
			DBT_MIN_SI,DBT_MAX_SI,2);
		if (m_dblPsychDBT < m_dblPsychWBT)
		{
			AfxMessageBox("Dry bulb must be greater than Wet bulb temperature.", MB_ICONEXCLAMATION);
			pDX->Fail();
		}
        break;

    case IDC_PSYCH_RADIO_ITEM3 :
		//---------------------------------------------------------------------
		// Get and Validate Altitude or Barometric Pressure.
		//---------------------------------------------------------------------
		if (m_bAltitude)
		{
			DDX_Text(pDX, IDC_PSYCH_EDIT_PRESSURE, m_dblAltitude);
//			DDV_MinMaxDouble(pDX,m_dblAltitude,-500,40000,-150,12200,1);
			DDV_MinMaxDouble(pDX,m_dblAltitude, ALT_MIN_IP, ALT_MAX_IP,
                ALT_MIN_SI, ALT_MAX_SI, 1);
		}
		else
		{
			double dblPressure;
			if (TPropPageBase::metricflag)
			{
				dblPressure = Altitude2KPA(m_dblAltitude);
			}
			else
			{
				dblPressure = calcPressureF(Altitude2PSI(m_dblAltitude));
			}
			dblPressure = truncit(dblPressure, 5);
			DDX_Text(pDX, IDC_PSYCH_EDIT_PRESSURE, dblPressure);
			DDV_MinMaxDouble(pDX, dblPressure, BAROMETRIC_PRESSURE_MIN_IP,
				BAROMETRIC_PRESSURE_MAX_IP,BAROMETRIC_PRESSURE_MIN_SI,
				BAROMETRIC_PRESSURE_MAX_SI,3);
			if (TPropPageBase::metricflag)
			{
				m_dblAltitude = KPA2Altitude(dblPressure);
			}
			else
			{
				m_dblAltitude = PSI2Altitude(calcPressureC(dblPressure));
			}
			m_dblAltitude = truncit(m_dblAltitude, 4);
		}
        DDX_Text(   pDX, IDC_PSYCH_EDIT_WB_TEMP, m_dblRelHumidity);
        DDV_MinMaxDouble(pDX,m_dblRelHumidity,0,100,0,100,2);
        DDX_Text(   pDX, IDC_PSYCH_EDIT_DB_TEMP, m_dblPsychDBT);
        DDV_MinMaxDouble(pDX,m_dblPsychDBT,DBT_MIN_IP,DBT_MAX_IP,
			DBT_MIN_SI,DBT_MAX_SI,2);
        break;

    case IDC_PSYCH_RADIO_ITEM4 :
		//---------------------------------------------------------------------
		// Get and Validate Altitude or Barometric Pressure.
		//---------------------------------------------------------------------
		if (m_bAltitude)
		{
			DDX_Text(pDX, IDC_PSYCH_EDIT_PRESSURE, m_dblAltitude);
//			DDV_MinMaxDouble(pDX,m_dblAltitude,-500,40000,-150,12200,1);
			DDV_MinMaxDouble(pDX,m_dblAltitude, ALT_MIN_IP, ALT_MAX_IP,
                ALT_MIN_SI, ALT_MAX_SI, 1);
		}
		else
		{
			double dblPressure;
			if (TPropPageBase::metricflag)
			{
				dblPressure = Altitude2KPA(m_dblAltitude);
			}
			else
			{
				dblPressure = calcPressureF(Altitude2PSI(m_dblAltitude));
			}
			dblPressure = truncit(dblPressure, 5);
			DDX_Text(pDX, IDC_PSYCH_EDIT_PRESSURE, dblPressure);
			DDV_MinMaxDouble(pDX, dblPressure, BAROMETRIC_PRESSURE_MIN_IP,
				BAROMETRIC_PRESSURE_MAX_IP,BAROMETRIC_PRESSURE_MIN_SI,
				BAROMETRIC_PRESSURE_MAX_SI,3);
			if (TPropPageBase::metricflag)
			{
				m_dblAltitude = KPA2Altitude(dblPressure);
			}
			else
			{
				m_dblAltitude = PSI2Altitude(calcPressureC(dblPressure));
			}
			m_dblAltitude = truncit(m_dblAltitude, 4);
		}
        DDX_Text(   pDX, IDC_PSYCH_EDIT_DB_TEMP, m_dblEnthalpy);
        DDV_MinMaxDouble(pDX,m_dblEnthalpy,0,2758,0,6030,4);
        break;
    default :
		ASSERT(0);
		break;
    }

	GetDlgItem(IDC_BUTTON_ALTITUDE)->SetWindowText(m_strAltitudeLable);
}
//---------------------------------------------------------------------
// END: TPropPageOne::DoDataExchange
//---------------------------------------------------------------------


//---------------------------------------------------------------------
// FUNCTION    : TPropPageOne::SetDefaultValues
//
// DESCRIPTION : None.
//
// INPUT       : None.
//
// RETURN (S)  : None.
//---------------------------------------------------------------------
// REMARKS     : None.
//---------------------------------------------------------------------
void TPropPageOne::SetDefaultValues()
{
	m_strDBTempLabel = _T("&DBT:");
    m_strWBTempLabel = _T("&WBT:");

    if (!TPropPageBase::metricflag)
    {
        m_strPsychWBTUnits = _T(L_DEGF);
        m_strPsychDBTUnits = _T(L_DEGF);
#ifndef _DEMO_VERSION
        m_dblPsychDBT      = 100.;
        m_dblPsychWBT      = 80.;
#else
        m_dblPsychDBT      = DBT_MAX_IP;
        m_dblPsychWBT      = WBT_MAX_IP;
#endif
        m_dblAltitude      = 0;
        m_dblRelHumidity   = 42.38;
        m_dblEnthalpy      = 43.46;
    }
    else
    {
        m_strPsychWBTUnits = _T(L_DEGC);
        m_strPsychDBTUnits = _T(L_DEGC);
#ifndef _DEMO_VERSION
        m_dblPsychDBT      = 37.78;
        m_dblPsychWBT      = 26.67;
#else
        m_dblPsychDBT      = DBT_MAX_SI;
        m_dblPsychWBT      = WBT_MAX_SI;
#endif
        m_dblAltitude      = 0;
        m_dblRelHumidity   = 42.38;
        m_dblEnthalpy      = 83.32;
    }

	if (IsWindow(m_hWnd))
	{
		CheckRadioButton( IDC_PSYCH_RADIO_ITEM1, IDC_PSYCH_RADIO_ITEM4, IDC_PSYCH_RADIO_ITEM1);
	}
	m_bAltitude = true;
	if (TPropPageBase::metricflag)
	{
	    m_strPsychPressureUnits = L_METERS;
	}
	else
	{
		m_strPsychPressureUnits = L_FEET;
	}
	m_strAltitudeLable.LoadString(IDS_ALTITUDE); 
}
//---------------------------------------------------------------------
// END: TPropPageOne::SetDefaultValues
//---------------------------------------------------------------------


//---------------------------------------------------------------------
// FUNCTION    : TPropPageOne::UpDateUnits
//
// DESCRIPTION : None.
//
// INPUT       : None.
//
// RETURN (S)  : None.
//---------------------------------------------------------------------
// REMARKS     : None.
//---------------------------------------------------------------------
void TPropPageOne::UpDateUnits() 
{
    if ((!TPropPageBase::metricflag) && 
		(m_strPsychPressureUnits != L_FEET) &&
		(m_strPsychPressureUnits != L_HG))
    {
		// Avoid conversion problems at the limits
		if (m_dblPsychWBT == WBT_MAX_SI)
			m_dblPsychWBT = WBT_MAX_IP;
		else if (m_dblPsychWBT == WBT_MIN_SI)
			m_dblPsychWBT = WBT_MIN_IP;
		else
			m_dblPsychWBT = truncit(fnCelcToFar( m_dblPsychWBT ),2);
		
		if (m_dblPsychDBT == DBT_MAX_SI)
			m_dblPsychDBT = DBT_MAX_IP;
		else if (m_dblPsychDBT == DBT_MIN_SI)
			m_dblPsychDBT = DBT_MIN_IP;
		else
			m_dblPsychDBT = truncit(fnCelcToFar( m_dblPsychDBT ),2);

        m_dblAltitude = truncit(fnMetersToFeet(m_dblAltitude), 4);
    }
    else if ((TPropPageBase::metricflag) &&
		(m_strPsychPressureUnits != L_METERS) &&
		(m_strPsychPressureUnits != L_KPA))
    {
		if (m_dblPsychWBT == WBT_MAX_IP)
			m_dblPsychWBT = WBT_MAX_SI;
		else if (m_dblPsychWBT == WBT_MIN_IP)
			m_dblPsychWBT = WBT_MIN_SI;
		else
			m_dblPsychWBT = truncit(fnFarToCelc( m_dblPsychWBT ),2);
        
		if (m_dblPsychDBT == DBT_MAX_IP)
			m_dblPsychDBT = DBT_MAX_SI;
		else if (m_dblPsychDBT == DBT_MIN_IP)
			m_dblPsychDBT = DBT_MIN_SI;
		else
			m_dblPsychDBT = truncit(fnFarToCelc( m_dblPsychDBT ),2);
        
		m_dblAltitude = truncit(fnFeetToMeters(m_dblAltitude), 4);
    }
	if (m_bAltitude)
	{
	    if (TPropPageBase::metricflag)
		{
	        m_strPsychPressureUnits= L_METERS;
		}
		else
		{
			m_strPsychPressureUnits= L_FEET;
		}
	}
	else
	{
	    if (TPropPageBase::metricflag)
		{
	        m_strPsychPressureUnits= L_KPA;
		}
		else
		{
			m_strPsychPressureUnits= L_HG;
		}
	}

    
	UpdateData(false);
	OnPsychRadioItem(); 
}
//---------------------------------------------------------------------
// END: TPropPageOne::UpDateUnits
//---------------------------------------------------------------------


//---------------------------------------------------------------------
// FUNCTION    : TPropPageOne::OnSize
//
// DESCRIPTION : None.
//
// INPUT       : UINT nType
//               int cx - 
//               int cy - 
//
// RETURN (S)  : None.
//---------------------------------------------------------------------
// REMARKS     : None.
//---------------------------------------------------------------------
void TPropPageOne::OnSize(UINT nType, int cx, int cy) 
{
	cdxCSizingPropPage::OnSize(nType, cx, cy);	

    long wStyle = GetWindowLong( m_hWnd, GWL_STYLE );
	wStyle ^= WS_CLIPCHILDREN;
    SetWindowLong(m_hWnd,GWL_STYLE, wStyle);
	InvalidateRect(NULL);
	wStyle |= WS_CLIPCHILDREN;
    SetWindowLong(m_hWnd,GWL_STYLE, wStyle);
}
//---------------------------------------------------------------------
// END: TPropPageOne::OnSize
//---------------------------------------------------------------------


//---------------------------------------------------------------------
// FUNCTION    : TPropPageOne::OnFilePrint
//
// DESCRIPTION : None.
//
// INPUT       : None.
//
// RETURN (S)  : None.
//---------------------------------------------------------------------
// REMARKS     : None.
//---------------------------------------------------------------------
void TPropPageOne::OnFilePrint() 
{
    CString title = " CTI Psychrometric Air Properties Report ";
    TPrint tp;
    if (tp.DoModal() == IDOK)
	{
	    wndPrintList.m_fnPrint( title, tp.m_csDescription, &m_wndPsychResults );
	}
}
//---------------------------------------------------------------------
// END: TPropPageOne::OnFilePrint
//---------------------------------------------------------------------


//---------------------------------------------------------------------
// FUNCTION    : TPropPageOne::OnFilePrintPreview
//
// DESCRIPTION : None.
//
// INPUT       : None.
//
// RETURN (S)  : None.
//---------------------------------------------------------------------
// REMARKS     : None.
//---------------------------------------------------------------------
void TPropPageOne::OnFilePrintPreview() 
{
    CString title = " CTI Psychrometric Air Properties Report ";
    TPrint tp;
    if (tp.DoModal() == IDOK)
	{
//	    wndPrintList.m_fnPrint( title, tp.m_csDescription, &m_wndPsychResults );
		wndPrintListFrame.ShowWindow(SW_NORMAL);
		wndPrintListFrame.m_bPrintPreview = TRUE;
		wndPrintList.ShowWindow(SW_NORMAL);
		wndPrintList.m_fnPrintPreview(title, tp.m_csDescription, 
			&m_wndPsychResults); 
	}
}
//---------------------------------------------------------------------
// END: TPropPageOne::OnFilePrintPreview
//---------------------------------------------------------------------

//---------------------------------------------------------------------
// FUNCTION    : TPropPageOne::OnPsychRadioItem1
//
// DESCRIPTION : None.
//
// INPUT       : None.
//
// RETURN (S)  : None.
//---------------------------------------------------------------------
// REMARKS     : None.
//---------------------------------------------------------------------
void TPropPageOne::OnPsychRadioItem1()
{
    GetDlgItem(IDC_PSYCH_EDIT_WB_TEMP)->EnableWindow();
    GetDlgItem(IDC_PSYCH_STATIC_WBTEMP_LABEL)->EnableWindow();
    GetDlgItem(IDC_PSYCH_STATIC_WBTEMP_UNITS)->EnableWindow();

	m_strDBTempLabel = _T("&DBT:");
	m_strWBTempLabel = _T("&WBT:");

	if (!TPropPageBase::metricflag)
	{
		// unit labels
		m_strPsychWBTUnits = _T(L_DEGF);
		m_strPsychDBTUnits = _T(L_DEGF);
	}
	else
	{
		// unit labels
		m_strPsychWBTUnits = _T(L_DEGC);
		m_strPsychDBTUnits = _T(L_DEGC);
	}

	
    if (UpdateData(false))
	{
		OnPsychButtonRecalc();
	}
}
//---------------------------------------------------------------------
// END: TPropPageOne::OnPsychRadioItem1
//---------------------------------------------------------------------


//---------------------------------------------------------------------
// FUNCTION    : TPropPageOne::OnPsychRadioItem3
//
// DESCRIPTION : None.
//
// INPUT       : None.
//
// RETURN (S)  : None.
//---------------------------------------------------------------------
// REMARKS     : None.
//---------------------------------------------------------------------
void TPropPageOne::OnPsychRadioItem3() 
{
    GetDlgItem(IDC_PSYCH_EDIT_WB_TEMP)->EnableWindow();
    GetDlgItem(IDC_PSYCH_STATIC_WBTEMP_LABEL)->EnableWindow();
    GetDlgItem(IDC_PSYCH_STATIC_WBTEMP_UNITS)->EnableWindow();

	m_strDBTempLabel = _T("&DBT:");
	m_strWBTempLabel = _T("&Humidity:");

	if (!TPropPageBase::metricflag)
	{
		// unit labels
		m_strPsychWBTUnits = _T(L_PERCENT);
		m_strPsychDBTUnits = _T(L_DEGF);
	}
	else
	{
		// unit labels
		m_strPsychWBTUnits = _T(L_PERCENT);
		m_strPsychDBTUnits = _T(L_DEGC);
	}

    if (UpdateData(false))
	{
		OnPsychButtonRecalc();
	}
}
//---------------------------------------------------------------------
// END: TPropPageOne::OnPsychRadioItem3
//---------------------------------------------------------------------


//---------------------------------------------------------------------
// FUNCTION    : TPropPageOne::OnPsychRadioItem4
//
// DESCRIPTION : None.
//
// INPUT       : None.
//
// RETURN (S)  : None.
//---------------------------------------------------------------------
// REMARKS     : None.
//---------------------------------------------------------------------
void TPropPageOne::OnPsychRadioItem4() 
{
    GetDlgItem(IDC_PSYCH_EDIT_WB_TEMP)->EnableWindow(false);
    GetDlgItem(IDC_PSYCH_STATIC_WBTEMP_LABEL)->EnableWindow(false);
    GetDlgItem(IDC_PSYCH_STATIC_WBTEMP_UNITS)->EnableWindow(false);

	// labels
	m_strDBTempLabel   = _T("&Enthalpy:");

	if (!TPropPageBase::metricflag)
	{
		// unit labels
		m_strPsychDBTUnits      = _T(L_BTULBM);
	}
	else
	{
		// unit labels
		m_strPsychDBTUnits      = _T(L_KJKG);
	}

    if (UpdateData(false))
	{
		OnPsychButtonRecalc();
	}
}
//---------------------------------------------------------------------
// END: TPropPageOne::OnPsychRadioItem4
//---------------------------------------------------------------------


//---------------------------------------------------------------------
// FUNCTION    : TPropPageOne::OnPsychRadioItem
//
// DESCRIPTION : None.
//
// INPUT       : None.
//
// RETURN (S)  : None.
//---------------------------------------------------------------------
// REMARKS     : None.
//---------------------------------------------------------------------
void TPropPageOne::OnPsychRadioItem()
{
    int button = GetCheckedRadioButton( IDC_PSYCH_RADIO_ITEM1, IDC_PSYCH_RADIO_ITEM4 );
    switch (button)
    {
    case IDC_PSYCH_RADIO_ITEM1 :
        OnPsychRadioItem1();
        break;
    case IDC_PSYCH_RADIO_ITEM3 :
        OnPsychRadioItem3();
        break;
    case IDC_PSYCH_RADIO_ITEM4 :
        OnPsychRadioItem4();
        break;
    }
    UpdateData(true);
	OnPsychButtonRecalc();
}
//---------------------------------------------------------------------
// END: TPropPageOne::OnPsychRadioItem
//---------------------------------------------------------------------



//---------------------------------------------------------------------
// FUNCTION    : TPropPageOne::OnPsychButtonRecalc
//
// DESCRIPTION : None.
//
// INPUT       : None.
//
// RETURN (S)  : None.
//---------------------------------------------------------------------
// REMARKS     : None.
//---------------------------------------------------------------------
void TPropPageOne::OnPsychButtonRecalc() 
{
	try
	{
		int button = GetCheckedRadioButton( IDC_PSYCH_RADIO_ITEM1, IDC_PSYCH_RADIO_ITEM4 );
		switch (button)
		{
		case IDC_PSYCH_RADIO_ITEM1 :
			OnPsychButtonRecalc1();
			break;
		case IDC_PSYCH_RADIO_ITEM3 :
			OnPsychButtonRecalc3();
			break;
		case IDC_PSYCH_RADIO_ITEM4 :
			OnPsychButtonRecalc4();
			break;
		}
	}
	catch(...)
	{
		MessageBox("Error in calculation.  Please check your input values.", "Calculation Error", MB_OK | MB_ICONEXCLAMATION);
	}
}
//---------------------------------------------------------------------
// END: TPropPageOne::OnPsychButtonRecalc
//---------------------------------------------------------------------


//---------------------------------------------------------------------
// FUNCTION    : TPropPageOne::OnPsychButtonRecalc1
//
// DESCRIPTION : None.
//
// INPUT       : None.
//
// RETURN (S)  : None.
//---------------------------------------------------------------------
// REMARKS     : None.
//---------------------------------------------------------------------
void TPropPageOne::OnPsychButtonRecalc1() 
{
	if (UpdateData(true))
	{
		if ( TPropPageBase::CheckData() ) return;

		double p;
		double altitude	= m_dblAltitude;
		double TWB		= m_dblPsychWBT;
		double TDB		= m_dblPsychDBT;
		double HumidRatio;
		double RelHumid;
		double Enthalpy;
		double SpVolume;
		double Density;
		double DEWPoint;
		CString UnitsTemperature;
		CString UnitsEnthalpy;
		CString UnitsDensity;
		CString UnitsVolume;
		CString UnitsHumidity;
		CString strTemp;

		m_wndPsychResults.DeleteAllItems();

		if (!TPropPageBase::metricflag) // english
		{
			p = Altitude2PSI(m_dblAltitude);    
			CalcIPProperties (p, TWB, TDB, HumidRatio, RelHumid, Enthalpy, SpVolume, Density, DEWPoint);
			UnitsTemperature = pszIPTemp;
			UnitsEnthalpy = pszIPEnthrapy;
			UnitsDensity = pszIPDensity;
			UnitsVolume = pszIPSPVolume;
			UnitsHumidity = pszIPHumidityRatio;
			if (Enthalpy > 2758) Enthalpy = -999;
		}
		else // metric
		{
			p = Altitude2KPA(m_dblAltitude);
			CalcSIProperties (p, TWB, TDB, HumidRatio, RelHumid, Enthalpy, SpVolume, Density, DEWPoint);
			UnitsTemperature = pszSITemp;
			UnitsEnthalpy = pszSIEnthrapy;
			UnitsDensity = pszSIDensity;
			UnitsVolume = pszSISPVolume;
			UnitsHumidity = pszSIHumidityRatio;
			if (Enthalpy > 6030) Enthalpy = -999;
		}

		if ((RelHumid < 0) || (Enthalpy < 0) || (SpVolume < 0) || (HumidRatio < 0) )
		{
			AfxMessageBox("Thermodynamically impossible combination of WBT & DBT", MB_ICONEXCLAMATION);
		}
		else
		{
			int x = 0;
			if (!TPropPageBase::metricflag)
				strTemp.Format("%.4f", calcPressureF(p));
			else
				strTemp.Format("%.4f", p);
			m_wndPsychResults.AddItem(x, 0, "Barometric Pressure");
			m_wndPsychResults.AddItem(x, 1, strTemp);
			if (!TPropPageBase::metricflag)
			{
				m_wndPsychResults.AddItem(x++, 2, L_HG);
			}
			else
			{
				m_wndPsychResults.AddItem(x++, 2, L_KPA);
			}
    
			strTemp.Format("%.2f", altitude);
			m_wndPsychResults.AddItem(x, 0, "Altitude above MSL");
			m_wndPsychResults.AddItem(x, 1, strTemp);
			if (!TPropPageBase::metricflag)
			{
				m_wndPsychResults.AddItem(x++, 2, L_FEET);
			}
			else
			{
				m_wndPsychResults.AddItem(x++, 2, L_METERS);
			}

			strTemp.Format("%.2f", TDB);
			m_wndPsychResults.AddItem(x, 0, "Dry Bulb Temperature");
			m_wndPsychResults.AddItem(x, 1, strTemp);
			m_wndPsychResults.AddItem(x++, 2, UnitsTemperature);
    
			strTemp.Format("%.2f", TWB);
			m_wndPsychResults.AddItem(x, 0, "Wet Bulb Temperature");
			m_wndPsychResults.AddItem(x, 1, strTemp);
			m_wndPsychResults.AddItem(x++, 2, UnitsTemperature);
    
			strTemp.Format("%.4f", Enthalpy);
			m_wndPsychResults.AddItem(x, 0, "Enthalpy");
			m_wndPsychResults.AddItem(x, 1, strTemp);
			m_wndPsychResults.AddItem(x++, 2, UnitsEnthalpy);
    
			strTemp.Format("%.2f", DEWPoint);
			m_wndPsychResults.AddItem(x, 0, "Dew Point");
			m_wndPsychResults.AddItem(x, 1, strTemp);
			m_wndPsychResults.AddItem(x++, 2, UnitsTemperature);
    
			strTemp.Format("%.2f", RelHumid*100.);
			m_wndPsychResults.AddItem(x, 0, "Relative Humidity");
			m_wndPsychResults.AddItem(x, 1, strTemp);
			m_wndPsychResults.AddItem(x++, 2, pszPercent);
    
			strTemp.Format("%.5f", Density);
			m_wndPsychResults.AddItem(x, 0, "Density");
			m_wndPsychResults.AddItem(x, 1, strTemp);
			m_wndPsychResults.AddItem(x++, 2, UnitsDensity);
    
			strTemp.Format("%.4f", SpVolume);
			m_wndPsychResults.AddItem(x, 0, "Specific Volume");
			m_wndPsychResults.AddItem(x, 1, strTemp);
			m_wndPsychResults.AddItem(x++, 2, UnitsVolume);
    
			strTemp.Format("%.5f", HumidRatio);
			m_wndPsychResults.AddItem(x, 0, "Humidity Ratio");
			m_wndPsychResults.AddItem(x, 1, strTemp);
			m_wndPsychResults.AddItem(x++, 2, UnitsHumidity);

			m_wndPsychResults.SetColumnWidth(0, LVSCW_AUTOSIZE);
			m_wndPsychResults.SetColumnWidth(1, LVSCW_AUTOSIZE);
			m_wndPsychResults.SetColumnWidth(2, LVSCW_AUTOSIZE);
		}
	}
}
//---------------------------------------------------------------------
// END: TPropPageOne::OnPsychButtonRecalc1
//---------------------------------------------------------------------


//---------------------------------------------------------------------
// FUNCTION    : TPropPageOne::OnPsychButtonRecalc3
//
// DESCRIPTION : None.
//
// INPUT       : None.
//
// RETURN (S)  : None.
//---------------------------------------------------------------------
// REMARKS     : None.
//---------------------------------------------------------------------
void TPropPageOne::OnPsychButtonRecalc3() 
{
    if (UpdateData(true))
	{
		if ( TPropPageBase::CheckData() ) return;

		double p;
		double TWB          = m_dblRelHumidity;
		double TDB          = m_dblPsychDBT;
		double HumidRatio;
		double RelHumid;
		double Enthalpy;
		double SpVolume;
		double Density;
		double DEWPoint;
		CString UnitsTemperature;
		CString UnitsEnthalpy;
		CString UnitsDensity;
		CString UnitsVolume;
		CString UnitsHumidity;
		CString strTemp;


		m_wndPsychResults.DeleteAllItems();

		if (!TPropPageBase::metricflag)
		{
			p = Altitude2PSI(m_dblAltitude);
			IPWBsearch (p, TWB/100, TDB, TWB);
			CalcIPProperties (p, TWB, TDB, HumidRatio, RelHumid, Enthalpy, SpVolume, Density, DEWPoint);
			UnitsTemperature = pszIPTemp;
			UnitsEnthalpy = pszIPEnthrapy;
			UnitsDensity = pszIPDensity;
			UnitsVolume = pszIPSPVolume;
			UnitsHumidity = pszIPHumidityRatio;
		}
		else
		{
			p =Altitude2KPA(m_dblAltitude);
			SIWBsearch( p, TWB/100, TDB, TWB );
			CalcSIProperties (p, TWB, TDB, HumidRatio, RelHumid, Enthalpy, SpVolume, Density, DEWPoint);
			UnitsTemperature = pszSITemp;
			UnitsEnthalpy = pszSIEnthrapy;
			UnitsDensity = pszSIDensity;
			UnitsVolume = pszSISPVolume;
			UnitsHumidity = pszSIHumidityRatio;
		}

		RelHumid = m_dblRelHumidity;
		
		if ((RelHumid < 0) || (Enthalpy < 0) || (SpVolume < 0) || (HumidRatio < 0))
		{
			AfxMessageBox("Thermodynamically impossible combination of RH & DBT", MB_ICONEXCLAMATION);
		}
		else
		{
			int x = 0;
			if (!TPropPageBase::metricflag)
				strTemp.Format("%.4f", calcPressureF(p));
			else
				strTemp.Format("%.4f", p);
			m_wndPsychResults.AddItem(x, 0, "Barometric Pressure");
			m_wndPsychResults.AddItem(x, 1, strTemp);
			if (!TPropPageBase::metricflag)
			{
				m_wndPsychResults.AddItem(x++, 2, L_HG);
			}
			else
			{
				m_wndPsychResults.AddItem(x++, 2, L_KPA);
			}
    
			strTemp.Format("%.2f", m_dblAltitude);
			m_wndPsychResults.AddItem(x, 0, "Altitude above MSL");
			m_wndPsychResults.AddItem(x, 1, strTemp);
			if (!TPropPageBase::metricflag)
			{
				m_wndPsychResults.AddItem(x++, 2, L_FEET);
			}
			else
			{
				m_wndPsychResults.AddItem(x++, 2, L_METERS);
			}
    
			strTemp.Format("%.2f", TDB);
			m_wndPsychResults.AddItem(x, 0, "Dry Bulb Temperature");
			m_wndPsychResults.AddItem(x, 1, strTemp);
			m_wndPsychResults.AddItem(x++, 2, UnitsTemperature);
    
			strTemp.Format("%.2f", TWB);
			m_wndPsychResults.AddItem(x, 0, "Wet Bulb Temperature");
			m_wndPsychResults.AddItem(x, 1, strTemp);
			m_wndPsychResults.AddItem(x++, 2, UnitsTemperature);
    
			strTemp.Format("%.4f", Enthalpy);
			m_wndPsychResults.AddItem(x, 0, "Enthalpy");
			m_wndPsychResults.AddItem(x, 1, strTemp);
			m_wndPsychResults.AddItem(x++, 2, UnitsEnthalpy);
    
			strTemp.Format("%.2f", DEWPoint);
			m_wndPsychResults.AddItem(x, 0, "Dew Point");
			m_wndPsychResults.AddItem(x, 1, strTemp);
			m_wndPsychResults.AddItem(x++, 2, UnitsTemperature);
    
			strTemp.Format("%.2f", RelHumid);
			m_wndPsychResults.AddItem(x, 0, "Relative Humidity");
			m_wndPsychResults.AddItem(x, 1, strTemp);
			m_wndPsychResults.AddItem(x++, 2, pszPercent);
    
			strTemp.Format("%.5f", Density);
			m_wndPsychResults.AddItem(x, 0, "Density");
			m_wndPsychResults.AddItem(x, 1, strTemp);
			m_wndPsychResults.AddItem(x++, 2, UnitsDensity);
    
			strTemp.Format("%.5f", SpVolume);
			m_wndPsychResults.AddItem(x, 0, "Specific Volume");
			m_wndPsychResults.AddItem(x, 1, strTemp);
			m_wndPsychResults.AddItem(x++, 2, UnitsVolume);
    
			strTemp.Format("%.5f", HumidRatio);
			m_wndPsychResults.AddItem(x, 0, "Humidity Ratio");
			m_wndPsychResults.AddItem(x, 1, strTemp);
			m_wndPsychResults.AddItem(x++, 2, UnitsHumidity);

			m_wndPsychResults.SetColumnWidth(0, LVSCW_AUTOSIZE);
			m_wndPsychResults.SetColumnWidth(1, LVSCW_AUTOSIZE);
			m_wndPsychResults.SetColumnWidth(2, LVSCW_AUTOSIZE);
		}
	}
}
//---------------------------------------------------------------------
// END: TPropPageOne::OnPsychButtonRecalc3
//---------------------------------------------------------------------


//---------------------------------------------------------------------
// FUNCTION    : TPropPageOne::OnPsychButtonRecalc4
//
// DESCRIPTION : None.
//
// INPUT       : None.
//
// RETURN (S)  : None.
//---------------------------------------------------------------------
// REMARKS     : None.
//---------------------------------------------------------------------
void TPropPageOne::OnPsychButtonRecalc4() 
{
    if (UpdateData(true))
	{
		if ( TPropPageBase::CheckData() ) return;

		double p;
		double TWB		= m_dblPsychWBT;
		double TDB		= m_dblEnthalpy;
		double HumidRatio;
		double RelHumid;
		double Enthalpy = m_dblEnthalpy;
		double SpVolume	= -999;
		double Density	= -999;
		double DEWPoint	= -999;
		double OutH;
		double IPHR = 0;
		double IPRH = 0;
		CString UnitsTemperature;
		CString UnitsEnthalpy;
		CString UnitsDensity;
		CString UnitsVolume;
		CString UnitsHumidity;
		CString strTemp;

		m_wndPsychResults.DeleteAllItems();
		if (!TPropPageBase::metricflag)
		{
			p = Altitude2PSI(m_dblAltitude);
			IPEnthalpysearch ( 1, p, TDB, OutH, TWB, TDB, IPHR, IPRH, SpVolume, Density, DEWPoint );
			CalcIPProperties (p, TWB, TDB, HumidRatio, RelHumid, Enthalpy, SpVolume, Density, DEWPoint);
			Enthalpy = m_dblEnthalpy;
			UnitsTemperature = pszIPTemp;
			UnitsEnthalpy = pszIPEnthrapy;
			UnitsDensity = pszIPDensity;
			UnitsVolume = pszIPSPVolume;
			UnitsHumidity = pszIPHumidityRatio;
		}
		else
		{
			TWB = 0;
			p =Altitude2KPA(m_dblAltitude);
			SIEnthalpysearch ( 1, p, TDB, OutH, TWB, TDB, IPHR, IPRH, SpVolume, Density, DEWPoint );
			CalcSIProperties (p, TWB, TDB, HumidRatio, RelHumid, Enthalpy, SpVolume, Density, DEWPoint);
			Enthalpy = m_dblEnthalpy;
			UnitsTemperature = pszSITemp;
			UnitsEnthalpy = pszSIEnthrapy;
			UnitsDensity = pszSIDensity;
			UnitsVolume = pszSISPVolume;
			UnitsHumidity = pszSIHumidityRatio;
		}

		if (Enthalpy == 0)
		{
			AfxMessageBox("Enthalpy out of range of program", MB_ICONEXCLAMATION);
		}
		else if ((RelHumid < 0) || (Enthalpy < 0) || (SpVolume < 0) || (HumidRatio < 0) )
		{
			AfxMessageBox("Thermodynamically impossible combination of Enthalpy & Barometric Pressure", MB_ICONEXCLAMATION);
		}
		else
		{
			int x = 0;
			if (!TPropPageBase::metricflag)
				strTemp.Format("%.4f", calcPressureF(p));
			else
				strTemp.Format("%.4f", p);
			m_wndPsychResults.AddItem(x, 0, "Barometric Pressure");
			m_wndPsychResults.AddItem(x, 1, strTemp);
			if (!TPropPageBase::metricflag)
			{
				m_wndPsychResults.AddItem(x++, 2, L_HG);
			}
			else
			{
				m_wndPsychResults.AddItem(x++, 2, L_KPA);
			}
    
			strTemp.Format("%.2f", m_dblAltitude);
			m_wndPsychResults.AddItem(x, 0, "Altitude above MSL");
			m_wndPsychResults.AddItem(x, 1, strTemp);
			if (!TPropPageBase::metricflag)
			{
				m_wndPsychResults.AddItem(x++, 2, L_FEET);
			}
			else
			{
				m_wndPsychResults.AddItem(x++, 2, L_METERS);
			}
    
			strTemp.Format("%.2f", TDB);
			m_wndPsychResults.AddItem(x, 0, "Dry Bulb Temperature");
			m_wndPsychResults.AddItem(x, 1, strTemp);
			m_wndPsychResults.AddItem(x++, 2, UnitsTemperature);
    
			strTemp.Format("%.2f", TWB);
			m_wndPsychResults.AddItem(x, 0, "Wet Bulb Temperature");
			m_wndPsychResults.AddItem(x, 1, strTemp);
			m_wndPsychResults.AddItem(x++, 2, UnitsTemperature);
    
			strTemp.Format("%.4f", Enthalpy);
			m_wndPsychResults.AddItem(x, 0, "Enthalpy");
			m_wndPsychResults.AddItem(x, 1, strTemp);
			m_wndPsychResults.AddItem(x++, 2, UnitsEnthalpy);
    
			strTemp.Format("%.2f", TWB); 
			m_wndPsychResults.AddItem(x, 0, "Dew Point");
			m_wndPsychResults.AddItem(x, 1, strTemp);
			m_wndPsychResults.AddItem(x++, 2, UnitsTemperature);
    
			strTemp.Format("%.2f", RelHumid*100.);
			m_wndPsychResults.AddItem(x, 0, "Relative Humidity");
			m_wndPsychResults.AddItem(x, 1, strTemp);
			m_wndPsychResults.AddItem(x++, 2, pszPercent);
    
			strTemp.Format("%.4f", Density);
			m_wndPsychResults.AddItem(x, 0, "Density");
			m_wndPsychResults.AddItem(x, 1, strTemp);
			m_wndPsychResults.AddItem(x++, 2, UnitsDensity);
    
			strTemp.Format("%.4f", SpVolume);
			m_wndPsychResults.AddItem(x, 0, "Specific Volume");
			m_wndPsychResults.AddItem(x, 1, strTemp);
			m_wndPsychResults.AddItem(x++, 2, UnitsVolume);
    
			strTemp.Format("%.4f", HumidRatio);
			m_wndPsychResults.AddItem(x, 0, "Humidity Ratio");
			m_wndPsychResults.AddItem(x, 1, strTemp);
			m_wndPsychResults.AddItem(x++, 2,UnitsHumidity);

			m_wndPsychResults.SetColumnWidth(0, LVSCW_AUTOSIZE);
			m_wndPsychResults.SetColumnWidth(1, LVSCW_AUTOSIZE);
			m_wndPsychResults.SetColumnWidth(2, LVSCW_AUTOSIZE);
		}
	}
}
//---------------------------------------------------------------------
// END: TPropPageOne::OnPsychButtonRecalc4
//---------------------------------------------------------------------
//---------------------------------------------------------------------
// End: TPropPageOne.cpp
//---------------------------------------------------------------------

