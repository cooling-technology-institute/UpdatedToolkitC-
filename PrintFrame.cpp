// PrintFrame.cpp : implementation file
//

#include "stdafx.h"
#include "CTIToolkit.h"
#include "PrintFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Scrolling logic taken (with some modifications) from 
// "Programming Windows with MFC" (Second Edition) by Jeff Prosise, pp 89-96.

// Logical dimension of a "line" (for one-line scrolling)
#define LINESIZE 20

//---------------------------------------------------------------------
// External info
//---------------------------------------------------------------------
extern CPrintFrame			wndPrintPerfDataFrame;

/////////////////////////////////////////////////////////////////////////////
// CPrintFrame

IMPLEMENT_DYNCREATE(CPrintFrame, CFrameWnd)

CPrintFrame::CPrintFrame() 
	: m_bPrintPreview(FALSE), m_nHScrollPos(0), m_nVScrollPos(0),
	m_nVPageSize(0), m_nHPageSize(0)
{
}


CPrintFrame::~CPrintFrame()
{
}


int CPrintFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	HICON hIcon = (HICON)LoadImage(AfxGetApp()->m_hInstance, 
		MAKEINTRESOURCE(IDR_MAINFRAME), IMAGE_ICON, 16, 16, 0);
	SetIcon(hIcon, false);

	CClientDC dc(this);
	m_nViewWidth = (int)(dc.GetDeviceCaps(LOGPIXELSY)*8.5);
	m_nViewHeight = dc.GetDeviceCaps(LOGPIXELSY)*11;
	return 0;;
}


void CPrintFrame::OnSize(UINT nType, int cx, int cy)
{
	CFrameWnd::OnSize(nType, cx, cy);

	// Horizontal scrolling
	int nHScrollMax(0);
	int nPrevHScrollPos(m_nHScrollPos);

	if (cx < m_nViewWidth)
	{
		nHScrollMax = m_nViewWidth - 1;
		m_nHPageSize = cx;
		m_nHScrollPos = min(m_nHScrollPos, m_nViewWidth - m_nHPageSize - 1);
	}
	else
		m_nHScrollPos = m_nHPageSize = 0;

	SCROLLINFO si;
	si.cbSize = sizeof(si);
	si.fMask = SIF_PAGE | SIF_RANGE | SIF_POS;
	si.nMin = 0;
	si.nMax = nHScrollMax;
	si.nPos = m_nHScrollPos;
	si.nPage = m_nHPageSize;

	SetScrollInfo(SB_HORZ, &si, TRUE);


	// Vertical scrolling
	int nVScrollMax(0);
	int nPrevVScrollPos(m_nVScrollPos);

	if (cy < m_nViewHeight)
	{
		nVScrollMax = m_nViewHeight - 1;
		m_nVPageSize = cy;
		m_nVScrollPos = min(m_nVScrollPos, m_nViewHeight - m_nVPageSize - 1);
	}
	else
		m_nVScrollPos = m_nVPageSize = 0;

	si.cbSize = sizeof(si);
	si.fMask = SIF_PAGE | SIF_RANGE | SIF_POS;
	si.nMin = 0;
	si.nMax = nVScrollMax;
	si.nPos = m_nVScrollPos;
	si.nPage = m_nVPageSize;

	SetScrollInfo(SB_VERT, &si, TRUE);

	/* Update position of window contents, if necessary */
	if ((nPrevHScrollPos != m_nHScrollPos) || (nPrevVScrollPos != m_nVScrollPos)) {
		ScrollWindow(nPrevHScrollPos - m_nHScrollPos, nPrevVScrollPos - m_nVScrollPos); }
}


void CPrintFrame::OnClose()
{
	if (m_bPrintPreview)
	{
		m_bPrintPreview = FALSE;
		ShowWindow(SW_HIDE);
		
		/* A new print preview will just unhide this window, 
		   so reset here to prepare for next show */
		if ((m_nHScrollPos > 0) || (m_nVScrollPos > 0)) 
		{
			SetScrollPos(SB_VERT, 0, TRUE);
			SetScrollPos(SB_HORZ, 0, TRUE);
			ScrollWindow(m_nHScrollPos, m_nVScrollPos); 
			m_nHScrollPos = m_nVScrollPos = 0;
		}
	}
	else
		CFrameWnd::OnClose();
}


