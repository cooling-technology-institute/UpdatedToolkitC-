//---------------------------------------------------------------------
//
// TPropPageZero.cpp : 
//
// Copyright c 1997-1998 Cosmo Engineering  -  All Rights Reserved.
//
// $Archive: /Toolkit/TPropPageZero.cpp $
// $Revision: 1 $
// $Modtime: 6/05/01 11:51p $
//
//---------------------------------------------------------------------
//
// $Log: /Toolkit/TPropPageZero.cpp $
// 
// 1     8/20/01 5:32a Davidl
// 
//---------------------------------------------------------------------
#include "stdafx.h"
#include "CTIToolkit.h"
#include "cdxCSizingDialog.h"
#include "TPropPageZero.h"
#include "TPropPageBase.h"
#include "ctihelp.hh"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//---------------------------------------------------------------------
// TPropPageZero property page
//---------------------------------------------------------------------
IMPLEMENT_DYNCREATE(TPropPageZero, cdxCSizingPropPage)


//---------------------------------------------------------------------
// Message Map
//---------------------------------------------------------------------
BEGIN_MESSAGE_MAP(TPropPageZero, cdxCSizingPropPage)
	//{{AFX_MSG_MAP(TPropPageZero)
	ON_WM_SIZE()
//	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//---------------------------------------------------------------------
// FUNCTION    : TPropPageZero::TPropPageZero
//
// DESCRIPTION : None.
//
// INPUT       : None.
//
// RETURN (S)  : None.
//---------------------------------------------------------------------
// REMARKS     : None.
//---------------------------------------------------------------------
TPropPageZero::TPropPageZero() : cdxCSizingPropPage(TPropPageZero::IDD),
	m_bInit(false)
{
	//{{AFX_DATA_INIT(TPropPageZero)
    m_dblMinWFR	= WATER_FLOW_RATE_MIN_IP;
    m_dblMaxWFR	= WATER_FLOW_RATE_MAX_IP;
	m_dblMinHWT	= HWT_MIN_IP;
	m_dblMaxHWT = HWT_MAX_IP;
	m_dblMinCWT = CWT_MIN_IP;
	m_dblMaxCWT = CWT_MAX_IP;
	m_dblMinWBT = WBT_MIN_IP;
	m_dblMaxWBT = WBT_MAX_IP;
	m_dblMinDBT = DBT_MIN_IP;
	m_dblMaxDBT = DBT_MAX_IP;
	m_dblMinFDP = FAN_DRIVER_POWER_MIN_IP;
	m_dblMaxFDP = FAN_DRIVER_POWER_MAX_IP;
	m_dblMinBP	= BAROMETRIC_PRESSURE_MIN_IP;
	m_dblMaxBP	= BAROMETRIC_PRESSURE_MAX_IP;
    m_dblMinLG	= LG_MIN_IP;
	m_dblMaxLG  = LG_MAX_IP;

    m_strWFR = L_GPM;
    m_strHWT = L_DEGF;
    m_strCWT = L_DEGF;
    m_strWBT = L_DEGF; 
	m_strFDP = L_BHP;
	m_strBP	 = L_HG;
	//}}AFX_DATA_INIT
	SetDefaultValues();		// is the init above necessary?
}
//---------------------------------------------------------------------
// END: TPropPageZero::TPropPageZero
//---------------------------------------------------------------------


//---------------------------------------------------------------------
// FUNCTION    : TPropPageZero::~TPropPageZero
//
// DESCRIPTION : None.
//
// INPUT       : None.
//
// RETURN (S)  : None.
//---------------------------------------------------------------------
// REMARKS     : None.
//---------------------------------------------------------------------
TPropPageZero::~TPropPageZero()
{
}
//---------------------------------------------------------------------
// END: TPropPageZero::~TPropPageZero
//---------------------------------------------------------------------



