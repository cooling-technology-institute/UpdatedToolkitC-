// TPerformanceDataDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CTIToolkit.h"
#include "TPerformanceDataDlg.h"
#include "TNewFlowRateDlg.h"
#include "TNewTestDlg.h"
#include "float.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static const TCHAR SZ_SPECS[] = _T("Tower Design Specs");
static const TCHAR SZ_CURVE[] = _T("Tower Design Curve Data");

/////////////////////////////////////////////////////////////////////////////
// TPerformanceDataDlg dialog


TPerformanceDataDlg::TPerformanceDataDlg(CWnd* pParent /*=NULL*/)
	: CDialog(TPerformanceDataDlg::IDD, pParent)
{
	m_bIPUnits = !TPropPageBase::metricflag;
	m_iCurrentFlow = 0;
	m_iTempIndex = 0;
	m_strSpecsCaption = SZ_SPECS;
	m_strCurveCaption = SZ_CURVE;
	//{{AFX_DATA_INIT(TPerformanceDataDlg)
	//}}AFX_DATA_INIT
}


void TPerformanceDataDlg::DoDataExchange(CDataExchange* pDX)
{
	double dblTemp;

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(TPerformanceDataDlg)
	DDX_Control(pDX, IDC_EDIT_WBT6,  m_wndWBT6);
	DDX_Control(pDX, IDC_EDIT_WBT5,  m_wndWBT5);
	DDX_Control(pDX, IDC_EDIT_WBT4,  m_wndWBT4);
	DDX_Control(pDX, IDC_EDIT_WBT3,  m_wndWBT3);
	DDX_Control(pDX, IDC_EDIT_WBT2,  m_wndWBT2);
	DDX_Control(pDX, IDC_EDIT_WBT1,  m_wndWBT1);
	DDX_Control(pDX, IDC_EDIT_R5_CWT1, m_wndR5WBT1);
	DDX_Control(pDX, IDC_EDIT_R5_CWT2, m_wndR5WBT2);
	DDX_Control(pDX, IDC_EDIT_R5_CWT3, m_wndR5WBT3);
	DDX_Control(pDX, IDC_EDIT_R5_CWT4, m_wndR5WBT4);
	DDX_Control(pDX, IDC_EDIT_R5_CWT5, m_wndR5WBT5);
	DDX_Control(pDX, IDC_EDIT_R5_CWT6, m_wndR5WBT6);
	DDX_Control(pDX, IDC_EDIT_R4_CWT1, m_wndR4WBT1);
	DDX_Control(pDX, IDC_EDIT_R4_CWT2, m_wndR4WBT2);
	DDX_Control(pDX, IDC_EDIT_R4_CWT3, m_wndR4WBT3);
	DDX_Control(pDX, IDC_EDIT_R4_CWT4, m_wndR4WBT4);
	DDX_Control(pDX, IDC_EDIT_R4_CWT5, m_wndR4WBT5);
	DDX_Control(pDX, IDC_EDIT_R4_CWT6, m_wndR4WBT6);
	DDX_Control(pDX, IDC_EDIT_R3_CWT1, m_wndR3WBT1);
	DDX_Control(pDX, IDC_EDIT_R3_CWT2, m_wndR3WBT2);
	DDX_Control(pDX, IDC_EDIT_R3_CWT3, m_wndR3WBT3);
	DDX_Control(pDX, IDC_EDIT_R3_CWT4, m_wndR3WBT4);
	DDX_Control(pDX, IDC_EDIT_R3_CWT5, m_wndR3WBT5);
	DDX_Control(pDX, IDC_EDIT_R3_CWT6, m_wndR3WBT6);
	DDX_Control(pDX, IDC_EDIT_R2_CWT1, m_wndR2WBT1);
	DDX_Control(pDX, IDC_EDIT_R2_CWT2, m_wndR2WBT2);
	DDX_Control(pDX, IDC_EDIT_R2_CWT3, m_wndR2WBT3);
	DDX_Control(pDX, IDC_EDIT_R2_CWT4, m_wndR2WBT4);
	DDX_Control(pDX, IDC_EDIT_R2_CWT5, m_wndR2WBT5);
	DDX_Control(pDX, IDC_EDIT_R2_CWT6, m_wndR2WBT6);
	DDX_Control(pDX, IDC_EDIT_R1_CWT1, m_wndR1WBT1);
	DDX_Control(pDX, IDC_EDIT_R1_CWT2, m_wndR1WBT2);
	DDX_Control(pDX, IDC_EDIT_R1_CWT3, m_wndR1WBT3);
	DDX_Control(pDX, IDC_EDIT_R1_CWT4, m_wndR1WBT4);
	DDX_Control(pDX, IDC_EDIT_R1_CWT5, m_wndR1WBT5);
	DDX_Control(pDX, IDC_EDIT_R1_CWT6, m_wndR1WBT6);
	DDX_Control(pDX, IDC_EDIT_R1,	   m_wndR1);
	DDX_Control(pDX, IDC_EDIT_R2,      m_wndR2);
	DDX_Control(pDX, IDC_EDIT_R3,      m_wndR3);
	DDX_Control(pDX, IDC_EDIT_R4,      m_wndR4);
	DDX_Control(pDX, IDC_EDIT_R5,      m_wndR5);
	DDX_Control(pDX, IDC_EDIT_DESIGN_WET_BULB, m_wndDesignWBT);
	DDX_Control(pDX, IDC_EDIT_DESIGN_WATER_FLOW, m_wndDesignWaterFlowRate);
	DDX_Control(pDX, IDC_EDIT_DESIGN_LG, m_wndDesignLG);
	DDX_Control(pDX, IDC_EDIT_DESIGN_HOT_WATER, m_wndDesignHWT);
	DDX_Control(pDX, IDC_EDIT_DESIGN_FAN_DRIVER, m_wndDesignFanDriverPower);
	DDX_Control(pDX, IDC_EDIT_DESIGN_DRY_BULB, m_wndDesignDBT);
	DDX_Control(pDX, IDC_EDIT_DESIGN_COLD_WATER, m_wndDesignCWT);
	DDX_Control(pDX, IDC_EDIT_DESIGN_BAROMETRIC_PRESSURE, m_wndDesignBarometricPressure);
	DDX_Control(pDX, IDC_TAB_FLOWS, m_wndFlowTabs);
	DDX_Text(pDX, IDC_STATIC_SPECS, m_strSpecsCaption);
	DDX_Text(pDX, IDC_STATIC_CURVE, m_strCurveCaption);
	//}}AFX_DATA_MAP

	if (pDX->m_bSaveAndValidate)
	{
		//
		// Get tower data
		//
		CString strTemp;
		DDX_Text(pDX, IDC_EDIT_LOCATION, strTemp);
		m_designData.m_fnSetTowerLocation(strTemp);
		DDX_Text(pDX, IDC_EDIT_MANUFACTURER, strTemp);
		m_designData.m_fnSetTowerManufacturer(strTemp);
		DDX_Text(pDX, IDC_EDIT_OWNER_NAME, strTemp);
		m_designData.m_fnSetTowerOwner(strTemp);
		DDX_Text(pDX, IDC_EDIT_PROJECT_NAME, strTemp);
		m_designData.m_fnSetProjectName(strTemp);

#ifdef _TOWER_TYPE
		if (GetCheckedRadioButton(IDC_RADIO_INDUCED, IDC_RADIO_FORCED) == IDC_RADIO_INDUCED)
		{
			m_designData.m_fnSetInducedFlow(true);
		}
		else
		{
			m_designData.m_fnSetInducedFlow(false);
		}
#endif // _TOWER_TYPE

		double dblTemp;

		//
		// Get Design Point data
		//
		if (m_wndDesignWaterFlowRate.GetModify()) 
		{
			DDX_Text(pDX, IDC_EDIT_DESIGN_WATER_FLOW, dblTemp);
			DDV_MinMaxDouble(pDX,dblTemp,WATER_FLOW_RATE_MIN_IP,
				WATER_FLOW_RATE_MAX_IP,WATER_FLOW_RATE_MIN_SI,
				WATER_FLOW_RATE_MAX_SI,1);
			m_designData.m_fnSetWaterFlowRate(dblTemp, m_bIPUnits);
		}
		if (m_wndDesignCWT.GetModify()) 
		{
			DDX_Text(pDX, IDC_EDIT_DESIGN_COLD_WATER, dblTemp);
			DDV_MinMaxDouble(pDX,dblTemp,CWT_MIN_IP,CWT_MAX_IP,
				CWT_MIN_SI,CWT_MAX_SI,2);
			m_designData.m_fnSetCWT(dblTemp, m_bIPUnits);
		}
		if (m_wndDesignHWT.GetModify()) 
		{
			DDX_Text(pDX, IDC_EDIT_DESIGN_HOT_WATER, dblTemp);
			DDV_MinMaxDouble(pDX,dblTemp,HWT_MIN_IP,HWT_MAX_IP,
				HWT_MIN_SI,HWT_MAX_SI,2);
			m_designData.m_fnSetHWT(dblTemp, m_bIPUnits);
		}
		if (m_wndDesignWBT.GetModify()) 
		{
			DDX_Text(pDX, IDC_EDIT_DESIGN_WET_BULB, dblTemp);
			DDV_MinMaxDouble(pDX,dblTemp,WBT_MIN_IP,WBT_MAX_IP,
				WBT_MIN_SI,WBT_MAX_SI,2);
			m_designData.m_fnSetWBT(dblTemp, m_bIPUnits);
		}
		if (m_wndDesignDBT.GetModify()) 
		{
			DDX_Text(pDX, IDC_EDIT_DESIGN_DRY_BULB, dblTemp);
			DDV_MinMaxDouble(pDX,dblTemp,DBT_MIN_IP,DBT_MAX_IP,
				DBT_MIN_SI,DBT_MAX_SI,2);
			m_designData.m_fnSetDBT(dblTemp, m_bIPUnits);
		}
		if (m_wndDesignBarometricPressure.GetModify()) 
		{
			DDX_Text(pDX, IDC_EDIT_DESIGN_BAROMETRIC_PRESSURE, dblTemp);
			DDV_MinMaxDouble(pDX,dblTemp, BAROMETRIC_PRESSURE_MIN_IP, 
				BAROMETRIC_PRESSURE_MAX_IP, BAROMETRIC_PRESSURE_MIN_SI, 
				BAROMETRIC_PRESSURE_MAX_SI, 3);
			m_designData.m_fnSetBarometricPressure(dblTemp, m_bIPUnits);
		}
		if (m_wndDesignFanDriverPower.GetModify()) 
		{
			DDX_Text(pDX, IDC_EDIT_DESIGN_FAN_DRIVER, dblTemp);
			DDV_MinMaxDouble(pDX,dblTemp,FAN_DRIVER_POWER_MIN_IP,
				FAN_DRIVER_POWER_MAX_IP, FAN_DRIVER_POWER_MIN_SI,
				FAN_DRIVER_POWER_MAX_SI,4);
			m_designData.m_fnSetFanDriverPower(dblTemp, m_bIPUnits);
		}
		if (m_wndDesignLG.GetModify()) 
		{
			DDX_Text(pDX, IDC_EDIT_DESIGN_LG, dblTemp);
			DDV_MinMaxDouble(pDX,dblTemp, LG_MIN_IP, LG_MAX_IP, 
				LG_MIN_SI, LG_MAX_SI, 4);		
			m_designData.m_fnSetLG(dblTemp, m_bIPUnits);
		}


		//
		// Get range data
		//
		if (m_wndR1.GetModify()) 
		{
			DDX_Text(pDX, IDC_EDIT_R1, dblTemp);
			m_designData.m_fnSetRange(dblTemp, 0, m_bIPUnits);
		}
		if (m_wndR2.GetModify()) 
		{
			DDX_Text(pDX, IDC_EDIT_R2, dblTemp);
			m_designData.m_fnSetRange(dblTemp, 1, m_bIPUnits);
		}
		if (m_wndR3.GetModify()) 
		{
			DDX_Text(pDX, IDC_EDIT_R3, dblTemp);
			m_designData.m_fnSetRange(dblTemp, 2, m_bIPUnits);
		}
		if (m_wndR4.GetModify()) 
		{
			DDX_Text(pDX, IDC_EDIT_R4, dblTemp);
			m_designData.m_fnSetRange(dblTemp, 3, m_bIPUnits);
		}
		if (m_wndR5.GetModify()) 
		{
			DDX_Text(pDX, IDC_EDIT_R5, dblTemp);
			m_designData.m_fnSetRange(dblTemp, 4, m_bIPUnits);
		}
		
		int iRangeCnt = 0;
		for (int iRangeIndex = 0; iRangeIndex < MAX_RANGES; iRangeIndex++)
		{
			if (m_designData.m_fnGetRange(iRangeIndex, m_bIPUnits) != 0.0)
			{
				iRangeCnt++;
			}
			else
			{
				break;
			}
		}
		m_designData.m_fnSetRangeCnt(iRangeCnt);


		//
		// Get WBT data
		//
		if (m_wndWBT1.GetModify()) 
		{
			DDX_Text(pDX, IDC_EDIT_WBT1, dblTemp);
			DDV_MinMaxDouble(pDX,dblTemp,WBT_MIN_IP,WBT_MAX_IP,
				WBT_MIN_SI,WBT_MAX_SI,2);
			m_designData.m_fnSetWBT(dblTemp, m_iCurrentFlow, 0, m_bIPUnits);
		}
		if (m_wndWBT2.GetModify()) 
		{
			DDX_Text(pDX, IDC_EDIT_WBT2, dblTemp);
			DDV_MinMaxDouble(pDX,dblTemp,WBT_MIN_IP,WBT_MAX_IP,
				WBT_MIN_SI,WBT_MAX_SI,2);
			m_designData.m_fnSetWBT(dblTemp, m_iCurrentFlow, 1, m_bIPUnits);
		}
		if (m_wndWBT3.GetModify()) 
		{
			DDX_Text(pDX, IDC_EDIT_WBT3, dblTemp);
			DDV_MinMaxDouble(pDX,dblTemp,WBT_MIN_IP,WBT_MAX_IP,
				WBT_MIN_SI,WBT_MAX_SI,2);
			m_designData.m_fnSetWBT(dblTemp, m_iCurrentFlow, 2, m_bIPUnits);
		}
		if (m_wndWBT4.GetModify()) 
		{
			DDX_Text(pDX, IDC_EDIT_WBT4, dblTemp);
			DDV_MinMaxDouble(pDX,dblTemp,WBT_MIN_IP,WBT_MAX_IP,
				WBT_MIN_SI,WBT_MAX_SI,2);
			m_designData.m_fnSetWBT(dblTemp, m_iCurrentFlow, 3, m_bIPUnits);
		}
		if (m_wndWBT5.GetModify()) 
		{
			DDX_Text(pDX, IDC_EDIT_WBT5, dblTemp);
			DDV_MinMaxDouble(pDX,dblTemp,WBT_MIN_IP,WBT_MAX_IP,
				WBT_MIN_SI,WBT_MAX_SI,2);
			m_designData.m_fnSetWBT(dblTemp, m_iCurrentFlow, 4, m_bIPUnits);
		}
		if (m_wndWBT6.GetModify()) 
		{
			DDX_Text(pDX, IDC_EDIT_WBT6, dblTemp);
			DDV_MinMaxDouble(pDX,dblTemp,WBT_MIN_IP,WBT_MAX_IP,
				WBT_MIN_SI,WBT_MAX_SI,2);
			m_designData.m_fnSetWBT(dblTemp, m_iCurrentFlow, 5, m_bIPUnits);
		}

		int iWBTCnt = 0;
		for (int iWBTIndex = 0; iWBTIndex < MAX_WBTS; iWBTIndex++)
		{
			if (m_designData.m_fnGetWBT(m_iCurrentFlow, iWBTIndex, m_bIPUnits) != 0.0)
			{
				iWBTCnt++;
			}
			else
			{
				break;
			}
		}
		m_designData.m_fnSetWBTCnt(iWBTCnt, m_iCurrentFlow);


		//
		// Get CWT data
		//
		if (m_wndR1WBT1.GetModify()) 
		{
			DDX_Text(pDX, IDC_EDIT_R1_CWT1, dblTemp);
			DDV_MinMaxDouble(pDX,dblTemp,CWT_MIN_IP,CWT_MAX_IP,
				CWT_MIN_SI,CWT_MAX_SI,2);
			m_designData.m_fnSetCWT(dblTemp, m_iCurrentFlow, 0, 0, m_bIPUnits);
		}
		if (m_wndR1WBT2.GetModify()) 
		{
			DDX_Text(pDX, IDC_EDIT_R1_CWT2, dblTemp);
			DDV_MinMaxDouble(pDX,dblTemp,CWT_MIN_IP,CWT_MAX_IP,
				CWT_MIN_SI,CWT_MAX_SI,2);
			m_designData.m_fnSetCWT(dblTemp, m_iCurrentFlow, 1, 0, m_bIPUnits);
		}
		if (m_wndR1WBT3.GetModify()) 
		{
			DDX_Text(pDX, IDC_EDIT_R1_CWT3, dblTemp);
			DDV_MinMaxDouble(pDX,dblTemp,CWT_MIN_IP,CWT_MAX_IP,
				CWT_MIN_SI,CWT_MAX_SI,2);
			m_designData.m_fnSetCWT(dblTemp, m_iCurrentFlow, 2, 0, m_bIPUnits);
		}
		if (m_wndR1WBT4.GetModify()) 
		{
			DDX_Text(pDX, IDC_EDIT_R1_CWT4, dblTemp);
			DDV_MinMaxDouble(pDX,dblTemp,CWT_MIN_IP,CWT_MAX_IP,
				CWT_MIN_SI,CWT_MAX_SI,2);
			m_designData.m_fnSetCWT(dblTemp, m_iCurrentFlow, 3, 0, m_bIPUnits);
		}
		if (m_wndR1WBT5.GetModify()) 
		{
			DDX_Text(pDX, IDC_EDIT_R1_CWT5, dblTemp);
			DDV_MinMaxDouble(pDX,dblTemp,CWT_MIN_IP,CWT_MAX_IP,
				CWT_MIN_SI,CWT_MAX_SI,2);
			m_designData.m_fnSetCWT(dblTemp, m_iCurrentFlow, 4, 0, m_bIPUnits);
		}
		if (m_wndR1WBT6.GetModify()) 
		{
			DDX_Text(pDX, IDC_EDIT_R1_CWT6, dblTemp);
			DDV_MinMaxDouble(pDX,dblTemp,CWT_MIN_IP,CWT_MAX_IP,
				CWT_MIN_SI,CWT_MAX_SI,2);
			m_designData.m_fnSetCWT(dblTemp, m_iCurrentFlow, 5, 0, m_bIPUnits);
		}

		
		if (m_wndR2WBT1.GetModify()) 
		{
			DDX_Text(pDX, IDC_EDIT_R2_CWT1, dblTemp);
			DDV_MinMaxDouble(pDX,dblTemp,CWT_MIN_IP,CWT_MAX_IP,
				CWT_MIN_SI,CWT_MAX_SI,2);
			m_designData.m_fnSetCWT(dblTemp, m_iCurrentFlow, 0, 1, m_bIPUnits);
		}
		if (m_wndR2WBT2.GetModify()) 
		{
			DDX_Text(pDX, IDC_EDIT_R2_CWT2, dblTemp);
			DDV_MinMaxDouble(pDX,dblTemp,CWT_MIN_IP,CWT_MAX_IP,
				CWT_MIN_SI,CWT_MAX_SI,2);
			m_designData.m_fnSetCWT(dblTemp, m_iCurrentFlow, 1, 1, m_bIPUnits);
		}
		if (m_wndR2WBT3.GetModify()) 
		{
			DDX_Text(pDX, IDC_EDIT_R2_CWT3, dblTemp);
			DDV_MinMaxDouble(pDX,dblTemp,CWT_MIN_IP,CWT_MAX_IP,
				CWT_MIN_SI,CWT_MAX_SI,2);
			m_designData.m_fnSetCWT(dblTemp, m_iCurrentFlow, 2, 1, m_bIPUnits);
		}
		if (m_wndR2WBT4.GetModify()) 
		{
			DDX_Text(pDX, IDC_EDIT_R2_CWT4, dblTemp);
			DDV_MinMaxDouble(pDX,dblTemp,CWT_MIN_IP,CWT_MAX_IP,
				CWT_MIN_SI,CWT_MAX_SI,2);
			m_designData.m_fnSetCWT(dblTemp, m_iCurrentFlow, 3, 1, m_bIPUnits);
		}
		if (m_wndR2WBT5.GetModify()) 
		{
			DDX_Text(pDX, IDC_EDIT_R2_CWT5, dblTemp);
			DDV_MinMaxDouble(pDX,dblTemp,CWT_MIN_IP,CWT_MAX_IP,
				CWT_MIN_SI,CWT_MAX_SI,2);
			m_designData.m_fnSetCWT(dblTemp, m_iCurrentFlow, 4, 1, m_bIPUnits);
		}
		if (m_wndR2WBT6.GetModify()) 
		{
			DDX_Text(pDX, IDC_EDIT_R2_CWT6, dblTemp);
			DDV_MinMaxDouble(pDX,dblTemp,CWT_MIN_IP,CWT_MAX_IP,
				CWT_MIN_SI,CWT_MAX_SI,2);
			m_designData.m_fnSetCWT(dblTemp, m_iCurrentFlow, 5, 1, m_bIPUnits);
		}

		
		if (m_wndR3WBT1.GetModify()) 
		{
			DDX_Text(pDX, IDC_EDIT_R3_CWT1, dblTemp);
			DDV_MinMaxDouble(pDX,dblTemp,CWT_MIN_IP,CWT_MAX_IP,
				CWT_MIN_SI,CWT_MAX_SI,2);
			m_designData.m_fnSetCWT(dblTemp, m_iCurrentFlow, 0, 2, m_bIPUnits);
		}
		if (m_wndR3WBT2.GetModify()) 
		{
			DDX_Text(pDX, IDC_EDIT_R3_CWT2, dblTemp);
			DDV_MinMaxDouble(pDX,dblTemp,CWT_MIN_IP,CWT_MAX_IP,
				CWT_MIN_SI,CWT_MAX_SI,2);
			m_designData.m_fnSetCWT(dblTemp, m_iCurrentFlow, 1, 2, m_bIPUnits);
		}
		if (m_wndR3WBT3.GetModify()) 
		{
			DDX_Text(pDX, IDC_EDIT_R3_CWT3, dblTemp);
			DDV_MinMaxDouble(pDX,dblTemp,CWT_MIN_IP,CWT_MAX_IP,
				CWT_MIN_SI,CWT_MAX_SI,2);
			m_designData.m_fnSetCWT(dblTemp, m_iCurrentFlow, 2, 2, m_bIPUnits);
		}
		if (m_wndR3WBT4.GetModify()) 
		{
			DDX_Text(pDX, IDC_EDIT_R3_CWT4, dblTemp);
			DDV_MinMaxDouble(pDX,dblTemp,CWT_MIN_IP,CWT_MAX_IP,
				CWT_MIN_SI,CWT_MAX_SI,2);
			m_designData.m_fnSetCWT(dblTemp, m_iCurrentFlow, 3, 2, m_bIPUnits);
		}
		if (m_wndR3WBT5.GetModify()) 
		{
			DDX_Text(pDX, IDC_EDIT_R3_CWT5, dblTemp);
			DDV_MinMaxDouble(pDX,dblTemp,CWT_MIN_IP,CWT_MAX_IP,
				CWT_MIN_SI,CWT_MAX_SI,2);
			m_designData.m_fnSetCWT(dblTemp, m_iCurrentFlow, 4, 2, m_bIPUnits);
		}
		if (m_wndR3WBT6.GetModify()) 
		{
			DDX_Text(pDX, IDC_EDIT_R3_CWT6, dblTemp);
			DDV_MinMaxDouble(pDX,dblTemp,CWT_MIN_IP,CWT_MAX_IP,
				CWT_MIN_SI,CWT_MAX_SI,2);
			m_designData.m_fnSetCWT(dblTemp, m_iCurrentFlow, 5, 2, m_bIPUnits);
		}

		
		if (m_wndR4WBT1.GetModify()) 
		{
			DDX_Text(pDX, IDC_EDIT_R4_CWT1, dblTemp);
			DDV_MinMaxDouble(pDX,dblTemp,CWT_MIN_IP,CWT_MAX_IP,
				CWT_MIN_SI,CWT_MAX_SI,2);
			m_designData.m_fnSetCWT(dblTemp, m_iCurrentFlow, 0, 3, m_bIPUnits);
		}
		if (m_wndR4WBT2.GetModify()) 
		{
			DDX_Text(pDX, IDC_EDIT_R4_CWT2, dblTemp);
			DDV_MinMaxDouble(pDX,dblTemp,CWT_MIN_IP,CWT_MAX_IP,
				CWT_MIN_SI,CWT_MAX_SI,2);
			m_designData.m_fnSetCWT(dblTemp, m_iCurrentFlow, 1, 3, m_bIPUnits);
		}
		if (m_wndR4WBT3.GetModify()) 
		{
			DDX_Text(pDX, IDC_EDIT_R4_CWT3, dblTemp);
			DDV_MinMaxDouble(pDX,dblTemp,CWT_MIN_IP,CWT_MAX_IP,
				CWT_MIN_SI,CWT_MAX_SI,2);
			m_designData.m_fnSetCWT(dblTemp, m_iCurrentFlow, 2, 3, m_bIPUnits);
		}
		if (m_wndR4WBT4.GetModify()) 
		{
			DDX_Text(pDX, IDC_EDIT_R4_CWT4, dblTemp);
			DDV_MinMaxDouble(pDX,dblTemp,CWT_MIN_IP,CWT_MAX_IP,
				CWT_MIN_SI,CWT_MAX_SI,2);
			m_designData.m_fnSetCWT(dblTemp, m_iCurrentFlow, 3, 3, m_bIPUnits);
		}
		if (m_wndR4WBT5.GetModify()) 
		{
			DDX_Text(pDX, IDC_EDIT_R4_CWT5, dblTemp);
			DDV_MinMaxDouble(pDX,dblTemp,CWT_MIN_IP,CWT_MAX_IP,
				CWT_MIN_SI,CWT_MAX_SI,2);
			m_designData.m_fnSetCWT(dblTemp, m_iCurrentFlow, 4, 3, m_bIPUnits);
		}
		if (m_wndR4WBT6.GetModify()) 
		{
			DDX_Text(pDX, IDC_EDIT_R4_CWT6, dblTemp);
			DDV_MinMaxDouble(pDX,dblTemp,CWT_MIN_IP,CWT_MAX_IP,
				CWT_MIN_SI,CWT_MAX_SI,2);
			m_designData.m_fnSetCWT(dblTemp, m_iCurrentFlow, 5, 3, m_bIPUnits);
		}

		
		if (m_wndR5WBT1.GetModify()) 
		{
			DDX_Text(pDX, IDC_EDIT_R5_CWT1, dblTemp);
			DDV_MinMaxDouble(pDX,dblTemp,CWT_MIN_IP,CWT_MAX_IP,
				CWT_MIN_SI,CWT_MAX_SI,2);
			m_designData.m_fnSetCWT(dblTemp, m_iCurrentFlow, 0, 4, m_bIPUnits);
		}
		if (m_wndR5WBT2.GetModify()) 
		{
			DDX_Text(pDX, IDC_EDIT_R5_CWT2, dblTemp);
			DDV_MinMaxDouble(pDX,dblTemp,CWT_MIN_IP,CWT_MAX_IP,
				CWT_MIN_SI,CWT_MAX_SI,2);
			m_designData.m_fnSetCWT(dblTemp, m_iCurrentFlow, 1, 4, m_bIPUnits);
		}
		if (m_wndR5WBT3.GetModify()) 
		{
			DDX_Text(pDX, IDC_EDIT_R5_CWT3, dblTemp);
			DDV_MinMaxDouble(pDX,dblTemp,CWT_MIN_IP,CWT_MAX_IP,
				CWT_MIN_SI,CWT_MAX_SI,2);
			m_designData.m_fnSetCWT(dblTemp, m_iCurrentFlow, 2, 4, m_bIPUnits);
		}
		if (m_wndR5WBT4.GetModify()) 
		{
			DDX_Text(pDX, IDC_EDIT_R5_CWT4, dblTemp);
			DDV_MinMaxDouble(pDX,dblTemp,CWT_MIN_IP,CWT_MAX_IP,
				CWT_MIN_SI,CWT_MAX_SI,2);
			m_designData.m_fnSetCWT(dblTemp, m_iCurrentFlow, 3, 4, m_bIPUnits);
		}
		if (m_wndR5WBT5.GetModify()) 
		{
			DDX_Text(pDX, IDC_EDIT_R5_CWT5, dblTemp);
			DDV_MinMaxDouble(pDX,dblTemp,CWT_MIN_IP,CWT_MAX_IP,
				CWT_MIN_SI,CWT_MAX_SI,2);
			m_designData.m_fnSetCWT(dblTemp, m_iCurrentFlow, 4, 4, m_bIPUnits);
		}
		if (m_wndR5WBT6.GetModify()) 
		{
			DDX_Text(pDX, IDC_EDIT_R5_CWT6, dblTemp);
			DDV_MinMaxDouble(pDX,dblTemp,CWT_MIN_IP,CWT_MAX_IP,
				CWT_MIN_SI,CWT_MAX_SI,2);
			m_designData.m_fnSetCWT(dblTemp, m_iCurrentFlow, 5, 4, m_bIPUnits);
		}

	}
	else
	{
		//
		// Set values
		//
		CString strTemp;
		strTemp = m_designData.m_fnGetTowerLocation();
		DDX_Text(pDX, IDC_EDIT_LOCATION, strTemp);
		strTemp = m_designData.m_fnGetTowerManufacturer();
		DDX_Text(pDX, IDC_EDIT_MANUFACTURER, strTemp);
		strTemp = m_designData.m_fnGetTowerOwner();
		DDX_Text(pDX, IDC_EDIT_OWNER_NAME, strTemp);
		strTemp = m_designData.m_fnGetProjectName();
		DDX_Text(pDX, IDC_EDIT_PROJECT_NAME, strTemp);

#ifdef _TOWER_TYPE
		if (m_designData.m_fnGetInducedFlow())
		{
			CheckRadioButton(IDC_RADIO_INDUCED, IDC_RADIO_FORCED, IDC_RADIO_INDUCED);
		}
		else
		{
			CheckRadioButton(IDC_RADIO_INDUCED, IDC_RADIO_FORCED, IDC_RADIO_FORCED);
		}
#endif // _TOWER_TYPE
	
		dblTemp = truncit(m_designData.m_fnGetFanDriverPower(m_bIPUnits)		, 1);
		DDX_Text(pDX, IDC_EDIT_DESIGN_FAN_DRIVER, dblTemp);
		dblTemp = truncit(m_designData.m_fnGetHWT(m_bIPUnits)					, 2);
		DDX_Text(pDX, IDC_EDIT_DESIGN_HOT_WATER, dblTemp);
		dblTemp = truncit(m_designData.m_fnGetCWT(m_bIPUnits)					, 2);
		DDX_Text(pDX, IDC_EDIT_DESIGN_COLD_WATER, dblTemp);
		dblTemp = truncit(m_designData.m_fnGetDBT(m_bIPUnits)					, 2);
		DDX_Text(pDX, IDC_EDIT_DESIGN_DRY_BULB, dblTemp);
		dblTemp = truncit(m_designData.m_fnGetWBT(m_bIPUnits)					, 2);
		DDX_Text(pDX, IDC_EDIT_DESIGN_WET_BULB, dblTemp);
		dblTemp = truncit(m_designData.m_fnGetBarometricPressure(m_bIPUnits)	, 3);
		DDX_Text(pDX, IDC_EDIT_DESIGN_BAROMETRIC_PRESSURE, dblTemp);
		dblTemp = truncit(m_designData.m_fnGetWaterFlowRate(m_bIPUnits)			, 1);
		DDX_Text(pDX, IDC_EDIT_DESIGN_WATER_FLOW, dblTemp);
		dblTemp = truncit(m_designData.m_fnGetLG(m_bIPUnits)					, 4);
		DDX_Text(pDX, IDC_EDIT_DESIGN_LG, dblTemp);
		
		dblTemp = truncit(m_designData.m_fnGetRange(0, m_bIPUnits)				, 2);
		DDX_Text(pDX, IDC_EDIT_R1, dblTemp);
		dblTemp = truncit(m_designData.m_fnGetRange(1, m_bIPUnits)				, 2);
		DDX_Text(pDX, IDC_EDIT_R2, dblTemp);
		dblTemp = truncit(m_designData.m_fnGetRange(2, m_bIPUnits)				, 2);
		DDX_Text(pDX, IDC_EDIT_R3, dblTemp);
		dblTemp = truncit(m_designData.m_fnGetRange(3, m_bIPUnits)				, 2);
		DDX_Text(pDX, IDC_EDIT_R4, dblTemp);
		dblTemp = truncit(m_designData.m_fnGetRange(4, m_bIPUnits)				, 2);
		DDX_Text(pDX, IDC_EDIT_R5, dblTemp);

		if ((m_iCurrentFlow < m_designData.m_fnGetFlowCnt()) && (m_iCurrentFlow > -1))
		{
			dblTemp = truncit(m_designData.m_fnGetWBT(m_iCurrentFlow, 0, m_bIPUnits)	, 2);
			DDX_Text(pDX, IDC_EDIT_WBT1, dblTemp);
			dblTemp = truncit(m_designData.m_fnGetWBT(m_iCurrentFlow, 1, m_bIPUnits)	, 2);
			DDX_Text(pDX, IDC_EDIT_WBT2, dblTemp);
			dblTemp = truncit(m_designData.m_fnGetWBT(m_iCurrentFlow, 2, m_bIPUnits)	, 2);
			DDX_Text(pDX, IDC_EDIT_WBT3, dblTemp);
			dblTemp = truncit(m_designData.m_fnGetWBT(m_iCurrentFlow, 3, m_bIPUnits)	, 2);
			DDX_Text(pDX, IDC_EDIT_WBT4, dblTemp);
			dblTemp = truncit(m_designData.m_fnGetWBT(m_iCurrentFlow, 4, m_bIPUnits)	, 2);
			DDX_Text(pDX, IDC_EDIT_WBT5, dblTemp);
			dblTemp = truncit(m_designData.m_fnGetWBT(m_iCurrentFlow, 5, m_bIPUnits)	, 2);
			DDX_Text(pDX, IDC_EDIT_WBT6, dblTemp);

			dblTemp = truncit(m_designData.m_fnGetCWT(m_iCurrentFlow, 0, 0, m_bIPUnits)	, 2);
			DDX_Text(pDX, IDC_EDIT_R1_CWT1, dblTemp);
			dblTemp = truncit(m_designData.m_fnGetCWT(m_iCurrentFlow, 1, 0, m_bIPUnits)	, 2);
			DDX_Text(pDX, IDC_EDIT_R1_CWT2, dblTemp);
			dblTemp = truncit(m_designData.m_fnGetCWT(m_iCurrentFlow, 2, 0, m_bIPUnits)	, 2);
			DDX_Text(pDX, IDC_EDIT_R1_CWT3, dblTemp);
			dblTemp = truncit(m_designData.m_fnGetCWT(m_iCurrentFlow, 3, 0, m_bIPUnits)	, 2);
			DDX_Text(pDX, IDC_EDIT_R1_CWT4, dblTemp);
			dblTemp = truncit(m_designData.m_fnGetCWT(m_iCurrentFlow, 4, 0, m_bIPUnits)	, 2);
			DDX_Text(pDX, IDC_EDIT_R1_CWT5, dblTemp);
			dblTemp = truncit(m_designData.m_fnGetCWT(m_iCurrentFlow, 5, 0, m_bIPUnits)	, 2);
			DDX_Text(pDX, IDC_EDIT_R1_CWT6, dblTemp);

			dblTemp = truncit(m_designData.m_fnGetCWT(m_iCurrentFlow, 0, 1, m_bIPUnits)	, 2);
			DDX_Text(pDX, IDC_EDIT_R2_CWT1, dblTemp);
			dblTemp = truncit(m_designData.m_fnGetCWT(m_iCurrentFlow, 1, 1, m_bIPUnits)	, 2);
			DDX_Text(pDX, IDC_EDIT_R2_CWT2, dblTemp);
			dblTemp = truncit(m_designData.m_fnGetCWT(m_iCurrentFlow, 2, 1, m_bIPUnits)	, 2);
			DDX_Text(pDX, IDC_EDIT_R2_CWT3, dblTemp);
			dblTemp = truncit(m_designData.m_fnGetCWT(m_iCurrentFlow, 3, 1, m_bIPUnits)	, 2);
			DDX_Text(pDX, IDC_EDIT_R2_CWT4, dblTemp);
			dblTemp = truncit(m_designData.m_fnGetCWT(m_iCurrentFlow, 4, 1, m_bIPUnits)	, 2);
			DDX_Text(pDX, IDC_EDIT_R2_CWT5, dblTemp);
			dblTemp = truncit(m_designData.m_fnGetCWT(m_iCurrentFlow, 5, 1, m_bIPUnits)	, 2);
			DDX_Text(pDX, IDC_EDIT_R2_CWT6, dblTemp);

			dblTemp = truncit(m_designData.m_fnGetCWT(m_iCurrentFlow, 0, 2, m_bIPUnits)	, 2);
			DDX_Text(pDX, IDC_EDIT_R3_CWT1, dblTemp);
			dblTemp = truncit(m_designData.m_fnGetCWT(m_iCurrentFlow, 1, 2, m_bIPUnits)	, 2);
			DDX_Text(pDX, IDC_EDIT_R3_CWT2, dblTemp);
			dblTemp = truncit(m_designData.m_fnGetCWT(m_iCurrentFlow, 2, 2, m_bIPUnits)	, 2);
			DDX_Text(pDX, IDC_EDIT_R3_CWT3, dblTemp);
			dblTemp = truncit(m_designData.m_fnGetCWT(m_iCurrentFlow, 3, 2, m_bIPUnits)	, 2);
			DDX_Text(pDX, IDC_EDIT_R3_CWT4, dblTemp);
			dblTemp = truncit(m_designData.m_fnGetCWT(m_iCurrentFlow, 4, 2, m_bIPUnits)	, 2);
			DDX_Text(pDX, IDC_EDIT_R3_CWT5, dblTemp);
			dblTemp = truncit(m_designData.m_fnGetCWT(m_iCurrentFlow, 5, 2, m_bIPUnits)	, 2);
			DDX_Text(pDX, IDC_EDIT_R3_CWT6, dblTemp);

			dblTemp = truncit(m_designData.m_fnGetCWT(m_iCurrentFlow, 0, 3, m_bIPUnits)	, 2);
			DDX_Text(pDX, IDC_EDIT_R4_CWT1, dblTemp);
			dblTemp = truncit(m_designData.m_fnGetCWT(m_iCurrentFlow, 1, 3, m_bIPUnits)	, 2);
			DDX_Text(pDX, IDC_EDIT_R4_CWT2, dblTemp);
			dblTemp = truncit(m_designData.m_fnGetCWT(m_iCurrentFlow, 2, 3, m_bIPUnits)	, 2);
			DDX_Text(pDX, IDC_EDIT_R4_CWT3, dblTemp);
			dblTemp = truncit(m_designData.m_fnGetCWT(m_iCurrentFlow, 3, 3, m_bIPUnits)	, 2);
			DDX_Text(pDX, IDC_EDIT_R4_CWT4, dblTemp);
			dblTemp = truncit(m_designData.m_fnGetCWT(m_iCurrentFlow, 4, 3, m_bIPUnits)	, 2);
			DDX_Text(pDX, IDC_EDIT_R4_CWT5, dblTemp);
			dblTemp = truncit(m_designData.m_fnGetCWT(m_iCurrentFlow, 5, 3, m_bIPUnits)	, 2);
			DDX_Text(pDX, IDC_EDIT_R4_CWT6, dblTemp);

			dblTemp = truncit(m_designData.m_fnGetCWT(m_iCurrentFlow, 0, 4, m_bIPUnits)	, 2);
			DDX_Text(pDX, IDC_EDIT_R5_CWT1, dblTemp);
			dblTemp = truncit(m_designData.m_fnGetCWT(m_iCurrentFlow, 1, 4, m_bIPUnits)	, 2);
			DDX_Text(pDX, IDC_EDIT_R5_CWT2, dblTemp);
			dblTemp = truncit(m_designData.m_fnGetCWT(m_iCurrentFlow, 2, 4, m_bIPUnits)	, 2);
			DDX_Text(pDX, IDC_EDIT_R5_CWT3, dblTemp);
			dblTemp = truncit(m_designData.m_fnGetCWT(m_iCurrentFlow, 3, 4, m_bIPUnits)	, 2);
			DDX_Text(pDX, IDC_EDIT_R5_CWT4, dblTemp);
			dblTemp = truncit(m_designData.m_fnGetCWT(m_iCurrentFlow, 4, 4, m_bIPUnits)	, 2);
			DDX_Text(pDX, IDC_EDIT_R5_CWT5, dblTemp);
			dblTemp = truncit(m_designData.m_fnGetCWT(m_iCurrentFlow, 5, 4, m_bIPUnits)	, 2);
			DDX_Text(pDX, IDC_EDIT_R5_CWT6, dblTemp);
		}
		else
		{
			dblTemp = 0;
			DDX_Text(pDX, IDC_EDIT_WBT1, dblTemp);
			DDX_Text(pDX, IDC_EDIT_WBT2, dblTemp);
			DDX_Text(pDX, IDC_EDIT_WBT3, dblTemp);
			DDX_Text(pDX, IDC_EDIT_WBT4, dblTemp);
			DDX_Text(pDX, IDC_EDIT_WBT5, dblTemp);
			DDX_Text(pDX, IDC_EDIT_WBT6, dblTemp);

			DDX_Text(pDX, IDC_EDIT_R1_CWT1, dblTemp);
			DDX_Text(pDX, IDC_EDIT_R1_CWT2, dblTemp);
			DDX_Text(pDX, IDC_EDIT_R1_CWT3, dblTemp);
			DDX_Text(pDX, IDC_EDIT_R1_CWT4, dblTemp);
			DDX_Text(pDX, IDC_EDIT_R1_CWT5, dblTemp);
			DDX_Text(pDX, IDC_EDIT_R1_CWT6, dblTemp);

			DDX_Text(pDX, IDC_EDIT_R2_CWT1, dblTemp);
			DDX_Text(pDX, IDC_EDIT_R2_CWT2, dblTemp);
			DDX_Text(pDX, IDC_EDIT_R2_CWT3, dblTemp);
			DDX_Text(pDX, IDC_EDIT_R2_CWT4, dblTemp);
			DDX_Text(pDX, IDC_EDIT_R2_CWT5, dblTemp);
			DDX_Text(pDX, IDC_EDIT_R2_CWT6, dblTemp);

			DDX_Text(pDX, IDC_EDIT_R3_CWT1, dblTemp);
			DDX_Text(pDX, IDC_EDIT_R3_CWT2, dblTemp);
			DDX_Text(pDX, IDC_EDIT_R3_CWT3, dblTemp);
			DDX_Text(pDX, IDC_EDIT_R3_CWT4, dblTemp);
			DDX_Text(pDX, IDC_EDIT_R3_CWT5, dblTemp);
			DDX_Text(pDX, IDC_EDIT_R3_CWT6, dblTemp);

			DDX_Text(pDX, IDC_EDIT_R4_CWT1, dblTemp);
			DDX_Text(pDX, IDC_EDIT_R4_CWT2, dblTemp);
			DDX_Text(pDX, IDC_EDIT_R4_CWT3, dblTemp);
			DDX_Text(pDX, IDC_EDIT_R4_CWT4, dblTemp);
			DDX_Text(pDX, IDC_EDIT_R4_CWT5, dblTemp);
			DDX_Text(pDX, IDC_EDIT_R4_CWT6, dblTemp);

			DDX_Text(pDX, IDC_EDIT_R5_CWT1, dblTemp);
			DDX_Text(pDX, IDC_EDIT_R5_CWT2, dblTemp);
			DDX_Text(pDX, IDC_EDIT_R5_CWT3, dblTemp);
			DDX_Text(pDX, IDC_EDIT_R5_CWT4, dblTemp);
			DDX_Text(pDX, IDC_EDIT_R5_CWT5, dblTemp);
			DDX_Text(pDX, IDC_EDIT_R5_CWT6, dblTemp);
		}
		m_wndWBT6.SetModify(false);
		m_wndWBT5.SetModify(false);
		m_wndWBT4.SetModify(false);
		m_wndWBT3.SetModify(false);
		m_wndWBT2.SetModify(false);
		m_wndWBT1.SetModify(false);
		m_wndR5WBT1.SetModify(false);
		m_wndR5WBT2.SetModify(false);
		m_wndR5WBT3.SetModify(false);
		m_wndR5WBT4.SetModify(false);
		m_wndR5WBT5.SetModify(false);
		m_wndR5WBT6.SetModify(false);
		m_wndR4WBT1.SetModify(false);
		m_wndR4WBT2.SetModify(false);
		m_wndR4WBT3.SetModify(false);
		m_wndR4WBT4.SetModify(false);
		m_wndR4WBT5.SetModify(false);
		m_wndR4WBT6.SetModify(false);
		m_wndR3WBT1.SetModify(false);
		m_wndR3WBT2.SetModify(false);
		m_wndR3WBT3.SetModify(false);
		m_wndR3WBT4.SetModify(false);
		m_wndR3WBT5.SetModify(false);
		m_wndR3WBT6.SetModify(false);
		m_wndR2WBT1.SetModify(false);
		m_wndR2WBT2.SetModify(false);
		m_wndR2WBT3.SetModify(false);
		m_wndR2WBT4.SetModify(false);
		m_wndR2WBT5.SetModify(false);
		m_wndR2WBT6.SetModify(false);
		m_wndR1WBT1.SetModify(false);
		m_wndR1WBT2.SetModify(false);
		m_wndR1WBT3.SetModify(false);
		m_wndR1WBT4.SetModify(false);
		m_wndR1WBT5.SetModify(false);
		m_wndR1WBT6.SetModify(false);
		m_wndR1.SetModify(false);
		m_wndR2.SetModify(false);
		m_wndR3.SetModify(false);
		m_wndR4.SetModify(false);
		m_wndR5.SetModify(false);
	}
}


