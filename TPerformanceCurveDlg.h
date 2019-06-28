//{{AFX_INCLUDES()
#include "tchart.h"
//}}AFX_INCLUDES
#if !defined(AFX_TPERFORMANCECURVEDLG_H__84FA198D_FCB0_4B5E_BC98_EA04409A9C92__INCLUDED_)
#define AFX_TPERFORMANCECURVEDLG_H__84FA198D_FCB0_4B5E_BC98_EA04409A9C92__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TPerformanceCurveDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// TPerformanceCurveDlg dialog

class TPerformanceCurveDlg : public cdxCSizingDialog
{
// Construction
public:
	void m_fnDrawCrossPlot2(bool bWhiteBackground);
	void m_fnDrawCrossPlot1(bool bWhiteBackground);
	void m_fnDrawFlow(int iFlowIndex, bool bWhiteBackground);
	bool				m_bTestPoint;
	bool				m_bIPUnits;
	bool				m_bReversedGraph;
	TPerformanceTest	m_testData;
	TPerformanceDesign	m_designData;
	TPerformanceCurveDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(TPerformanceCurveDlg)
	enum { IDD = IDD_DIALOG_PERF_CURVES };
	CTabCtrl	m_wndCurveTabs;
	CTChart	m_wndGraph;
	CString	m_strLegend;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(TPerformanceCurveDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	void m_fnDrawGraph();
	void ResetWindow();

	// Generated message map functions
	//{{AFX_MSG(TPerformanceCurveDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnClickTabCurves(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonCopyGraph();
	afx_msg void OnButtonReverse();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TPERFORMANCECURVEDLG_H__84FA198D_FCB0_4B5E_BC98_EA04409A9C92__INCLUDED_)
