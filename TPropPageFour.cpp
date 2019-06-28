// TPropPageFour.cpp : implementation file
//
#include "stdafx.h"
#include "CTIToolkit.h"
#include "TPropPageBase.h"
#include "TPropPageFour.h"
#include "TPerformanceDataDlg.h"
#include "TPerformanceCurveDlg.h"
#include "TNewPerformanceDataFileDlg.h"
#include "TNewTestDlg.h"
#include "PrintFrame.h"
#include "PrintPerfReport.h"
#include "TPrintPerfReportDlg.h"
#include "CTIhelp.hh"
#include "float.h"
#ifdef _MFC_PREVIEW
#include "ToolkitDoc.h"
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

 
// Note:
// Code marked by the define "_DEMO_VERSION_NO_SAVE" is currently 
// de-activated.  This code was an attempt to disable saves in the
// Demo version, but it resulted in unpredictable behavior and so
// was removed.  It could be re-activated (and fixed) in a future 
// version, however ....

// Some limits to avoid problems:
static const int MIN_FLOW_COUNT  = 3;
static const int MIN_RANGE_COUNT = 3;
static const int MIN_WBT_COUNT   = 3;	// Dean's notes say 5 ....
static const unsigned int MAX_CWT_DEV_ITERATIONS    = 1000U;


//---------------------------------------------------------------------
// External info
//---------------------------------------------------------------------
extern CPrintFrame			wndPrintPerfDataFrame;
extern CPrintPerfReport		wndPrintPerfDataView;

//---------------------------------------------------------------------
// New min/max structure for design/test data
//---------------------------------------------------------------------
TPerfData::TPerfData(double dblWaterFlowRate, double dblHWT, 
					 double dblCWT, double dblWBT, double dblDBT, 
					 double dblFanDriverPower, double dblBarometricPressure, 
					 double dblLG)
					 : m_dblWaterFlowRate(dblWaterFlowRate),
					 m_dblHWT(dblHWT), m_dblCWT(dblCWT), 
					 m_dblWBT(dblWBT), m_dblDBT(dblDBT), 
					 m_dblFanDriverPower(dblFanDriverPower),
					 m_dblBarometricPressure(dblBarometricPressure),
					 m_dblLG(dblLG)
{
}


TPerfData::TPerfData(const TPropPageFour & ppf, bool bDesignData)
{
	if (bDesignData)
	{
		m_dblWaterFlowRate = ppf.m_dblDesignWaterFlowRate;
		m_dblHWT = ppf.m_dblDesignHWT;
		m_dblCWT = ppf.m_dblDesignCWT;
		m_dblWBT = ppf.m_dblDesignWBT;
		m_dblDBT = ppf.m_dblDesignDBT;
		m_dblFanDriverPower = ppf.m_dblDesignFanDriverPower;
		m_dblBarometricPressure = ppf.m_dblDesignBarometricPressure;
		m_dblLG = ppf.m_dblDesignLG;
	}
	else
	{
		m_dblWaterFlowRate = ppf.m_dblTestWaterFlowRate;
		m_dblHWT = ppf.m_dblTestHWT;
		m_dblCWT = ppf.m_dblTestCWT;
		m_dblWBT = ppf.m_dblTestWBT;
		m_dblDBT = ppf.m_dblTestDBT;
		m_dblFanDriverPower = ppf.m_dblTestFanDriverPower;
		m_dblBarometricPressure = ppf.m_dblTestBarometricPressure;
		m_dblLG = ppf.m_dblTestLG;
	}

}

/**
TPerfData
TPerfData::SICopy() const
{
	// Convert all values to SI
	TPerfData pdNew( 
		m_dblWaterFlowRate == DBL_MIN || m_dblWaterFlowRate == DBL_MAX ? 
			m_dblWaterFlowRate : fnGPMToLS(m_dblWaterFlowRate),
		fnFarToCelc(m_dblHWT),
		fnFarToCelc(m_dblCWT),
		fnFarToCelc(m_dblWBT), 
		fnFarToCelc(m_dblDBT),
		m_dblFanDriverPower == DBL_MIN || m_dblFanDriverPower == DBL_MAX ?
			m_dblFanDriverPower : fnBHPToKW(m_dblFanDriverPower),
		m_dblBarometricPressure == DBL_MIN || m_dblBarometricPressure == DBL_MAX ?
			m_dblBarometricPressure :  calcPressureC(PSIToKPA(m_dblBarometricPressure)),
		m_dblLG); // l/g is dimensionless??
	return pdNew;
}
**/

// All bound values are initialized in IP
static TPerfData g_pdMinIP(WATER_FLOW_RATE_MIN_IP, HWT_MIN_IP, CWT_MIN_IP, 
						 WBT_MIN_IP, DBT_MIN_IP, FAN_DRIVER_POWER_MIN_IP,  
						 BAROMETRIC_PRESSURE_MIN_IP, LG_MIN_IP);
static TPerfData g_pdMaxIP(WATER_FLOW_RATE_MAX_IP, HWT_MAX_IP, CWT_MAX_IP, 
						 WBT_MAX_IP, DBT_MAX_IP, FAN_DRIVER_POWER_MAX_IP, 
						 BAROMETRIC_PRESSURE_MAX_IP, LG_MAX_IP);
static TPerfData g_pdMinSI(WATER_FLOW_RATE_MIN_SI, HWT_MIN_SI, CWT_MIN_SI, 
						 WBT_MIN_SI, DBT_MIN_SI, FAN_DRIVER_POWER_MIN_SI,  
						 BAROMETRIC_PRESSURE_MIN_SI, LG_MIN_SI);
static TPerfData g_pdMaxSI(WATER_FLOW_RATE_MAX_SI, HWT_MAX_SI, CWT_MAX_SI, 
						 WBT_MAX_SI, DBT_MAX_SI, FAN_DRIVER_POWER_MAX_SI, 
						 BAROMETRIC_PRESSURE_MAX_SI, LG_MAX_SI);


/////////////////////////////////////////////////////////////////////////////
// TPropPageFour property page

IMPLEMENT_DYNCREATE(TPropPageFour, cdxCSizingPropPage)

TPropPageFour::TPropPageFour() : cdxCSizingPropPage(TPropPageFour::IDD)
{
	m_bTestDataChanged	= false;
	m_iCurrentTest		= -1;
	m_strPreviewSubject = "";
    m_bCWTDevCalcOk     = true;
	//{{AFX_DATA_INIT(TPropPageFour)
	m_strUnitsCWTDev = _T("");
	m_strUnitsWBT = _T("");
	m_strUnitsWaterFlowRate = _T("");
	m_strUnitsHWT = _T("");
	m_strUnitsFanDriverPower = _T("");
	m_strUnitsDBT = _T("");
	m_strUnitsCWT = _T("");
	m_strUnitsBarometricPressure = _T("");
	m_strUnitsAdjustedFlow = _T("");
	m_strUnitsPredictedFlow = _T("");
	m_strProjectName = _T("");
	m_strTowerOwner = _T("");
	m_strTowerManufacturer = _T("");
	m_strTowerLocation = _T("");
	m_dblCWTDev = 0.0;
	m_dblCapability = 0.0;
	m_dblDesignBarometricPressure = 0.0;
	m_dblDesignCWT = 0.0;
	m_dblDesignDBT = 0.0;
	m_dblDesignFanDriverPower = 0.0;
	m_dblDesignHWT = 0.0;
	m_dblDesignLG = 0.0;
	m_dblDesignWaterFlowRate = 0.0;
	m_dblDesignWBT = 0.0;
	m_dblTestBarometricPressure = 0.0;
	m_dblTestCWT = 0.0;
	m_dblTestDBT = 0.0;
	m_dblTestFanDriverPower = 0.0;
	m_dblTestHWT = 0.0;
	m_dblTestLG = 0.0;
	m_dblTestWaterFlowRate = 0.0;
	m_dblTestWBT = 0.0;
	m_dblPredictedFlow = 0.0;
	m_dblAdjustedFlow = 0.0;
	//}}AFX_DATA_INIT
	UpdateUnits();
}

TPropPageFour::~TPropPageFour()
{
}

