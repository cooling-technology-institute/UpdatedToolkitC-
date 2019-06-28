#if !defined(AFX_TNEWPERFORMANCEDATAFILEDLG_H__79ADBB71_0DAE_4F61_8056_82A496EB1C0F__INCLUDED_)
#define AFX_TNEWPERFORMANCEDATAFILEDLG_H__79ADBB71_0DAE_4F61_8056_82A496EB1C0F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TNewPerformanceDataFileDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// TNewPerformanceDataFileDlg dialog

class TNewPerformanceDataFileDlg : public CDialog
{
// Construction
public:
	CString m_strCopyFile;
	TNewPerformanceDataFileDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(TNewPerformanceDataFileDlg)
	enum { IDD = IDD_DIALOG_NEW_PERF_FILE };
	CEdit	m_wndNewFileEdit;
	CComboBox	m_wndDataFileList;
	CString	m_strNewFileName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(TNewPerformanceDataFileDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(TNewPerformanceDataFileDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TNEWPERFORMANCEDATAFILEDLG_H__79ADBB71_0DAE_4F61_8056_82A496EB1C0F__INCLUDED_)
