#if !defined(AFX_PRINTPERFREPORT_H__0A239043_8B67_4334_9EA2_A1DE8F53E624__INCLUDED_)
#define AFX_PRINTPERFREPORT_H__0A239043_8B67_4334_9EA2_A1DE8F53E624__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PrintPerfReport.h : header file
//

//#define _MFC_PREVIEW

/////////////////////////////////////////////////////////////////////////////
// CPrintPerfReport view

class CPrintPerfReport : public CView
{
public:
	CPrintPerfReport();
	~CPrintPerfReport();

protected:
	DECLARE_DYNCREATE(CPrintPerfReport)

// Attributes
public:

// Operations
public:
	void m_fnDrawText(CDC* pDC, LPCTSTR lpszText, CFont* pFont, UINT uiFormat, double dblTblStartX, double dblTblStartY, double dblColWidth, double dblCellHeight, int iRow, int iCol);
	void m_fnDrawText(CDC* pDC, LPCTSTR lpszText, CFont* pFont, CRect* pRect, UINT uiFormat);
	CString				m_strTitle;
	bool				m_bIPUnits;
	TPerformanceTest	m_testData;
	TPerformanceDesign	m_designData;
	double				m_dblCWTDev;	//DDP Hack for wetbulb dev
	bool				m_bPerfReport;  //DDP 11/5/02
#ifdef _MFC_PREVIEW
	bool				m_bOk;
#endif

	void m_fnPrintPreview(TPerformanceDesign* pDesignData, TPerformanceTest* pTestData, bool bIP, LPCTSTR lpszTitle, bool bPerfReport);
	void m_fnPrint(TPerformanceDesign* pDesignData, TPerformanceTest* pTestData, bool bIP, LPCTSTR lpszTitle, bool bPerfReport);
	void m_fnDrawPerfReport(CDC* pDC);
	void m_fnDrawDesignReport(CDC* pDC);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPrintPerfReport)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
//	virtual void OnInitialUpdate();
#ifdef _MFC_PREVIEW
	void OnEndPrintPreview(CDC* pDC, CPrintInfo* pInfo,
							POINT point, CPreviewView * pView);
#endif


	//}}AFX_VIRTUAL

// Implementation
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CPrintPerfReport)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PRINTPERFREPORT_H__0A239043_8B67_4334_9EA2_A1DE8F53E624__INCLUDED_)