BEGIN_MESSAGE_MAP(TPerformanceDataDlg, CDialog)
	//{{AFX_MSG_MAP(TPerformanceDataDlg)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_FLOWS, OnSelChangeTabFlows)
	ON_NOTIFY(NM_CLICK, IDC_TAB_FLOWS, OnClickTabFlows)
	ON_EN_CHANGE(IDC_EDIT_R1, OnChangeEditR1)
	ON_EN_CHANGE(IDC_EDIT_R2, OnChangeEditR2)
	ON_EN_CHANGE(IDC_EDIT_R3, OnChangeEditR3)
	ON_EN_CHANGE(IDC_EDIT_R4, OnChangeEditR4)
	ON_EN_CHANGE(IDC_EDIT_R5, OnChangeEditR5)
	ON_NOTIFY(NM_RCLICK, IDC_TAB_FLOWS, OnRclickTabFlows)
	ON_COMMAND(IDM_FLOWTABS_DELETE, OnFlowtabsDelete)
	ON_COMMAND(IDM_FLOWTABS_EDIT, OnFlowtabsEdit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// TPerformanceDataDlg message handlers

void TPerformanceDataDlg::OnSelChangeTabFlows(NMHDR* /*pNMHDR*/, LRESULT* pResult) 
{
	int iIndex = m_wndFlowTabs.GetCurSel();

	if (iIndex == m_wndFlowTabs.GetItemCount()-1)
	{
	}
	
	*pResult = 0;
}

void TPerformanceDataDlg::OnRclickTabFlows(NMHDR* /* pNMHDR */, LRESULT* pResult) 
{
	if (m_designData.m_fnCanModify())
	{
		TCHITTESTINFO hitTestInfo;
		DWORD dwPos = GetMessagePos();
		hitTestInfo.pt.x = LOWORD (dwPos);
		hitTestInfo.pt.y = HIWORD (dwPos);
		m_wndFlowTabs.ScreenToClient(&(hitTestInfo.pt));
		int iIndex = m_wndFlowTabs.HitTest(&hitTestInfo );


		UpdateData();

		if ((iIndex < m_wndFlowTabs.GetItemCount()-1) || ((m_wndFlowTabs.GetItemCount() == MAX_FLOWS) && (iIndex == MAX_FLOWS-1)))
		{
			m_iTempIndex = iIndex;
			
			CMenu menuFlows;
			CMenu *psubmenuFlows;
			menuFlows.LoadMenu(IDR_MENU_FLOWS);
			psubmenuFlows = menuFlows.GetSubMenu(0);
			psubmenuFlows->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, LOWORD(GetMessagePos()), HIWORD(GetMessagePos()), this);
		}
	}
	*pResult = 0;
}