void TPropPageFour::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(TPropPageFour)
	DDX_Control(pDX, IDC_EDIT_TEST_WET_BULB, m_wndTestWBT);
	DDX_Control(pDX, IDC_EDIT_TEST_WATER_FLOW, m_wndTestWaterFlowRate);
	DDX_Control(pDX, IDC_EDIT_TEST_HOT_WATER, m_wndTestHWT);
	DDX_Control(pDX, IDC_EDIT_TEST_FAN_DRIVER, m_wndTestFanDriverPower);
	DDX_Control(pDX, IDC_EDIT_TEST_DRY_BULB, m_wndTestDBT);
	DDX_Control(pDX, IDC_EDIT_TEST_COLD_WATER, m_wndTestCWT);
	DDX_Control(pDX, IDC_EDIT_TEST_BAROMETRIC_PRESSURE, m_wndTestBarometricPressure);
	DDX_Control(pDX, IDC_COMBO_TEST_LIST, m_wndTestList);
	DDX_Control(pDX, IDC_COMBO_DATA_FILES, m_wndDataFileList);
	DDX_Text(pDX, IDC_STATIC_UNITS_COLD_WATER_DEV, m_strUnitsCWTDev);
	DDX_Text(pDX, IDC_STATIC_UNITS_WET_BULB, m_strUnitsWBT);
	DDX_Text(pDX, IDC_STATIC_UNITS_WATER_FLOW, m_strUnitsWaterFlowRate);
	DDX_Text(pDX, IDC_STATIC_UNITS_HOT_WATER, m_strUnitsHWT);
	DDX_Text(pDX, IDC_STATIC_UNITS_FAN_DRIVER, m_strUnitsFanDriverPower);
	DDX_Text(pDX, IDC_STATIC_UNITS_DRY_BULB, m_strUnitsDBT);
	DDX_Text(pDX, IDC_STATIC_UNITS_COLD_WATER, m_strUnitsCWT);
	DDX_Text(pDX, IDC_STATIC_UNITS_BAROMETRIC_PRESSURE, m_strUnitsBarometricPressure);
	DDX_Text(pDX, IDC_STATIC_UNITS_ADJUSTED_FLOW, m_strUnitsAdjustedFlow);
	DDX_Text(pDX, IDC_STATIC_UNITS_PREDICTED_FLOW, m_strUnitsPredictedFlow);
	DDX_Text(pDX, IDC_STATIC_PROJECT, m_strProjectName);
	DDX_Text(pDX, IDC_STATIC_OWNER, m_strTowerOwner);
	DDX_Text(pDX, IDC_STATIC_MANUFACTURER, m_strTowerManufacturer);
	DDX_Text(pDX, IDC_STATIC_LOCATION, m_strTowerLocation);
	//}}AFX_DATA_MAP

	if (pDX->m_bSaveAndValidate)
	{
		//
		// Get Values
		//
		//DDX_Text(pDX, IDC_EDIT_DESIGN_WATER_FLOW, m_dblDesignWaterFlowRate);
		//DDX_Text(pDX, IDC_EDIT_DESIGN_HOT_WATER, m_dblDesignHWT);
		//DDX_Text(pDX, IDC_EDIT_DESIGN_COLD_WATER, m_dblDesignCWT);
		//DDX_Text(pDX, IDC_EDIT_DESIGN_DRY_BULB, m_dblDesignDBT);
		//DDX_Text(pDX, IDC_EDIT_DESIGN_WET_BULB, m_dblDesignWBT);
		//DDX_Text(pDX, IDC_EDIT_DESIGN_BAROMETRIC_PRESSURE, m_dblDesignBarometricPressure);
		//DDX_Text(pDX, IDC_EDIT_DESIGN_FAN_DRIVER, m_dblDesignFanDriverPower);
		//DDX_Text(pDX, IDC_EDIT_DESIGN_LG, m_dblDesignLG);

		//
		// only get the test data that was changed by the user
		//
		if (m_wndTestWaterFlowRate.GetModify())
			DDX_Text(pDX, IDC_EDIT_TEST_WATER_FLOW, m_dblTestWaterFlowRate);
		if (m_wndTestHWT.GetModify())
			DDX_Text(pDX, IDC_EDIT_TEST_HOT_WATER, m_dblTestHWT);
		if (m_wndTestCWT.GetModify())
			DDX_Text(pDX, IDC_EDIT_TEST_COLD_WATER, m_dblTestCWT);
		if (m_wndTestDBT.GetModify())
			DDX_Text(pDX, IDC_EDIT_TEST_DRY_BULB, m_dblTestDBT);
		if (m_wndTestWBT.GetModify())
			DDX_Text(pDX, IDC_EDIT_TEST_WET_BULB, m_dblTestWBT);
		if (m_wndTestBarometricPressure.GetModify())	
			DDX_Text(pDX, IDC_EDIT_TEST_BAROMETRIC_PRESSURE, m_dblTestBarometricPressure);
		if (m_wndTestFanDriverPower.GetModify())
			DDX_Text(pDX, IDC_EDIT_TEST_FAN_DRIVER, m_dblTestFanDriverPower);

#ifdef _PAGE_FOUR_TOWER_TYPE
		if (GetCheckedRadioButton(IDC_RADIO_INDUCED, IDC_RADIO_FORCED) == IDC_RADIO_INDUCED)
		{
			m_filePerfData.m_designData.m_fnSetInducedFlow(true);
		}
		else
		{
			m_filePerfData.m_designData.m_fnSetInducedFlow(false);
		}
#endif // _PAGE_FOUR_TOWER_TYPE
	}

	else
	{
		//
		// Set Values
		//
		double dblTemp;

		dblTemp = truncit(m_dblPredictedFlow			, 1);
		DDX_Text(pDX, IDC_EDIT_PREDICTED_FLOW, dblTemp);
		dblTemp = truncit(m_dblAdjustedFlow				, 1);
		DDX_Text(pDX, IDC_EDIT_ADJUSTED_FLOW, dblTemp);
		dblTemp = truncit(m_dblCWTDev					, 2);
        if (m_bCWTDevCalcOk)
        {
            DDX_Text(pDX, IDC_EDIT_COLD_WATER_DEV, dblTemp);
        }
		dblTemp = truncit(m_dblCapability				, 2);
		DDX_Text(pDX, IDC_EDIT_CAPABILITY, dblTemp);
		dblTemp = truncit(m_dblTestLG					, 4);
		DDX_Text(pDX, IDC_EDIT_TEST_LG, dblTemp);

		dblTemp = truncit(m_dblDesignWaterFlowRate		, 1);
		DDX_Text(pDX, IDC_EDIT_DESIGN_WATER_FLOW, dblTemp);
		dblTemp = truncit(m_dblDesignHWT				, 2);
		DDX_Text(pDX, IDC_EDIT_DESIGN_HOT_WATER, dblTemp);
		dblTemp = truncit(m_dblDesignCWT				, 2);
		DDX_Text(pDX, IDC_EDIT_DESIGN_COLD_WATER, dblTemp);
		dblTemp = truncit(m_dblDesignDBT				, 2);
		DDX_Text(pDX, IDC_EDIT_DESIGN_DRY_BULB, dblTemp);
		dblTemp = truncit(m_dblDesignWBT				, 2);
		DDX_Text(pDX, IDC_EDIT_DESIGN_WET_BULB, dblTemp);
		dblTemp = truncit(m_dblDesignBarometricPressure	, 4);
		DDX_Text(pDX, IDC_EDIT_DESIGN_BAROMETRIC_PRESSURE, dblTemp);
		dblTemp = truncit(m_dblDesignFanDriverPower		, 4);
		DDX_Text(pDX, IDC_EDIT_DESIGN_FAN_DRIVER, dblTemp);
		dblTemp = truncit(m_dblDesignLG					, 4);
		DDX_Text(pDX, IDC_EDIT_DESIGN_LG, dblTemp);

		dblTemp = truncit(m_dblTestWaterFlowRate		, 1);
		DDX_Text(pDX, IDC_EDIT_TEST_WATER_FLOW, dblTemp);
		dblTemp = truncit(m_dblTestHWT					, 2);
		DDX_Text(pDX, IDC_EDIT_TEST_HOT_WATER, dblTemp);
		dblTemp = truncit(m_dblTestCWT					, 2);
		DDX_Text(pDX, IDC_EDIT_TEST_COLD_WATER, dblTemp);
		dblTemp = truncit(m_dblTestDBT					, 2);
		DDX_Text(pDX, IDC_EDIT_TEST_DRY_BULB, dblTemp);
		dblTemp = truncit(m_dblTestWBT					, 2);
		DDX_Text(pDX, IDC_EDIT_TEST_WET_BULB, dblTemp);
		dblTemp = truncit(m_dblTestBarometricPressure	, 4);
		DDX_Text(pDX, IDC_EDIT_TEST_BAROMETRIC_PRESSURE, dblTemp);
		dblTemp = truncit(m_dblTestFanDriverPower		, 4);
		DDX_Text(pDX, IDC_EDIT_TEST_FAN_DRIVER, dblTemp);

		m_wndTestWaterFlowRate.SetModify(false);
		m_wndTestHWT.SetModify(false);		
		m_wndTestCWT.SetModify(false);
		m_wndTestDBT.SetModify(false);
		m_wndTestWBT.SetModify(false);
		m_wndTestBarometricPressure.SetModify(false);
		m_wndTestFanDriverPower.SetModify(false);

#ifdef _PAGE_FOUR_TOWER_TYPE
		if (m_filePerfData.m_designData.m_fnGetInducedFlow())
		{
			CheckRadioButton(IDC_RADIO_INDUCED, IDC_RADIO_FORCED, IDC_RADIO_INDUCED);
		}
		else
		{
			CheckRadioButton(IDC_RADIO_INDUCED, IDC_RADIO_FORCED, IDC_RADIO_FORCED);
		}
#endif // _PAGE_FOUR_TOWER_TYPE

	}
}


BEGIN_MESSAGE_MAP(TPropPageFour, cdxCSizingPropPage)
	//{{AFX_MSG_MAP(TPropPageFour)
	ON_BN_CLICKED(IDC_BUTTON_EDIT, OnButtonEdit)
	ON_BN_CLICKED(IDC_BUTTON_NEW, OnButtonNew)
	ON_BN_CLICKED(IDC_BUTTON_VIEW_GRAPH, OnButtonViewGraph)
	ON_CBN_SELCHANGE(IDC_COMBO_DATA_FILES, OnSelChangeComboDataFiles)
	ON_CBN_DROPDOWN(IDC_COMBO_DATA_FILES, OnFileListDropDown)
	ON_CBN_SELCHANGE(IDC_COMBO_TEST_LIST, OnSelChangeComboTestList)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_TEST_DATA, OnButtonSaveTestData)
	ON_EN_CHANGE(IDC_EDIT_TEST_BAROMETRIC_PRESSURE, OnChangeEditTestBarometricPressure)
	ON_EN_CHANGE(IDC_EDIT_TEST_COLD_WATER, OnChangeEditTestColdWater)
	ON_EN_CHANGE(IDC_EDIT_TEST_DRY_BULB, OnChangeEditTestDryBulb)
	ON_EN_CHANGE(IDC_EDIT_TEST_FAN_DRIVER, OnChangeEditTestFanDriver)
	ON_EN_CHANGE(IDC_EDIT_TEST_HOT_WATER, OnChangeEditTestHotWater)
	ON_EN_CHANGE(IDC_EDIT_TEST_WATER_FLOW, OnChangeEditTestWaterFlow)
	ON_EN_CHANGE(IDC_EDIT_TEST_WET_BULB, OnChangeEditTestWetBulb)
	ON_COMMAND(ID_FILE_PRINT, OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
	ON_BN_CLICKED(IDC_PERF_BUTTON, OnPerformanceButtonRecalc)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, OnFilePrint)
