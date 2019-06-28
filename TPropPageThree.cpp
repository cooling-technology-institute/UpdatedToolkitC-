//---------------------------------------------------------------------
//
// TPropPageThree.cpp : 
//
// Copyright c 1997-2017 Cooling Technology Institute Software Engineering  -  All Rights Reserved.
//
// $Archive: /Toolkit/TPropPageThree.cpp $
// $Revision: 6 $
// $Modtime: 6/19/01 1:02a $
//
//---------------------------------------------------------------------
//
// $Log: /Toolkit/TPropPageThree.cpp $
// 
// 6     7/04/01 4:12a Davidl
// Changes for Demo version; also, fix bug with unit conversion before
// window is initialized.
// 
// 5     5/28/01 3:54a Davidl
// Changes for variable app name.
// 
// 4     5/03/01 5:22a Davidl
// Changed minmax values to constants.
// 
// 3     4/19/01 6:19a Davidl
// Fixed message box caption.
// 
// 2     4/12/01 5:24a Davidl
// Name change (to "CTI Toolkit"), and change of icon from exclamation
// point to info bubble when right-click on graph.
// 
// 1     11/06/00 9:46a Deanp
// Initial 2.0 source
// 
// 69    5/12/99 9:07a Administrator
// fixes for version 1.0.0.3 to fix IP/SI problems
// 
// 68    5/05/99 7:32a Administrator
// fixed range values for SI
// 
// 67    4/29/99 11:43a Administrator
// fixed units on curve for SI calculations
// 
// 66    3/01/99 9:33a Deanp
// added validation of characteristic line end pints
// 
// 65    2/25/99 8:34a Deanp
// removed menu enables from dodataexchange
// 
// 64    2/25/99 6:50a Deanp
// change so all altitude inputs are toggled between altitude and
// barometric pressure
// 
// 63    2/19/99 11:41a Deanp
// flicker problem fix caused a size redraw problem.  Resize message now
// has additional functionallity
// 
// 62    2/19/99 10:50a Deanp
// removed prp base variables
// 
// 61    2/19/99 9:21a Deanp
// handle the OCX not being registered.
// 
// 60    2/18/99 12:39p Deanp
// new logic for init of demand curves
// 
// 59    2/18/99 11:59a Deanp
// calculate what approach curves to use
// 
// 58    2/18/99 10:40a Deanp
// increments changed on y axis
// 
// 57    2/18/99 10:32a Deanp
// Reset the KaV/L on demand curve to 5
// 
// 56    2/17/99 11:03a Deanp
// removed extra set increment calls
// 
// 55    2/17/99 10:54a Deanp
// draw lines to 10 kavl 
// 
// 54    2/17/99 10:06a Deanp
// new input validation from rich
// 
// 53    2/15/99 6:33a Deanp
// Changes to print out and cleanup of code
// 
//---------------------------------------------------------------------

//---------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------
#include "stdafx.h"
#include "CTIToolkit.h"
#include "cdxCSizingDialog.h"
#include "TPropPageThree.h"
#include "TeeChartDefines.h"
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
#include "axislabels.h"
#include "chartfont.h"
#include "strings.h"
#include "printer.h"
#include "legend.h"
#include "TPropPageBase.h"
#include "PrintView.h"
#include "PrintFrame.h"
#include "CError.h"
#include "TPrint.h"
#include "Walls.h"
#include "Gradient.h"
#include "TMultiEdit.h"
#include "ValueList.h"
#include "ctihelp.hh"
#include "TNewDemandCurveDataDlg.h"

//---------------------------------------------------------------------
// Debug helpers
//---------------------------------------------------------------------
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//#define _NEW_GRAPH_LIMITS   // test for changing LG max to 20 (from 5)

//---------------------------------------------------------------------
// External info
//---------------------------------------------------------------------
extern CPrintFrame	wndPrintFrame;
extern CPrintView	wndPrintView;


//---------------------------------------------------------------------
// TPropPageThree property page
//---------------------------------------------------------------------
IMPLEMENT_DYNCREATE(TPropPageThree, cdxCSizingPropPage)
BEGIN_MESSAGE_MAP(TPropPageThree, cdxCSizingPropPage)
	//{{AFX_MSG_MAP(TPropPageThree)
	ON_WM_SIZE()
	ON_WM_VSCROLL()
    ON_COMMAND(IDM_MULTIEDIT, OnMultiEdit)
	ON_CONTROL(EN_CHANGE, IDC_CURVE_EDIT_WBT, OnDataChanged)
	ON_BN_CLICKED(IDC_BUTTON_KAVL_APPROACH, OnButtonKavlApproach)
    ON_BN_CLICKED(IDC_CURVE_BUTTON_RECALC, OnCurveButtonRecalc)
	ON_COMMAND(ID_FILE_PRINT, OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_ALTITUDE, OnButtonAltitude)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnButtonSave)
	ON_CBN_SELCHANGE(IDC_COMBO_DATA_FILES, OnSelchangeComboDataFiles)
	ON_WM_LBUTTONUP()
	ON_CONTROL(EN_CHANGE, IDC_CURVE_EDIT_RANGE, OnDataChanged)
	ON_CONTROL(EN_CHANGE, IDC_CURVE_EDIT_ALTITUDE, OnDataChanged)
	ON_CONTROL(EN_CHANGE, IDC_EDIT_CURVE_C1, OnDataChanged)
	ON_CONTROL(EN_CHANGE, IDC_EDIT_CURVE_C2, OnDataChanged)
	ON_CONTROL(EN_CHANGE, IDC_EDIT_MIN, OnDataChanged)
	ON_CONTROL(EN_CHANGE, IDC_EDIT_MAX, OnDataChanged)
	ON_CONTROL(EN_CHANGE, IDC_EDIT_CURVE_LG, OnDataChanged)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, OnFilePrint)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(TPropPageThree, cdxCSizingPropPage)
    //{{AFX_EVENTSINK_MAP(TPropPageThree)
	ON_EVENT(TPropPageThree, IDC_TCHART_GRAPH, 21 /* OnMouseUp */, OnOnMouseUpTchartGraph, VTS_I4 VTS_I4 VTS_I4 VTS_I4)
    ON_EVENT(TPropPageThree, IDC_TCHART_GRAPH,  8 /* OnDblClick */, OnOnDblClickTchartGraph, VTS_NONE)
    //}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()


//---------------------------------------------------------------------
// FUNCTION    : TPropPageThree::TPropPageThree
//
// DESCRIPTION : Constructor.
//
// INPUT       : None.
//
// RETURN (S)  : None.
//---------------------------------------------------------------------
// REMARKS     : None.
//---------------------------------------------------------------------
TPropPageThree::TPropPageThree() : cdxCSizingPropPage(TPropPageThree::IDD)
{
	m_bDataChanged			= false;
	m_strDataName			= "";

	m_bAltitude				= true;
	m_strAltitudeLable.LoadString(IDS_ALTITUDE); 
	m_bShowUserApproach		= false;
	m_dblTargetApproach		= 0.0;
	m_dblUserApproach		= 0.0;
	//{{AFX_DATA_INIT(TPropPageThree)
	m_dblKavl = 0.0;
	m_dblLg = 1.0;
	m_dblCurveMax = 0.0;
	m_dblCurveMin = 0.0;
	m_dblAltitude = 0.0;
	m_strAltitude = _T("");
	m_strRange = _T("");
	m_strWBT = _T("");
	//}}AFX_DATA_INIT
    SetDefaultValues();
	memset(m_listApp, 0, sizeof(m_listApp));
	m_bNotified = false;
}
//---------------------------------------------------------------------
// END: TPropPageThree::TPropPageThree
//---------------------------------------------------------------------


//---------------------------------------------------------------------
// FUNCTION    : TPropPageThree::~TPropPageThree
//
// DESCRIPTION : None.
//
// INPUT       : None.
//
// RETURN (S)  : None.
//---------------------------------------------------------------------
// REMARKS     : None.
//---------------------------------------------------------------------
TPropPageThree::~TPropPageThree()
{
}
//---------------------------------------------------------------------
// END: TPropPageThree::~TPropPageThree
//---------------------------------------------------------------------