void TPerformanceDataDlg::OnClickTabFlows(NMHDR* /*pNMHDR*/, LRESULT* pResult) 
{
	int iIndex = m_wndFlowTabs.GetCurSel();
	TCITEM stItem;
	char szTabName[50];

	UpdateData();

	stItem.mask = TCIF_TEXT;
	stItem.pszText = szTabName;
	stItem.cchTextMax = 50;
	m_wndFlowTabs.GetItem(iIndex, &stItem);

	if (strcmp(szTabName, "Click to add flow") == 0)
	{
		TNewFlowRateDlg dlgFlowRate;

		if (m_wndFlowTabs.GetItemCount() > 1)
		{
			dlgFlowRate.m_dblClosestFlow = m_designData.m_fnGetFlow(m_wndFlowTabs.GetItemCount()-2, m_bIPUnits);
		}

		if (dlgFlowRate.DoModal() == IDOK)
		{
			CString strTab;
			m_wndFlowTabs.DeleteItem(iIndex);
			strTab.Format("%.2f", dlgFlowRate.m_dblFlowRate);
			
			if (m_wndFlowTabs.GetItemCount() < (MAX_FLOWS - 1))
			{
				m_wndFlowTabs.InsertItem(m_wndFlowTabs.GetItemCount(), "Click to add flow");
			}

			m_wndFlowTabs.InsertItem(iIndex, strTab);
			m_wndFlowTabs.SetCurSel(iIndex);
			m_designData.m_fnSetFlowCnt(m_designData.m_fnGetFlowCnt()+1);
			m_designData.m_fnSetFlow(dlgFlowRate.m_dblFlowRate, iIndex, m_bIPUnits);

			m_iCurrentFlow = iIndex;

			//
			// Init new Values
			//
			m_designData.m_fnSetCWT(0.0, m_iCurrentFlow, 0, 0, m_bIPUnits);
			m_designData.m_fnSetCWT(0.0, m_iCurrentFlow, 1, 0, m_bIPUnits);
			m_designData.m_fnSetCWT(0.0, m_iCurrentFlow, 2, 0, m_bIPUnits);
			m_designData.m_fnSetCWT(0.0, m_iCurrentFlow, 3, 0, m_bIPUnits);
			m_designData.m_fnSetCWT(0.0, m_iCurrentFlow, 4, 0, m_bIPUnits);
			m_designData.m_fnSetCWT(0.0, m_iCurrentFlow, 5, 0, m_bIPUnits);

			m_designData.m_fnSetCWT(0.0, m_iCurrentFlow, 0, 1, m_bIPUnits);
			m_designData.m_fnSetCWT(0.0, m_iCurrentFlow, 1, 1, m_bIPUnits);
			m_designData.m_fnSetCWT(0.0, m_iCurrentFlow, 2, 1, m_bIPUnits);
			m_designData.m_fnSetCWT(0.0, m_iCurrentFlow, 3, 1, m_bIPUnits);
			m_designData.m_fnSetCWT(0.0, m_iCurrentFlow, 4, 1, m_bIPUnits);
			m_designData.m_fnSetCWT(0.0, m_iCurrentFlow, 5, 1, m_bIPUnits);

			m_designData.m_fnSetCWT(0.0, m_iCurrentFlow, 0, 2, m_bIPUnits);
			m_designData.m_fnSetCWT(0.0, m_iCurrentFlow, 1, 2, m_bIPUnits);
			m_designData.m_fnSetCWT(0.0, m_iCurrentFlow, 2, 2, m_bIPUnits);
			m_designData.m_fnSetCWT(0.0, m_iCurrentFlow, 3, 2, m_bIPUnits);
			m_designData.m_fnSetCWT(0.0, m_iCurrentFlow, 4, 2, m_bIPUnits);
			m_designData.m_fnSetCWT(0.0, m_iCurrentFlow, 5, 2, m_bIPUnits);

			m_designData.m_fnSetCWT(0.0, m_iCurrentFlow, 0, 3, m_bIPUnits);
			m_designData.m_fnSetCWT(0.0, m_iCurrentFlow, 1, 3, m_bIPUnits);
			m_designData.m_fnSetCWT(0.0, m_iCurrentFlow, 2, 3, m_bIPUnits);
			m_designData.m_fnSetCWT(0.0, m_iCurrentFlow, 3, 3, m_bIPUnits);
			m_designData.m_fnSetCWT(0.0, m_iCurrentFlow, 4, 3, m_bIPUnits);
			m_designData.m_fnSetCWT(0.0, m_iCurrentFlow, 5, 3, m_bIPUnits);

			m_designData.m_fnSetCWT(0.0, m_iCurrentFlow, 0, 4, m_bIPUnits);
			m_designData.m_fnSetCWT(0.0, m_iCurrentFlow, 1, 4, m_bIPUnits);
			m_designData.m_fnSetCWT(0.0, m_iCurrentFlow, 2, 4, m_bIPUnits);
			m_designData.m_fnSetCWT(0.0, m_iCurrentFlow, 3, 4, m_bIPUnits);
			m_designData.m_fnSetCWT(0.0, m_iCurrentFlow, 4, 4, m_bIPUnits);
			m_designData.m_fnSetCWT(0.0, m_iCurrentFlow, 5, 4, m_bIPUnits);

			m_designData.m_fnSetWBT(0.0, m_iCurrentFlow, 0, m_bIPUnits);
			m_designData.m_fnSetWBT(0.0, m_iCurrentFlow, 1, m_bIPUnits);
			m_designData.m_fnSetWBT(0.0, m_iCurrentFlow, 2, m_bIPUnits);
			m_designData.m_fnSetWBT(0.0, m_iCurrentFlow, 3, m_bIPUnits);
			m_designData.m_fnSetWBT(0.0, m_iCurrentFlow, 4, m_bIPUnits);
			m_designData.m_fnSetWBT(0.0, m_iCurrentFlow, 5, m_bIPUnits);

			if (m_iCurrentFlow != 0)
			{
				m_designData.m_fnSetWBT(m_designData.m_fnGetWBT(m_iCurrentFlow-1, 0, m_bIPUnits), m_iCurrentFlow, 0, m_bIPUnits);
				m_designData.m_fnSetWBT(m_designData.m_fnGetWBT(m_iCurrentFlow-1, 1, m_bIPUnits), m_iCurrentFlow, 1, m_bIPUnits);
				m_designData.m_fnSetWBT(m_designData.m_fnGetWBT(m_iCurrentFlow-1, 2, m_bIPUnits), m_iCurrentFlow, 2, m_bIPUnits);
				m_designData.m_fnSetWBT(m_designData.m_fnGetWBT(m_iCurrentFlow-1, 3, m_bIPUnits), m_iCurrentFlow, 3, m_bIPUnits);
				m_designData.m_fnSetWBT(m_designData.m_fnGetWBT(m_iCurrentFlow-1, 4, m_bIPUnits), m_iCurrentFlow, 4, m_bIPUnits);
				m_designData.m_fnSetWBT(m_designData.m_fnGetWBT(m_iCurrentFlow-1, 5, m_bIPUnits), m_iCurrentFlow, 5, m_bIPUnits);
			}
		}
		else
		{
			iIndex = m_iCurrentFlow;
			m_wndFlowTabs.SetCurSel(m_iCurrentFlow);
		}
	}
	else
	{
		m_iCurrentFlow = iIndex;
	}

	m_fnShowWBTs();
	m_fnHideRange(1);
	m_fnHideRange(2);
	m_fnHideRange(3);
	m_fnHideRange(4);
	m_fnHideRange(5);
	for (int iRIndex = 1; iRIndex <= m_designData.m_fnGetRangeCnt(); iRIndex++)
	{
		m_fnShowRange(iRIndex);
	}

	UpdateData(false);
	
	*pResult = 0;
}