#ifdef _PAGE_FOUR_TOWER_TYPE
	ON_BN_CLICKED(IDC_RADIO_INDUCED, OnButtonInduced)
	ON_BN_CLICKED(IDC_RADIO_FORCED, OnButtonForced)
#endif // _PAGE_FOUR_TOWER_TYPE
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

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
void TPropPageFour::SetDefaultValues()
{
    if (!TPropPageBase::metricflag)
    {
    }
    else
    {
    }
}
//---------------------------------------------------------------------
// END: TPropPageFour::SetDefaultValues
//---------------------------------------------------------------------

//---------------------------------------------------------------------
// FUNCTION    : TPropPageFour::ChangeUnits
//
// DESCRIPTION : None.
//
// INPUT       : None.
//
// RETURN (S)  : None.
//---------------------------------------------------------------------
// REMARKS     : None.
//---------------------------------------------------------------------
void TPropPageFour::ChangeUnits() 
{
	//---------------------------------------------------------------------
	// Do not retrieve the data because it will be retrieved incorrectly
	//---------------------------------------------------------------------
    UpdateUnits();
	if (IsWindow(m_hWnd))
	{
		m_fnInitControls();
		//DDP Reset the selected Test????
		UpdateData(false);
		OnPerformanceButtonRecalc();
	}
}
//---------------------------------------------------------------------
// END: TPropPageFour::ChangeUnits
//---------------------------------------------------------------------


//---------------------------------------------------------------------
// FUNCTION    : TPropPageFour::UpdateUnits
//
// DESCRIPTION : None.
//
// INPUT       : None.
//
// RETURN (S)  : None.
//---------------------------------------------------------------------
// REMARKS     : None.
//---------------------------------------------------------------------
void TPropPageFour::UpdateUnits() 
{
    if (!TPropPageBase::metricflag)
    {
		m_strUnitsCWTDev				= _T("°F");
		m_strUnitsWBT					= _T("°F");
		m_strUnitsWaterFlowRate			= _T("gpm");
		m_strUnitsHWT					= _T("°F");
		m_strUnitsFanDriverPower		= _T("bhp");
		m_strUnitsDBT					= _T("°F");
		m_strUnitsCWT					= _T("°F");
		m_strUnitsBarometricPressure	= _T("in. hg.");
		m_strUnitsAdjustedFlow			= _T("gpm");
		m_strUnitsPredictedFlow			= _T("gpm");
    }
    else
    {
		m_strUnitsCWTDev				= _T("°C");
		m_strUnitsWBT					= _T("°C");
		m_strUnitsWaterFlowRate			= _T("l/s");
		m_strUnitsHWT					= _T("°C");
		m_strUnitsFanDriverPower		= _T("kW");
		m_strUnitsDBT					= _T("°C");
		m_strUnitsCWT					= _T("°C");
		m_strUnitsBarometricPressure	= _T("kPa");
		m_strUnitsAdjustedFlow			= _T("l/s");
		m_strUnitsPredictedFlow			= _T("l/s");
    }

	//m_filePerfData.m_fnSetUnits(!TPropPageBase::metricflag);
}
//---------------------------------------------------------------------
// END: TPropPageFour::UpdateUnits
//---------------------------------------------------------------------

void TPropPageFour::OnPerformanceButtonRecalc() 
{
	UpdateData();

	if (this->m_iCurrentTest >= 0)
	{
		//
		// Validate current data
		//
		if (!ValidateData(true) || !ValidateData(false))
			return;
/***
		if  (
			(m_dblDesignLG				== 0) ||
			(m_dblDesignWaterFlowRate	== 0) ||
			(m_dblDesignFanDriverPower	== 0) ||
			(m_dblTestWaterFlowRate		== 0) ||
			(m_dblTestHWT				== 0) ||
			(m_dblTestCWT				== 0) ||
			(m_dblTestDBT				== 0) ||
			(m_dblTestWBT				== 0) ||
			(m_dblTestFanDriverPower	== 0)
			)
		{
			AfxMessageBox("Your test or design data is out of range for calculating % Capability.");
			GetDlgItem(IDC_EDIT_PREDICTED_FLOW)->SetWindowText("");
			GetDlgItem(IDC_EDIT_ADJUSTED_FLOW)->SetWindowText("");
			GetDlgItem(IDC_EDIT_CAPABILITY)->SetWindowText("");
			GetDlgItem(IDC_EDIT_TEST_LG)->SetWindowText("");
			m_dblCapability = 0;
			m_dblPredictedFlow = 0.0;
			m_dblAdjustedFlow = 0.0;
			return;
		}
***/
		m_fnCalcCapability();
	}

	UpdateData(false);
}

BOOL TPropPageFour::OnInitDialog() 
{
	cdxCSizingPropPage::OnInitDialog();
    SetHelpID(Performance_Help);
	UpdateUnits();
	m_fnInitControls();

#ifdef _DEMO_VERSION_NO_SAVE    
	GetDlgItem(IDC_BUTTON_SAVE_TEST_DATA)->EnableWindow(false);
#endif
#if _DEMO_VERSION
	GetDlgItem(IDC_BUTTON_NEW)->EnableWindow(false);
#endif

	m_fnFillFileList();
	
	return TRUE;
}

int TPropPageFour::m_fnFillFileList()
{
	//
	// Get list of data Files
	//
	CString strSelectedFileName("");
	
	// Empty combobox but save current selection (if any)
	if (m_wndDataFileList.GetCount() > 0)
	{
		int iSel(m_wndDataFileList.GetCurSel());
		if (iSel != CB_ERR)
		{
			m_wndDataFileList.GetLBText(iSel, strSelectedFileName);
		}
		m_wndDataFileList.ResetContent();
	}

	CString strPath;
	GetModuleFileName(NULL, strPath.GetBuffer(MAX_PATH+1), MAX_PATH);
	strPath.ReleaseBuffer();
	strPath = strPath.Left(strPath.ReverseFind(_T('\\'))+1);
	strPath += _T("*.bbp");

	// This code works on WinME (and 98?) ...
	WIN32_FIND_DATA fd;
	HANDLE handle = FindFirstFile(strPath, &fd);

	int nFiles(-1);
	bool bFound(handle != INVALID_HANDLE_VALUE);
	while (bFound)
	{
		m_wndDataFileList.AddString(fd.cFileName);
		bFound = (FindNextFile(handle, &fd) != 0);
		nFiles++;
	}
	if (bFound) 
	{
		FindClose(handle); 
	}

	// ... But this code doesn't (only on NT/2000/XP)
//	nFiles = m_wndDataFileList.Dir(DDL_READWRITE | DDL_READONLY, strPath);

	// Restore any previous selection
	if (nFiles >= 0)
	{
		nFiles++;
		if (strSelectedFileName != "")
			m_wndDataFileList.SelectString(-1, strSelectedFileName);
	}

	return nFiles;
}

void TPropPageFour::OnFileListDropDown()
{
	m_fnFillFileList();
}

void TPropPageFour::OnButtonEdit() 
{
	TPerformanceDataDlg dlgEditData;

	if (m_filePerfData.m_strFileName.GetLength() > 0)
	{
		UpdateData();

		dlgEditData.m_designData = m_filePerfData.m_designData;
		dlgEditData.m_bIPUnits   = !TPropPageBase::metricflag;

		if (dlgEditData.DoModal() == IDOK)
		{
			int iTest = m_wndTestList.GetCurSel();
			if (m_filePerfData.m_fnCanModify())
			{
				m_filePerfData.m_designData = dlgEditData.m_designData;
#ifndef _DEMO_VERSION_NO_SAVE
				m_filePerfData.m_fnSaveFile();
#endif
			}
			m_fnInitControls();
			if (iTest > 0)
			{
				m_wndTestList.SetCurSel(iTest);
				OnSelChangeComboTestList();
			}

			UpdateData(false);
		}
	}
	else
	{
		MessageBox("You must first select or create a tower design file.");
	}
}

void TPropPageFour::OnButtonNew() 
{
	TNewPerformanceDataFileDlg dlgNewFile;

	if (dlgNewFile.DoModal() == IDOK)
	{
		CString strNewFileName;
		CString strCopyFileName;
		CString strPath;
		TTowerPerformanceFile fileNewData;
		TTowerPerformanceFile fileCopyData;

		GetModuleFileName(NULL, strPath.GetBuffer(MAX_PATH+1), MAX_PATH);
		strPath.ReleaseBuffer();
		strPath = strPath.Left(strPath.ReverseFind('\\')+1);

	
		strNewFileName = strPath + dlgNewFile.m_strNewFileName;
		strCopyFileName = strPath + dlgNewFile.m_strCopyFile;

		fileCopyData.m_fnOpenFile(strCopyFileName);
		fileCopyData.m_fnSetUnits(!TPropPageBase::metricflag);
		fileNewData = fileCopyData;
		fileNewData.m_strFileName = strNewFileName;
		fileNewData.m_arrayTests.RemoveAll();
#ifndef _DEMO_VERSION_NO_SAVE
		fileNewData.m_fnSaveFile();
#endif

		m_filePerfData = fileNewData;

		m_fnInitControls();

		//
		// Get list of data Files
		//
		m_fnFillFileList();
		/***
		while (m_wndDataFileList.GetCount() > 0)
		{
			m_wndDataFileList.DeleteString(0);
		}

		strPath += "*.bbp";
		WIN32_FIND_DATA fd;
		HANDLE handle = FindFirstFile(strPath, &fd);
		if (handle != INVALID_HANDLE_VALUE) 
		{
			bool bFound = true;
			while (bFound)
			{
				m_wndDataFileList.AddString(fd.cFileName);
				bFound = FindNextFile(handle, &fd);
			}
			FindClose(handle);
		}
		**/
		m_wndDataFileList.SelectString(0, dlgNewFile.m_strNewFileName);
	}
}

