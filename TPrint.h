#if !defined(AFX_TPRINT_H__8FDD0583_A977_11D2_A794_00C04F73051D__INCLUDED_)
#define AFX_TPRINT_H__8FDD0583_A977_11D2_A794_00C04F73051D__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// TPrint.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// TPrint dialog

class TPrint : public CDialog
{
// Construction
public:
	TPrint(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(TPrint)
	enum { IDD = IDD_PRINT_QUERY };
	CString	m_csDescription;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(TPrint)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(TPrint)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TPRINT_H__8FDD0583_A977_11D2_A794_00C04F73051D__INCLUDED_)