//---------------------------------------------------------------------
// FUNCTION    : TPropPageThree::OnInitDialog
//
// DESCRIPTION : None.
//
// INPUT       : None.
//
// RETURN (S)  : BOOL 
//---------------------------------------------------------------------
// REMARKS     : None.
//---------------------------------------------------------------------
BOOL TPropPageThree::OnInitDialog() 
{
	cdxCSizingPropPage::OnInitDialog();

#ifdef _DEMO_VERSION    
	GetDlgItem(IDC_EDIT_CURVE_C1)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_CURVE_C2)->EnableWindow(false);
	if (m_bAltitude)
		GetDlgItem(IDC_CURVE_EDIT_ALTITUDE)->EnableWindow(false);
#endif

	SetHelpID(Demand_Curve_Help);
	AddSzControl( m_wndGraph,  cdxCSizingDialog::mdResize,cdxCSizingDialog::mdResize); 

	if (m_wndGraph.GetHeader().GetText().Count() > 0)
	{
		m_wndGraph.GetHeader().GetText().Remove(0);
	}
	m_wndGraph.GetPanel().SetColor(0xFFFFFF);

    CSpinButtonCtrl* pSpin = (CSpinButtonCtrl*) GetDlgItem(IDC_EDIT_MIN);
    m_SpinMin.SetBuddy( (CWnd*)pSpin );
    pSpin = (CSpinButtonCtrl*) GetDlgItem(IDC_EDIT_MAX);
    m_SpinMax.SetBuddy( (CWnd*)pSpin );

	UpdateUnits();
	UpdateData(false);
    OnCurveButtonRecalc();
	m_wndGraph.GetAxis().GetLeft().SetIncrement(.75);
	m_wndGraph.GetAxis().GetBottom().SetIncrement(.5);

	//
	// Get list of data Files
	//
	CString strPath;

	GetModuleFileName(NULL, strPath.GetBuffer(MAX_PATH+1), MAX_PATH);
	strPath.ReleaseBuffer();
	strPath = strPath.Left(strPath.ReverseFind('\\')+1);
	strPath += "*.bbd";

	WIN32_FIND_DATA fd;
	HANDLE handle = FindFirstFile(strPath, &fd);

	if (handle != INVALID_HANDLE_VALUE) 
	{
		BOOL bFound(true);
		while (bFound)
		{
			m_wndDataFileList.AddString(fd.cFileName);
			bFound = FindNextFile(handle, &fd);
		}
		FindClose(handle);
	}
	m_wndDataFileList.InsertString(0, "New Data File");

//	UpdateData(false);
	
	return TRUE;
}
//---------------------------------------------------------------------
// END: TPropPageThree::OnInitDialog
//---------------------------------------------------------------------


//---------------------------------------------------------------------
// FUNCTION    : TPropPageThree::OnDataChanged
//
// DESCRIPTION : Remove data from graph if data changes.
//
// INPUT       : None.
//
// RETURN (S)  : None.
//---------------------------------------------------------------------
// REMARKS     : None.
//---------------------------------------------------------------------
void TPropPageThree::OnDataChanged()
{
	m_bDataChanged		= true;
	m_dblKavl			= 0; // clear value
	m_dblTargetApproach = 0; // clear value
	GetDlgItem(IDC_EDIT_CURVE_KAVL)->SetWindowText("");
	m_wndGraph.RemoveAllSeries();
}
//---------------------------------------------------------------------
// END: TPropPageThree::OnDataChanged
//---------------------------------------------------------------------


//---------------------------------------------------------------------
// FUNCTION    : TPropPageThree::SetDefaultValues
//
// DESCRIPTION : Set page defaults.
//
// INPUT       : None.
//
// RETURN (S)  : None.
//---------------------------------------------------------------------
// REMARKS     : None.
//---------------------------------------------------------------------
void TPropPageThree::SetDefaultValues()
{
	//---------------------------------------------------------------------
	// Set Default input values
	//---------------------------------------------------------------------
#ifdef _DEMO_VERSION
	m_dblCurveC1	= 2.0;
	m_dblCurveC2	= (-0.75);
#else
	m_dblCurveC1	= 0;
	m_dblCurveC2	= 0;
#endif
	m_dblLg			= 1.0;
	m_dblAltitude	= 0;
	m_dblKavl		= 0;
	m_dblCurveMin	= 0.5;
	m_dblCurveMax	= 2.5;
	m_dblAltitude	= 0;
	if (TPropPageBase::metricflag)
	{
		m_dblCurveWBT	= 26.667;
		m_dblCurveRange = 18;
	}
	else
	{
		m_dblCurveWBT	= 80;
		m_dblCurveRange = 10;
	}

	//---------------------------------------------------------------------
	// Set unit lables
	//---------------------------------------------------------------------
	if (TPropPageBase::metricflag)
	{
		m_strWBT = L_DEGC;
		m_strRange = L_DEGK;
		m_strAltitude = L_METERS;
	}
	else
	{
		m_strWBT = L_DEGF;
		m_strRange = L_DEGF;
		m_strAltitude = L_FEET;
	}

	//---------------------------------------------------------------------
	// Curves are all initially on
	//---------------------------------------------------------------------
	coef = true;
	Lg = true;
	m_bShowKaVLLine = true;
	bShowApproach[0] = true;
	bShowApproach[1] = true;
	bShowApproach[2] = true;
	bShowApproach[3] = true;
	bShowApproach[4] = true;
	bShowApproach[5] = true;
	bShowApproach[6] = true;
	bShowApproach[7] = true;
	bShowApproach[8] = true;
	bShowApproach[9] = true;
	bShowApproach[10]= true;
	bShowApproach[11]= true;
	bShowApproach[12]= true;
	bShowApproach[13]= true;
	bShowApproach[14]= true;
	bShowApproach[15]= true;
	bShowApproach[16]= true;
	bShowApproach[17]= true;  
	bShowApproach[18]= true;  // Target Approach
	bShowApproach[19]= true;  // User Approach
	bShowApproach[20]= false;  
	bShowApproach[21]= false;  
	bShowApproach[22]= false;  
	bShowApproach[23]= false;  
	bShowApproach[24]= false;  

	m_bAltitude = true;
	m_strAltitudeLable.LoadString(IDS_ALTITUDE); 
}
//---------------------------------------------------------------------
// END: TPropPageThree::SetDefaultValues
//---------------------------------------------------------------------



//---------------------------------------------------------------------
// FUNCTION    : TPropPageThree::ChangeUnits
//
// DESCRIPTION : None.
//
// INPUT       : None.
//
// RETURN (S)  : None.
//---------------------------------------------------------------------
// REMARKS     : None.
//---------------------------------------------------------------------
void TPropPageThree::ChangeUnits() 
{
	//---------------------------------------------------------------------
	// Do not retrieve the data because it will be retrived incorrectly
	//---------------------------------------------------------------------
    UpdateUnits();
    if (IsWindow(m_hWnd))
	{
		UpdateData(false);
	    OnCurveButtonRecalc();
	}
}
//---------------------------------------------------------------------
// END: TPropPageThree::ChangeUnits
//---------------------------------------------------------------------


