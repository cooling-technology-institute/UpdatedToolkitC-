#if !defined(AFX_TPROPPAGEBASE_H__6CD4E8A6_7FCA_11D2_A74F_00C04F73051D__INCLUDED_)
#define AFX_TPROPPAGEBASE_H__6CD4E8A6_7FCA_11D2_A74F_00C04F73051D__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// TPropPageBase.h : header file
//
//#include "TChart.h"
#include "cdxCSizingdialog.h"
#include "TPropPageOne.h"
#include "TPropPageTwo.h"
#include "TPropPageThree.h"
#include "TPropPageFour.h"
#include "float.h"

#ifdef _DEMO_VERSION
#include "TPropPageZero.h"
#endif

// Some max and min data values
#ifndef _DEMO_VERSION
#define WATER_FLOW_RATE_MIN_IP		   (0.1)
#define WATER_FLOW_RATE_MAX_IP		(DBL_MAX)
#define WATER_FLOW_RATE_MIN_SI		   (0.1)
#define WATER_FLOW_RATE_MAX_SI		(DBL_MAX)
#define HWT_MIN_IP					  (32.0)
#define HWT_MAX_IP					 (212.0)
#define HWT_MIN_SI					   (0.0)
#define HWT_MAX_SI					 (100.0)
#define CWT_MIN_IP					  (32.0)
#define CWT_MAX_IP					 (212.0)
#define CWT_MIN_SI					   (0.0)
#define CWT_MAX_SI					 (100.0)
#define WBT_MIN_IP					   (0.0)
#define WBT_MAX_IP					 (200.0)
#define WBT_MIN_SI					 (-18.0)
#define WBT_MAX_SI					  (93.0)
#define DBT_MIN_IP					   (0.0)
#define DBT_MAX_IP					 (200.0)
#define DBT_MIN_SI				 	 (-18.0)
#define DBT_MAX_SI					  (93.0)
#define FAN_DRIVER_POWER_MIN_IP		   (0.0001)
#define FAN_DRIVER_POWER_MAX_IP		(1000.0)
#define FAN_DRIVER_POWER_MIN_SI		   (0.0001)  
#define FAN_DRIVER_POWER_MAX_SI		 (745.7)
#define BAROMETRIC_PRESSURE_MIN_IP	   (5.0)
#define BAROMETRIC_PRESSURE_MAX_IP	  (31.5)
#define BAROMETRIC_PRESSURE_MIN_SI	  (16.932) 
#define BAROMETRIC_PRESSURE_MAX_SI	 (103.285)
#define LG_MIN_IP					   (0.01)
#define LG_MAX_IP					  (20.0)
#define LG_MIN_SI					   (0.01)
#define LG_MAX_SI					  (20.0)
#define ALT_MIN_IP                 (-1000.00)
#define ALT_MAX_IP                 (40000.00)
#define ALT_MIN_SI                  (-300.00)
#define ALT_MAX_SI                 (12200.00)
#define LG_P3_MIN_IP				   (0.0)    // L/G for Demand-curve
#define LG_P3_MAX_IP				   (5.0)
#define LG_P3_MIN_SI				   (0.0)
#define LG_P3_MAX_SI				   (5.0)