//---------------------------------------------------------------------
// FUNCTION    : TPropPageZero::SetDefaultValues
//
// DESCRIPTION : None.
//
// INPUT       : None.
//
// RETURN (S)  : None.
//---------------------------------------------------------------------
// REMARKS     : None.
//---------------------------------------------------------------------
void TPropPageZero::SetDefaultValues()
{
    if (!TPropPageBase::metricflag)
    {
        m_dblMinWFR	= WATER_FLOW_RATE_MIN_IP;
    	m_dblMaxWFR	= WATER_FLOW_RATE_MAX_IP;
		m_dblMinHWT	= HWT_MIN_IP;
		m_dblMaxHWT = HWT_MAX_IP;
		m_dblMinCWT = CWT_MIN_IP;
		m_dblMaxCWT = CWT_MAX_IP;
		m_dblMinWBT = WBT_MIN_IP;
		m_dblMaxWBT = WBT_MAX_IP;
		m_dblMinDBT = DBT_MIN_IP;
		m_dblMaxDBT = DBT_MAX_IP;
		m_dblMinFDP = FAN_DRIVER_POWER_MIN_IP;
		m_dblMaxFDP = FAN_DRIVER_POWER_MAX_IP;
		m_dblMinBP	= BAROMETRIC_PRESSURE_MIN_IP;
		m_dblMaxBP	= BAROMETRIC_PRESSURE_MAX_IP;
		m_dblMinLG	= LG_MIN_IP;
		m_dblMaxLG  = LG_MAX_IP;

        m_strWFR = L_GPM;
        m_strHWT = L_DEGF;
        m_strCWT = L_DEGF;
        m_strWBT = L_DEGF;
		m_strDBT = L_DEGF;
		m_strFDP = L_BHP;
		m_strBP  = L_HG;
    }
    else
    {
        m_dblMinWFR	= WATER_FLOW_RATE_MIN_SI;
    	m_dblMaxWFR	= WATER_FLOW_RATE_MAX_SI;
		m_dblMinHWT	= HWT_MIN_SI;
		m_dblMaxHWT = HWT_MAX_SI;
		m_dblMinCWT = CWT_MIN_SI;
		m_dblMaxCWT = CWT_MAX_SI;
		m_dblMinWBT = WBT_MIN_SI;
		m_dblMaxWBT = WBT_MAX_SI;
		m_dblMinDBT = DBT_MIN_SI;
		m_dblMaxDBT = DBT_MAX_SI;
		m_dblMinFDP = FAN_DRIVER_POWER_MIN_SI;
		m_dblMaxFDP = FAN_DRIVER_POWER_MAX_SI;
		m_dblMinBP	= BAROMETRIC_PRESSURE_MIN_SI;
		m_dblMaxBP	= BAROMETRIC_PRESSURE_MAX_SI;
		m_dblMinLG	= LG_MIN_SI;
		m_dblMaxLG  = LG_MAX_SI;

        m_strWFR = L_LPS;
        m_strHWT = L_DEGC;
        m_strCWT = L_DEGC;
        m_strWBT = L_DEGC;
		m_strDBT = L_DEGC;
		m_strFDP = L_KW;
		m_strBP  = L_KPA;
    }
}
//---------------------------------------------------------------------
// END: TPropPageZero::SetDefaultValues
//---------------------------------------------------------------------