//---------------------------------------------------------------------
// FUNCTION    : TPropPageThree::UpdateUnits
//
// DESCRIPTION : None.
//
// INPUT       : None.
//
// RETURN (S)  : None.
//---------------------------------------------------------------------
// REMARKS     : None.
//---------------------------------------------------------------------
void TPropPageThree::UpdateUnits() 
{
    if ((!TPropPageBase::metricflag) && (m_strWBT != L_DEGF))
    {
		// Avoid problems with conversion at limits
		if (m_dblCurveWBT == WBT_MAX_SI)
			m_dblCurveWBT = WBT_MAX_IP;
		else if (m_dblCurveWBT == WBT_MIN_SI)
			m_dblCurveWBT = WBT_MIN_IP;
		else
	        m_dblCurveWBT	= truncit(fnCelcToFar(m_dblCurveWBT),2);
    
		m_dblAltitude	= truncit(fnMetersToFeet(m_dblAltitude), 2);
        m_dblCurveRange	= truncit(m_dblCurveRange * 1.8, 2);
        m_strWBT = L_DEGF;
        m_strRange = L_DEGF;
    }
    else if ((TPropPageBase::metricflag) && (m_strWBT != L_DEGC))
    {
		if (m_dblCurveWBT == WBT_MAX_IP)
			m_dblCurveWBT = WBT_MAX_SI;
		else if (m_dblCurveWBT == WBT_MIN_IP)
			m_dblCurveWBT = WBT_MIN_SI;
		else
			m_dblCurveWBT	= truncit(fnFarToCelc(m_dblCurveWBT),2);
        m_dblAltitude	= truncit(fnFeetToMeters(m_dblAltitude), 2);
        m_dblCurveRange	= truncit(m_dblCurveRange / 1.8, 2);
        m_strWBT = L_DEGC;
        m_strRange = L_DEGK;
    }

	if (m_bAltitude)
	{
	    if (TPropPageBase::metricflag)
		{
	        m_strAltitude= L_METERS;
		}
		else
		{
			m_strAltitude= L_FEET;
		}
	}
	else
	{
	    if (TPropPageBase::metricflag)
		{
	        m_strAltitude= L_KPA;
		}
		else
		{
			m_strAltitude= L_HG;
		}
	}
}
//---------------------------------------------------------------------
// END: TPropPageThree::UpdateUnits
//---------------------------------------------------------------------

//---------------------------------------------------------------------
// FUNCTION    : TPropPageThree::OnButtonAltitude
//
// DESCRIPTION : None.
//
// INPUT       : None.
//
// RETURN (S)  : None.
//---------------------------------------------------------------------
// REMARKS     : None.
//---------------------------------------------------------------------
void TPropPageThree::OnButtonAltitude() 
{
	UpdateData(true);

	m_bAltitude = !m_bAltitude;
	if (m_bAltitude)
	{
	    if (TPropPageBase::metricflag)
		{
	        m_strAltitude= L_METERS;
		}
		else
		{
			m_strAltitude= L_FEET;
		}
		m_strAltitudeLable.LoadString(IDS_ALTITUDE); 
#ifdef _DEMO_VERSION    
		GetDlgItem(IDC_CURVE_EDIT_ALTITUDE)->EnableWindow(false);
#endif
	}
	else
	{
	    if (TPropPageBase::metricflag)
		{
	        m_strAltitude= L_KPA;
		}
		else
		{
			m_strAltitude= L_HG;
		}
		m_strAltitudeLable.LoadString(IDS_BAROMETRIC_PRESSURE); 
#ifdef _DEMO_VERSION    
		GetDlgItem(IDC_CURVE_EDIT_ALTITUDE)->EnableWindow(true);
#endif
	}

	UpdateData(false);
}
//---------------------------------------------------------------------
// END: TPropPageThree::OnButtonAltitude
//---------------------------------------------------------------------


