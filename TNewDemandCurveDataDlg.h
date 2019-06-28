#if !defined(AFX_TNEWDEMANDCURVEDATADLG_H__61961285_F3DC_4372_BE7E_FBCE6B789773__INCLUDED_)
#define AFX_TNEWDEMANDCURVEDATADLG_H__61961285_F3DC_4372_BE7E_FBCE6B789773__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TNewDemandCurveDataDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// TNewDemandCurveDataDlg dialog

class TNewDemandCurveDataDlg : public CDialog
{
// Construction
public:
	TNewDemandCurveDataDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(TNewDemandCurveDataDlg)
	enum { IDD = IDD_DIALOG_NEW_DEMAND_DATA };
	CEdit	m_wndNewFileEdit;
	CString	m_strNewFileName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(TNewDemandCurveDataDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(TNewDemandCurveDataDlg)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TNEWDEMANDCURVEDATADLG_H__61961285_F3DC_4372_BE7E_FBCE6B789773__INCLUDED_)
