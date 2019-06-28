//---------------------------------------------------------------------
//
// TPropPageTwo.cpp : 
//
// Copyright c 1997-1998 Cosmo Engineering  -  All Rights Reserved.
//
// $Archive: /Toolkit/TPropPageTwo.cpp $
// $Revision: 4 $
// $Modtime: 6/19/01 12:57a $
//
//---------------------------------------------------------------------
//
// $Log: /Toolkit/TPropPageTwo.cpp $
// 
// 4     7/04/01 4:13a Davidl
// Changes for Demo version; also, small changes dealing with unit
// conversion problems.
// 
// 3     5/03/01 5:22a Davidl
// Changed minmax values to constants.
// 
// 2     4/19/01 6:20a Davidl
// Changed handling of units change, especially on init.
// 
// 1     11/06/00 9:46a Deanp
// Initial 2.0 source
// 
// 34    2/25/99 8:34a Deanp
// removed menu enables from dodataexchange
// 
// 33    2/25/99 7:38a Deanp
// fixed change units function to verify the window exsits
// 
// 32    2/25/99 6:50a Deanp
// change so all altitude inputs are toggled between altitude and
// barometric pressure
// 
//---------------------------------------------------------------------
#include "stdafx.h"
#include "CTIToolkit.h"
#include "cdxCSizingDialog.h"
#include "TPropPageTwo.h"
#include "ctihelp.hh"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//---------------------------------------------------------------------
// TPropPageTwo property page
//---------------------------------------------------------------------
IMPLEMENT_DYNCREATE(TPropPageTwo, cdxCSizingPropPage)


//---------------------------------------------------------------------
// Message Map
//---------------------------------------------------------------------
BEGIN_MESSAGE_MAP(TPropPageTwo, cdxCSizingPropPage)
	//{{AFX_MSG_MAP(TPropPageTwo)
	ON_WM_SIZE()
	ON_CONTROL(EN_CHANGE, IDC_MERKEL_EDIT_T1, OnDataChanged)
    ON_BN_CLICKED(IDC_MERKEL_BUTTON_RECALC, OnMerkelButtonRecalc)
	ON_CONTROL(EN_CHANGE, IDC_MERKEL_EDIT_T2, OnDataChanged)
	ON_CONTROL(EN_CHANGE, IDC_MERKEL_EDIT_WBT, OnDataChanged)
	ON_CONTROL(EN_CHANGE, IDC_MERKEL_EDIT_ALTITUDE, OnDataChanged)
	ON_CONTROL(EN_CHANGE, IDC_MERKEL_EDIT_LG, OnDataChanged)
	ON_BN_CLICKED(IDC_BUTTON_ALTITUDE, OnButtonAltitude)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//---------------------------------------------------------------------
// FUNCTION    : TPropPageTwo::TPropPageTwo
//
// DESCRIPTION : None.
//
// INPUT       : None.
//
// RETURN (S)  : None.
//---------------------------------------------------------------------
// REMARKS     : None.
//---------------------------------------------------------------------
TPropPageTwo::TPropPageTwo() : cdxCSizingPropPage(TPropPageTwo::IDD),
	m_bInit(false), m_bAltitude(true)
{
	//{{AFX_DATA_INIT(TPropPageTwo)
    m_dblMerkelT1		= 110.;
    m_dblMerkelT2		= 84.;
    m_dblMerkelLG		= 1.3;
    m_dblMerkelWBT		= 80;
    m_dblMerkelAltitude	= 0;
    m_dblMerkelResults	= 0;
    m_strT1 = L_DEGF;
    m_strT2 = L_DEGF;
    m_strWBT= L_DEGF;
    m_strAlt= L_FEET; 
	//}}AFX_DATA_INIT
	SetDefaultValues();		// is the init above necessary?
}
//---------------------------------------------------------------------
// END: TPropPageTwo::TPropPageTwo
//---------------------------------------------------------------------


//---------------------------------------------------------------------
// FUNCTION    : TPropPageTwo::~TPropPageTwo
//
// DESCRIPTION : None.
//
// INPUT       : None.
//
// RETURN (S)  : None.
//---------------------------------------------------------------------
// REMARKS     : None.
//---------------------------------------------------------------------
TPropPageTwo::~TPropPageTwo()
{
}
//---------------------------------------------------------------------
// END: TPropPageTwo::~TPropPageTwo
//---------------------------------------------------------------------



