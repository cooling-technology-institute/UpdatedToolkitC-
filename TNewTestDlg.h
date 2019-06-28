#if !defined(AFX_TNEWTESTDLG_H__4F50DD81_1459_459B_8DFF_2E7E5D2FDB4B__INCLUDED_)
#define AFX_TNEWTESTDLG_H__4F50DD81_1459_459B_8DFF_2E7E5D2FDB4B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TNewTestDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// TNewTestDlg dialog

class TNewTestDlg : public CDialog
{
// Construction
public:
	TNewTestDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(TNewTestDlg)
	enum { IDD = IDD_DIALOG_NEW_TEST };
	CString	m_strTestName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(TNewTestDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(TNewTestDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TNEWTESTDLG_H__4F50DD81_1459_459B_8DFF_2E7E5D2FDB4B__INCLUDED_)
