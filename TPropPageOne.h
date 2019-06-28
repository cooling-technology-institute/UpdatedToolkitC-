//---------------------------------------------------------------------
//
// TPropPageOne.h : 
//
// Copyright c 1997-1998 Cosmo Engineering  -  All Rights Reserved.
//
// $Archive: /BlueBook 2.0/TPropPageOne.h $
// $Revision: 1 $
// $Modtime: 2/25/99 5:50a $
//
//---------------------------------------------------------------------
//
// $Log: /BlueBook 2.0/TPropPageOne.h $
// 
// 1     11/06/00 9:46a Deanp
// Initial 2.0 source
// 
// 13    2/25/99 6:50a Deanp
// change so all altitude inputs are toggled between altitude and
// barometric pressure
// 
//---------------------------------------------------------------------
#if !defined(AFX_TPROPPAGEONE_H__6CD4E8A3_7FCA_11D2_A74F_00C04F73051D__INCLUDED_)
#define AFX_TPROPPAGEONE_H__6CD4E8A3_7FCA_11D2_A74F_00C04F73051D__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "ListCtrl.h"
#include "tchart.h"
#include "cdxCSizingDialog.h"


extern void AFXAPI DDV_MinMaxDouble(CDataExchange*, double&, double, double, double, double, int);

//---------------------------------------------------------------------
// CLASS       : TPropPageOne
//
// DESCRIPTION : None.
//
//---------------------------------------------------------------------
// REMARKS     : None.
//---------------------------------------------------------------------
class TPropPageOne : public cdxCSizingPropPage
{
	DECLARE_DYNCREATE(TPropPageOne)

// Construction
public:
	TPropPageOne();
	~TPropPageOne();

    void OnPsychRadioItem();
    void SetHideEdit(int);
    void SetDefaultValues();
    void UpDateUnits();

    afx_msg void OnPsychButtonRecalc();
    afx_msg void OnPsychButtonRecalc1();
    afx_msg void OnPsychButtonRecalc3();
    afx_msg void OnPsychButtonRecalc4();
// Dialog Data
	//{{AFX_DATA(TPropPageOne)
	enum { IDD = IDD_PROPPAGE_ONE };
	CListCtrlEx	m_wndPsychResults;
    double	m_dblPsychDBT;
    double	m_dblPsychWBT;
    CString	m_strPsychDBTUnits;
    CString	m_strPsychPressureUnits;
    CString	m_strPsychWBTUnits;
	CString	m_strWBTempLabel;
	CString	m_strDBTempLabel;
	//}}AFX_DATA
    double  m_dblAltitude;
    double  m_dblRelHumidity;
    double  m_dblEnthalpy;

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(TPropPageOne)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	// Generated message map functions
	//{{AFX_MSG(TPropPageOne)
	virtual BOOL OnInitDialog();
	afx_msg void OnPsychRadioItem1();
	afx_msg void OnPsychRadioItem3();
	afx_msg void OnPsychRadioItem4();
	afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnFilePrint();
    afx_msg void OnFilePrintPreview();
	afx_msg void OnDataChanged();
	afx_msg void OnButtonAltitude();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	CString m_strAltitudeLable;
	bool m_bAltitude;
};
//---------------------------------------------------------------------
// END: TPropPageOne
//---------------------------------------------------------------------

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TPROPPAGEONE_H__6CD4E8A3_7FCA_11D2_A74F_00C04F73051D__INCLUDED_)

//---------------------------------------------------------------------
// End: TPropPageOne.h
//---------------------------------------------------------------------
