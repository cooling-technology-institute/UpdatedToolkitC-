#if !defined(AFX_TPRINTPERFREPORTDLG_H__67034042_DC4F_41C6_9916_48C29AB41037__INCLUDED_)
#define AFX_TPRINTPERFREPORTDLG_H__67034042_DC4F_41C6_9916_48C29AB41037__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TPrintPerfReportDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// TPrintPerfReportDlg dialog

class TPrintPerfReportDlg : public CDialog
{
// Construction
public:
	bool m_bPerfReport;
	TPrintPerfReportDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(TPrintPerfReportDlg)
	enum { IDD = IDD_PRINT_PERF_REPORT_QUERY };
	CString	m_csDescription;
	int m_nReportType;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(TPrintPerfReportDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(TPrintPerfReportDlg)
	afx_msg void OnRadioDesignData();
	afx_msg void OnRadioPerfReport();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TPRINTPERFREPORTDLG_H__67034042_DC4F_41C6_9916_48C29AB41037__INCLUDED_)
