//---------------------------------------------------------------------
//
// TPropPageFour.h : 
//
// Copyright c 1997-1998 Cosmo Engineering  -  All Rights Reserved.
//
// $Archive: /BlueBook 2.0/TPropPageFour.h $
// $Revision: 8 $
// $Modtime: 5/03/01 6:45p $
//
//---------------------------------------------------------------------
//
// $Log: /BlueBook 2.0/TPropPageFour.h $
// 
// 8     5/07/01 3:26a Davidl
// Added OnSize message handling.
// 
// 7     4/19/01 6:19a Davidl
// Removed duplicate header comment.
// 
// 6     4/16/01 6:15a Davidl
// Added bounds checking on test and design data.
// 
// 5     3/13/01 12:40p Deanp
// Data held in objects not in member vars of the dialogs.
// 
// 4     2/07/01 3:40p Deanp
// Several bug fixes and new features
// 
// 3     12/11/00 10:35a Deanp
// new calc of corrected flow
// 
// 2     11/16/00 11:53a Deanp
// Added printing of the Performance data and added some data validation
// 
// 1     11/06/00 9:46a Deanp
// Initial 2.0 source
// 
//---------------------------------------------------------------------
#if !defined(AFX_TPROPPAGEFOUR_H__889B1FD7_57B6_4E47_982D_5BB81B62C8A1__INCLUDED_)
#define AFX_TPROPPAGEFOUR_H__889B1FD7_57B6_4E47_982D_5BB81B62C8A1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TPropPageFour.h : header file
//

#include "cdxCSizingDialog.h"
#include "TTowerPerformanceFile.h"	// Added by ClassView
extern void AFXAPI DDV_MinMaxDouble(CDataExchange*, double&, double, double, double, double, int);

#define _PAGE_FOUR_TOWER_TYPE	// Tower type (forced/induced) is on this dialog

//---------------------------------------------------------------------
// CLASS       : TPropPageFour
//
// DESCRIPTION : None.
//
//---------------------------------------------------------------------
// REMARKS     : None.
//---------------------------------------------------------------------
class TPropPageFour : public cdxCSizingPropPage
{
	DECLARE_DYNCREATE(TPropPageFour)

// Construction
public:
	void m_fnCalcWBTDeviation();
	int m_iCurrentTest;
	bool m_fnCheckForTestSelection();
	bool m_bTestDataChanged;
	void m_fnCalcCapability(bool bCalcWBTDeviation = true);
	void m_fnInitControls();
	void m_fnClearCalculatedValues();
	int  m_fnFillFileList();

	TTowerPerformanceFile m_filePerfData;
	TPropPageFour();
	~TPropPageFour();

    void    SetDefaultValues();
    void    ChangeUnits(); 
    void    UpdateUnits();
	afx_msg void OnPerformanceButtonRecalc();
	bool ValidateData(bool bDesignData, bool bDisplayMsg=true);

// Dialog Data
	//{{AFX_DATA(TPropPageFour)
	enum { IDD = IDD_PROPPAGE_FOUR };
	CEdit	m_wndTestWBT;
	CEdit	m_wndTestWaterFlowRate;
	CEdit	m_wndTestHWT;
	CEdit	m_wndTestFanDriverPower;
	CEdit	m_wndTestDBT;
	CEdit	m_wndTestCWT;
	CEdit	m_wndTestBarometricPressure;
	CComboBox	m_wndTestList;
	CComboBox	m_wndDataFileList;
	CString	m_strUnitsCWTDev;
	CString	m_strUnitsWBT;
	CString	m_strUnitsWaterFlowRate;
	CString	m_strUnitsHWT;
	CString	m_strUnitsFanDriverPower;
	CString	m_strUnitsDBT;
	CString	m_strUnitsCWT;
	CString	m_strUnitsBarometricPressure;
	CString	m_strUnitsAdjustedFlow;
	CString	m_strUnitsPredictedFlow;
	CString	m_strProjectName;
	CString	m_strTowerOwner;
	CString	m_strTowerManufacturer;
	CString	m_strTowerLocation;
	double	m_dblCWTDev;
	double	m_dblCapability;
	double  m_dblAdjustedFlow;
	double  m_dblPredictedFlow;
	double	m_dblDesignBarometricPressure;
	double	m_dblDesignCWT;
	double	m_dblDesignDBT;
	double	m_dblDesignFanDriverPower;
	double	m_dblDesignHWT;
	double	m_dblDesignLG;
	double	m_dblDesignWaterFlowRate;
	double	m_dblDesignWBT;
	double	m_dblTestBarometricPressure;
	double	m_dblTestCWT;
	double	m_dblTestDBT;
	double	m_dblTestFanDriverPower;
	double	m_dblTestHWT;
	double	m_dblTestLG;
	double	m_dblTestWaterFlowRate;
	double	m_dblTestWBT;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(TPropPageFour)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	
	CString	m_strPreviewSubject;	// hold last print preview header line
    bool    m_bCWTDevCalcOk;

	// Generated message map functions
	//{{AFX_MSG(TPropPageFour)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonEdit();
	afx_msg void OnButtonNew();
	afx_msg void OnButtonViewGraph();
	afx_msg void OnSelChangeComboDataFiles();
	afx_msg void OnSelChangeComboTestList();
	afx_msg void OnButtonSaveTestData();
	afx_msg void OnChangeEditTestBarometricPressure();
	afx_msg void OnChangeEditTestColdWater();
	afx_msg void OnChangeEditTestDryBulb();
	afx_msg void OnChangeEditTestFanDriver();
	afx_msg void OnChangeEditTestHotWater();
	afx_msg void OnChangeEditTestWaterFlow();
	afx_msg void OnChangeEditTestWetBulb();
#ifdef _PAGE_FOUR_TOWER_TYPE
	afx_msg void OnButtonInduced();
	afx_msg void OnButtonForced();
#endif	// _PAGE_FOUR_TOWER_TYPE
	afx_msg void OnFileListDropDown();
    afx_msg void OnFilePrint();
    afx_msg void OnFilePrintPreview();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

// Utility class for copying & storing perf. curve data
class TPerfData
{
public:

	TPerfData(double dblWaterFlowRate, double dblHWT, 
		double dblCWT, double dblWBT, double dblDBT, 
		double dblFanDriverPower, double dblBarometricPressure, 
		double dblLG);
	TPerfData(const TPropPageFour& ppf, bool bDesignData);
	TPerfData SICopy() const;
	
	double	m_dblWaterFlowRate;
	double	m_dblHWT;
	double	m_dblCWT;
	double	m_dblWBT;
	double	m_dblDBT;
	double	m_dblFanDriverPower;
	double	m_dblBarometricPressure;
	double	m_dblLG;
};

//---------------------------------------------------------------------
// END: TPropPageFour
//---------------------------------------------------------------------

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.


#endif // !defined(AFX_TPROPPAGEFOUR_H__889B1FD7_57B6_4E47_982D_5BB81B62C8A1__INCLUDED_)
//---------------------------------------------------------------------
// End: TPropPageOne.h
//---------------------------------------------------------------------
