#if !defined(AFX_PRINTLIST_H__1EE83D12_B20A_11D2_A79F_00C04F73051D__INCLUDED_)
#define AFX_PRINTLIST_H__1EE83D12_B20A_11D2_A79F_00C04F73051D__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// PrintList.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPrintList view

class CPrintList : public CView
{
public:
	CPrintList();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CPrintList)

// Attributes
public:
    CListCtrlEx* pLC;
    CString Title;
    CString Subtitle;

// Operations
    void m_fnPrint(CString,CString,CListCtrlEx*);
    void m_fnPrintPreview(CString,CString,CListCtrlEx*);
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPrintList)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPrintList();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CPrintList)
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PRINTLIST_H__1EE83D12_B20A_11D2_A79F_00C04F73051D__INCLUDED_)