void TPropPageFour::OnButtonSaveTestData() 
{
	UpdateData();
	if ((m_filePerfData.m_strFileName.GetLength() > 0)  && (m_iCurrentTest > -1))
	{
		m_filePerfData.m_arrayTests[m_iCurrentTest].m_fnSetBarometricPressure(m_dblTestBarometricPressure, !TPropPageBase::metricflag);
		m_filePerfData.m_arrayTests[m_iCurrentTest].m_fnSetCWT(m_dblTestCWT, !TPropPageBase::metricflag);
		m_filePerfData.m_arrayTests[m_iCurrentTest].m_fnSetDBT(m_dblTestDBT, !TPropPageBase::metricflag);
		m_filePerfData.m_arrayTests[m_iCurrentTest].m_fnSetFanDriverPower(m_dblTestFanDriverPower, !TPropPageBase::metricflag);
		m_filePerfData.m_arrayTests[m_iCurrentTest].m_fnSetHWT(m_dblTestHWT, !TPropPageBase::metricflag);
		m_filePerfData.m_arrayTests[m_iCurrentTest].m_fnSetWaterFlowRate(m_dblTestWaterFlowRate, !TPropPageBase::metricflag);
		m_filePerfData.m_arrayTests[m_iCurrentTest].m_fnSetWBT(m_dblTestWBT, !TPropPageBase::metricflag);
		m_filePerfData.m_fnSaveFile();
		m_bTestDataChanged = false;
#ifndef _DEMO_VERSION_NO_SAVE
		m_filePerfData.m_fnSaveFile();
#endif
	}
	else
	{
		MessageBox("There is no test data to save.");
	}
}

void TPropPageFour::OnButtonViewGraph() 
{
	TPerformanceCurveDlg dlgPerfCurvs;

	if (m_filePerfData.m_strFileName.GetLength() > 0)
	{
		int iSel = m_wndTestList.GetCurSel();
		if (iSel > 0)
		{
			//
			// Check that the proper data is specified.
			//
	
			if (m_filePerfData.m_designData.m_fnGetFlowCnt() < MIN_FLOW_COUNT)
			{
				CString cstrMsg;
				cstrMsg.Format("You must specify a minimum of %d flows in the Tower Design Data to calculate Tower Capability.", MIN_FLOW_COUNT);
				MessageBox(cstrMsg);
				return;
			}
			else if (m_filePerfData.m_designData.m_fnGetRangeCnt() < MIN_RANGE_COUNT)
			{
				CString cstrMsg;
				cstrMsg.Format("You must specify a minimum of %d ranges in the Tower Design Data to calculate Tower Capability.", MIN_RANGE_COUNT);
				MessageBox(cstrMsg);
				return;
			}
			else if (m_filePerfData.m_designData.m_fnGetWBTCnt(0) < MIN_WBT_COUNT)
			{
				CString cstrMsg;
				cstrMsg.Format("You must specify a minimum of %d WBT values in the Tower Design Data to calculate Tower Capability.", MIN_WBT_COUNT);
				MessageBox(cstrMsg);
				return;
			}
			
			OnButtonSaveTestData();
			/***
			if  (
				(m_dblDesignLG				== 0) ||
				(m_dblDesignWaterFlowRate	== 0) ||
				(m_dblDesignFanDriverPower	== 0) ||
				(m_dblTestWaterFlowRate		== 0) ||
				(m_dblTestHWT				== 0) ||
				(m_dblTestCWT				== 0) ||
				(m_dblTestDBT				== 0) ||
				(m_dblTestWBT				== 0) ||
				(m_dblTestFanDriverPower	== 0)
				)
			{
				AfxMessageBox("Your test or design data is out of range for creating the crossplots.");
				return;
			}
***/
		}
		
		if (!ValidateData(true) || !ValidateData(false))
			return;

		dlgPerfCurvs.m_bIPUnits		= !TPropPageBase::metricflag;
		dlgPerfCurvs.m_designData	= m_filePerfData.m_designData;
		if (m_iCurrentTest >= 0)
		{
			dlgPerfCurvs.m_testData		= m_filePerfData.m_arrayTests[m_iCurrentTest];
			dlgPerfCurvs.m_bTestPoint	= true;
		}
		else
		{
			dlgPerfCurvs.m_bTestPoint	= false;
		}

		// Returns -1 if graph display fails
		if (dlgPerfCurvs.DoModal() == -1)
		{
			CString szMsg(SZ_APP_NAME);
			szMsg += " does not appear to be completely installed.  Please exit the application, reinstall and try again.";
			AfxMessageBox(szMsg, MB_ICONEXCLAMATION);
		}
	}
	else
	{
		MessageBox("You must first select or create a tower design file.");
	}
}




void TPropPageFour::OnSelChangeComboDataFiles() 
{
	CString strFileName;
	CString strPath;

	GetModuleFileName(NULL, strPath.GetBuffer(MAX_PATH+1), MAX_PATH);
	strPath.ReleaseBuffer();
	strPath = strPath.Left(strPath.ReverseFind('\\')+1);

	int iSel = m_wndDataFileList.GetCurSel();
	if (iSel > -1) // DDP 2-9-01
	{
		m_wndDataFileList.GetLBText(iSel, strFileName);
		
		strFileName = strPath + strFileName;
		m_filePerfData.m_fnOpenFile(strFileName);
		//m_filePerfData.m_fnSetUnits(!TPropPageBase::metricflag);

		if (m_filePerfData.m_designData.m_fnGetUnits())
		{
			((TPropPageBase*)AfxGetMainWnd())->OnEditCut();
		}
		else
		{
			((TPropPageBase*)AfxGetMainWnd())->OnEditCopy();
		}
		//m_filePerfData.m_fnOpenFile(strFileName);

		m_bTestDataChanged	= false;
		m_iCurrentTest		= -1;

		m_fnInitControls();
	}
}

void TPropPageFour::m_fnInitControls()
{
	m_strProjectName				= m_filePerfData.m_designData.m_fnGetProjectName();
	m_strTowerOwner					= m_filePerfData.m_designData.m_fnGetTowerOwner();
	m_strTowerManufacturer			= m_filePerfData.m_designData.m_fnGetTowerManufacturer();
	m_strTowerLocation				= m_filePerfData.m_designData.m_fnGetTowerLocation();
	m_dblDesignBarometricPressure	= m_filePerfData.m_designData.m_fnGetBarometricPressure(!TPropPageBase::metricflag);
	m_dblDesignCWT					= m_filePerfData.m_designData.m_fnGetCWT(!TPropPageBase::metricflag);
	m_dblDesignDBT					= m_filePerfData.m_designData.m_fnGetDBT(!TPropPageBase::metricflag);
	m_dblDesignFanDriverPower		= m_filePerfData.m_designData.m_fnGetFanDriverPower(!TPropPageBase::metricflag);
	m_dblDesignHWT					= m_filePerfData.m_designData.m_fnGetHWT(!TPropPageBase::metricflag);
	m_dblDesignWaterFlowRate		= m_filePerfData.m_designData.m_fnGetWaterFlowRate(!TPropPageBase::metricflag);
	m_dblDesignWBT					= m_filePerfData.m_designData.m_fnGetWBT(!TPropPageBase::metricflag);
	m_dblDesignLG					= m_filePerfData.m_designData.m_fnGetLG(!TPropPageBase::metricflag);

	m_dblTestBarometricPressure		= 0.0;
	m_dblTestCWT					= 0.0;
	m_dblTestDBT					= 0.0;
	m_dblTestFanDriverPower			= 0.0;
	m_dblTestHWT					= 0.0;
	m_dblTestWaterFlowRate			= 0.0;
	m_dblTestWBT					= 0.0;
	m_dblTestLG						= 0.0;

	m_dblCWTDev						= 0.0;
	m_dblCapability					= 0.0;
	m_dblPredictedFlow				= 0.0;
	m_dblAdjustedFlow				= 0.0;

	m_bTestDataChanged				= false;
    m_bCWTDevCalcOk                 = true;
	m_iCurrentTest					= -1;

	m_wndTestList.ResetContent();
	m_wndTestList.AddString("New Test");
	m_strPreviewSubject				= "";

	for (int iIndex = 0; iIndex <= m_filePerfData.m_arrayTests.GetUpperBound(); iIndex++)
	{
		m_wndTestList.AddString(m_filePerfData.m_arrayTests[iIndex].m_fnGetName());
	}

	UpdateData(false);
}