void CPrintFrame::OnHScroll(UINT nCode, UINT nPos, CScrollBar *)
{
	int nDelta(0);

	switch (nCode) {
		case SB_LINELEFT:
			nDelta = -LINESIZE;
			break;

		case SB_PAGELEFT:
			nDelta = -m_nHPageSize;
			break;

		case SB_THUMBTRACK:
			nDelta = (int)nPos - m_nHScrollPos;
			break;

		case SB_PAGERIGHT:
			nDelta = m_nHPageSize;
			break;

		case SB_LINERIGHT:
			nDelta = LINESIZE;
			break;

		default:
			return;
		}

	int nScrollPos(m_nHScrollPos + nDelta);
	int nMaxPos(m_nViewWidth - m_nHPageSize);

	if (nScrollPos < 0)
		nDelta = -m_nHScrollPos;
	else if (nScrollPos > nMaxPos)
		nDelta = nMaxPos - m_nHScrollPos;

	if (nDelta != 0)
	{
		m_nHScrollPos += nDelta;
		SetScrollPos(SB_HORZ, m_nHScrollPos, TRUE);
		ScrollWindow(-nDelta, 0);
	}
}


void CPrintFrame::OnVScroll(UINT nCode, UINT nPos, CScrollBar *)
{
	int nDelta(0);

	switch (nCode) {
		case SB_LINEUP:
			nDelta = -LINESIZE;
			break;

		case SB_PAGEUP:
			nDelta = -m_nVPageSize;
			break;

		case SB_THUMBTRACK:
			nDelta = (int)nPos - m_nVScrollPos;
			break;

		case SB_PAGEDOWN:
			nDelta = m_nVPageSize;
			break;

		case SB_LINEDOWN:
			nDelta = LINESIZE;
			break;

		default:
			return;
	}

	int nScrollPos(m_nVScrollPos + nDelta);
	int nMaxPos(m_nViewHeight - m_nVPageSize);

	if (nScrollPos < 0)
		nDelta = -m_nVScrollPos;
	else if (nScrollPos > nMaxPos)
		nDelta = nMaxPos - m_nVScrollPos;

	if (nDelta != 0)
	{
		m_nVScrollPos += nDelta;
		SetScrollPos(SB_VERT, m_nVScrollPos, TRUE);
		ScrollWindow(0, -nDelta);
	}
}


void CPrintFrame::OnKeyDown(UINT nChar, UINT /*nRepCnt*/, UINT /*nFlags*/)
{
	WPARAM uMessage(0);
	UINT   wScrollNotify(0);
    switch (nChar) 
    {
        case VK_UP:
            wScrollNotify = SB_LINEUP;
            uMessage = WM_VSCROLL;
            break;

        case VK_PRIOR:    //PAGEUP key
            wScrollNotify = SB_PAGEUP;
            uMessage = WM_VSCROLL;
            break;

        case VK_NEXT:     // PAGEDOWN key
            wScrollNotify = SB_PAGEDOWN;
            uMessage = WM_VSCROLL;
            break;

        case VK_DOWN:
            wScrollNotify = SB_LINEDOWN;
            uMessage = WM_VSCROLL;
            break;

        case VK_HOME:
            wScrollNotify = SB_BOTTOM;
            uMessage = WM_HSCROLL;
            break;

        case VK_END:
            wScrollNotify = SB_TOP;
            uMessage = WM_HSCROLL;
            break;

        case VK_RIGHT:
            wScrollNotify = SB_LINEDOWN;
            uMessage = WM_HSCROLL;
            break;

        case VK_LEFT:
            wScrollNotify = SB_LINEUP;
            uMessage = WM_HSCROLL;
            break;

        default:
            wScrollNotify = 0xFFFF;
            break;
     }
	if (wScrollNotify != -1) {
		SendMessage(uMessage, MAKELONG(wScrollNotify, 0), 0L); }
}


BEGIN_MESSAGE_MAP(CPrintFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CPrintFrame)
	ON_WM_CREATE ()
	ON_WM_SIZE ()
	ON_WM_CLOSE ()
	ON_WM_HSCROLL ()
	ON_WM_VSCROLL ()
	ON_WM_KEYDOWN ()
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPrintFrame message handlers
