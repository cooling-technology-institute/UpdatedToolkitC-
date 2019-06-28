//---------------------------------------------------------------------
//
// TPropPageTwo.h : 
//
// Copyright c 1997-1998 Cosmo Engineering  -  All Rights Reserved.
//
// $Archive: /BlueBook 2.0/TPropPageTwo.h $
// $Revision: 2 $
// $Modtime: 4/19/01 4:01a $
//
//---------------------------------------------------------------------
//
// $Log: /BlueBook 2.0/TPropPageTwo.h $
// 
// 2     4/19/01 6:21a Davidl
// Added m_bInit member to aid in Unit change check upon initialization.
// 
// 1     11/06/00 9:46a Deanp
// Initial 2.0 source
// 
// 12    2/25/99 6:50a Deanp
// change so all altitude inputs are toggled between altitude and
// barometric pressure
// 
//---------------------------------------------------------------------
#if !defined(AFX_TPROPPAGETWO_H__6CD4E8A4_7FCA_11D2_A74F_00C04F73051D__INCLUDED_)
#define AFX_TPROPPAGETWO_H__6CD4E8A4_7FCA_11D2_A74F_00C04F73051D__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "ctiutils.h"
#include "tchart.h"
#include "cdxCSizingDialog.h"

//---------------------------------------------------------------------
// 
//---------------------------------------------------------------------
extern void AFXAPI DDV_MinMaxDouble(CDataExchange*, double&, double, double, double, double, int);

//---------------------------------------------------------------------
// TPropPageTwo dialog
//---------------------------------------------------------------------
class TPropPageTwo : public cdxCSizingPropPage
{
	DECLARE_DYNCREATE(TPropPageTwo)

// Construction
public:
	TPropPageTwo();
	~TPropPageTwo();

// Dialog Data
    void    SetDefaultValues();
    void    ChangeUnits(); 
    void    UpdateUnits();
    afx_msg void OnMerkelButtonRecalc();

	//{{AFX_DATA(TPropPageTwo)
	enum { IDD = IDD_PROPPAGE_TWO };
	CString	m_strT1;
	CString	m_strT2;
	CString	m_strWBT;
    CString m_strAlt;
	double	m_dblMerkelLG;
	double	m_dblMerkelT1;
	double	m_dblMerkelT2;
	double	m_dblMerkelWBT;
	double	m_dblMerkelResults;
    double  m_dblMerkelAltitude;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(TPropPageTwo)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	// Generated message map functions
	//{{AFX_MSG(TPropPageTwo)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDataChanged();
	afx_msg void OnButtonAltitude();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	CString m_strAltitudeLable;
	bool m_bAltitude;
	bool m_bInit;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TPROPPAGETWO_H__6CD4E8A4_7FCA_11D2_A74F_00C04F73051D__INCLUDED_)

//---------------------------------------------------------------------
// End: TPropPageTwo.h
//---------------------------------------------------------------------