void TPropPageFour::OnSelChangeComboTestList() 
{
	CString strTemp;
	bool bClearCap = false;

	if (m_iCurrentTest != -1)
	{
		UpdateData();
	}

#ifndef _DEMO_VERSION_NO_SAVE
	if (m_bTestDataChanged && (m_iCurrentTest > 0))
	{
		if (MessageBox("Do you want to save the changes to the current test data?", "Test Data", MB_YESNO) == IDYES)
		{
			OnButtonSaveTestData();
		}
	}
#endif

	m_dblTestBarometricPressure = 0.0;
	m_dblTestCWT				= 0.0;
	m_dblTestDBT				= 0.0;
	m_dblTestFanDriverPower		= 0.0;
	m_dblTestHWT				= 0.0;
	m_dblTestWaterFlowRate		= 0.0;
	m_dblTestWBT				= 0.0;
	m_dblTestLG					= 0.0;
	m_dblCWTDev					= 0.0;
	m_dblCapability				= 0.0;
	m_dblPredictedFlow			= 0.0;
	m_dblAdjustedFlow			= 0.0;

	if (m_wndTestList.GetCurSel() == 0)
	{
		TPerformanceTest stTest;
		TNewTestDlg dlgNewTest;

		if (m_filePerfData.m_strFileName.GetLength() > 0)
		{
			if (dlgNewTest.DoModal() == IDOK)
			{
				bClearCap = true;

				//
				// Check and see if the Test exists
				//
				bool bTestFound = false;
				for (int iTestIndex = 0; iTestIndex <= m_filePerfData.m_arrayTests.GetUpperBound(); iTestIndex++)
				{
					if (dlgNewTest.m_strTestName.CompareNoCase(m_filePerfData.m_arrayTests[iTestIndex].m_fnGetName()) == 0)
					{
						bTestFound = true;
						break;
					}
				}

				if (!bTestFound)
				{
					stTest.m_fnSetName(dlgNewTest.m_strTestName);
					stTest.m_fnSetUnits(!TPropPageBase::metricflag);

					m_iCurrentTest = m_filePerfData.m_arrayTests.Add(stTest);
					m_wndTestList.AddString(m_filePerfData.m_arrayTests[m_iCurrentTest].m_fnGetName());
					m_wndTestList.SelectString(0, m_filePerfData.m_arrayTests[m_iCurrentTest].m_fnGetName());
					m_bTestDataChanged = false;
				}
				else
				{
					AfxMessageBox("The test name already exists.  Please choose a unique name.");
					m_iCurrentTest = -1;
					m_wndTestList.SetCurSel(-1);
				}
			}
			else if (m_iCurrentTest != -1)
			{
				m_dblTestBarometricPressure = m_filePerfData.m_arrayTests[m_iCurrentTest].m_fnGetBarometricPressure(!TPropPageBase::metricflag);
				m_dblTestCWT				= m_filePerfData.m_arrayTests[m_iCurrentTest].m_fnGetCWT(!TPropPageBase::metricflag);
				m_dblTestDBT				= m_filePerfData.m_arrayTests[m_iCurrentTest].m_fnGetDBT(!TPropPageBase::metricflag);
				m_dblTestFanDriverPower		= m_filePerfData.m_arrayTests[m_iCurrentTest].m_fnGetFanDriverPower(!TPropPageBase::metricflag);
				m_dblTestHWT				= m_filePerfData.m_arrayTests[m_iCurrentTest].m_fnGetHWT(!TPropPageBase::metricflag);
				m_dblTestWaterFlowRate		= m_filePerfData.m_arrayTests[m_iCurrentTest].m_fnGetWaterFlowRate(!TPropPageBase::metricflag);
				m_dblTestWBT				= m_filePerfData.m_arrayTests[m_iCurrentTest].m_fnGetWBT(!TPropPageBase::metricflag);

				//
				// Validate current data
				//
				if (!ValidateData(true, false) || !ValidateData(false, false))
					/***
				if  (
					(m_dblDesignLG				== 0) ||
					(m_dblDesignWaterFlowRate	== 0) ||
					(m_dblDesignFanDriverPower	== 0) ||
					(m_dblTestWaterFlowRate		== 0) ||
					(m_dblTestHWT				== 0) ||
					(m_dblTestCWT				== 0) ||
					(m_dblTestDBT				== 0) ||
					(m_dblTestWBT				== 0) ||
					(m_dblTestFanDriverPower	== 0)
					)
					***/
				{
					bClearCap = true;
				}
				else
				{
					m_fnCalcCapability();
				}
				m_wndTestList.SelectString(0, m_filePerfData.m_arrayTests[m_iCurrentTest].m_fnGetName());
			}
			else
			{
				m_wndTestList.SetCurSel(-1);
			}
		}
		else
		{
			MessageBox("You must first select or create a tower design file.");
		}
	}
	else if (m_wndTestList.GetCurSel() != LB_ERR)
	{
		m_wndTestList.GetLBText(m_wndTestList.GetCurSel(), strTemp);

		for (int iIndex = 0; iIndex <= m_filePerfData.m_arrayTests.GetUpperBound(); iIndex++)
		{
			if (m_filePerfData.m_arrayTests[iIndex].m_fnGetName() == strTemp)
			{
				m_dblTestBarometricPressure = m_filePerfData.m_arrayTests[iIndex].m_fnGetBarometricPressure(!TPropPageBase::metricflag);
				m_dblTestCWT				= m_filePerfData.m_arrayTests[iIndex].m_fnGetCWT(!TPropPageBase::metricflag);
				m_dblTestDBT				= m_filePerfData.m_arrayTests[iIndex].m_fnGetDBT(!TPropPageBase::metricflag);
				m_dblTestFanDriverPower		= m_filePerfData.m_arrayTests[iIndex].m_fnGetFanDriverPower(!TPropPageBase::metricflag);
				m_dblTestHWT				= m_filePerfData.m_arrayTests[iIndex].m_fnGetHWT(!TPropPageBase::metricflag);
				m_dblTestWaterFlowRate		= m_filePerfData.m_arrayTests[iIndex].m_fnGetWaterFlowRate(!TPropPageBase::metricflag);
				m_dblTestWBT				= m_filePerfData.m_arrayTests[iIndex].m_fnGetWBT(!TPropPageBase::metricflag);

				m_iCurrentTest = iIndex;

				//
				// Validate current data
				//
				if (!ValidateData(true, false) || !ValidateData(false, false))
					/***
				if  (
					(m_dblDesignLG				== 0) ||
					(m_dblDesignWaterFlowRate	== 0) ||
					(m_dblDesignFanDriverPower	== 0) ||
					(m_dblTestWaterFlowRate		== 0) ||
					(m_dblTestHWT				== 0) ||
					(m_dblTestCWT				== 0) ||
					(m_dblTestDBT				== 0) ||
					(m_dblTestWBT				== 0) ||
					(m_dblTestFanDriverPower	== 0)
					)
					***/
				{
					bClearCap = true;
				}
				else
				{
					m_fnCalcCapability();
				}
			}
		}
	}

	UpdateData(false);
	if (bClearCap)
	{
		GetDlgItem(IDC_EDIT_PREDICTED_FLOW)->SetWindowText("");
		GetDlgItem(IDC_EDIT_ADJUSTED_FLOW)->SetWindowText("");
		GetDlgItem(IDC_EDIT_COLD_WATER_DEV)->SetWindowText("");
		GetDlgItem(IDC_EDIT_CAPABILITY)->SetWindowText("");
		GetDlgItem(IDC_EDIT_TEST_LG)->SetWindowText("");
	}
}

