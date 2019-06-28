#if !defined(AFX_TNEWFLOWRATEDLG_H__DE753A6C_2F0C_4775_97E7_444ED38212B1__INCLUDED_)
#define AFX_TNEWFLOWRATEDLG_H__DE753A6C_2F0C_4775_97E7_444ED38212B1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TNewFlowRateDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// TNewFlowRateDlg dialog

class TNewFlowRateDlg : public CDialog
{
// Construction
public:
	double m_dblClosestFlow;
	TNewFlowRateDlg(CWnd* pParent = NULL);   // standard constructor
	bool m_bNewFlow;

// Dialog Data
	//{{AFX_DATA(TNewFlowRateDlg)
	enum { IDD = IDD_DIALOG_NEW_TEST_FLOW };
	double	m_dblFlowRate;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(TNewFlowRateDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(TNewFlowRateDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TNEWFLOWRATEDLG_H__DE753A6C_2F0C_4775_97E7_444ED38212B1__INCLUDED_)