BOOL TPerformanceDataDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_fnHideWBTs();
	m_fnHideRange(1);
	m_fnHideRange(2);
	m_fnHideRange(3);
	m_fnHideRange(4);
	m_fnHideRange(5);

	m_strSpecsCaption = SZ_SPECS;
	m_strCurveCaption = SZ_CURVE;
    if (TPropPageBase::metricflag)
	{
		m_strSpecsCaption += " (SI)";
		m_strCurveCaption += " (SI)";
	}
	else
	{
		m_strSpecsCaption += " (IP)";
		m_strCurveCaption += " (IP)";
	}

	for (int iFlowIndex = 0; iFlowIndex < m_designData.m_fnGetFlowCnt(); iFlowIndex++)
	{
		CString strFlow;
		strFlow.Format("%.02f", m_designData.m_fnGetFlow(iFlowIndex, m_bIPUnits));
		m_wndFlowTabs.InsertItem(iFlowIndex, strFlow);
	}

	if (m_wndFlowTabs.GetItemCount() > 0)
	{
		m_wndFlowTabs.SetCurSel(0);

		m_fnShowWBTs();
		for (int iRangeIndex = 0; iRangeIndex < m_designData.m_fnGetRangeCnt(); iRangeIndex++)
		{
			m_fnShowRange(iRangeIndex+1);
		}
	}

	UpdateData(false);

	if (!m_designData.m_fnCanModify())
	{
		GetDlgItem(IDC_EDIT_OWNER_NAME  )->EnableWindow(false);
		GetDlgItem(IDC_EDIT_PROJECT_NAME)->EnableWindow(false);
		GetDlgItem(IDC_EDIT_LOCATION    )->EnableWindow(false);
		GetDlgItem(IDC_EDIT_MANUFACTURER)->EnableWindow(false);
#ifdef _TOWER_TYPE
		GetDlgItem(IDC_RADIO_INDUCED)->EnableWindow(false);
		GetDlgItem(IDC_RADIO_FORCED)->EnableWindow(false);
#endif // _TOWER_TYPE

		GetDlgItem(IDC_EDIT_DESIGN_WATER_FLOW			)->EnableWindow(false);
		GetDlgItem(IDC_EDIT_DESIGN_HOT_WATER			)->EnableWindow(false);
		GetDlgItem(IDC_EDIT_DESIGN_COLD_WATER			)->EnableWindow(false);
		GetDlgItem(IDC_EDIT_DESIGN_WET_BULB				)->EnableWindow(false);
		GetDlgItem(IDC_EDIT_DESIGN_DRY_BULB				)->EnableWindow(false);
		GetDlgItem(IDC_EDIT_DESIGN_FAN_DRIVER			)->EnableWindow(false);
		GetDlgItem(IDC_EDIT_DESIGN_BAROMETRIC_PRESSURE	)->EnableWindow(false);
		GetDlgItem(IDC_EDIT_DESIGN_LG					)->EnableWindow(false);

		GetDlgItem(IDC_EDIT_R1          )->EnableWindow(false);
		GetDlgItem(IDC_EDIT_R2          )->EnableWindow(false);
		GetDlgItem(IDC_EDIT_R3          )->EnableWindow(false);
		GetDlgItem(IDC_EDIT_R4          )->EnableWindow(false);
		GetDlgItem(IDC_EDIT_R5          )->EnableWindow(false);
		GetDlgItem(IDC_EDIT_WBT1        )->EnableWindow(false);
		GetDlgItem(IDC_EDIT_WBT2        )->EnableWindow(false);
		GetDlgItem(IDC_EDIT_WBT3        )->EnableWindow(false);
		GetDlgItem(IDC_EDIT_WBT4        )->EnableWindow(false);
		GetDlgItem(IDC_EDIT_WBT5        )->EnableWindow(false);
		GetDlgItem(IDC_EDIT_WBT6        )->EnableWindow(false);
		GetDlgItem(IDC_EDIT_R1_CWT1     )->EnableWindow(false);
		GetDlgItem(IDC_EDIT_R1_CWT2     )->EnableWindow(false);
		GetDlgItem(IDC_EDIT_R1_CWT3     )->EnableWindow(false);
		GetDlgItem(IDC_EDIT_R1_CWT4     )->EnableWindow(false);
		GetDlgItem(IDC_EDIT_R1_CWT5     )->EnableWindow(false);
		GetDlgItem(IDC_EDIT_R1_CWT6     )->EnableWindow(false);
		GetDlgItem(IDC_EDIT_R2_CWT1     )->EnableWindow(false);
		GetDlgItem(IDC_EDIT_R2_CWT2     )->EnableWindow(false);
		GetDlgItem(IDC_EDIT_R2_CWT3     )->EnableWindow(false);
		GetDlgItem(IDC_EDIT_R2_CWT4     )->EnableWindow(false);
		GetDlgItem(IDC_EDIT_R2_CWT5     )->EnableWindow(false);
		GetDlgItem(IDC_EDIT_R2_CWT6     )->EnableWindow(false);
		GetDlgItem(IDC_EDIT_R3_CWT1     )->EnableWindow(false);
		GetDlgItem(IDC_EDIT_R3_CWT2     )->EnableWindow(false);
		GetDlgItem(IDC_EDIT_R3_CWT3     )->EnableWindow(false);
		GetDlgItem(IDC_EDIT_R3_CWT4     )->EnableWindow(false);
		GetDlgItem(IDC_EDIT_R3_CWT5     )->EnableWindow(false);
		GetDlgItem(IDC_EDIT_R3_CWT6     )->EnableWindow(false);
		GetDlgItem(IDC_EDIT_R4_CWT1     )->EnableWindow(false);
		GetDlgItem(IDC_EDIT_R4_CWT2     )->EnableWindow(false);
		GetDlgItem(IDC_EDIT_R4_CWT3     )->EnableWindow(false);
		GetDlgItem(IDC_EDIT_R4_CWT4     )->EnableWindow(false);
		GetDlgItem(IDC_EDIT_R4_CWT5     )->EnableWindow(false);
		GetDlgItem(IDC_EDIT_R4_CWT6     )->EnableWindow(false);
		GetDlgItem(IDC_EDIT_R5_CWT1     )->EnableWindow(false);
		GetDlgItem(IDC_EDIT_R5_CWT2     )->EnableWindow(false);
		GetDlgItem(IDC_EDIT_R5_CWT3     )->EnableWindow(false);
		GetDlgItem(IDC_EDIT_R5_CWT4     )->EnableWindow(false);
		GetDlgItem(IDC_EDIT_R5_CWT5     )->EnableWindow(false);
		GetDlgItem(IDC_EDIT_R5_CWT6     )->EnableWindow(false);
		
	}
	else
	{
		if (m_wndFlowTabs.GetItemCount() < MAX_FLOWS)
		{
			m_wndFlowTabs.InsertItem(m_wndFlowTabs.GetItemCount(), "Click to add flow");
		}
	}

	GetDlgItem(IDC_EDIT_OWNER_NAME)->SetFocus();
	return false;
}

