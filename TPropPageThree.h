//---------------------------------------------------------------------
//
// TPropPageThree.h : 
//
// Copyright c 1997-1998 Cosmo Engineering  -  All Rights Reserved.
//
// $Archive: /BlueBook 2.0/TPropPageThree.h $
// $Revision: 1 $
// $Modtime: 9/25/00 2:40p $
//
//---------------------------------------------------------------------
//
// $Log: /BlueBook 2.0/TPropPageThree.h $
// 
// 1     11/06/00 9:46a Deanp
// Initial 2.0 source
// 
// 34    2/25/99 6:50a Deanp
// change so all altitude inputs are toggled between altitude and
// barometric pressure
// 
// 33    2/19/99 9:21a Deanp
// handle the OCX not being registered.
// 
// 32    2/18/99 12:39p Deanp
// new logic for init of demand curves
// 
// 31    2/18/99 12:00p Deanp
// calculate what approach curves to use
// 
// 30    2/15/99 6:33a Deanp
// Changes to print out and cleanup of code
// 
//---------------------------------------------------------------------
#if !defined(AFX_TPROPPAGETHREE_H__6CD4E8A5_7FCA_11D2_A74F_00C04F73051D__INCLUDED_)
#define AFX_TPROPPAGETHREE_H__6CD4E8A5_7FCA_11D2_A74F_00C04F73051D__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "tchart.h"
#include "cdxCSizingDialog.h"

extern void AFXAPI DDV_MinMaxDouble(CDataExchange*, double&, double, double, double, double, int);

//---------------------------------------------------------------------
// TPropPageThree dialog
//---------------------------------------------------------------------
class TPropPageThree : public cdxCSizingPropPage
{
	DECLARE_DYNCREATE(TPropPageThree)

public:
	//---------------------------------------------------------------------
	// Constructors
	//---------------------------------------------------------------------
	TPropPageThree();
	~TPropPageThree();

    void    SetDefaultValues();
    void    ChangeUnits(); 
    void    UpdateUnits();
    bool    getapp(int);
    void    setapp(int,bool);

	//{{AFX_DATA(TPropPageThree)
	enum { IDD = IDD_PROPPAGE_THREE };
	CComboBox	m_wndDataFileList;
	CSpinButtonCtrl	m_SpinMin;
	CSpinButtonCtrl	m_SpinMax;
	double	m_dblKavl;
	double	m_dblLg;
	double	m_dblCurveMax;
	double	m_dblCurveMin;
	CString	m_strAltitude;
	CString	m_strRange;
	CString	m_strWBT;
	double	m_dblCurveC1;
	double	m_dblCurveC2;
	double  m_dblAltitude;
	double	m_dblCurveRange;
	double	m_dblCurveWBT;
	CTChart m_wndGraph;
	//}}AFX_DATA

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(TPropPageThree)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	CString m_strDataName;
	bool	m_bDataChanged;
	// Generated message map functions
	//{{AFX_MSG(TPropPageThree)
	virtual BOOL OnInitDialog();
    afx_msg void OnOnMouseUpTchartGraph(long Button, long Shift, long X, long Y);
    afx_msg void OnOnDblClickTchartGraph();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    afx_msg void OnMultiEdit();
	afx_msg void OnDataChanged();
	afx_msg void OnButtonKavlApproach();
    afx_msg void OnCurveButtonRecalc(); 
    afx_msg void OnFilePrint();
    afx_msg void OnFilePrintPreview();
	afx_msg void OnPaint();
	afx_msg void OnButtonAltitude();
	afx_msg void OnButtonSave();
	afx_msg void OnSelchangeComboDataFiles();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
    
	DECLARE_MESSAGE_MAP()

protected:
	bool m_bAltitude;
	CString m_strAltitudeLable;
	bool m_bNotified;
	void m_fnInitApproachList(double *dblAppList, double dblWBT, double m_dblRange, double dblAltitude);
	double m_fnGetExactApproach(double dblCurveWBT, double dblCurveRange, double dblLg, double dblAltitude, double dblKaVl);
    int findApproach(long X, long Y);
    
	//---------------------------------------------------------------------
	// Status for the showing of graph lines
	//---------------------------------------------------------------------
	bool coef;
    bool Lg;
    bool bShowApproach[25];
	bool m_bShowKaVLLine;
	bool m_bShowUserApproach;
	double m_dblTargetApproach;
	double m_dblUserApproach;
	double	m_listApp[50];
};

//---------------------------------------------------------------------
// Indexes to the series on the graph.
//---------------------------------------------------------------------
#define INDEX_TARGETAPPROACH	18
#define INDEX_USERAPPROACH		19
#define INDEX_COEF				20
#define INDEX_LG				21
#define INDEX_KAVL				22


//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TPROPPAGETHREE_H__6CD4E8A5_7FCA_11D2_A74F_00C04F73051D__INCLUDED_)

//---------------------------------------------------------------------
// End: TPropPageThree.h
//---------------------------------------------------------------------