//---------------------------------------------------------------------
// FUNCTION    : TPropPageTwo::SetDefaultValues
//
// DESCRIPTION : None.
//
// INPUT       : None.
//
// RETURN (S)  : None.
//---------------------------------------------------------------------
// REMARKS     : None.
//---------------------------------------------------------------------
void TPropPageTwo::SetDefaultValues()
{
    if (!TPropPageBase::metricflag)
    {
        m_dblMerkelT1		= 110;
    	m_dblMerkelT2		= 84;
    	m_dblMerkelLG		= 1.3;
        m_dblMerkelWBT		= 80;
        m_dblMerkelAltitude = 0;
        m_dblMerkelResults	= 0;
        m_strT1 = L_DEGF;
        m_strT2 = L_DEGF;
        m_strWBT= L_DEGF;
        m_strAlt= L_FEET;
    }
    else
    {
        m_dblMerkelT1		= 43.33;
    	m_dblMerkelT2		= 28.88;
    	m_dblMerkelLG		= 1.3;
        m_dblMerkelWBT		= 26.667;
        m_dblMerkelAltitude	= 0;
        m_dblMerkelResults	= 0;
        m_strT1 = L_DEGC;
        m_strT2 = L_DEGC;
        m_strWBT= L_DEGC;
        m_strAlt= L_METERS;
    }
	m_bAltitude = true;
	if (TPropPageBase::metricflag)
	{
	    m_strAlt= L_METERS;
	}
	else
	{
		m_strAlt= L_FEET;
	}
	m_strAltitudeLable.LoadString(IDS_ALTITUDE); 
}
//---------------------------------------------------------------------
// END: TPropPageTwo::SetDefaultValues
//---------------------------------------------------------------------


//---------------------------------------------------------------------
// FUNCTION    : TPropPageTwo::OnButtonAltitude
//
// DESCRIPTION : None.
//
// INPUT       : None.
//
// RETURN (S)  : None.
//---------------------------------------------------------------------
// REMARKS     : None.
//---------------------------------------------------------------------
void TPropPageTwo::OnButtonAltitude() 
{
	UpdateData(true);

	m_bAltitude = !m_bAltitude;
	if (m_bAltitude)
	{
	    if (TPropPageBase::metricflag)
		{
	        m_strAlt= L_METERS;
		}
		else
		{
			m_strAlt= L_FEET;
		}
		m_strAltitudeLable.LoadString(IDS_ALTITUDE); 
#ifdef _DEMO_VERSION    
		GetDlgItem(IDC_MERKEL_EDIT_ALTITUDE)->EnableWindow(false);
#endif
	}
	else
	{
	    if (TPropPageBase::metricflag)
		{
	        m_strAlt= L_KPA;
		}
		else
		{
			m_strAlt= L_HG;
		}
		m_strAltitudeLable.LoadString(IDS_BAROMETRIC_PRESSURE); 
#ifdef _DEMO_VERSION    
		GetDlgItem(IDC_MERKEL_EDIT_ALTITUDE)->EnableWindow(true);
#endif
	}

	UpdateData(false);
}
//---------------------------------------------------------------------
// END: TPropPageTwo::OnButtonAltitude
//---------------------------------------------------------------------


