// StaticHyperLink.cpp : implementation file
//

#include "stdafx.h"
#include "CTIToolkit.h"
#include "StaticHyperLink.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStaticHyperLink

CStaticHyperLink::CStaticHyperLink()
{
	m_hCursor = AfxGetApp()->LoadCursor(IDC_CURSOR_HAND);
	m_bFontSet = false;
}

CStaticHyperLink::~CStaticHyperLink()
{
}


BEGIN_MESSAGE_MAP(CStaticHyperLink, CStatic)
	//{{AFX_MSG_MAP(CStaticHyperLink)
	ON_WM_LBUTTONUP()
	ON_WM_CREATE()
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
	ON_WM_CTLCOLOR_REFLECT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStaticHyperLink message handlers

void CStaticHyperLink::OnLButtonUp(UINT nFlags, CPoint point) 
{
	ShellExecute(NULL, "open" , m_strLink, NULL, NULL, SW_SHOW);

	CStatic::OnLButtonUp(nFlags, point);
}

void CStaticHyperLink::m_fnSetLink(LPCTSTR lpszLink)
{
	m_strLink = lpszLink;
}

int CStaticHyperLink::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CStatic::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

void CStaticHyperLink::OnMouseMove(UINT nFlags, CPoint point) 
{
	CStatic::OnMouseMove(nFlags, point);
}

BOOL CStaticHyperLink::OnSetCursor(CWnd* /* pWnd */, UINT /* nHitTest */, UINT /* message */) 
{
	::SetCursor(m_hCursor);
	
	return true; //CStatic::OnSetCursor(pWnd, nHitTest, message);
}

HBRUSH CStaticHyperLink::CtlColor(CDC* /* pDC */, UINT /* nCtlColor */) 
{
	if (!m_bFontSet)
	{
		m_bFontSet = true;
		CFont *pFont = GetFont();
		LOGFONT stLogFont;

		pFont->GetLogFont(&stLogFont);
		stLogFont.lfUnderline = true;
		m_fontLink.CreateFontIndirect(&stLogFont);
		SetFont(&m_fontLink, true);
		InvalidateRect(NULL);
	}
	return NULL;
}