bool TPerformanceDataDlg::m_fnHideRange(int iIndex)
{
	bool bReturn		= true;
	UINT uiStartIndex	= 0;
	UINT uiEndIndex		= 0;

	switch (iIndex)
	{
		case 1:
			uiStartIndex	= IDC_STATIC_GB_RANGE1;
			uiEndIndex		= IDC_EDIT_R1_CWT6;
			break;
		case 2:
			uiStartIndex	= IDC_STATIC_GB_RANGE2;
			uiEndIndex		= IDC_EDIT_R2_CWT6;
			break;
		case 3:
			uiStartIndex	= IDC_STATIC_GB_RANGE3;
			uiEndIndex		= IDC_EDIT_R3_CWT6;
			break;
		case 4:
			uiStartIndex	= IDC_STATIC_GB_RANGE4;
			uiEndIndex		= IDC_EDIT_R4_CWT6;
			break;
		case 5:
			uiStartIndex	= IDC_STATIC_GB_RANGE5;
			uiEndIndex		= IDC_EDIT_R5_CWT6;
			break;
	}

	for (UINT iItem = uiStartIndex; iItem <= uiEndIndex; iItem++)
	{
		::ShowWindow(::GetDlgItem(m_hWnd, iItem), SW_HIDE);
	}

	return bReturn;
}