void TPropPageFour::m_fnCalcCapability(bool bCalcWBTDeviation)
{
	CWaitCursor  curWait;
	double dblAdjustedFlow;
	double dblDesignAirDensity;
	double dblTestAirDensity;
	double dblDesignSpecificVolume;
	double dblTestSpecificVolume;

	double LWBTnew	= 0;
	double DenOutT	= 0;
	double SVOutT	= 0;
	double HLWBT	= 0; 

	if (!TPropPageBase::metricflag)
	{
		dblDesignAirDensity		= CalcDensityIP(   calcPressureC(m_dblDesignBarometricPressure), m_dblDesignDBT, m_dblDesignWBT);
		dblTestAirDensity		= CalcDensityIP(   calcPressureC(m_dblTestBarometricPressure)  , m_dblTestDBT  , m_dblTestWBT);
		dblDesignSpecificVolume = SpecificVolumeIP(calcPressureC(m_dblDesignBarometricPressure), m_dblDesignDBT, m_dblDesignWBT);
		dblTestSpecificVolume	= SpecificVolumeIP(calcPressureC(m_dblTestBarometricPressure)  , m_dblTestDBT  , m_dblTestWBT);
	}
	else
	{
		dblDesignAirDensity		= CalcDensitySI(   m_dblDesignBarometricPressure, m_dblDesignDBT, m_dblDesignWBT);
		dblTestAirDensity		= CalcDensitySI(   m_dblTestBarometricPressure  , m_dblTestDBT  , m_dblTestWBT);
		dblDesignSpecificVolume = SpecificVolumeSI(m_dblDesignBarometricPressure, m_dblDesignDBT, m_dblDesignWBT);
		dblTestSpecificVolume	= SpecificVolumeSI(m_dblTestBarometricPressure  , m_dblTestDBT  , m_dblTestWBT);
	}

	m_dblTestLG		= CalcTestLG(m_dblDesignLG, m_dblDesignWaterFlowRate, m_dblTestWaterFlowRate, m_dblDesignFanDriverPower, m_dblTestFanDriverPower, dblDesignAirDensity, dblTestAirDensity, dblDesignSpecificVolume, dblTestSpecificVolume);
	DetermineAdjTestFlow(!TPropPageBase::metricflag, (int)m_filePerfData.m_designData.m_fnGetInducedFlow(), m_dblDesignHWT, m_dblDesignCWT, m_dblDesignWBT, m_dblDesignDBT, m_dblDesignBarometricPressure, m_dblDesignWaterFlowRate, m_dblDesignFanDriverPower, m_dblDesignLG, m_dblTestHWT, m_dblTestCWT, m_dblTestWBT, m_dblTestDBT, m_dblTestBarometricPressure, m_dblTestWaterFlowRate, m_dblTestFanDriverPower, LWBTnew, DenOutT, SVOutT, HLWBT, dblAdjustedFlow, m_dblTestLG);

	if (m_filePerfData.m_designData.m_fnGetFlowCnt() < MIN_FLOW_COUNT)
	{
		CString cstrMsg;
		cstrMsg.Format("You must specify a minimum of %d flows in the Tower Design Data to calculate Tower Capability.", MIN_FLOW_COUNT);
		if (bCalcWBTDeviation)
		{
			MessageBox(cstrMsg);
		}
		return;
	}
	else if (m_filePerfData.m_designData.m_fnGetRangeCnt() < MIN_RANGE_COUNT)
	{
		CString cstrMsg;
		cstrMsg.Format("You must specify a minimum of %d ranges in the Tower Design Data to calculate Tower Capability.", MIN_RANGE_COUNT);
		if (bCalcWBTDeviation)
		{
			MessageBox(cstrMsg);
		}
		return;
	}
	else if (m_filePerfData.m_designData.m_fnGetWBTCnt(0) < MIN_WBT_COUNT)
	{
		CString cstrMsg;
		cstrMsg.Format("You must specify a minimum of %d WBT values in the Tower Design Data to calculate Tower Capability.", MIN_WBT_COUNT);
		if (bCalcWBTDeviation)
		{
			MessageBox(cstrMsg);
		}
		return;
	}

	//
	// Copy data to temp vars and do calcs
	//
	CString strTemp;
	int IFlow;
	int IR;

    double Y[24] /*FLOW[5]*/;
	double YFIT;

    double /*X[24],*/ /*YFIT[24],*/ /*YA[3],*/ /*Y2A[3],*/ Y2[24];
    double /*R[5],*/ /*CWT[5][10][5],*/ CWT_WB[5][5], CWT_WB_R[5];
//'                    (FLOW, WB, RANGE)

// Init Arrays
	double R[MAX_RANGES];
	double FLOW[MAX_FLOWS];
	double WB[MAX_WBTS];
	double WBTemp[MAX_FLOWS][MAX_WBTS];
	double CWT[MAX_FLOWS][MAX_WBTS][MAX_RANGES];

	m_filePerfData.m_designData.m_fnGetCWTArray(CWT, !TPropPageBase::metricflag);
	m_filePerfData.m_designData.m_fnGetRangeArray(R, !TPropPageBase::metricflag);
	m_filePerfData.m_designData.m_fnGetFlowArray(FLOW, !TPropPageBase::metricflag);
	m_filePerfData.m_designData.m_fnGetWBTArray(WBTemp, !TPropPageBase::metricflag);

	//WBs may change each flow so put the copy in the flow loop


// Init Counts
	int IFlowNo		=	m_filePerfData.m_designData.m_fnGetFlowCnt();
	int IRangeNo	=	m_filePerfData.m_designData.m_fnGetRangeCnt();
	int IWBNo		=	m_filePerfData.m_designData.m_fnGetWBTCnt(0);

//Init Others
	double TestWB	= m_dblTestWBT;
	double TestR	= m_dblTestHWT - m_dblTestCWT;
	double TestCWT	= m_dblTestCWT;


//'Interpolate for WB
#ifdef _DEBUG
//    PRINT USING "@ TestWB=##.##"; TestWB
//    PRINT #2, USING "@ TestWB=##.###"; TestWB
	strTemp.Format("@ TestWB=%.03f\r\n", TestWB);
	OutputDebugString(strTemp);
#endif // _DEBUG

    for (IFlow = 0; IFlow < IFlowNo; IFlow++)
	{
		IWBNo =	m_filePerfData.m_designData.m_fnGetWBTCnt(IFlow);

        for (IR = 0; IR < IRangeNo; IR++)
		{
            for (int IWB = 0; IWB < IWBNo; IWB++)
			{
                Y[IWB] = CWT[IFlow][IWB][IR];
			}
			memcpy(WB, WBTemp[IFlow], sizeof(WB));
            CalcPerfData(IWBNo, WB, Y, TestWB, YFIT, Y2);

            CWT_WB[IFlow][IR] = YFIT;

#ifdef _DEBUG
			//PRINT USING "FLOW=###### RANGE=##.## CWT=##.###"; FLOW(IFlow); R(IR); CWT_WB(IFlow, IR)            
			strTemp.Format("FLOW=%.0f RANGE=%.02f CWT=%.03f\r\n", FLOW[IFlow], R[IR], CWT_WB[IFlow][IR]);
			OutputDebugString(strTemp);
#endif // _DEBUG
        }
		TRACE0("\r\n");
    }
//'Interpolate for Range
#ifdef _DEBUG
//    PRINT #2, USING "@ TestWB=##.## and TestR=##.##"; TestWB; TestR
	strTemp.Format("@ TestWB=%.02f and TestR=%.02f\r\n", TestWB, TestR);
	OutputDebugString(strTemp);
#endif // _DEBUG

    for (IFlow = 0; IFlow < IFlowNo; IFlow++)
	{
        for (IR = 0; IR < IRangeNo; IR++)
		{
            Y[IR] = CWT_WB[IFlow][IR];
        }
        CalcPerfData(IRangeNo, R, Y, TestR, YFIT, Y2);
        CWT_WB_R[IFlow] = YFIT;

#ifdef _DEBUG
        //PRINT USING "FLOW=######  CWT=##.###"; FLOW(IFlow); CWT_WB_R(IFlow)
		strTemp.Format("FLOW=%.0f CWT=%.03f\r\n", FLOW[IFlow], CWT_WB_R[IFlow]);
		OutputDebugString(strTemp);
#endif // _DEBUG
	}
	TRACE0("\r\n");

//'Final interpolation for Flow (Y) vs CWT (X)
#ifdef _DEBUG
//    PRINT #2, USING "@ TestWB=##.## and TestR=##.## and TestCWT=###.##"; TestWB; TestR; TestCWT
	strTemp.Format("@ TestWB=%.02f and TestR=%.02f and TestCWT=%.02f\r\n", TestWB, TestR, TestCWT);
	OutputDebugString(strTemp);
#endif // _DEBUG

    CalcPerfData(IFlowNo, CWT_WB_R, FLOW, TestCWT, YFIT, Y2);
    double PredFlow = YFIT;


//'Rev:1-23-01 Added check for EXTRAPOLATED flow
//'Determine Min and Max Flows
    double MinFlow = FLOW[0];
	double MaxFlow = FLOW[0];
    for (int iExt = 1; iExt < IFlowNo; iExt++)
	{
        if (FLOW[iExt] < MinFlow)
		{
			MinFlow = FLOW[iExt];
		}
        if (FLOW[iExt] > MaxFlow)
		{
			MaxFlow = FLOW[iExt];
		}
    }
    if ((PredFlow < MinFlow) || (PredFlow > MaxFlow))
	{
        //PRINT #2, STRING$(80, 42)
        //PRINT #2, USING "CAUTION: Predicted Flow ####.## is EXTRAPOLATED beyond the"; PredFlow
        //PRINT #2, USING "Supplied Curve Flows of #####.## to #####.##"; MinFlow; MaxFlow
        //PRINT #2, STRING$(80, 42)
		CString strWarning;
		strWarning.Format("CAUTION: Predicted Flow %.02f is EXTRAPOLATED beyond the\r\nSupplied Curve Flows of %.02f to %.02f", PredFlow, MinFlow, MaxFlow);
		if (bCalcWBTDeviation)
		{
			MessageBox(strWarning, "Extrapolation Warning", MB_ICONWARNING);
		}
    }
//'End check for Extrapolation

	LWBTnew	= 0;
	DenOutT	= 0;
	SVOutT	= 0;
	HLWBT	= 0; 
	dblAdjustedFlow	= DetermineAdjTestFlow(!TPropPageBase::metricflag, (int)m_filePerfData.m_designData.m_fnGetInducedFlow(), m_dblDesignHWT, m_dblDesignCWT, m_dblDesignWBT, m_dblDesignDBT, m_dblDesignBarometricPressure, m_dblDesignWaterFlowRate, m_dblDesignFanDriverPower, m_dblDesignLG, m_dblTestHWT, m_dblTestCWT, m_dblTestWBT, m_dblTestDBT, m_dblTestBarometricPressure, m_dblTestWaterFlowRate, m_dblTestFanDriverPower, LWBTnew, DenOutT, SVOutT, HLWBT, dblAdjustedFlow, m_dblTestLG);
	double AdjTestFlow = dblAdjustedFlow;

#ifdef _DEBUG
//    PRINT USING "TestCWT=##.###  PredFlow=######.###"; TestCWT; PredFlow
//    PRINT USING "AdjTestFlow=######.###  PredFlow=######.###  Percent Capability=###.##"; AdjTestFlow; PredFlow; AdjTestFlow / PredFlow * 100
	strTemp.Format("TestCWT=%.02f PredFlow=%.03f\r\n",TestCWT, PredFlow);
	OutputDebugString(strTemp);
	strTemp.Format("AdjTestFlow=%.03f  PredFlow=%.03f  Percent Capability=%.03f", 
		AdjTestFlow, PredFlow, PredFlow != 0.0 ? (AdjTestFlow / PredFlow * 100.0) : 0.0);
	TRACE0(strTemp);
#endif // _DEBUG

	m_dblPredictedFlow = PredFlow;
	m_dblAdjustedFlow = AdjTestFlow;

	m_dblCapability = PredFlow != 0.0 ? (AdjTestFlow / PredFlow * 100.0) : 0.0;

	if (bCalcWBTDeviation)
	{
		m_fnCalcWBTDeviation();
	}
}

