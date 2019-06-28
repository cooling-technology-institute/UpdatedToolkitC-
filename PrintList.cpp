// PrintList.cpp : implementation file
//

#include "stdafx.h"
#include "CTIToolkit.h"
#include "PrintList.h"
#include "series.h"
#include "valuelist.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPrintList

IMPLEMENT_DYNCREATE(CPrintList, CView)

CPrintList::CPrintList()
{
}

CPrintList::~CPrintList()
{
}


BEGIN_MESSAGE_MAP(CPrintList, CView)
	//{{AFX_MSG_MAP(CPrintList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPrintList drawing

BOOL CPrintList::OnPreparePrinting(CPrintInfo* pInfo) 
{
	if (pInfo->m_bPreview)
		pInfo->SetMaxPage(1);
	return DoPreparePrinting(pInfo);
}

void CPrintList::OnDraw(CDC* pDC)
{
    CString	strTemp;
    CFont	cTitleFont;
    CFont	cDataFont;
    CFont   cTextFont;
	LOGFONT	stTitleLogFont;
    LOGFONT stSubLogFont;
    LOGFONT stTxtLogFont;

	pDC->SetTextColor(COLORREF(0x000000));

	// Title
	stTitleLogFont.lfHeight			= (LONG)(pDC->GetDeviceCaps(LOGPIXELSY)*.30);
    stTitleLogFont.lfWidth			= 0;
	stTitleLogFont.lfEscapement		= 0;
	stTitleLogFont.lfOrientation	= 0;
	stTitleLogFont.lfWeight			= FW_NORMAL;
	stTitleLogFont.lfItalic			= FALSE;
	stTitleLogFont.lfUnderline		= FALSE;
	stTitleLogFont.lfStrikeOut		= FALSE;
	stTitleLogFont.lfCharSet		= DEFAULT_CHARSET;
	stTitleLogFont.lfOutPrecision	= OUT_DEFAULT_PRECIS;
	stTitleLogFont.lfClipPrecision	= CLIP_DEFAULT_PRECIS;
	stTitleLogFont.lfQuality		= PROOF_QUALITY;
	stTitleLogFont.lfPitchAndFamily	= FF_DONTCARE;
	strcpy(stTitleLogFont.lfFaceName, "Times New Roman");
	cTitleFont.CreateFontIndirect(&stTitleLogFont);
	pDC->SelectObject(&cTitleFont);

	CRect rectTitleText(pDC->GetDeviceCaps(LOGPIXELSX)*1, 
                        pDC->GetDeviceCaps(LOGPIXELSY)/2,
                        pDC->GetDeviceCaps(LOGPIXELSX)*7,
                        pDC->GetDeviceCaps(LOGPIXELSY) );
	strTemp = Title;
    pDC->SetBkColor(0x0);
    pDC->SetTextColor(0xFFFFFF);
	pDC->DrawText(strTemp, rectTitleText, DT_CENTER | DT_VCENTER);
    pDC->SetBkColor(0xFFFFFF);
    pDC->SetTextColor(0x0);
    // subtitle
	CRect rectTitleDesc(pDC->GetDeviceCaps(LOGPIXELSX)*1, 
                        pDC->GetDeviceCaps(LOGPIXELSY),
                        pDC->GetDeviceCaps(LOGPIXELSX)*7,
                        (int)(pDC->GetDeviceCaps(LOGPIXELSY) + pDC->GetDeviceCaps(LOGPIXELSY)*1.25));

	strTemp = Subtitle;
	pDC->DrawText(strTemp, rectTitleDesc, DT_CENTER | DT_VCENTER); 

	CRect rectData1Text(pDC->GetDeviceCaps(LOGPIXELSX)*1, 
                        pDC->GetDeviceCaps(LOGPIXELSY)*1, 
                        pDC->GetDeviceCaps(LOGPIXELSX)*7,
                        (int)(pDC->GetDeviceCaps(LOGPIXELSY)*1 + pDC->GetDeviceCaps(LOGPIXELSY)*.3));

	// Title
	stSubLogFont.lfHeight			= (LONG)(pDC->GetDeviceCaps(LOGPIXELSY)*.20);
	stSubLogFont.lfWidth			= 0;
	stSubLogFont.lfEscapement		= 0;
	stSubLogFont.lfOrientation		= 0;
	stSubLogFont.lfWeight			= 700;
	stSubLogFont.lfItalic			= FALSE;
	stSubLogFont.lfUnderline		= FALSE;
	stSubLogFont.lfStrikeOut		= FALSE;
	stSubLogFont.lfCharSet			= DEFAULT_CHARSET;
	stSubLogFont.lfOutPrecision	= OUT_DEFAULT_PRECIS;
	stSubLogFont.lfClipPrecision	= CLIP_DEFAULT_PRECIS;
	stSubLogFont.lfQuality			= PROOF_QUALITY;
	stSubLogFont.lfPitchAndFamily	= FF_DONTCARE;
	strcpy(stSubLogFont.lfFaceName, "Times New Roman");
	cDataFont.CreateFontIndirect(&stSubLogFont);
	pDC->SelectObject(&cDataFont);

	stTxtLogFont.lfHeight			= (LONG)(pDC->GetDeviceCaps(LOGPIXELSY)*.20);
	stTxtLogFont.lfWidth			= 0;
	stTxtLogFont.lfEscapement		= 0;
	stTxtLogFont.lfOrientation		= 0;
	stTxtLogFont.lfWeight			= 400;
	stTxtLogFont.lfItalic			= FALSE;
	stTxtLogFont.lfUnderline		= FALSE;
	stTxtLogFont.lfStrikeOut		= FALSE;
	stTxtLogFont.lfCharSet			= DEFAULT_CHARSET;
	stTxtLogFont.lfOutPrecision	= OUT_DEFAULT_PRECIS;
	stTxtLogFont.lfClipPrecision	= CLIP_DEFAULT_PRECIS;
	stTxtLogFont.lfQuality			= PROOF_QUALITY;
	stTxtLogFont.lfPitchAndFamily	= FF_DONTCARE;
	strcpy(stTxtLogFont.lfFaceName, "Times New Roman");
	cTextFont.CreateFontIndirect(&stTxtLogFont);

    char* lpszText = new char [80];
    char  line[80];
    int   count = 0;
    int   nitems = pLC->GetItemCount();
    CPoint topleft = rectData1Text.TopLeft();
    CPoint rightbottom = rectData1Text.BottomRight();
    int   left = topleft.x;
    int  right = rightbottom.x;
    int    top = topleft.y;
    int bottom = rightbottom.y;
    int  shift = (right - left) / 2;

    // draw column headings
    top += stSubLogFont.lfHeight * 2;
    bottom += stSubLogFont.lfHeight * 2;
    rectData1Text.SetRect( left, top, right, bottom );
    strTemp = "Name";
    pDC->DrawText(strTemp, rectData1Text, DT_LEFT | DT_VCENTER);
    strTemp = "Units";
    rectData1Text.SetRect( left+shift, top, right+shift, bottom );
    pDC->DrawText(strTemp, rectData1Text, DT_LEFT | DT_VCENTER);
    rectData1Text.SetRect( left, top, right, bottom );

	pDC->SelectObject(&cTextFont);
    top += 20;
    bottom += 20;
    
    // draw line between headings and data
    CPen	cDataPen(PS_SOLID, 4, COLORREF(0x000000));
    pDC->SelectObject(&cDataPen);
    pDC->MoveTo( left,  top+stSubLogFont.lfHeight );
    pDC->LineTo( right, top+stSubLogFont.lfHeight );

    while (count < nitems)
    {
        pLC->GetItemText( count, 0, lpszText, 25 ); 
        strcpy(line,lpszText);
        top    += stTxtLogFont.lfHeight + 20; 
        bottom += stTxtLogFont.lfHeight + 20;
        rectData1Text.SetRect( left, top, right, bottom );
        pDC->DrawText( line, rectData1Text, DT_LEFT | DT_VCENTER );

        pLC->GetItemText( count, 1, lpszText, 25 ); 
        strcpy(line,lpszText);
        strcat(line," ");
        pLC->GetItemText( count, 2, lpszText, 25 ); 
        strcat(line,lpszText);

        rectData1Text.SetRect( left+shift, top, right+shift, bottom );
        pDC->DrawText( line, rectData1Text, DT_LEFT | DT_VCENTER );
        count++;
    }
    delete [] lpszText;

}

/////////////////////////////////////////////////////////////////////////////
// CPrintList diagnostics

#ifdef _DEBUG
void CPrintList::AssertValid() const
{
	CView::AssertValid();
}

void CPrintList::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

void CPrintList::m_fnPrint(CString title, CString subtitle, CListCtrlEx* plc)
{
    Title = title;
    Subtitle = subtitle;
    pLC = plc; 
    OnFilePrint();
}

void CPrintList::m_fnPrintPreview(CString title, CString subtitle, CListCtrlEx* plc)
{
    Title = title;
    Subtitle = subtitle;
    pLC = plc; 
	InvalidateRect(NULL);
}

/////////////////////////////////////////////////////////////////////////////
// CPrintList message handlers

