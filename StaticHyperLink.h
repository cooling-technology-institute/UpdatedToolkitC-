#if !defined(AFX_STATICHYPERLINK_H__E499FEE1_8967_11D2_898D_00105AA70924__INCLUDED_)
#define AFX_STATICHYPERLINK_H__E499FEE1_8967_11D2_898D_00105AA70924__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StaticHyperLink.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStaticHyperLink window

class CStaticHyperLink : public CStatic
{
// Construction
public:
	CStaticHyperLink();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStaticHyperLink)
	//}}AFX_VIRTUAL

// Implementation
public:
	void m_fnSetLink(LPCTSTR lpszLink);
	virtual ~CStaticHyperLink();

	// Generated message map functions
protected:
	bool m_bFontSet;
	CFont m_fontLink;
	HCURSOR m_hCursor;
	CString m_strLink;
	//{{AFX_MSG(CStaticHyperLink)
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STATICHYPERLINK_H__E499FEE1_8967_11D2_898D_00105AA70924__INCLUDED_)