//---------------------------------------------------------------------
// FUNCTION    : TPropPageTwo::DoDataExchange
//
// DESCRIPTION : None.
//
// INPUT       : CDataExchange* pDX
//
// RETURN (S)  : None.
//---------------------------------------------------------------------
// REMARKS     : None.
//---------------------------------------------------------------------
void TPropPageTwo::DoDataExchange(CDataExchange* pDX)
{
	cdxCSizingPropPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(TPropPageTwo)
	DDX_Text(pDX, IDC_MERKEL_STATIC_T1_UNITS, m_strT1);
	DDX_Text(pDX, IDC_MERKEL_STATIC_T2_UNITS, m_strT2);
	DDX_Text(pDX, IDC_MERKEL_STATIC_WBT_UNITS, m_strWBT);
	DDX_Text(pDX, IDC_MERKEL_STATIC_ALTITUDE, m_strAlt);
	//}}AFX_DATA_MAP

	// If OnInitDialog hasn't yet been called, we may not yet know
	// about unit changes in other tabs
	if (!m_bInit)
		UpdateUnits();

    DDX_Text(pDX, IDC_MERKEL_EDIT_LG, m_dblMerkelLG);
    DDV_MinMaxDouble(pDX,m_dblMerkelLG,0.1,10.0,0.1,10.0,2);
    DDX_Text(pDX, IDC_MERKEL_EDIT_T1, m_dblMerkelT1);
    DDV_MinMaxDouble(pDX,m_dblMerkelT1,32,200,0,93,2);
    DDX_Text(pDX, IDC_MERKEL_EDIT_T2, m_dblMerkelT2);
    DDV_MinMaxDouble(pDX,m_dblMerkelT2,32,200,0,93,2);
    DDX_Text(pDX, IDC_MERKEL_EDIT_WBT, m_dblMerkelWBT);
    DDV_MinMaxDouble(pDX,m_dblMerkelWBT,WBT_MIN_IP,WBT_MAX_IP,
		WBT_MIN_SI,WBT_MAX_SI,2);

	//---------------------------------------------------------------------
	// Get and Validate Altitude or Barometric Pressure.
	//---------------------------------------------------------------------
	if (m_bAltitude)
	{
		DDX_Text(pDX, IDC_MERKEL_EDIT_ALTITUDE, m_dblMerkelAltitude);
//		DDV_MinMaxDouble(pDX,m_dblMerkelAltitude,-500,40000,-150,12200,4);
		DDV_MinMaxDouble(pDX,m_dblMerkelAltitude, ALT_MIN_IP, ALT_MAX_IP,
            ALT_MIN_SI, ALT_MAX_SI, 4);
	}
	else
	{
		double dblPressure;
		if (TPropPageBase::metricflag)
		{
			dblPressure = Altitude2KPA(m_dblMerkelAltitude);
		}
		else
		{
			dblPressure = calcPressureF(Altitude2PSI(m_dblMerkelAltitude));
		}
		dblPressure = truncit(dblPressure, 5);
		DDX_Text(pDX, IDC_MERKEL_EDIT_ALTITUDE, dblPressure);
		DDV_MinMaxDouble(pDX,dblPressure,BAROMETRIC_PRESSURE_MIN_IP,
			BAROMETRIC_PRESSURE_MAX_IP,BAROMETRIC_PRESSURE_MIN_SI,
			BAROMETRIC_PRESSURE_MAX_SI,3);
		if (TPropPageBase::metricflag)
		{
			m_dblMerkelAltitude = KPA2Altitude(dblPressure);
		}
		else
		{
			m_dblMerkelAltitude = PSI2Altitude(calcPressureC(dblPressure));
		}
		m_dblMerkelAltitude = truncit(m_dblMerkelAltitude, 4);
	}

	//---------------------------------------------------------------------
	// Additional Validation conditions
	//---------------------------------------------------------------------
    DDX_Text(pDX, IDC_MERKEL_EDIT_T2, m_dblMerkelT2);
	if (m_dblMerkelT2 >= m_dblMerkelT1)
	{
		AfxMessageBox("T2 cannot be greater than or equal to T1.", MB_ICONEXCLAMATION);
		pDX->Fail();
	}
	if (m_dblMerkelT2 <= m_dblMerkelWBT)
	{
		AfxMessageBox("T2 cannot be less than or equal to Wet Bulb.", MB_ICONEXCLAMATION);
		pDX->Fail();
	}

	
	//---------------------------------------------------------------------
	// Merkel Result value
	//---------------------------------------------------------------------
	if (!pDX->m_bSaveAndValidate && (m_dblMerkelResults > 998))
	{
		CString strError("ERROR!");
	    DDX_Text(pDX, IDC_MERKEL_EDIT_RESULTS, strError);
	}
	else if (!pDX->m_bSaveAndValidate)
	{
		DDX_Text(pDX, IDC_MERKEL_EDIT_RESULTS, m_dblMerkelResults);
	}


	GetDlgItem(IDC_BUTTON_ALTITUDE)->SetWindowText(m_strAltitudeLable);
}
//---------------------------------------------------------------------
// END: TPropPageTwo::DoDataExchange
//---------------------------------------------------------------------