//---------------------------------------------------------------------
// FUNCTION    : TPropPageZero::DoDataExchange
//
// DESCRIPTION : None.
//
// INPUT       : CDataExchange* pDX
//
// RETURN (S)  : None.
//---------------------------------------------------------------------
// REMARKS     : None.
//---------------------------------------------------------------------
void TPropPageZero::DoDataExchange(CDataExchange* pDX)
{
	cdxCSizingPropPage::DoDataExchange(pDX);

	// If OnInitDialog hasn't yet been called, we may not yet know
	// about unit changes in other tabs
	if (!m_bInit)
		UpdateUnits();

	//{{AFX_DATA_MAP(TPropPageZero)
	DDX_Text(pDX, IDC_DEMO_STATIC_WFR_UNITS, m_strWFR);
	DDX_Text(pDX, IDC_DEMO_STATIC_HWT_UNITS, m_strHWT);
	DDX_Text(pDX, IDC_DEMO_STATIC_CWT_UNITS, m_strCWT);
	DDX_Text(pDX, IDC_DEMO_STATIC_WBT_UNITS, m_strWBT);
	DDX_Text(pDX, IDC_DEMO_STATIC_DBT_UNITS, m_strDBT);
	DDX_Text(pDX, IDC_DEMO_STATIC_FDP_UNITS, m_strFDP);
	DDX_Text(pDX, IDC_DEMO_STATIC_BP_UNITS,  m_strBP);

    DDX_Text(pDX, IDC_DEMO_STATIC_WFR_MIN, m_dblMinWFR);
    DDX_Text(pDX, IDC_DEMO_STATIC_WFR_MAX, m_dblMaxWFR);
	DDX_Text(pDX, IDC_DEMO_STATIC_HWT_MIN, m_dblMinHWT);
	DDX_Text(pDX, IDC_DEMO_STATIC_HWT_MAX, m_dblMaxHWT);
	DDX_Text(pDX, IDC_DEMO_STATIC_CWT_MIN, m_dblMinCWT);
	DDX_Text(pDX, IDC_DEMO_STATIC_CWT_MAX, m_dblMaxCWT);
	DDX_Text(pDX, IDC_DEMO_STATIC_WBT_MIN, m_dblMinWBT);
	DDX_Text(pDX, IDC_DEMO_STATIC_WBT_MAX, m_dblMaxWBT);
	DDX_Text(pDX, IDC_DEMO_STATIC_DBT_MIN, m_dblMinDBT);
	DDX_Text(pDX, IDC_DEMO_STATIC_DBT_MAX, m_dblMaxDBT);
    DDX_Text(pDX, IDC_DEMO_STATIC_LG_MIN,  m_dblMinLG);
	DDX_Text(pDX, IDC_DEMO_STATIC_LG_MAX,  m_dblMaxLG);
	DDX_Text(pDX, IDC_DEMO_STATIC_FDP_MIN, m_dblMinFDP);
	DDX_Text(pDX, IDC_DEMO_STATIC_FDP_MAX, m_dblMaxFDP);
	DDX_Text(pDX, IDC_DEMO_STATIC_BP_MIN,  m_dblMinBP);
	DDX_Text(pDX, IDC_DEMO_STATIC_BP_MAX,  m_dblMaxBP);

//	DDX_Control(pDX, IDC_DEMO_STATIC_CTI, m_wndCTILink);

	//}}AFX_DATA_MAP


  }
//---------------------------------------------------------------------
// END: TPropPageZero::DoDataExchange
//---------------------------------------------------------------------


//---------------------------------------------------------------------
// FUNCTION    : TPropPageZero::OnInitDialog
//
// DESCRIPTION : None.
//
// INPUT       : None.
//
// RETURN (S)  : BOOL 
//---------------------------------------------------------------------
// REMARKS     : None.
//---------------------------------------------------------------------
BOOL TPropPageZero::OnInitDialog() 
{
	cdxCSizingPropPage::OnInitDialog();
    SetHelpID(Welcome_Help);
//	m_wndCTILink.m_fnSetLink("http://www.cti.org");
	m_bInit = true;
	return FALSE;
}
//---------------------------------------------------------------------
// END: TPropPageZero::OnInitDialog
//---------------------------------------------------------------------


//---------------------------------------------------------------------
// FUNCTION    : TPropPageZero::OnDataChanged
//
// DESCRIPTION : None.
//
// INPUT       : None.
//
// RETURN (S)  : None.
//---------------------------------------------------------------------
// REMARKS     : None.
//---------------------------------------------------------------------
void TPropPageZero::OnDataChanged()
{
}
//---------------------------------------------------------------------
// END: TPropPageZero::OnDataChanged
//---------------------------------------------------------------------


//---------------------------------------------------------------------
// FUNCTION    : TPropPageZero::ChangeUnits
//
// DESCRIPTION : None.
//
// INPUT       : None.
//
// RETURN (S)  : None.
//---------------------------------------------------------------------
// REMARKS     : None.
//---------------------------------------------------------------------
void TPropPageZero::ChangeUnits() 
{
	//---------------------------------------------------------------------
	// Do not retrieve the data because it will be retrived incorrectly
	//---------------------------------------------------------------------
    UpdateUnits();
	if (IsWindow(m_hWnd))
	{
		UpdateData(false);
	}
}
//---------------------------------------------------------------------
// END: TPropPageZero::ChangeUnits
//---------------------------------------------------------------------


