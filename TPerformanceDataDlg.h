#if !defined(AFX_TPERFORMANCEDATADLG_H__5448567D_C0E1_4B1E_9614_CFA6B0523A7E__INCLUDED_)
#define AFX_TPERFORMANCEDATADLG_H__5448567D_C0E1_4B1E_9614_CFA6B0523A7E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TPerformanceDataDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// TPerformanceDataDlg dialog

class TPerformanceDataDlg : public CDialog
{
// Construction
public:
	bool m_fnShowWBTs();
	bool m_fnHideWBTs();
	bool m_fnShowRange(int iIndex);
	bool m_fnHideRange(int iIndex);
	TPerformanceDataDlg(CWnd* pParent = NULL);   // standard constructor

	TPerformanceDesign m_designData;

	bool m_bIPUnits;
	int m_iTempIndex;
	int m_iCurrentFlow;

// Dialog Data
	//{{AFX_DATA(TPerformanceDataDlg)
	enum { IDD = IDD_DIALOG_PERFORMANCE_INPUT };
	CEdit	m_wndWBT6;
	CEdit	m_wndWBT5;
	CEdit	m_wndWBT4;
	CEdit	m_wndWBT3;
	CEdit	m_wndWBT2;
	CEdit	m_wndWBT1;
	CEdit	m_wndR5WBT1;
	CEdit	m_wndR5WBT2;
	CEdit	m_wndR5WBT3;
	CEdit	m_wndR5WBT4;
	CEdit	m_wndR5WBT5;
	CEdit	m_wndR5WBT6;
	CEdit	m_wndR4WBT1;
	CEdit	m_wndR4WBT2;
	CEdit	m_wndR4WBT3;
	CEdit	m_wndR4WBT4;
	CEdit	m_wndR4WBT5;
	CEdit	m_wndR4WBT6;
	CEdit	m_wndR3WBT1;
	CEdit	m_wndR3WBT2;
	CEdit	m_wndR3WBT3;
	CEdit	m_wndR3WBT4;
	CEdit	m_wndR3WBT5;
	CEdit	m_wndR3WBT6;
	CEdit	m_wndR2WBT1;
	CEdit	m_wndR2WBT2;
	CEdit	m_wndR2WBT3;
	CEdit	m_wndR2WBT4;
	CEdit	m_wndR2WBT5;
	CEdit	m_wndR2WBT6;
	CEdit	m_wndR1WBT1;
	CEdit	m_wndR1WBT2;
	CEdit	m_wndR1WBT3;
	CEdit	m_wndR1WBT4;
	CEdit	m_wndR1WBT5;
	CEdit	m_wndR1WBT6;
	CEdit	m_wndR1;
	CEdit	m_wndR2;
	CEdit	m_wndR3;
	CEdit	m_wndR4;
	CEdit	m_wndR5;
	CEdit	m_wndDesignWBT;
	CEdit	m_wndDesignWaterFlowRate;
	CEdit	m_wndDesignLG;
	CEdit	m_wndDesignHWT;
	CEdit	m_wndDesignFanDriverPower;
	CEdit	m_wndDesignDBT;
	CEdit	m_wndDesignCWT;
	CEdit	m_wndDesignBarometricPressure;
	CTabCtrl	m_wndFlowTabs;
	CString m_strSpecsCaption;
	CString m_strCurveCaption;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(TPerformanceDataDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(TPerformanceDataDlg)
	afx_msg void OnSelChangeTabFlows(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickTabFlows(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeEditR1();
	afx_msg void OnChangeEditR2();
	afx_msg void OnChangeEditR3();
	afx_msg void OnChangeEditR4();
	afx_msg void OnChangeEditR5();
	virtual void OnOK();
	afx_msg void OnRclickTabFlows(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnFlowtabsDelete();
	afx_msg void OnFlowtabsEdit();
	afx_msg BOOL PreTranslateMessage(MSG *pMsg);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TPERFORMANCEDATADLG_H__5448567D_C0E1_4B1E_9614_CFA6B0523A7E__INCLUDED_)