//---------------------------------------------------------------------
// FUNCTION    : TPropPageTwo::OnMerkelButtonRecalc
//
// DESCRIPTION : None.
//
// INPUT       : None.
//
// RETURN (S)  : None.
//---------------------------------------------------------------------
// REMARKS     : None.
//---------------------------------------------------------------------
void TPropPageTwo::OnMerkelButtonRecalc() 
{
	try
	{
		if (UpdateData(true))
		{
			double WBT          = m_dblMerkelWBT;
			double LG           = m_dblMerkelLG;
			double dblrange     = m_dblMerkelT1-m_dblMerkelT2;
			double approach     = m_dblMerkelT2-m_dblMerkelWBT;
			double altitude     = m_dblMerkelAltitude;
			double T1			= m_dblMerkelT1;
			double T2			= m_dblMerkelT2;
    
			//---------------------------------------------------------------------
			// Convert to IP units for calculation
			//---------------------------------------------------------------------
			if (TPropPageBase::metricflag)
			{
				WBT = fnCelcToFar(WBT);
				T1  = fnCelcToFar(T1);
				T2  = fnCelcToFar(T2);
				dblrange = T1 - T2;
				approach = T2 - WBT;
				altitude = fnMetersToFeet(altitude);
			}

			double dblMerkel = Merkel(WBT, dblrange, approach, LG, altitude);
			m_dblMerkelResults = truncit(dblMerkel, 5);
			UpdateData(false);
		}
	}
	catch(...)
	{
		MessageBox("Error in calculation.  Please check your input values.", "Calculation Error", MB_OK | MB_ICONEXCLAMATION);
	}
}
//---------------------------------------------------------------------
// END: TPropPageTwo::OnMerkelButtonRecalc
//---------------------------------------------------------------------




//---------------------------------------------------------------------
// FUNCTION    : TPropPageTwo::OnInitDialog
//
// DESCRIPTION : None.
//
// INPUT       : None.
//
// RETURN (S)  : BOOL 
//---------------------------------------------------------------------
// REMARKS     : None.
//---------------------------------------------------------------------
BOOL TPropPageTwo::OnInitDialog() 
{
	cdxCSizingPropPage::OnInitDialog();
#ifdef _DEMO_VERSION    
	if (m_bAltitude)
		GetDlgItem(IDC_MERKEL_EDIT_ALTITUDE)->EnableWindow(false);
#endif
    SetHelpID(Merkel_Help);
	UpdateUnits();
    OnMerkelButtonRecalc(); 
	GetDlgItem(IDC_MERKEL_STATIC_T1_UNITS)->SetFocus();
	m_bInit = true;
	return FALSE;
}
//---------------------------------------------------------------------
// END: TPropPageTwo::OnInitDialog
//---------------------------------------------------------------------


//---------------------------------------------------------------------
// FUNCTION    : TPropPageTwo::OnDataChanged
//
// DESCRIPTION : None.
//
// INPUT       : None.
//
// RETURN (S)  : None.
//---------------------------------------------------------------------
// REMARKS     : None.
//---------------------------------------------------------------------
void TPropPageTwo::OnDataChanged()
{
	//---------------------------------------------------------------------
	// Clear the results
	//---------------------------------------------------------------------
	GetDlgItem(IDC_MERKEL_EDIT_RESULTS)->SetWindowText("");
}
//---------------------------------------------------------------------
// END: TPropPageTwo::OnDataChanged
//---------------------------------------------------------------------