void TPropPageFour::m_fnCalcWBTDeviation()
{
	// Store off the current Calculated values
	double	dblCapability				= m_dblCapability;
	double  dblAdjustedFlow				= m_dblAdjustedFlow;
	double  dblPredictedFlow			= m_dblPredictedFlow;

	// Store off the Real Test Values
	double	dblTestBarometricPressure	= m_dblTestBarometricPressure;
	double	dblTestCWT					= m_dblTestCWT;
	double	dblTestDBT					= m_dblTestDBT;
	double	dblTestFanDriverPower		= m_dblTestFanDriverPower;
	double	dblTestHWT					= m_dblTestHWT;
	double	dblTestLG					= m_dblTestLG;
	double	dblTestWaterFlowRate		= m_dblTestWaterFlowRate;
	double	dblTestWBT					= m_dblTestWBT;

	// Set test values to design values
	m_dblTestBarometricPressure	= m_dblDesignBarometricPressure;
	m_dblTestCWT				= m_dblDesignCWT;
	m_dblTestDBT				= m_dblDesignDBT;
	m_dblTestFanDriverPower		= m_dblDesignFanDriverPower;
	m_dblTestHWT				= m_dblDesignHWT;
	m_dblTestLG					= m_dblDesignLG;
	m_dblTestWaterFlowRate		= m_dblDesignWaterFlowRate;
	m_dblTestWBT				= m_dblDesignWBT;

#define _NEW_CODE
//#define _MOD_CODE

    unsigned int nIterationCount(0);

#ifdef _NEW_CODE
    double dblEstSlope(-10.0);   // original estimate of slope (Cap vs. CWT)
    m_fnCalcCapability(false);
    double dblDeltaCap(dblCapability - m_dblCapability);
    while (fabs(dblDeltaCap) > 0.01)
    {
        double dblDeltaCWT(dblDeltaCap / dblEstSlope);
        double dblPriorCap(m_dblCapability);
        m_dblTestCWT += dblDeltaCWT;
        m_dblTestHWT += dblDeltaCWT;
        m_fnCalcCapability(false);
        dblEstSlope = (m_dblCapability - dblPriorCap) / dblDeltaCWT;
        dblDeltaCap = dblCapability - m_dblCapability;
        if (++nIterationCount >= MAX_CWT_DEV_ITERATIONS)
        {
            break;
        }
    }
#else

#ifdef _MOD_CODE
    double dblFactor = 10.0;
#else   // Original code
    double dblFactor = 10.0 * ((100 / m_dblDesignCWT) / 2);
#endif // _MOD_CODE
    m_fnCalcCapability(false);
    while (abs(dblCapability - m_dblCapability) > 0.01)
	{
		if ((dblCapability - m_dblCapability) > 0)
		{
			m_dblTestCWT	= m_dblTestCWT - (abs(dblCapability - m_dblCapability)/dblFactor);
			m_dblTestHWT	= m_dblTestHWT - (abs(dblCapability - m_dblCapability)/dblFactor);
			m_fnCalcCapability(false);
		}
		else
		{
			m_dblTestCWT	= m_dblTestCWT + (abs(dblCapability - m_dblCapability)/dblFactor);
			m_dblTestHWT	= m_dblTestHWT + (abs(dblCapability - m_dblCapability)/dblFactor);
			m_fnCalcCapability(false);
		}
        if (++nIterationCount > MAX_CWT_DEV_ITERATIONS)
        {
            break;
        }
	}
#endif  // _NEW_CODE

    CString strTemp;
    strTemp.Format("\nCWT deviation calculation took %u iterations.\n", nIterationCount);
    OutputDebugString(strTemp);

    if (nIterationCount > MAX_CWT_DEV_ITERATIONS)
    {
        m_dblCWTDev = 0.0;
        m_bCWTDevCalcOk = false;
        GetDlgItem(IDC_EDIT_COLD_WATER_DEV)->SetWindowText("");
        strTemp.Format("\n*** CWT deviation calculation exceeded max iterations (%u) - CWT dev set to 0! ***\n",
            MAX_CWT_DEV_ITERATIONS);
        OutputDebugString(strTemp);
        strTemp = "The CWT Deviation calculation did not converge - please contact CTI support for assistance.\n";
        strTemp += "(Choose \"About CTI Toolkit\" from the Help menu for a link to the CTI website.)";
        MessageBox(strTemp, "Calculation error");
    }
    else
    {
        m_bCWTDevCalcOk = true;
	    m_dblCWTDev = m_dblTestCWT - m_dblDesignCWT;
    }


	// Restore the Calculated values
	m_dblCapability		= dblCapability;
	m_dblAdjustedFlow	= dblAdjustedFlow;
	m_dblPredictedFlow	= dblPredictedFlow;

	// Restore Real Test Values
	m_dblTestBarometricPressure	= 	dblTestBarometricPressure;
	m_dblTestCWT				= 	dblTestCWT;
	m_dblTestDBT				= 	dblTestDBT;
	m_dblTestFanDriverPower		= 	dblTestFanDriverPower;
	m_dblTestHWT				= 	dblTestHWT;
	m_dblTestLG					= 	dblTestLG;
	m_dblTestWaterFlowRate		= 	dblTestWaterFlowRate;
	m_dblTestWBT				= 	dblTestWBT;

}

void TPropPageFour::OnChangeEditTestBarometricPressure() 
{
	if (GetDlgItem(IDC_EDIT_TEST_BAROMETRIC_PRESSURE)->GetWindowTextLength() > 0)
	{
		if (!m_fnCheckForTestSelection())
		{
			GetDlgItem(IDC_EDIT_TEST_BAROMETRIC_PRESSURE)->SetWindowText("");
		}
	}
}

void TPropPageFour::OnChangeEditTestColdWater() 
{
	if (GetDlgItem(IDC_EDIT_TEST_COLD_WATER)->GetWindowTextLength() > 0)
	{
		if (!m_fnCheckForTestSelection())
		{
			GetDlgItem(IDC_EDIT_TEST_COLD_WATER)->SetWindowText("");
		}
	}
}

void TPropPageFour::OnChangeEditTestDryBulb() 
{
	if (GetDlgItem(IDC_EDIT_TEST_DRY_BULB)->GetWindowTextLength() > 0)
	{
		if (!m_fnCheckForTestSelection())
		{
			GetDlgItem(IDC_EDIT_TEST_DRY_BULB)->SetWindowText("");
		}
	}
}

void TPropPageFour::OnChangeEditTestFanDriver() 
{
	if (GetDlgItem(IDC_EDIT_TEST_FAN_DRIVER)->GetWindowTextLength() > 0)
	{
		if (!m_fnCheckForTestSelection())
		{
			GetDlgItem(IDC_EDIT_TEST_FAN_DRIVER)->SetWindowText("");
		}
	}
}

void TPropPageFour::OnChangeEditTestHotWater() 
{
	if (GetDlgItem(IDC_EDIT_TEST_HOT_WATER)->GetWindowTextLength() > 0)
	{
		if (!m_fnCheckForTestSelection())
		{
			GetDlgItem(IDC_EDIT_TEST_HOT_WATER)->SetWindowText("");
		}
	}
}

void TPropPageFour::OnChangeEditTestWaterFlow() 
{
	if (GetDlgItem(IDC_EDIT_TEST_WATER_FLOW)->GetWindowTextLength() > 0)
	{
		if (!m_fnCheckForTestSelection())
		{
			GetDlgItem(IDC_EDIT_TEST_WATER_FLOW)->SetWindowText("");
		}
	}
}

void TPropPageFour::OnChangeEditTestWetBulb() 
{
	if (GetDlgItem(IDC_EDIT_TEST_WET_BULB)->GetWindowTextLength() > 0)
	{
		if (!m_fnCheckForTestSelection())
		{
			GetDlgItem(IDC_EDIT_TEST_WET_BULB)->SetWindowText("");
		}
	}
}

#ifdef _PAGE_FOUR_TOWER_TYPE
void TPropPageFour::OnButtonInduced()
{
	m_filePerfData.m_designData.m_fnSetInducedFlow(true);
	m_fnClearCalculatedValues();
}

void TPropPageFour::OnButtonForced()
{
	m_filePerfData.m_designData.m_fnSetInducedFlow(false);
	m_fnClearCalculatedValues();
}
#endif	// _PAGE_FOUR_TOWER_TYPE

void TPropPageFour::m_fnClearCalculatedValues()
{
	GetDlgItem(IDC_EDIT_PREDICTED_FLOW)->SetWindowText("");
	m_dblPredictedFlow = 0;

	GetDlgItem(IDC_EDIT_ADJUSTED_FLOW)->SetWindowText("");
	m_dblAdjustedFlow = 0;

	GetDlgItem(IDC_EDIT_COLD_WATER_DEV)->SetWindowText("");
	m_dblCWTDev = 0;
    m_bCWTDevCalcOk = true;

	GetDlgItem(IDC_EDIT_CAPABILITY)->SetWindowText("");
	m_dblCapability = 0;

	GetDlgItem(IDC_EDIT_TEST_LG)->SetWindowText("");
	m_dblTestLG = 0;
}

bool TPropPageFour::m_fnCheckForTestSelection()
{
	bool bReturn = true;

	m_fnClearCalculatedValues();

	if (m_wndTestList.GetCurSel() < 1)
	{
		MessageBox("Please select a test first");
		bReturn = false;
	}
	else
	{
		m_bTestDataChanged = true;
	}

	return bReturn;
}

//---------------------------------------------------------------------
// FUNCTION    : TPropPageFour::OnFilePrint
//
// DESCRIPTION : None.
//
// INPUT       : None.
//
// RETURN (S)  : None.
//---------------------------------------------------------------------
// REMARKS     : None.
//---------------------------------------------------------------------
void TPropPageFour::OnFilePrint() 
{
	if ((m_iCurrentTest >= 0) && (m_filePerfData.m_strFileName.GetLength()))
	{
		if (m_filePerfData.m_designData.m_fnGetFlowCnt() == 0)
		{
			MessageBox("You must specify flow data in the Tower Design Data to calculate Tower Capability.");
			return;
		}
		else if (m_filePerfData.m_designData.m_fnGetRangeCnt() == 0)
		{
			MessageBox("You must specify range data in the Design Data to calculate Tower Capability.");
			return;
		}
		else if (m_filePerfData.m_designData.m_fnGetWBTCnt(0) == 0)
		{
			MessageBox("You must specify cold water temperature data in the Design Data to calculate Tower Capability.");
			return;
		}


		TPrintPerfReportDlg tp;
		if (m_strPreviewSubject == "")
			tp.m_csDescription = m_filePerfData.m_designData.m_fnGetProjectName() + " " +CTime::GetCurrentTime().Format("%x");
		else
			tp.m_csDescription = m_strPreviewSubject;
		if (tp.DoModal() == IDOK)
		{
			OnPerformanceButtonRecalc();	// fix v3.0.5 bug below
			OnButtonSaveTestData();
			// Version 3.0, Build 5: (05/23/03)
			// If no recalc has been done after switching to forced draft, this can print 0.0 for the CWTDev
			wndPrintPerfDataView.m_dblCWTDev = m_dblCWTDev;
			wndPrintPerfDataView.m_fnPrint(&(m_filePerfData.m_designData), &(m_filePerfData.m_arrayTests[m_iCurrentTest]), !TPropPageBase::metricflag, tp.m_csDescription, tp.m_bPerfReport);
		}
	}
	else
	{
		MessageBox("You must select a Performance Data File and a Test.");
	}
}
//---------------------------------------------------------------------
// END: TPropPageFour::OnFilePrint
//---------------------------------------------------------------------