// Demo values:
#else
#define WATER_FLOW_RATE_MIN_IP	   (56500.0)
#define WATER_FLOW_RATE_MAX_IP     (57500.0)
#define WATER_FLOW_RATE_MIN_SI		(3565.0)
#define WATER_FLOW_RATE_MAX_SI		(3630.0)
#define HWT_MIN_IP					 (110.0)
#define HWT_MAX_IP					 (130.0)
#define HWT_MIN_SI					  (43.0)
#define HWT_MAX_SI					  (54.0)
#define CWT_MIN_IP					  (80.0)
#define CWT_MAX_IP					  (90.0)
#define CWT_MIN_SI					  (27.0)
#define CWT_MAX_SI					  (32.0)
#define WBT_MIN_IP					  (75.0)
#define WBT_MAX_IP					  (80.0)
#define WBT_MIN_SI					  (24.0)
#define WBT_MAX_SI					  (27.0)
#define DBT_MIN_IP					  (75.0)
#define DBT_MAX_IP					  (90.0)
#define DBT_MIN_SI				 	  (24.0)
#define DBT_MAX_SI					  (32.0)
#define FAN_DRIVER_POWER_MIN_IP		  (140.0)
#define FAN_DRIVER_POWER_MAX_IP	      (155.0)
#define FAN_DRIVER_POWER_MIN_SI		  (100.0)  
#define FAN_DRIVER_POWER_MAX_SI		  (115.0)
#define BAROMETRIC_PRESSURE_MIN_IP	  (29.0)
#define BAROMETRIC_PRESSURE_MAX_IP	  (30.0)
#define BAROMETRIC_PRESSURE_MIN_SI	  (98.0) 
#define BAROMETRIC_PRESSURE_MAX_SI	 (102.0)
#define LG_MIN_IP					   (0.01)//(1.0)
#define LG_MAX_IP					   (1.5)
#define LG_MIN_SI					   (0.01)//(1.0)
#define LG_MAX_SI					   (1.5)
#define ALT_MIN_IP                 (-1000.00)
#define ALT_MAX_IP                 (40000.00)
#define ALT_MIN_SI                  (-300.00)
#define ALT_MAX_SI                 (12200.00)
#define LG_P3_MIN_IP				   (0.0)    // L/G for Demand-curve
#define LG_P3_MAX_IP				   (5.0)
#define LG_P3_MIN_SI				   (0.0)
#define LG_P3_MAX_SI				   (5.0)
#endif

/////////////////////////////////////////////////////////////////////////////
// TPropPageBase

extern bool     print_flag;

class TPropPageBase : public cdxCSizingPropSheet
{
	DECLARE_DYNAMIC(TPropPageBase)

// Construction
public:
	TPropPageBase(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	TPropPageBase(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:
    CEdit           m_edit;
#ifdef _DEMO_VERSION
	TPropPageZero	limitspage;
#endif
    TPropPageOne    psychopage;
    TPropPageTwo    merkelpage;
    TPropPageThree  curvespage;
    TPropPageFour	performancepage;
    static bool     metricflag;
    static bool     metricflagsetting;
    static bool     berrorFlag;
    static bool     changingUnits;
/*
    static double   dblPressure;
    static double   dblWBT;
    static double   dblDBT;
    static double   dblT1;
    static double   dblT2;
    static double   dblRange;
    static double   dblAltitude;
    static double   dblEnthalpy;
    static double   dblRelHumidity;
    static double   dblC;
    static double   dblSlope;
    static double   dblLg;
*/
    static bool     SetMetricFlag();
    static bool     CheckData();
    void            SetRegKey(bool);
    void            GetRegistryValues();
    DWORD           m_fnGetPageHelpID(int);

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(TPropPageBase)
	public:
	virtual BOOL OnInitDialog();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

// Implementation
public:
    HICON       m_hIcon;
	CMenu       m_menuBar;
	virtual     ~TPropPageBase();
	// Generated message map functions

public:
    CToolBar    m_toolBar;
	//{{AFX_MSG(TPropPageBase)
	afx_msg void OnAppExit();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
    afx_msg void OnOK();
    afx_msg void OnCancel();
	afx_msg void OnEditCopy();
	afx_msg void OnEditCut();
    afx_msg void OnMultiEdit();
	afx_msg void OnHelphelp();
	afx_msg void OnUpdateFilePrint(CCmdUI* pCmdUI);
	afx_msg BOOL PreTranslateMessage(MSG *pMsg);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	LONG OnHelp(WPARAM, LPARAM);
    CMenu& getMenuBar() { return m_menuBar; }
	int GetActiveIndex() const;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TPROPPAGEBASE_H__6CD4E8A6_7FCA_11D2_A74F_00C04F73051D__INCLUDED_)
