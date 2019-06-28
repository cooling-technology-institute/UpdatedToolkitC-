//---------------------------------------------------------------------
//
// TPropPageZero.h : 
//
// Copyright c 1997-1998 Cosmo Engineering  -  All Rights Reserved.
//
// $Archive: /Toolkit/TPropPageZero.h $
// $Revision: 1 $
// $Modtime: 6/05/01 11:52p $
//
//---------------------------------------------------------------------
//
// $Log: /Toolkit/TPropPageZero.h $
// 
// 1     8/20/01 5:32a Davidl
// 
// 
//---------------------------------------------------------------------
#ifndef PROP_PAGE_ZERO_H
#define PROP_PAGE_ZERO_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "cdxCSizingDialog.h"
//#include "StaticHyperLink.h"  

//---------------------------------------------------------------------
// TPropPageZero dialog
//---------------------------------------------------------------------
class TPropPageZero : public cdxCSizingPropPage
{
	DECLARE_DYNCREATE(TPropPageZero)

// Construction
public:
	TPropPageZero();
	~TPropPageZero();

// Dialog Data
    void    SetDefaultValues();
    void    ChangeUnits(); 
    void    UpdateUnits();

	//{{AFX_DATA(TPropPageZero)
	enum { IDD = IDD_PROPPAGE_ZERO };
	CString m_strWFR;
	CString	m_strHWT;
	CString	m_strCWT;
    CString m_strWBT;
	CString m_strDBT;
	CString m_strFDP;
	CString m_strBP;
	double	m_dblMinWFR;
	double	m_dblMaxWFR;
	double	m_dblMinHWT;
	double  m_dblMaxHWT;
	double	m_dblMinCWT;
	double  m_dblMaxCWT;
	double	m_dblMinWBT;
	double	m_dblMaxWBT;
	double	m_dblMinDBT;
	double	m_dblMaxDBT;
	double  m_dblMinFDP;
	double  m_dblMaxFDP;
	double  m_dblMinBP;
	double  m_dblMaxBP;
    double  m_dblMinLG;
	double  m_dblMaxLG;
//	CStaticHyperLink m_wndCTILink;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(TPropPageZero)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	// Generated message map functions
	//{{AFX_MSG(TPropPageZero)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDataChanged();
//	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	bool m_bInit;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(

//---------------------------------------------------------------------
// End: TPropPageZero.h
//---------------------------------------------------------------------