//#define _PRINT_AFTER_PREVIEW
//---------------------------------------------------------------------
// FUNCTION    : TPropPageFour::OnFilePrintPreview
//
// DESCRIPTION : None.
//
// INPUT       : None.
//
// RETURN (S)  : None.
//---------------------------------------------------------------------
// REMARKS     : None.
//---------------------------------------------------------------------
void TPropPageFour::OnFilePrintPreview() 
{
	if ((m_iCurrentTest >= 0) && (m_filePerfData.m_strFileName.GetLength()))
	{
		TPrintPerfReportDlg tp;
		tp.m_csDescription = m_strPreviewSubject;
		if (tp.DoModal() == IDOK)
		{
			OnPerformanceButtonRecalc();	// fix v3.0.5 bug below
			OnButtonSaveTestData();
			// Version 3.0, Build 5: (05/23/03)
			// If no recalc has been done after switching to forced draft, this can print 0.0 for the CWTDev
			wndPrintPerfDataView.m_dblCWTDev = m_dblCWTDev;
#ifdef _MFC_PREVIEW
			CFrameWnd * pMainWnd = (CFrameWnd *)AfxGetApp()->m_pMainWnd;
			CToolkitDoc * pDoc = (CToolkitDoc *)pMainWnd->GetActiveDocument();
			pDoc->m_bLoaded = TRUE;
			pMainWnd->ShowWindow(SW_SHOW);
			wndPrintPerfDataView.m_fnPrintPreview(&(m_filePerfData.m_designData), &(m_filePerfData.m_arrayTests[m_iCurrentTest]), !TPropPageBase::metricflag, tp.m_csDescription, tp.m_bPerfReport);
#else
			wndPrintPerfDataFrame.ShowWindow(SW_NORMAL);
			wndPrintPerfDataFrame.m_bPrintPreview = TRUE;
			wndPrintPerfDataView.ShowWindow(SW_NORMAL);

#ifdef _PRINT_AFTER_PREVIEW
			// Set preview window to always be on top ...
			wndPrintPerfDataFrame.SetWindowPos(&wndTopMost, 0, 0, 0, 0, 
				SWP_NOMOVE | SWP_NOSIZE);
			
			wndPrintPerfDataView.m_fnPrint(&(m_filePerfData.m_designData), &(m_filePerfData.m_arrayTests[m_iCurrentTest]), !TPropPageBase::metricflag, tp.m_csDescription, tp.m_bPerfReport);

			// On return, reset window to non-topmost
			wndPrintPerfDataFrame.SetWindowPos(&wndNoTopMost, 0, 0, 0, 0, 
				SWP_NOMOVE | SWP_NOSIZE);
#else
			wndPrintPerfDataView.m_fnPrintPreview(&(m_filePerfData.m_designData), &(m_filePerfData.m_arrayTests[m_iCurrentTest]), !TPropPageBase::metricflag, tp.m_csDescription, tp.m_bPerfReport);
			m_strPreviewSubject = tp.m_csDescription;
#endif // _PRINT_AFTER_PREVIEW
			
#endif	// !_MFC_PREVIEW
		}
	}
	else
	{
		MessageBox("You must select a Performance Data File and a Test.");
	}
}
//---------------------------------------------------------------------
// END: TPropPageFour::OnFilePrintPreview
//---------------------------------------------------------------------

//---------------------------------------------------------------------
// FUNCTION    : TPropPageFour::ValidateData
//
// DESCRIPTION : Check design or test data, optionally prompting the
//               user with bounds if errors are found.
//
// INPUT       : None.
//
// RETURN (S)  : None.
//---------------------------------------------------------------------
// REMARKS     : None.
//---------------------------------------------------------------------
bool TPropPageFour::ValidateData(bool bDesignData, bool bDisplayMsg)
{
	bool bWaterFlowRate, bHWT, bCWT, bWBT, bDBT, bFanDriverPower, 
		bBarometricPressure, bLG;
	TPerfData pdMin(g_pdMinIP), pdMax(g_pdMaxIP);

	if (TPropPageBase::metricflag)
	{
		pdMin = g_pdMinSI;
		pdMax = g_pdMaxSI;
	}

	TPerfData pdThis(*this, bDesignData);

	// Check each value against min and max bounds
	bWaterFlowRate = ((pdThis.m_dblWaterFlowRate < pdMin.m_dblWaterFlowRate) ||
		(pdThis.m_dblWaterFlowRate > pdMax.m_dblWaterFlowRate));
	bHWT = ((pdThis.m_dblHWT < pdMin.m_dblHWT) ||
		(pdThis.m_dblHWT > pdMax.m_dblHWT) ||
		(pdThis.m_dblHWT <= pdThis.m_dblCWT));
	bCWT = ((pdThis.m_dblCWT < pdMin.m_dblCWT) ||
		(pdThis.m_dblCWT > pdMax.m_dblCWT));
	bWBT = ((pdThis.m_dblWBT < pdMin.m_dblWBT) ||
		(pdThis.m_dblWBT > pdMax.m_dblWBT));
	bDBT = ((pdThis.m_dblDBT < pdMin.m_dblDBT) ||
		(pdThis.m_dblDBT > pdMax.m_dblDBT) ||
		(pdThis.m_dblDBT < pdThis.m_dblWBT));
	bFanDriverPower = ((pdThis.m_dblFanDriverPower < pdMin.m_dblFanDriverPower) ||
		(pdThis.m_dblFanDriverPower > pdMax.m_dblFanDriverPower));
	bBarometricPressure = ((pdThis.m_dblBarometricPressure < pdMin.m_dblBarometricPressure) ||
		(pdThis.m_dblBarometricPressure > pdMax.m_dblBarometricPressure));
	if (bDesignData)
		bLG = ((pdThis.m_dblLG < pdMin.m_dblLG) || (pdThis.m_dblLG > pdMax.m_dblLG));
	else
		bLG = false;

	bool bError(bWaterFlowRate || bHWT || bCWT || bWBT || bDBT || bFanDriverPower ||
		bBarometricPressure || bLG);

	// If requested, display a message box pointing out the errors and displaying
	// the min and max values for each.
	if ((bError) && (bDisplayMsg))
	{
		GetDlgItem(IDC_EDIT_PREDICTED_FLOW)->SetWindowText("");
		GetDlgItem(IDC_EDIT_ADJUSTED_FLOW)->SetWindowText("");
		GetDlgItem(IDC_EDIT_COLD_WATER_DEV)->SetWindowText("");
		GetDlgItem(IDC_EDIT_CAPABILITY)->SetWindowText("");
		GetDlgItem(IDC_EDIT_TEST_LG)->SetWindowText("");
		m_dblCWTDev			= 0.0;
		m_dblCapability		= 0.0;
		m_dblPredictedFlow	= 0.0;
		m_dblAdjustedFlow	= 0.0;

		CString cstrType(bDesignData ? "Design" : "Test");
		CString cstrMsg("Your " + cstrType + 
			" data is out of range for calculating % Capability.\n\n" +
			"Please check the following " + cstrType + " values:\n\n");
		CString cstrLine;
		unsigned int i(1);
		if (bWaterFlowRate)
		{
			cstrLine.Format("%d. Water Flow Rate:   min = %.5g,   max = %.5g\n",
				i++, pdMin.m_dblWaterFlowRate, pdMax.m_dblWaterFlowRate); 
			cstrMsg += cstrLine;
		}
		if (bHWT)
		{
			cstrLine.Format(
				"%d. Hot Water Temp:   min = %.5g,   max = %.5g,   > Cold Water Temp\n",
				i++, pdMin.m_dblHWT, pdMax.m_dblHWT); 
			cstrMsg += cstrLine;
		}
		if (bCWT)
		{
			cstrLine.Format("%d. Cold Water Temp:   min = %.5g,   max = %.5g\n",
				i++, pdMin.m_dblCWT, pdMax.m_dblCWT); 
			cstrMsg += cstrLine;
		}
		if (bWBT)
		{
			cstrLine.Format("%d. Wet Bulb Temp:   min = %.5g,   max = %.5g\n",
				i++, pdMin.m_dblWBT, pdMax.m_dblWBT); 
			cstrMsg += cstrLine;
		}
		if (bDBT)
		{
			cstrLine.Format(
				"%d. Dry Bulb Temp:   min = %.5g,   max = %.5g,   >= Wet Bulb Temp\n",
				i++, pdMin.m_dblDBT, pdMax.m_dblDBT); 
			cstrMsg += cstrLine;
		}
		if (bFanDriverPower)
		{
			cstrLine.Format("%d. Fan Driver Power:   min = %.5g,   max = %.5g\n",
				i++, pdMin.m_dblFanDriverPower, pdMax.m_dblFanDriverPower); 
			cstrMsg += cstrLine;
		}
		if (bBarometricPressure)
		{
			cstrLine.Format("%d. Barometric Press:   min = %.5g,   max = %.5g\n",
				i++, pdMin.m_dblBarometricPressure, pdMax.m_dblBarometricPressure); 
			cstrMsg += cstrLine;
		}
		if (bLG)
		{
			cstrLine.Format("%d. Liquid to Gas Ratio:   min = %.5g,   max = %.5g\n",
				i++, pdMin.m_dblLG, pdMax.m_dblLG); 
			cstrMsg += cstrLine;
		}
		AfxMessageBox(cstrMsg);
	}
	return !bError;
}
//---------------------------------------------------------------------
// END: TPropPageFour::ValidateData
//---------------------------------------------------------------------


//---------------------------------------------------------------------
// FUNCTION    : TPropPageFour::OnSize
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
void TPropPageFour::OnSize(UINT nType, int cx, int cy) 
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
// END: TPropPageFour::OnSize
//---------------------------------------------------------------------

