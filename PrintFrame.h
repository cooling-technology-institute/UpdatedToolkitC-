#if !defined(AFX_PRINTFRAME_H__289C3064_8F9A_11D2_8104_00A0C9D7D02F__INCLUDED_)
#define AFX_PRINTFRAME_H__289C3064_8F9A_11D2_8104_00A0C9D7D02F__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// PrintFrame.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPrintFrame frame

class CPrintFrame : public CFrameWnd
{
	DECLARE_DYNCREATE(CPrintFrame)
public:
	CPrintFrame();           // protected constructor used by dynamic creation
	virtual ~CPrintFrame();

protected:

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPrintFrame)
	//}}AFX_VIRTUAL
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClose();
	afx_msg void OnHScroll(UINT nCode, UINT nPos, CScrollBar * pScrollBar);
	afx_msg void OnVScroll(UINT nCode, UINT nPos, CScrollBar * pScrollBar);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	bool m_bPrintPreview;

// Implementation
protected:

	int  m_nHScrollPos;
	int  m_nVScrollPos;
	int  m_nHPageSize;
	int  m_nVPageSize;
	int  m_nViewWidth;
	int	 m_nViewHeight;

	// Generated message map functions
	//{{AFX_MSG(CPrintFrame)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PRINTFRAME_H__289C3064_8F9A_11D2_8104_00A0C9D7D02F__INCLUDED_)