//---------------------------------------------------------------------
// FUNCTION    : TPropPageTwo::ChangeUnits
//
// DESCRIPTION : None.
//
// INPUT       : None.
//
// RETURN (S)  : None.
//---------------------------------------------------------------------
// REMARKS     : None.
//---------------------------------------------------------------------
void TPropPageTwo::ChangeUnits() 
{
	//---------------------------------------------------------------------
	// Do not retrieve the data because it will be retrived incorrectly
	//---------------------------------------------------------------------
    UpdateUnits();
	if (IsWindow(m_hWnd))
	{
		UpdateData(false);
		OnMerkelButtonRecalc();
	}
}
//---------------------------------------------------------------------
// END: TPropPageTwo::ChangeUnits
//---------------------------------------------------------------------


//---------------------------------------------------------------------
// FUNCTION    : TPropPageTwo::UpdateUnits
//
// DESCRIPTION : None.
//
// INPUT       : None.
//
// RETURN (S)  : None.
//---------------------------------------------------------------------
// REMARKS     : None.
//---------------------------------------------------------------------
void TPropPageTwo::UpdateUnits() 
{
    if ((!TPropPageBase::metricflag) && (m_strT1 != L_DEGF))
    {
		// Avoid problems with conversion at limits:
		if (m_dblMerkelWBT == WBT_MAX_SI)
			m_dblMerkelWBT = WBT_MAX_IP;
		else if (m_dblMerkelWBT == WBT_MIN_SI)
			m_dblMerkelWBT = WBT_MIN_IP;
		else
			m_dblMerkelWBT = truncit(fnCelcToFar(m_dblMerkelWBT),2);

	    m_dblMerkelT1  = truncit(fnCelcToFar(m_dblMerkelT1),2);
	    m_dblMerkelT2  = truncit(fnCelcToFar(m_dblMerkelT2),2);
        m_dblMerkelAltitude = truncit(fnMetersToFeet(m_dblMerkelAltitude), 4);
        m_strT1 = L_DEGF;
        m_strT2 = L_DEGF;
        m_strWBT= L_DEGF;
    }
    else if ((TPropPageBase::metricflag) && (m_strT1 != L_DEGC))
    {
		if (m_dblMerkelWBT == WBT_MAX_IP)
			m_dblMerkelWBT = WBT_MAX_SI;
		else if (m_dblMerkelWBT == WBT_MIN_IP)
			m_dblMerkelWBT = WBT_MIN_SI;
		else
			m_dblMerkelWBT = truncit(fnFarToCelc(m_dblMerkelWBT),2);

	    m_dblMerkelT1  = truncit(fnFarToCelc(m_dblMerkelT1),2);
	    m_dblMerkelT2  = truncit(fnFarToCelc(m_dblMerkelT2),2);
        m_dblMerkelAltitude = truncit(fnFeetToMeters(m_dblMerkelAltitude), 4);
        m_strT1 = L_DEGC;
        m_strT2 = L_DEGC;
        m_strWBT= L_DEGC;
    }

	if (m_bAltitude)
	{
	    if (TPropPageBase::metricflag)
		{
	        m_strAlt= L_METERS;
		}
		else
		{
			m_strAlt= L_FEET;
		}
	}
	else
	{
	    if (TPropPageBase::metricflag)
		{
	        m_strAlt= L_KPA;
		}
		else
		{
			m_strAlt= L_HG;
		}
	}
}
//---------------------------------------------------------------------
// END: TPropPageTwo::UpdateUnits
//---------------------------------------------------------------------


//---------------------------------------------------------------------
// FUNCTION    : TPropPageTwo::OnSize
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
void TPropPageTwo::OnSize(UINT nType, int cx, int cy) 
{
	cdxCSizingPropPage::OnSize(nType, cx, cy);	

	//---------------------------------------------------------------------
	// Flicker reduction
	//---------------------------------------------------------------------
    long wStyle = GetWindowLong( m_hWnd, GWL_STYLE );
	wStyle ^= WS_CLIPCHILDREN;
    SetWindowLong(m_hWnd,GWL_STYLE, wStyle);
	InvalidateRect(NULL);
	wStyle |= WS_CLIPCHILDREN;
    SetWindowLong(m_hWnd,GWL_STYLE, wStyle);
}
//---------------------------------------------------------------------
// END: TPropPageTwo::OnSize
//---------------------------------------------------------------------
//---------------------------------------------------------------------
// End: TPropPageTwo.cpp
//---------------------------------------------------------------------