//---------------------------------------------------------------------
// FUNCTION    : TPropPageZero::UpdateUnits
//
// DESCRIPTION : None.
//
// INPUT       : None.
//
// RETURN (S)  : None.
//---------------------------------------------------------------------
// REMARKS     : None.
//---------------------------------------------------------------------
void TPropPageZero::UpdateUnits() 
{
    if ((!TPropPageBase::metricflag) && (m_strHWT != L_DEGF))
    {
        m_dblMinWFR	= WATER_FLOW_RATE_MIN_IP;
    	m_dblMaxWFR	= WATER_FLOW_RATE_MAX_IP;
		m_dblMinHWT	= HWT_MIN_IP;
		m_dblMaxHWT = HWT_MAX_IP;
		m_dblMinCWT = CWT_MIN_IP;
		m_dblMaxCWT = CWT_MAX_IP;
		m_dblMinWBT = WBT_MIN_IP;
		m_dblMaxWBT = WBT_MAX_IP;
		m_dblMinDBT = DBT_MIN_IP;
		m_dblMaxDBT = DBT_MAX_IP;
		m_dblMinFDP = FAN_DRIVER_POWER_MIN_IP;
		m_dblMaxFDP = FAN_DRIVER_POWER_MAX_IP;
		m_dblMinBP	= BAROMETRIC_PRESSURE_MIN_IP;
		m_dblMaxBP	= BAROMETRIC_PRESSURE_MAX_IP;
		m_dblMinLG	= LG_MIN_IP;
		m_dblMaxLG  = LG_MAX_IP;
		
 		m_strWFR = L_GPM;
        m_strHWT = L_DEGF;
        m_strCWT = L_DEGF;
        m_strWBT = L_DEGF;
		m_strDBT = L_DEGF;
		m_strFDP = L_BHP;
		m_strBP  = L_HG;
    }
    else if ((TPropPageBase::metricflag) && (m_strHWT != L_DEGC))
    {
        m_dblMinWFR	= WATER_FLOW_RATE_MIN_SI;
    	m_dblMaxWFR	= WATER_FLOW_RATE_MAX_SI;
		m_dblMinHWT	= HWT_MIN_SI;
		m_dblMaxHWT = HWT_MAX_SI;
		m_dblMinCWT = CWT_MIN_SI;
		m_dblMaxCWT = CWT_MAX_SI;
		m_dblMinWBT = WBT_MIN_SI;
		m_dblMaxWBT = WBT_MAX_SI;
		m_dblMinDBT = DBT_MIN_SI;
		m_dblMaxDBT = DBT_MAX_SI;
		m_dblMinLG	= LG_MIN_SI;
		m_dblMaxLG  = LG_MAX_SI;
		m_dblMinFDP = FAN_DRIVER_POWER_MIN_SI;
		m_dblMaxFDP = FAN_DRIVER_POWER_MAX_SI;
		m_dblMinBP	= BAROMETRIC_PRESSURE_MIN_SI;
		m_dblMaxBP	= BAROMETRIC_PRESSURE_MAX_SI;

        m_strWFR = L_LPS;
        m_strHWT = L_DEGC;
        m_strCWT = L_DEGC;
		m_strWBT = L_DEGC;
		m_strDBT = L_DEGC;
		m_strFDP = L_KW;
		m_strBP  = L_KPA;
    }

}
//---------------------------------------------------------------------
// END: TPropPageZero::UpdateUnits
//---------------------------------------------------------------------


//---------------------------------------------------------------------
// FUNCTION    : TPropPageZero::OnSize
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
void TPropPageZero::OnSize(UINT nType, int cx, int cy) 
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
// END: TPropPageZero::OnSize
//---------------------------------------------------------------------
/***
HBRUSH TPropPageZero::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if (pWnd->m_hWnd == m_wndCTILink.m_hWnd)
	{
		pDC->SetTextColor(0xFF0000);
	}
	
	return hbr;
}
***/
//---------------------------------------------------------------------
// End: TPropPageZero.cpp
//---------------------------------------------------------------------

