//#include "tchart.h"

#if !defined(AFX_PRINTVIEW_H__289C3063_8F9A_11D2_8104_00A0C9D7D02F__INCLUDED_)
#define AFX_PRINTVIEW_H__289C3063_8F9A_11D2_8104_00A0C9D7D02F__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// PrintView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPrintView view

class CPrintView : public CView
{
public:
	CPrintView();
	virtual ~CPrintView();

protected:
	DECLARE_DYNCREATE(CPrintView)

// Attributes
public:

// Operations
public:
	CString m_csDesc;
	void m_fnPrintPreview(CString csDescription, double dblCurveRange, double dblCurveWBT, double dblAltitude, double dblC1, double dblC2, double dblKavL, double dblLG, int iTargetIndex, int iUserIndex, CTChart *pGraph );
	void m_fnPrint(CString desc, double dblCurveRange, double dblCurveWBT, double dblAltitude, double dblC1, double dblC2, double dblKavL, double dblLG, int iTargetIndex, int iUserIndex, CTChart *pGraph);
	double m_dblCurveRange;
	double m_dblCurveWBT;
    double m_dblAltitude;
	double m_dblCurveC1;
	double m_dblCurveC2;
	double m_dblKaVL;
	double m_dblLG;
	int		m_iTargetIndex;
	int		m_iUserIndex;
    double m_maxVal;
    double m_minVal;
	CTChart	*m_pwndGraph;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPrintView)
	public:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
protected:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	double m_dblWBT;
	//{{AFX_MSG(CPrintView)
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PRINTVIEW_H__289C3063_8F9A_11D2_8104_00A0C9D7D02F__INCLUDED_)