bool TPerformanceDataDlg::m_fnShowRange(int iIndex)
{
	bool bReturn		= true;
	UINT uiStartIndex	= 0;
	UINT uiEndIndex		= 0;

	switch (iIndex)
	{
		case 1:
			uiStartIndex	= IDC_STATIC_GB_RANGE1;
			uiEndIndex		= IDC_EDIT_R1_CWT6;
			break;
		case 2:
			uiStartIndex	= IDC_STATIC_GB_RANGE2;
			uiEndIndex		= IDC_EDIT_R2_CWT6;
			break;
		case 3:
			uiStartIndex	= IDC_STATIC_GB_RANGE3;
			uiEndIndex		= IDC_EDIT_R3_CWT6;
			break;
		case 4:
			uiStartIndex	= IDC_STATIC_GB_RANGE4;
			uiEndIndex		= IDC_EDIT_R4_CWT6;
			break;
		case 5:
			uiStartIndex	= IDC_STATIC_GB_RANGE5;
			uiEndIndex		= IDC_EDIT_R5_CWT6;
			break;
	}

	for (UINT iItem = uiStartIndex; iItem <= uiEndIndex; iItem++)
	{
		::ShowWindow(::GetDlgItem(m_hWnd, iItem), SW_SHOW);
	}

	return bReturn;
}