//---------------------------------------------------------------------
// FUNCTION    : TPropPageThree::DoDataExchange
//
// DESCRIPTION : None.
//
// INPUT       : CDataExchange* pDX
//
// RETURN (S)  : None.
//---------------------------------------------------------------------
// REMARKS     : None.
//---------------------------------------------------------------------
void TPropPageThree::DoDataExchange(CDataExchange* pDX)
{
	cdxCSizingPropPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(TPropPageThree)
	DDX_Control(pDX, IDC_COMBO_DATA_FILES, m_wndDataFileList);
	DDX_Control(pDX, IDC_SPIN_MIN, m_SpinMin);
	DDX_Control(pDX, IDC_SPIN_MAX, m_SpinMax);
	DDX_Text(pDX, IDC_EDIT_MAX, m_dblCurveMax);
	DDV_MinMaxDouble(pDX, m_dblCurveMax, 0.1, 20.0);
	DDX_Text(pDX, IDC_EDIT_MIN, m_dblCurveMin);
	DDV_MinMaxDouble(pDX, m_dblCurveMin, 0.1, 5.);
	DDX_Text(pDX, IDC_CURVE_STATIC_ALTITUDE_UNITS, m_strAltitude);
	DDX_Text(pDX, IDC_CURVE_STATIC_RANGE_UNITS, m_strRange);
	DDX_Text(pDX, IDC_CURVE_STATIC_WBT_UNITS, m_strWBT);
    DDX_Control(pDX, IDC_TCHART_GRAPH, m_wndGraph);
	//}}AFX_DATA_MAP

	//---------------------------------------------------------------------
	// Minmax values
	//---------------------------------------------------------------------
	DDX_Text(pDX, IDC_EDIT_CURVE_LG, m_dblLg);
#ifdef _NEW_GRAPH_LIMITS
    DDV_MinMaxDouble(pDX, m_dblLg, LG_MIN_IP, LG_MAX_IP,
        LG_MIN_SI, LG_MAX_SI, 2);
#else
    DDV_MinMaxDouble(pDX, m_dblLg, LG_P3_MIN_IP, LG_P3_MAX_IP,
        LG_P3_MIN_SI, LG_P3_MAX_SI, 2);
#endif // _NEW_GRAPH_LIMITS
    DDX_Text(pDX, IDC_CURVE_EDIT_WBT, m_dblCurveWBT);
    DDV_MinMaxDouble(pDX, m_dblCurveWBT, WBT_MIN_IP, WBT_MAX_IP,
		WBT_MIN_SI, WBT_MAX_SI, 2);
    DDX_Text(pDX, IDC_CURVE_EDIT_RANGE, m_dblCurveRange);
    DDV_MinMaxDouble(pDX, m_dblCurveRange, .1, 160, .2, 88.9, 2);
    DDX_Text(pDX, IDC_EDIT_CURVE_C1, m_dblCurveC1);
    DDV_MinMaxDouble(pDX, m_dblCurveC1, 0, 10, 0, 10, 2);
    DDX_Text(pDX, IDC_EDIT_CURVE_C2, m_dblCurveC2);
    DDV_MinMaxDouble(pDX,m_dblCurveC2, -2, 0, -2, 0, 2);

	//---------------------------------------------------------------------
	// Get and Validate Altitude or Barometric Pressure.
	//---------------------------------------------------------------------
	if (m_bAltitude)
	{
		DDX_Text(pDX, IDC_CURVE_EDIT_ALTITUDE, m_dblAltitude);
		DDV_MinMaxDouble(pDX, m_dblAltitude, ALT_MIN_IP, ALT_MAX_IP, 
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
		DDX_Text(pDX, IDC_CURVE_EDIT_ALTITUDE, dblPressure);
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

	//---------------------------------------------------------------------
	// KAVL or Target Approach value
	//---------------------------------------------------------------------
	if (pDX && !pDX->m_bSaveAndValidate)
	{
		if (!m_bShowUserApproach)
		{
			if (m_dblKavl > 0.001)
			{
				DDX_Text(pDX, IDC_EDIT_CURVE_KAVL, m_dblKavl);
			}
			else
			{
				CString strEmpty;
				DDX_Text(pDX, IDC_EDIT_CURVE_KAVL, strEmpty);
			}
		}
		else
		{
			if (m_dblTargetApproach > 0.001)
			{
				DDX_Text(pDX, IDC_EDIT_CURVE_KAVL, m_dblTargetApproach);
			}
			else
			{
				CString strEmpty;
				DDX_Text(pDX, IDC_EDIT_CURVE_KAVL, strEmpty);
			}
		}
	}

	GetDlgItem(IDC_BUTTON_ALTITUDE)->SetWindowText(m_strAltitudeLable);
}
//---------------------------------------------------------------------
// END: TPropPageThree::DoDataExchange
//---------------------------------------------------------------------


//---------------------------------------------------------------------
// FUNCTION    : TPropPageThree::OnButtonKavlApproach
//
// DESCRIPTION : None.
//
// INPUT       : None.
//
// RETURN (S)  : None.
//---------------------------------------------------------------------
// REMARKS     : None.
//---------------------------------------------------------------------
void TPropPageThree::OnButtonKavlApproach()
{
	if (UpdateData(true))
	{
		m_bShowUserApproach = !m_bShowUserApproach;
		if (m_bShowUserApproach)
		{
			GetDlgItem(IDC_BUTTON_KAVL_APPROACH)->SetWindowText("Approach:");
		}
		else
		{
			GetDlgItem(IDC_BUTTON_KAVL_APPROACH)->SetWindowText("Kav/L:");
		}
		UpdateData(false);
	}
}
//---------------------------------------------------------------------
// END: TPropPageThree::OnButtonKavlApproach
//---------------------------------------------------------------------




//---------------------------------------------------------------------
// FUNCTION    : TPropPageThree::m_fnInitApproachList
//
// DESCRIPTION : None.
//
// INPUT       : double * dblAppList
//               double dblWBT - 
//               double m_dblRange - 
//               double dblAltitude - 
//
// RETURN (S)  : None.
//---------------------------------------------------------------------
// REMARKS     : None.
//---------------------------------------------------------------------
void TPropPageThree::m_fnInitApproachList(double * dblAppList, double dblWBT, double m_dblRange, double dblAltitude)
{
	double	dblApp[] = {2,3,4,5,6,7,8,9,10,11,12,14,16,18,20,22,24,26,28,30,35,40,45,50,55,60,65,70,75,80,85,90,95,100};
	int		iAppItem = 0;

	for (int iIndex = 0; iIndex < (sizeof(dblApp)/sizeof(double)) ; iIndex++)
	{
		double dblKaVL	= Merkel(dblWBT, m_dblRange, dblApp[iIndex], .1, dblAltitude);

		if ((dblKaVL > .1) && (dblKaVL < 5))
		{
			dblAppList[iAppItem++] = dblApp[iIndex];
		}
	}
}
//---------------------------------------------------------------------
// END: TPropPageThree::m_fnInitApproachList
//---------------------------------------------------------------------


//---------------------------------------------------------------------
// FUNCTION    : TPropPageThree::OnCurveButtonRecalc
//
// DESCRIPTION : None.
//
// INPUT       : None.
//
// RETURN (S)  : None.
//---------------------------------------------------------------------
// REMARKS     : None.
//---------------------------------------------------------------------
void TPropPageThree::OnCurveButtonRecalc() 
{
	CWaitCursor curWait;
	try
	{
		if (UpdateData(true))
		{
			double	dLG;
			double  sDLG;
			double  prev_kavl;
			double  incAmount = 0.0;
			int		iIndex;
			double	dTempRange;
			CString	strTitle;
			double	App[sizeof(m_listApp)/sizeof(double)];
			unsigned long color;

#ifdef _NEW_GRAPH_LIMITS
            // Try to reconcile graph LG limits with the rest of the program
            // (speculative change, DBL, 12/08/2003)
            const double dLG_MIN = LG_MIN_IP, dLG_MAX = LG_MAX_IP;
#else
            const double dLG_MIN = 0.1, dLG_MAX = 5.0;
#endif

			//---------------------------------------------------------------------
			// Clean up graph
			//---------------------------------------------------------------------
			m_wndGraph.RemoveAllSeries();

			//---------------------------------------------------------------------
			// Setup the graph appearance
			//---------------------------------------------------------------------
			m_wndGraph.GetWalls().SetBackColor(0x0);  // black
			m_wndGraph.GetPanel().GetGradient().SetStartColor(0x0);
			m_wndGraph.GetPanel().GetGradient().SetEndColor(0x0);
			m_wndGraph.GetPanel().GetGradient().SetVisible(true);
			m_wndGraph.GetAspect().SetView3D(false);
			m_wndGraph.SetDragMode(dmManual);

			// Standard approach curve pen
			color = 0xFFFF;  // bright yellow

			m_wndGraph.GetLegend().SetVisible(false);

			m_wndGraph.GetAxis().GetLeft().SetMinMax(.1, 10);
			m_wndGraph.GetAxis().GetRight().SetMinMax(.1, 10);
			m_wndGraph.GetAxis().GetTop().SetMinMax(.1, 10);
#ifdef _NEW_GRAPH_LIMITS
			m_wndGraph.GetAxis().GetBottom().SetMinMax(LG_MIN_IP, LG_MAX_IP);
#else
			m_wndGraph.GetAxis().GetBottom().SetMinMax(.1, 10);
#endif

			m_wndGraph.GetAxis().GetLeft().SetLogarithmic(true);
			m_wndGraph.GetAxis().GetRight().SetLogarithmic(true);
			m_wndGraph.GetAxis().GetTop().SetLogarithmic(true);
			m_wndGraph.GetAxis().GetBottom().SetLogarithmic(true);

			m_wndGraph.GetAxis().GetLeft().GetMinorTicks().SetColor(  0xCCCCCC );
			m_wndGraph.GetAxis().GetRight().GetMinorTicks().SetColor( 0xCCCCCC );
			m_wndGraph.GetAxis().GetTop().GetMinorTicks().SetColor(   0xCCCCCC );
			m_wndGraph.GetAxis().GetBottom().GetMinorTicks().SetColor(0xCCCCCC );

			// grid color
			m_wndGraph.GetAxis().GetLeft().GetGridPen().SetColor(  0x777777);
			m_wndGraph.GetAxis().GetRight().GetGridPen().SetColor( 0x777777 );
			m_wndGraph.GetAxis().GetTop().GetGridPen().SetColor(   0x777777 );
			m_wndGraph.GetAxis().GetBottom().GetGridPen().SetColor(0x777777 );

			// Axis color
			m_wndGraph.GetAxis().GetLeft().GetAxisPen().SetColor(  0xE4E4E4 );
			m_wndGraph.GetAxis().GetRight().GetAxisPen().SetColor( 0xE4E4E4 );
			m_wndGraph.GetAxis().GetTop().GetAxisPen().SetColor(   0xE4E4E4 );
			m_wndGraph.GetAxis().GetBottom().GetAxisPen().SetColor(0xE4E4E4 );

			// grid text
			m_wndGraph.GetAxis().GetLeft().GetTitle().GetFont().SetColor(0xFFFFFF);
			m_wndGraph.GetAxis().GetBottom().GetTitle().GetFont().SetColor(0xFFFFFF);

			// axis labels
			m_wndGraph.GetAxis().GetLeft().GetLabels().GetFont().SetColor(0xFFFFFF);
			m_wndGraph.GetAxis().GetBottom().GetLabels().GetFont().SetColor(0xFFFFFF);

			// set major axis to solid lines - psSolid
			m_wndGraph.GetAxis().GetLeft().GetGridPen().SetStyle(  psSolid );
			m_wndGraph.GetAxis().GetRight().GetGridPen().SetStyle( psSolid );
			m_wndGraph.GetAxis().GetTop().GetGridPen().SetStyle(   psSolid );
			m_wndGraph.GetAxis().GetBottom().GetGridPen().SetStyle(psSolid );

			m_wndGraph.GetAxis().GetLeft().GetGridPen().SetWidth(  1 );
			m_wndGraph.GetAxis().GetRight().GetGridPen().SetWidth( 1 );
			m_wndGraph.GetAxis().GetTop().GetGridPen().SetWidth(   1 );
			m_wndGraph.GetAxis().GetBottom().GetGridPen().SetWidth(1 );

			m_wndGraph.GetAxis().GetTop().GetLabels().SetAngle(90);
			m_wndGraph.GetAxis().GetBottom().GetLabels().SetAngle(90);

			m_wndGraph.GetAxis().GetLeft().GetLabels().GetFont().SetSize(6);
			m_wndGraph.GetAxis().GetRight().GetLabels().GetFont().SetSize(6);
			m_wndGraph.GetAxis().GetTop().GetLabels().GetFont().SetSize(6);
			m_wndGraph.GetAxis().GetBottom().GetLabels().GetFont().SetSize(6);

			m_wndGraph.GetAxis().GetLeft().SetAutomaticMinimum(true);
			m_wndGraph.GetAxis().GetRight().SetAutomaticMinimum(true);
			m_wndGraph.GetAxis().GetTop().SetAutomaticMinimum(true);
			m_wndGraph.GetAxis().GetBottom().SetAutomaticMinimum(true);

			m_wndGraph.GetAxis().GetLeft().SetAutomaticMaximum(true);
			m_wndGraph.GetAxis().GetRight().SetAutomaticMaximum(true);
			m_wndGraph.GetAxis().GetTop().SetAutomaticMaximum(true);
			m_wndGraph.GetAxis().GetBottom().SetAutomaticMaximum(true);

			m_wndGraph.GetAxis().GetLeft().GetTitle().SetCaption("KaV/L");
			m_wndGraph.GetAxis().GetBottom().GetTitle().SetCaption("L/G");

			m_wndGraph.GetAxis().GetLeft().GetTitle().GetFont().SetSize(10);
			m_wndGraph.GetAxis().GetBottom().GetTitle().GetFont().SetSize(10);
			m_wndGraph.GetAxis().GetLeft().GetTitle().GetFont().SetBold(true);
			m_wndGraph.GetAxis().GetBottom().GetTitle().GetFont().SetBold(true);

			//---------------------------------------------------------------------
			// if metric, convert to english units before calculating
			//---------------------------------------------------------------------
			double WBTC = 0.0;
			double ALTC = 0.0;
			double RANC = 0.0;
			if (TPropPageBase::metricflag)
			{
				WBTC = m_dblCurveWBT;
				ALTC = m_dblAltitude;
				RANC = m_dblCurveRange;
				m_dblCurveWBT = fnCelcToFar(m_dblCurveWBT);
				m_dblAltitude = fnMetersToFeet(m_dblAltitude);
				m_dblCurveRange = m_dblCurveRange * 1.8;
			}
			// pre-scan for min/max value
			double kavl		= 0.0;
			double min4Lg	= 999.0;

			// Init range.
			double  maxVal = 5.0;   // for kavl
			double  minVal = 0.01;  // for kavl


			memset(m_listApp, 0, sizeof(m_listApp));
			m_fnInitApproachList(m_listApp, m_dblCurveWBT, m_dblCurveRange, m_dblAltitude);
			memcpy(App, m_listApp, sizeof(m_listApp));


			//---------------------------------------------------------------------
			// Clear the output values
			//---------------------------------------------------------------------
			m_dblKavl			= 0; // clear value
			m_dblTargetApproach = 0; // clear value
			UpdateData(false);

			//---------------------------------------------------------------------
			// Calculate the Target Approach
			//---------------------------------------------------------------------
			if ((m_dblLg >= dLG_MIN) && (m_dblLg <= dLG_MAX))
			{
				if (m_dblCurveC1 && m_dblCurveC2)
				{
					m_dblKavl = truncit((m_dblCurveC1 * pow( m_dblLg, m_dblCurveC2 )), 5);
					m_dblTargetApproach = m_fnGetExactApproach(m_dblCurveWBT, m_dblCurveRange, m_dblLg, m_dblAltitude, m_dblKavl);

					if ((m_dblKavl < .01) || (m_dblKavl > maxVal))
					{
						m_dblKavl = 0.0;
						m_dblTargetApproach = 0;
					}

					if (m_dblTargetApproach >= 100)
					{
						m_dblTargetApproach = 0;
					}

					if (TPropPageBase::metricflag)
					{
						m_dblTargetApproach *= 5.0/9.0;
					}
				}
			}

			//---------------------------------------------------------------------
			// Add the series to the graph
			//---------------------------------------------------------------------
			//---------------------------------------------------------------------
			// Standard Approach Curves
			//---------------------------------------------------------------------
			for (iIndex = 0; iIndex < 18; iIndex++)
			{
				m_wndGraph.AddSeries(scLine);
				strTitle.Format("%i", (int)App[iIndex]);
				m_wndGraph.GetSeries(iIndex).SetTitle(strTitle);
			}

			//---------------------------------------------------------------------
			// User and target Approach Curves
			//---------------------------------------------------------------------
			m_wndGraph.AddSeries(scLine);
			strTitle.Format("%.3f", m_dblTargetApproach);
			m_wndGraph.GetSeries(INDEX_TARGETAPPROACH).SetTitle(strTitle);
			App[INDEX_TARGETAPPROACH] = m_dblTargetApproach;

			m_wndGraph.AddSeries(scLine);
			strTitle.Format("%.3f", m_dblUserApproach);
			m_wndGraph.GetSeries(INDEX_USERAPPROACH).SetTitle(strTitle);
			App[INDEX_USERAPPROACH] = m_dblUserApproach;

			//---------------------------------------------------------------------
			// Characteristic Line
			//---------------------------------------------------------------------
			m_wndGraph.AddSeries(scLine);
			strTitle = "C";
			m_wndGraph.GetSeries(INDEX_COEF).SetTitle(strTitle);

			//---------------------------------------------------------------------
			// L/G Line
			//---------------------------------------------------------------------
			m_wndGraph.AddSeries(scLine);
			strTitle = "L/G";
			m_wndGraph.GetSeries(INDEX_LG).SetTitle(strTitle);

			//---------------------------------------------------------------------
			// KAVL Line
			//---------------------------------------------------------------------
			m_wndGraph.AddSeries(scLine);
			strTitle = "KaV/L";
			m_wndGraph.GetSeries(INDEX_KAVL).SetTitle(strTitle);

			
			//---------------------------------------------------------------------
			// Draw Apprach Lines
			//---------------------------------------------------------------------
			for (dLG = dLG_MIN; dLG < dLG_MAX; dLG += .05)
			{
				incAmount = 0.0;
				for (iIndex = 0; iIndex <= INDEX_USERAPPROACH; iIndex++)
				{
					if (getapp(iIndex) && (App[iIndex] > 0.0001))
					{
						dTempRange = m_dblCurveWBT + (1.8 * (double)iIndex);
            
						if (iIndex > 0) prev_kavl = kavl;
						
						if (TPropPageBase::metricflag)
						{
							kavl = Merkel(m_dblCurveWBT, m_dblCurveRange, 1.8*App[iIndex], dLG, m_dblAltitude);
						}
						else
						{
							kavl = Merkel(m_dblCurveWBT, m_dblCurveRange, App[iIndex], dLG, m_dblAltitude);
						}
            
						// ddp
						if (( kavl < minVal ) || ( kavl >= maxVal ))
						{
							double dInterp;
							for (dInterp = dLG; (( kavl < minVal ) || ( kavl >= maxVal )) && (dInterp > .1); dInterp -= 0.0002)
							{
								if (TPropPageBase::metricflag)
								{
									kavl = Merkel(m_dblCurveWBT, m_dblCurveRange, 1.8*App[iIndex], dInterp, m_dblAltitude);
								}
								else
								{
									kavl = Merkel(m_dblCurveWBT, m_dblCurveRange, App[iIndex], dInterp, m_dblAltitude);
								}
							}
							sDLG = dInterp;
							App[iIndex] = 0;  //DDP This is the last point
						}
						else
						{
							sDLG = dLG;
						}

						if ((min4Lg > kavl) && (kavl > .1))
							min4Lg = kavl;

						if ((kavl <= 10.0) && (kavl >= .1))
						{
							switch (iIndex)
							{
								case INDEX_TARGETAPPROACH:
								case INDEX_USERAPPROACH:
									m_wndGraph.GetSeries(iIndex).AddXY(sDLG, kavl, NULL, 0x0099FF);
									break;

								default:
									m_wndGraph.GetSeries(iIndex).AddXY(sDLG, kavl, NULL, color);
									break;
							}
						}
					}
				}
			}

			//---------------------------------------------------------------------
			// Draw Fill Line
			//---------------------------------------------------------------------
			if ((m_dblCurveC1 != 0) && (m_dblCurveC2 != 0) && coef)
			{        
				for (dLG = m_dblCurveMin; dLG <= m_dblCurveMax; dLG += .05)
				{
					if ((dLG >= m_dblCurveMin) && (dLG <= m_dblCurveMax))
					{
						double dblK = m_dblCurveC1 * pow(dLG, m_dblCurveC2);

						if ((dblK >= min4Lg) && (dblK <= maxVal))
						{
							m_wndGraph.GetSeries(INDEX_COEF).AddXY(dLG, dblK, NULL, 0x0099FF);	
						}
					}
				}
			}

			//---------------------------------------------------------------------
			// Draw L/G line
			//---------------------------------------------------------------------
			if (m_dblLg > dLG_MIN && m_dblLg <= dLG_MAX && Lg)
			{
				m_wndGraph.GetSeries(INDEX_LG).AddXY(m_dblLg, min4Lg, NULL, 0x0099FF);
				m_wndGraph.GetSeries(INDEX_LG).AddXY(m_dblLg, maxVal, NULL, 0x0099FF);
			}

			//---------------------------------------------------------------------
			// Draw KaV/L line
			//---------------------------------------------------------------------
			if (m_bShowKaVLLine && (m_dblKavl > 0.1) && (m_dblKavl <= maxVal))
			{
				m_wndGraph.GetSeries(INDEX_KAVL).AddXY(0.1, m_dblKavl, NULL, 0x0099FF);
				m_wndGraph.GetSeries(INDEX_KAVL).AddXY(5.0, m_dblKavl, NULL, 0x0099FF);
			}

			//---------------------------------------------------------------------
			// Reset to metric if needed and update the output
			//---------------------------------------------------------------------
			if (TPropPageBase::metricflag) // set values back to metric
			{
				m_dblCurveWBT = WBTC;
				m_dblAltitude = ALTC;
				m_dblCurveRange = RANC;
			}

			UpdateData(false);
		}
	}
	catch(...)
	{
		MessageBox("Error in calculation.  Please check your input values.", "Calculation Error", MB_OK | MB_ICONEXCLAMATION);
	}
}
//---------------------------------------------------------------------
// END: TPropPageThree::OnCurveButtonRecalc
//---------------------------------------------------------------------


//---------------------------------------------------------------------
// FUNCTION    : TPropPageThree::OnFilePrint
//
// DESCRIPTION : None.
//
// INPUT       : None.
//
// RETURN (S)  : None.
//---------------------------------------------------------------------
// REMARKS     : None.
//---------------------------------------------------------------------
void TPropPageThree::OnFilePrint() 
{
    TPrint tp;
    if (tp.DoModal() == IDOK)
	{
		wndPrintView.m_fnPrint(tp.m_csDescription, m_dblCurveRange, m_dblCurveWBT, m_dblAltitude, m_dblCurveC1, m_dblCurveC2, m_dblKavl, m_dblLg, INDEX_TARGETAPPROACH, INDEX_USERAPPROACH, &m_wndGraph);
	}
}
//---------------------------------------------------------------------
// END: TPropPageThree::OnFilePrint
//---------------------------------------------------------------------


//---------------------------------------------------------------------
// FUNCTION    : TPropPageThree::OnFilePrintPreview
//
// DESCRIPTION : None.
//
// INPUT       : None.
//
// RETURN (S)  : None.
//---------------------------------------------------------------------
// REMARKS     : None.
//---------------------------------------------------------------------
void TPropPageThree::OnFilePrintPreview() 
{
    TPrint tp;
    if (tp.DoModal() == IDOK)
	{
		wndPrintFrame.ShowWindow(SW_NORMAL);
		wndPrintView.ShowWindow(SW_NORMAL);
		wndPrintFrame.m_bPrintPreview = TRUE;
		wndPrintView.m_fnPrintPreview(tp.m_csDescription, m_dblCurveRange, m_dblCurveWBT, m_dblAltitude, m_dblCurveC1, m_dblCurveC2, m_dblKavl, m_dblLg, INDEX_TARGETAPPROACH, INDEX_USERAPPROACH, &m_wndGraph );
	}
}
//---------------------------------------------------------------------
// END: TPropPageThree::OnFilePrintPreview
//---------------------------------------------------------------------



//---------------------------------------------------------------------
// FUNCTION    : TPropPageThree::findApproach
//
// DESCRIPTION : None.
//
// INPUT       : long X
//               long Y - 
//
// RETURN (S)  : int 
//---------------------------------------------------------------------
// REMARKS     : None.
//---------------------------------------------------------------------
int TPropPageThree::findApproach(long X, long Y)
{
    int approach = 0;
    int curveIndex = 0;
    double distance = 5000.0;
    double dX = m_wndGraph.GetSeries(0).XScreenToValue(X);
    double dY = m_wndGraph.GetSeries(0).YScreenToValue(Y);
    for (int x=0; x<m_wndGraph.GetSeriesCount(); x++)
    {
        CSeries cs;
        CValueList cx;
        CValueList cy;

		//---------------------------------------------------------------------
		// Find only approach lines
		//---------------------------------------------------------------------
        if (m_wndGraph.GetSeries(x).GetTitle() == "C" || m_wndGraph.GetSeries(x).GetTitle() == "L/G" )
		{
			break;
		}

        cs = m_wndGraph.GetSeries(x);
        cx = cs.GetXValues();
        cy = cs.GetYValues();

		//---------------------------------------------------------------------
		// Find the closest approach line index
		//---------------------------------------------------------------------
        for (int y=0; y< cx.GetCount(); y++)
        {
            double xc = cx.GetValue(y);
            double yc = cy.GetValue(y);
            double delta = fabs(xc-dX) + fabs(yc-dY);
            if ( distance > delta )
            {
                distance = delta;
                curveIndex = x;
            }
        }
    }
    approach = curveIndex;
    return approach;
}
//---------------------------------------------------------------------
// END: TPropPageThree::findApproach
//---------------------------------------------------------------------


//---------------------------------------------------------------------
// FUNCTION    : TPropPageThree::m_fnGetExactApproach
//
// DESCRIPTION : None.
//
// INPUT       : double dblCurveWBT
//               double dblCurveRange - 
//               double dblLg - 
//               double dblAltitude - 
//               double dblKaVl - 
//
// RETURN (S)  : double 
//---------------------------------------------------------------------
// REMARKS     : None.
//---------------------------------------------------------------------
double TPropPageThree::m_fnGetExactApproach(double dblCurveWBT, double dblCurveRange, double dblLg, double dblAltitude, double dblKaVl)
{
	double dblApproach;
	double dblDelta = 1.0;

	//---------------------------------------------------------------------
	// Find approach within .001
	//---------------------------------------------------------------------
	for (dblApproach = 0.0; dblApproach < 100.0; dblApproach += dblDelta)
	{
		double kavl = Merkel(dblCurveWBT, dblCurveRange, dblApproach, dblLg, dblAltitude);

		if (dblDelta > 0.9)
		{
			if (kavl < dblKaVl)
			{
				dblDelta = -0.001;
			}
		}
		else
		{
			if (kavl >= dblKaVl)
			{
				dblApproach = truncit(dblApproach, 3);
				break;
			}
		}
	}
	return dblApproach;
}
//---------------------------------------------------------------------
// END: TPropPageThree::m_fnGetExactApproach
//---------------------------------------------------------------------


//---------------------------------------------------------------------
// FUNCTION    : TPropPageThree::OnOnDblClickTchartGraph
//
// DESCRIPTION : Turn off approach lines.
//
// INPUT       : None.
//
// RETURN (S)  : None.
//---------------------------------------------------------------------
// REMARKS     : None.
//---------------------------------------------------------------------
void TPropPageThree::OnOnDblClickTchartGraph()
{
    const MSG *msg = GetCurrentMessage();
    POINT pt = msg->pt;
    m_wndGraph.ScreenToClient(&pt);
    int approach = findApproach(pt.x,pt.y);
    setapp(approach, false);
    OnCurveButtonRecalc();
}
//---------------------------------------------------------------------
// END: TPropPageThree::OnOnDblClickTchartGraph
//---------------------------------------------------------------------


//---------------------------------------------------------------------
// FUNCTION    : TPropPageThree::OnOnMouseUpTchartGraph
//
// DESCRIPTION : Show message box with data about point.
//
// INPUT       : long Button
//               long /*Shift*/ - 
//               long X - 
//               long Y - 
//
// RETURN (S)  : None.
//---------------------------------------------------------------------
// REMARKS     : None.
//---------------------------------------------------------------------
void TPropPageThree::OnOnMouseUpTchartGraph(long Button, long /*Shift*/, long X, long Y) 
{
	if (Button == 2)
	{
		UpdateData(true);

		double dX = m_wndGraph.GetSeries(0).XScreenToValue(X);
		double dY = m_wndGraph.GetSeries(0).YScreenToValue(Y);
		
		double WBTC;
		double ALTC;
		double RANC;

		WBTC = m_dblCurveWBT;
		ALTC = m_dblAltitude;
		RANC = m_dblCurveRange;
		if (TPropPageBase::metricflag)
		{
			WBTC = fnCelcToFar(m_dblCurveWBT);
			ALTC = fnMetersToFeet(m_dblAltitude);
			RANC = m_dblCurveRange * 1.8;
		}
		
		double dblApproach = m_fnGetExactApproach(WBTC, RANC, dX, ALTC, dY);

		if (TPropPageBase::metricflag)
		{
			dblApproach *= 5.0/9.0;
		}

		CString strPos;
		strPos.Format("Approach\t= %.3f\n\nL/G\t= %.3f\nKaV/L\t= %.5f", dblApproach, dX, dY);
		MessageBox(strPos, "Mouse Clicked at", MB_OK | MB_ICONINFORMATION);
	}
}
//---------------------------------------------------------------------
// END: TPropPageThree::OnOnMouseUpTchartGraph
//---------------------------------------------------------------------



//---------------------------------------------------------------------
// FUNCTION    : TPropPageThree::OnSize
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
void TPropPageThree::OnSize(UINT nType, int cx, int cy) 
{
	cdxCSizingPropPage::OnSize(nType, cx, cy);

	//---------------------------------------------------------------------
	// Set the tick increments based on the window size
	//---------------------------------------------------------------------
	if (m_wndGraph.m_hWnd)
	{
		if (cy < 550)
		{
			m_wndGraph.GetAxis().GetLeft().SetIncrement(.75);
		}
		else if (cx < 850)
		{
			m_wndGraph.GetAxis().GetLeft().SetIncrement(.5);
		}
		else
		{
			m_wndGraph.GetAxis().GetLeft().SetIncrement(.25);
		}

		if (cx < 650)
		{
			m_wndGraph.GetAxis().GetBottom().SetIncrement(.5);
		}
		else if (cx < 850)
		{
			m_wndGraph.GetAxis().GetBottom().SetIncrement(.25);
		}
		else
		{
			m_wndGraph.GetAxis().GetBottom().SetIncrement(.2);
		}
	}

    long wStyle = GetWindowLong( m_hWnd, GWL_STYLE );
	wStyle ^= WS_CLIPCHILDREN;
    SetWindowLong(m_hWnd,GWL_STYLE, wStyle);
	InvalidateRect(NULL);
	wStyle |= WS_CLIPCHILDREN;
    SetWindowLong(m_hWnd,GWL_STYLE, wStyle);
}
//---------------------------------------------------------------------
// END: TPropPageThree::OnSize
//---------------------------------------------------------------------


//---------------------------------------------------------------------
// FUNCTION    : TPropPageThree::OnVScroll
//
// DESCRIPTION : Spin control function.
//
// INPUT       : UINT nSBCode
//               UINT nPos - 
//               CScrollBar* pScrollBar - 
//
// RETURN (S)  : None.
//---------------------------------------------------------------------
// REMARKS     : None.
//---------------------------------------------------------------------
void TPropPageThree::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{	
	cdxCSizingPropPage::OnVScroll(nSBCode, nPos, pScrollBar);
    int pos = nPos;
    static int Doldpos = -1;
    static int Uoldpos = -1;

    if ( nSBCode == SB_ENDSCROLL )
    {
        return;
    }

    if (pScrollBar->GetDlgCtrlID() == IDC_SPIN_MIN)
    {
        CString strValue;
        if ( (pos == 0) || (pos < Doldpos) ) // 0 = up
        {
            if ( m_dblCurveMin+0.1 < m_dblCurveMax )
                m_dblCurveMin += 0.1;
        }
        else // down   
        {
            if ( m_dblCurveMin-0.1 >= 0.1)
                m_dblCurveMin -= 0.1;            
        }
        strValue.Format("%2.1f",m_dblCurveMin);
        ((CSpinButtonCtrl*) pScrollBar)->GetBuddy()->SetWindowText(strValue);
        Doldpos = pos;
    }

    if (pScrollBar->GetDlgCtrlID() == IDC_SPIN_MAX)
    {
        CString strValue;
        if (pos == 0 || (pos < Uoldpos) ) // 0 = up
        {
            if ( m_dblCurveMax+0.1 <= 5.0 )
                m_dblCurveMax += 0.1;
        }
        else // down   
        {
            if ( m_dblCurveMax-0.1 > m_dblCurveMin)
                m_dblCurveMax -= 0.1;            
        }
        strValue.Format("%2.1f",m_dblCurveMax);
        ((CSpinButtonCtrl*) pScrollBar)->GetBuddy()->SetWindowText(strValue);
        Uoldpos = pos;
    }
}
//---------------------------------------------------------------------
// END: TPropPageThree::OnVScroll
//---------------------------------------------------------------------


//---------------------------------------------------------------------
// FUNCTION    : TPropPageThree::OnMultiEdit
//
// DESCRIPTION : Curve slection dialog.
//
// INPUT       : None.
//
// RETURN (S)  : None.
//---------------------------------------------------------------------
// REMARKS     : None.
//---------------------------------------------------------------------
void TPropPageThree::OnMultiEdit() 
{
    TMultiEdit dlgCurveStatus;
    dlgCurveStatus.m_fnInitData(m_listApp, bShowApproach, coef, Lg, m_bShowKaVLLine, bShowApproach[INDEX_TARGETAPPROACH], bShowApproach[INDEX_USERAPPROACH], m_dblUserApproach);
    if (dlgCurveStatus.DoModal() == IDOK)
	{
		dlgCurveStatus.m_fnGetData(bShowApproach, coef, Lg, m_bShowKaVLLine, bShowApproach[INDEX_TARGETAPPROACH], bShowApproach[INDEX_USERAPPROACH], m_dblUserApproach);
		OnCurveButtonRecalc();
	}
}
//---------------------------------------------------------------------
// END: TPropPageThree::OnMultiEdit
//---------------------------------------------------------------------


//---------------------------------------------------------------------
// FUNCTION    : TPropPageThree::getapp
//
// DESCRIPTION : Get the On Off status of the approach line.
//
// INPUT       : int index
//
// RETURN (S)  : bool 
//---------------------------------------------------------------------
// REMARKS     : None.
//---------------------------------------------------------------------
bool TPropPageThree::getapp(int index)
{
    if ((index >= 0) && (index <= INDEX_USERAPPROACH))
	{
        return bShowApproach[index];
	}
	else
	{
		ASSERT(0);
	}
    return 0;
}
//---------------------------------------------------------------------
// END: TPropPageThree::getapp
//---------------------------------------------------------------------


//---------------------------------------------------------------------
// FUNCTION    : TPropPageThree::setapp
//
// DESCRIPTION : Set the on / off status of the approach line.
//
// INPUT       : int index
//               bool value - 
//
// RETURN (S)  : None.
//---------------------------------------------------------------------
// REMARKS     : None.
//---------------------------------------------------------------------
void TPropPageThree::setapp(int index, bool value)
{
    if ((index >=0) && (index <= INDEX_USERAPPROACH))
	{
        bShowApproach[index] = value;
	}
	else
	{
		ASSERT(0);
	}
}
//---------------------------------------------------------------------
// END: TPropPageThree::setapp
//---------------------------------------------------------------------


//---------------------------------------------------------------------
// FUNCTION    : TPropPageThree::OnPaint
//
// DESCRIPTION : None.
//
// INPUT       : None.
//
// RETURN (S)  : None.
//---------------------------------------------------------------------
// REMARKS     : None.
//---------------------------------------------------------------------
void TPropPageThree::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	if (!m_bNotified && ((GetDlgItem(IDC_TCHART_GRAPH) == NULL) || 
		(!IsWindow(GetDlgItem(IDC_TCHART_GRAPH)->m_hWnd))))
	{
		// This can be caused by an unregistered teechart.ocx, but
		// re-registering it here does not seeem to work.  User
		// must re-start the app or reinstall.
		CString szMsg(SZ_APP_NAME);
		szMsg += " does not appear to be completely installed.  Please exit the application, reinstall and try again.";
		AfxMessageBox(szMsg, MB_ICONEXCLAMATION);
	}
	m_bNotified = true;
	
	// Do not call cdxCSizingPropPage::OnPaint() for painting messages
}
//---------------------------------------------------------------------
// END: TPropPageThree::OnPaint
//---------------------------------------------------------------------

//---------------------------------------------------------------------
// End: TPropPageThree.cpp
//---------------------------------------------------------------------



void TPropPageThree::OnButtonSave() 
{
	CString strSection;
	CString strTemp;

	UpdateData();

	if (m_strDataName.GetLength())
	{
		//---------------------------------------------------------------------
		// Set Default input values
		//---------------------------------------------------------------------
		strSection = "Demand Curve Data";

		strTemp.Format("%.04f", m_dblCurveC1);
		WritePrivateProfileString(strSection, "CurveC1", strTemp, m_strDataName);

		strTemp.Format("%.04f", m_dblCurveC2);
		WritePrivateProfileString(strSection, "CurveC2", strTemp, m_strDataName);

		strTemp.Format("%.04f", m_dblAltitude);
		WritePrivateProfileString(strSection, "Altitude", strTemp, m_strDataName);

		strTemp.Format("%.04f", m_dblKavl);
		WritePrivateProfileString(strSection, "Kavl", strTemp, m_strDataName);

		strTemp.Format("%.04f", m_dblLg);
		WritePrivateProfileString(strSection, "Lg", strTemp, m_strDataName);

		strTemp.Format("%.04f", m_dblCurveMin);
		WritePrivateProfileString(strSection, "CurveMin", strTemp, m_strDataName);

		strTemp.Format("%.04f", m_dblCurveMax);
		WritePrivateProfileString(strSection, "CurveMax", strTemp, m_strDataName);

		strTemp.Format("%.04f", m_dblCurveWBT);
		WritePrivateProfileString(strSection, "CurveWBT", strTemp, m_strDataName);

		strTemp.Format("%.04f", m_dblCurveRange);
		WritePrivateProfileString(strSection, "CurveRange", strTemp, m_strDataName);

		//---------------------------------------------------------------------
		// Set unit lables
		//---------------------------------------------------------------------
		WritePrivateProfileString(strSection, "WBTUnits", m_strWBT, m_strDataName);
		WritePrivateProfileString(strSection, "RangeUnits", m_strRange, m_strDataName);
		WritePrivateProfileString(strSection, "AltUnits", m_strAltitude, m_strDataName);

		m_bDataChanged = false;
	}
}

void TPropPageThree::OnSelchangeComboDataFiles() 
{
	UpdateData();

	if (m_bDataChanged && (m_strDataName.GetLength()))
	{
		if (MessageBox("Do you want to save the changes to the current data?", "Demand Curve Data", MB_YESNO) == IDYES)
		{
			OnButtonSave();
		}
	}

	if (m_wndDataFileList.GetCurSel() == 0)
	{
		TNewDemandCurveDataDlg dlgNewData;

		if (dlgNewData.DoModal() == IDOK)
		{
			if (m_wndDataFileList.SelectString(0, dlgNewData.m_strNewFileName) == CB_ERR)
			{
				CString strPath;
				CString strNewFileName;

				GetModuleFileName(NULL, strPath.GetBuffer(MAX_PATH+1), MAX_PATH);
				strPath.ReleaseBuffer();
				strPath = strPath.Left(strPath.ReverseFind('\\')+1);

				m_strDataName = strPath + dlgNewData.m_strNewFileName;

				m_wndDataFileList.AddString(dlgNewData.m_strNewFileName);
				m_wndDataFileList.SelectString(0, dlgNewData.m_strNewFileName);
				OnButtonSave();
			}
		}
		else
		{
			m_strDataName == "";
		}
	}
	
	if ((m_wndDataFileList.GetCurSel() != LB_ERR) && (m_wndDataFileList.GetCurSel() != 0))
	{
		CString strSection;
		char szValue[256];

	
		m_wndDataFileList.GetLBText(m_wndDataFileList.GetCurSel(), m_strDataName);

		CString strPath;
		CString strNewFileName;

		GetModuleFileName(NULL, strPath.GetBuffer(MAX_PATH+1), MAX_PATH);
		strPath.ReleaseBuffer();
		strPath = strPath.Left(strPath.ReverseFind('\\')+1);

		m_strDataName = strPath + m_strDataName;

		//---------------------------------------------------------------------
		// Set Default input values
		//---------------------------------------------------------------------
		strSection = "Demand Curve Data";
		GetPrivateProfileString(strSection, "CurveC1", "0.0", szValue, 256, m_strDataName);
		m_dblCurveC1	= atof(szValue);

		GetPrivateProfileString(strSection, "CurveC2", "0.0", szValue, 256, m_strDataName);
		m_dblCurveC2	= atof(szValue);

		GetPrivateProfileString(strSection, "Altitude", "0.0", szValue, 256, m_strDataName);
		m_dblAltitude	= atof(szValue);

		GetPrivateProfileString(strSection, "Kavl", "0.0", szValue, 256, m_strDataName);
		m_dblKavl		= atof(szValue);

		GetPrivateProfileString(strSection, "Lg", "0.0", szValue, 256, m_strDataName);
		m_dblLg			= atof(szValue);

		GetPrivateProfileString(strSection, "CurveMin", "0.5", szValue, 256, m_strDataName);
		m_dblCurveMin	= atof(szValue);

		GetPrivateProfileString(strSection, "CurveMax", "2.5", szValue, 256, m_strDataName);
		m_dblCurveMax	= atof(szValue);

		GetPrivateProfileString(strSection, "CurveWBT", "80.0", szValue, 256, m_strDataName);
		m_dblCurveWBT	= atof(szValue);

		GetPrivateProfileString(strSection, "CurveRange", "10.0", szValue, 256, m_strDataName);
		m_dblCurveRange = atof(szValue);

		//---------------------------------------------------------------------
		// Set unit lables
		//---------------------------------------------------------------------
		GetPrivateProfileString(strSection, "WBTUnits", L_DEGF, szValue, 256, m_strDataName);
		m_strWBT = szValue;

		GetPrivateProfileString(strSection, "RangeUnits", L_DEGF, szValue, 256, m_strDataName);
		m_strRange = szValue;

		GetPrivateProfileString(strSection, "AltUnits", L_FEET, szValue, 256, m_strDataName);
		m_strAltitude = szValue;

		//---------------------------------------------------------------------
		// Curves are all initially on
		//---------------------------------------------------------------------
		coef = true;
		Lg = true;
		m_bShowKaVLLine = true;
		bShowApproach[0] = true;
		bShowApproach[1] = true;
		bShowApproach[2] = true;
		bShowApproach[3] = true;
		bShowApproach[4] = true;
		bShowApproach[5] = true;
		bShowApproach[6] = true;
		bShowApproach[7] = true;
		bShowApproach[8] = true;
		bShowApproach[9] = true;
		bShowApproach[10]= true;
		bShowApproach[11]= true;
		bShowApproach[12]= true;
		bShowApproach[13]= true;
		bShowApproach[14]= true;
		bShowApproach[15]= true;
		bShowApproach[16]= true;
		bShowApproach[17]= true;  
		bShowApproach[18]= true;  // Target Approach
		bShowApproach[19]= true;  // User Approach
		bShowApproach[20]= false;  
		bShowApproach[21]= false;  
		bShowApproach[22]= false;  
		bShowApproach[23]= false;  
		bShowApproach[24]= false;  

		m_bAltitude = true;
		m_strAltitudeLable.LoadString(IDS_ALTITUDE);

		UpdateUnits();
	}

	UpdateData(false);
}