bool TPerformanceDataDlg::m_fnHideWBTs()
{
	bool bReturn		= true;
	UINT uiStartIndex	= IDC_STATIC_WBT1;
	UINT uiEndIndex		= IDC_EDIT_WBT6;

	for (UINT iItem = uiStartIndex; iItem <= uiEndIndex; iItem++)
	{
		::ShowWindow(::GetDlgItem(m_hWnd, iItem), SW_HIDE);
	}

	return bReturn;
}

bool TPerformanceDataDlg::m_fnShowWBTs()
{
	bool bReturn		= true;
	UINT uiStartIndex	= IDC_STATIC_WBT1;
	UINT uiEndIndex		= IDC_EDIT_WBT6;

	for (UINT iItem = uiStartIndex; iItem <= uiEndIndex; iItem++)
	{
		::ShowWindow(::GetDlgItem(m_hWnd, iItem), SW_SHOW);
	}

	return bReturn;
}

void TPerformanceDataDlg::OnChangeEditR1() 
{
	CString strTemp;
	GetDlgItem(IDC_EDIT_R1)->GetWindowText(strTemp);
	if (atof(strTemp) == 0.0)
	{
		m_fnHideRange(1);
		m_fnHideRange(2);
		m_fnHideRange(3);
		m_fnHideRange(4);
		m_fnHideRange(5);
	}
	else
	{
		m_fnShowRange(1);
	}
}

void TPerformanceDataDlg::OnChangeEditR2() 
{
	CString strTemp;
	GetDlgItem(IDC_EDIT_R2)->GetWindowText(strTemp);
	if (atof(strTemp) == 0.0)
	{
		m_fnHideRange(2);
		m_fnHideRange(3);
		m_fnHideRange(4);
		m_fnHideRange(5);
	}
	else
	{
		m_fnShowRange(2);
	}
	
}

void TPerformanceDataDlg::OnChangeEditR3() 
{
	CString strTemp;
	GetDlgItem(IDC_EDIT_R3)->GetWindowText(strTemp);
	if (atof(strTemp) == 0.0)
	{
		m_fnHideRange(3);
		m_fnHideRange(4);
		m_fnHideRange(5);
	}
	else
	{
		m_fnShowRange(3);
	}
}

void TPerformanceDataDlg::OnChangeEditR4() 
{
	CString strTemp;
	GetDlgItem(IDC_EDIT_R4)->GetWindowText(strTemp);
	if (atof(strTemp) == 0.0)
	{
		m_fnHideRange(4);
		m_fnHideRange(5);
	}
	else
	{
		m_fnShowRange(4);
	}
}

void TPerformanceDataDlg::OnChangeEditR5() 
{
	CString strTemp;
	GetDlgItem(IDC_EDIT_R5)->GetWindowText(strTemp);
	if (atof(strTemp) == 0.0)
	{
		m_fnHideRange(5);
	}
	else
	{
		m_fnShowRange(5);
	}
}


void TPerformanceDataDlg::OnOK() 
{
//	UpdateData();	// CDialog::OnOK does this already

	CDialog::OnOK();
}

void TPerformanceDataDlg::OnFlowtabsDelete() 
{
	UpdateData();

	m_designData.m_fnDeleteFlow(m_iTempIndex);
	m_wndFlowTabs.DeleteItem(m_iTempIndex);


	TCITEM stItem;
	char szTabName[50];
	stItem.mask = TCIF_TEXT;
	stItem.pszText = szTabName;
	stItem.cchTextMax = 50;
	m_wndFlowTabs.GetItem(m_wndFlowTabs.GetItemCount()-1, &stItem);
	if ((strcmp(szTabName, "Click to add flow") != 0))
	{
		m_wndFlowTabs.InsertItem(m_wndFlowTabs.GetItemCount(), "Click to add flow");
	}

	int iIndex = m_wndFlowTabs.GetCurSel();

	if (iIndex == -1)
	{
		m_wndFlowTabs.SetCurSel(0);
		m_iCurrentFlow = 0;
	}
	else
	{
		m_iCurrentFlow = iIndex;
	}
	UpdateData(false);
}

void TPerformanceDataDlg::OnFlowtabsEdit() 
{
	int iIndex = m_iTempIndex;

	TNewFlowRateDlg dlgFlowRate;

	dlgFlowRate.m_bNewFlow		= false; // Edit Flow Rate
	dlgFlowRate.m_dblFlowRate	= m_designData.m_fnGetFlow(iIndex, m_bIPUnits);
	
	if (iIndex == 0)
	{
		if (m_wndFlowTabs.GetItemCount() > 2)
		{
			dlgFlowRate.m_dblClosestFlow = m_designData.m_fnGetFlow(1, m_bIPUnits);
		}
	}
	else
	{
		dlgFlowRate.m_dblClosestFlow = m_designData.m_fnGetFlow(iIndex-1, m_bIPUnits);
	}

	if (dlgFlowRate.DoModal() == IDOK)
	{
		m_designData.m_fnSetFlow(dlgFlowRate.m_dblFlowRate, iIndex, m_bIPUnits);
		
		CString strTab;
		TCITEM tabCtrlItem;
		m_wndFlowTabs.GetItem(iIndex, &tabCtrlItem);
		strTab.Format("%.2f", dlgFlowRate.m_dblFlowRate);
		tabCtrlItem.mask	= TCIF_TEXT;
		tabCtrlItem.pszText = strTab.GetBuffer(200);
		m_wndFlowTabs.SetItem(iIndex, &tabCtrlItem);
	}
}

BOOL TPerformanceDataDlg::PreTranslateMessage(MSG *pMsg)
{
	if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_RETURN)) {
		CWnd * pControl = GetFocus();
		if (pControl != NULL) {
			HWND controlHandle = ::GetDlgItem(m_hWnd, pControl->GetDlgCtrlID());
			TCHAR className[128];
			if ((controlHandle != NULL) && (::GetClassName(controlHandle, className, 128) > 0)) {
				if (_tcsnicmp(className, "edit", 4) == 0) {
					pMsg->wParam = VK_TAB;
				}
			}
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

