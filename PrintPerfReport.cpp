// PrintPerfReport.cpp : implementation file
//

#include "stdafx.h"
#include "CTIToolkit.h"
#include "PrintPerfReport.h"

#ifdef _MFC_PREVIEW
#include "afxpriv.h"
#include "ToolkitDoc.h"
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define INCHX(X)		((int)((double)pDC->GetDeviceCaps(LOGPIXELSX) * (double)(X)))
#define INCHY(Y)		((int)((double)pDC->GetDeviceCaps(LOGPIXELSY) * (double)(Y)))
#define FONT_POINT(P)	(MulDiv((P), pDC->GetDeviceCaps(LOGPIXELSY), 72) * -1)

/***
static int NumColors ( CDC *pDC )
{
	int iBits = 16; // default to standard VGA
	int i, j, iPower = 1, iRet=1;
	HDC hDC = pDC->m_hDC;

	iBits = GetDeviceCaps ( hDC, BITSPIXEL );
	for ( i = 0; i < iBits; i++ )   // little kludge to avoid
	{
		// having to include math.h
		for ( j = 0; j < i; j++ )
		{
		  iPower *= 2;
		}
		iRet += iPower;
		iPower = 1;
	}
	return iRet;
}
***/

static BOOL GetBitmapAndPalette(UINT nIDResource, CBitmap &bitmap, CPalette &pal)
{
	LPCTSTR lpszResourceName = (LPCTSTR)nIDResource;
	HBITMAP hBmp = (HBITMAP)::LoadImage( AfxGetInstanceHandle(), lpszResourceName, IMAGE_BITMAP, 0,0, LR_CREATEDIBSECTION );
	if( hBmp == NULL )
	{
		return FALSE;
	}
	bitmap.Attach( hBmp );

	// Create a logical palette for the bitmap	
	DIBSECTION ds;
	BITMAPINFOHEADER &bmInfo = ds.dsBmih;
	bitmap.GetObject( sizeof(ds), &ds );
	int nColors = bmInfo.biClrUsed ? bmInfo.biClrUsed : 1 << bmInfo.biBitCount;

	// Create a halftone palette if colors > 256. 
	CClientDC dc(NULL);
	// Desktop DC
	if( nColors > 256 )
	{
		pal.CreateHalftonePalette( &dc );
	}
	else	
	{
		// Create the palette
		RGBQUAD *pRGB = new RGBQUAD[nColors];
		CDC memDC;
		memDC.CreateCompatibleDC(&dc);
		memDC.SelectObject( &bitmap );
		::GetDIBColorTable( memDC, 0, nColors, pRGB );
		UINT nSize = sizeof(LOGPALETTE) + (sizeof(PALETTEENTRY) * nColors);
		LOGPALETTE *pLP = (LOGPALETTE *) new BYTE[nSize];
		pLP->palVersion = 0x300;
		pLP->palNumEntries = (WORD)nColors;
		for( int i=0; i < nColors; i++)		
		{
			pLP->palPalEntry[i].peRed = pRGB[i].rgbRed;
			pLP->palPalEntry[i].peGreen = pRGB[i].rgbGreen;
			pLP->palPalEntry[i].peBlue = pRGB[i].rgbBlue;
			pLP->palPalEntry[i].peFlags = 0;		
		}
		pal.CreatePalette( pLP );
		delete[] pLP;
		delete[] pRGB;	
	}	
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CPrintPerfReport

IMPLEMENT_DYNCREATE(CPrintPerfReport, CView)

CPrintPerfReport::CPrintPerfReport()
#ifdef _MFC_PREVIEW
: m_bOk(FALSE)
#endif
{
}

CPrintPerfReport::~CPrintPerfReport()
{
}

BEGIN_MESSAGE_MAP(CPrintPerfReport, CView)
	//{{AFX_MSG_MAP(CPrintPerfReport)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**
void CPrintPerfReport::OnInitialUpdate()
{
#ifdef _MFC_PREVIEW
	CView::OnInitialUpdate();
#else
	CScrollView::OnInitialUpdate();
	CClientDC dc(this);
	SetScrollSizes(MM_LOENGLISH, CSize(850, 1100));
#endif
}
**/

/////////////////////////////////////////////////////////////////////////////
// CPrintPerfReport drawing

void CPrintPerfReport::OnDraw(CDC* pDC)
{
#ifdef _MFC_PREVIEW
	if (!m_bOk)
	{
//		CToolkitDoc * pDoc = (CToolkitDoc *)GetDocument();
//		if ((pDoc == 0) || (pDoc->m_bLoaded == FALSE))
			return;
	}
#endif
	if (m_bPerfReport)
	{
		m_fnDrawPerfReport(pDC);
	}
	else
	{
		m_fnDrawDesignReport(pDC);
	}
}

void CPrintPerfReport::m_fnDrawPerfReport(CDC* pDC)
{
	// Note: the report layout depends upon several constants, 
	// set below immediately after font structures.

	CBitmap *poldbmp;
	CDC		memdc;
	memdc.CreateCompatibleDC( pDC );

	CBitmap		tBitmap;
	CPalette 	tPalette;
	/***
	switch (NumColors(pDC))
	{
		case 1:
        case 2:
			tBitmap.LoadBitmap(IDB_BW_LOGO);
			break;

		case 16:
			tBitmap.LoadBitmap(IDB_HICOLOR_LOGO);
			break;

		case 256:
			GetBitmapAndPalette(IDB_256COLOR_LOGO, tBitmap, tPalette);
			break;

		default:
			//tBitmap.LoadBitmap(IDB_HICOLOR_LOGO);
			GetBitmapAndPalette(IDB_256COLOR_LOGO, tBitmap, tPalette);  // only print the 256 color bitmap
			break;
	}
	***/

	// only print the 256 color bitmap
	GetBitmapAndPalette(IDB_256COLOR_LOGO, tBitmap, tPalette);  

	// Select the bitmap into the DC
	poldbmp = memdc.SelectObject( &tBitmap );

	BITMAP BitMap;
	tBitmap.GetBitmap(&BitMap);

    double  hpixels = (double)pDC->GetDeviceCaps(LOGPIXELSX);
    double  vpixels = (double)pDC->GetDeviceCaps(LOGPIXELSY);

	int iDataLineWidth = (int)(vpixels * 0.013);
	int iUserLineWidth = (int)(vpixels * 0.017);

    // preview - scale to fit a property page
	/*
    if (!pDC->IsPrinting())
    {
        CRect r;                              
        getApp().tps->GetClientRect(&r);
        double horizontal = r.right;
        double vertical   = r.bottom;
        double horzpage   = hpixels * 8.5;
        double vertpage   = vpixels * 11.0;
        double scalex     = horizontal / horzpage;
        double scaley     = vertical   / vertpage;
        hpixels *= scalex;
        vpixels *= scaley;
        CBrush brush;
        brush.CreateSolidBrush(0xFFFFFF);
        pDC->FillRect(r,&brush);
    }
	*/

	// Paint the Safe on with a transparent background
	pDC->StretchBlt( int(hpixels*.93), int(vpixels*.9), 
		int(hpixels*1.5), int(vpixels*.8), &memdc, 0, 0, BitMap.bmWidth, BitMap.bmHeight, SRCCOPY);

	memdc.SelectObject( poldbmp );


	bool	bExtrapolated = false;
    CString	strTemp;
    CFont	cTitleFont;
    CFont	cSubjectFont;
    CFont   cTblNameFont;
    CFont   cTblHdrFont;
    CFont   cTextFont;
    CFont   cFooterFont;
	CFont	cInfoFont;
	CFont	cItalFont;
	LOGFONT	stTitleLogFont;
    LOGFONT stSubLogFont;
    LOGFONT stTblNameLogFont;
    LOGFONT stTblHdrLogFont;
    LOGFONT stTxtLogFont;
    LOGFONT stFooterLogFont;
	LOGFONT stInfoLogFont;
	LOGFONT stItalLogFont;

	// Title
	stTitleLogFont.lfHeight			= FONT_POINT(18);
    stTitleLogFont.lfWidth			= 0;
	stTitleLogFont.lfEscapement		= 0;
	stTitleLogFont.lfOrientation	= 0;
	stTitleLogFont.lfWeight			= FW_BOLD;
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

	// Subject
	stSubLogFont.lfHeight			= FONT_POINT(12);
	stSubLogFont.lfWidth			= 0;
	stSubLogFont.lfEscapement		= 0;
	stSubLogFont.lfOrientation		= 0;
	stSubLogFont.lfWeight			= FW_NORMAL;
	stSubLogFont.lfItalic			= FALSE;
	stSubLogFont.lfUnderline		= FALSE;
	stSubLogFont.lfStrikeOut		= FALSE;
	stSubLogFont.lfCharSet			= DEFAULT_CHARSET;
	stSubLogFont.lfOutPrecision		= OUT_DEFAULT_PRECIS;
	stSubLogFont.lfClipPrecision	= CLIP_DEFAULT_PRECIS;
	stSubLogFont.lfQuality			= PROOF_QUALITY;
	stSubLogFont.lfPitchAndFamily	= FF_DONTCARE;
	strcpy(stSubLogFont.lfFaceName, "Times New Roman");
	cSubjectFont.CreateFontIndirect(&stSubLogFont);

	// Table Name
	stTblNameLogFont.lfHeight			= FONT_POINT(14);
	stTblNameLogFont.lfWidth			= 0;
	stTblNameLogFont.lfEscapement		= 0;
	stTblNameLogFont.lfOrientation		= 0;
	stTblNameLogFont.lfWeight			= FW_BOLD;
	stTblNameLogFont.lfItalic			= TRUE;
	stTblNameLogFont.lfUnderline		= FALSE;
	stTblNameLogFont.lfStrikeOut		= FALSE;
	stTblNameLogFont.lfCharSet			= DEFAULT_CHARSET;
	stTblNameLogFont.lfOutPrecision		= OUT_DEFAULT_PRECIS;
	stTblNameLogFont.lfClipPrecision	= CLIP_DEFAULT_PRECIS;
	stTblNameLogFont.lfQuality			= PROOF_QUALITY;
	stTblNameLogFont.lfPitchAndFamily	= FF_DONTCARE;
	strcpy(stTblNameLogFont.lfFaceName, "Times New Roman");
	cTblNameFont.CreateFontIndirect(&stTblNameLogFont);

	// Table Header
	stTblHdrLogFont.lfHeight			= FONT_POINT(12);
	stTblHdrLogFont.lfWidth				= 0;
	stTblHdrLogFont.lfEscapement		= 0;
	stTblHdrLogFont.lfOrientation		= 0;
	stTblHdrLogFont.lfWeight			= FW_BOLD;
	stTblHdrLogFont.lfItalic			= FALSE;
	stTblHdrLogFont.lfUnderline			= FALSE;
	stTblHdrLogFont.lfStrikeOut			= FALSE;
	stTblHdrLogFont.lfCharSet			= DEFAULT_CHARSET;
	stTblHdrLogFont.lfOutPrecision		= OUT_DEFAULT_PRECIS;
	stTblHdrLogFont.lfClipPrecision		= CLIP_DEFAULT_PRECIS;
	stTblHdrLogFont.lfQuality			= PROOF_QUALITY;
	stTblHdrLogFont.lfPitchAndFamily	= FF_DONTCARE;
	strcpy(stTblHdrLogFont.lfFaceName, "Times New Roman");
	cTblHdrFont.CreateFontIndirect(&stTblHdrLogFont);

	// Text
	stTxtLogFont.lfHeight			= FONT_POINT(12);
	stTxtLogFont.lfWidth			= 0;
	stTxtLogFont.lfEscapement		= 0;
	stTxtLogFont.lfOrientation		= 0;
	stTxtLogFont.lfWeight			= FW_NORMAL;
	stTxtLogFont.lfItalic			= FALSE;
	stTxtLogFont.lfUnderline		= FALSE;
	stTxtLogFont.lfStrikeOut		= FALSE;
	stTxtLogFont.lfCharSet			= DEFAULT_CHARSET;
	stTxtLogFont.lfOutPrecision		= OUT_DEFAULT_PRECIS;
	stTxtLogFont.lfClipPrecision	= CLIP_DEFAULT_PRECIS;
	stTxtLogFont.lfQuality			= PROOF_QUALITY;
	stTxtLogFont.lfPitchAndFamily	= FF_DONTCARE;
	strcpy(stTxtLogFont.lfFaceName, "Times New Roman");
	cTextFont.CreateFontIndirect(&stTxtLogFont);

	// Tower Info
	stInfoLogFont.lfHeight			= FONT_POINT(10);
	stInfoLogFont.lfWidth			= 0;
	stInfoLogFont.lfEscapement		= 0;
	stInfoLogFont.lfOrientation		= 0;
	stInfoLogFont.lfWeight			= FW_NORMAL;
	stInfoLogFont.lfItalic			= FALSE;
	stInfoLogFont.lfUnderline		= FALSE;
	stInfoLogFont.lfStrikeOut		= FALSE;
	stInfoLogFont.lfCharSet			= DEFAULT_CHARSET;
	stInfoLogFont.lfOutPrecision	= OUT_DEFAULT_PRECIS;
	stInfoLogFont.lfClipPrecision	= CLIP_DEFAULT_PRECIS;
	stInfoLogFont.lfQuality			= PROOF_QUALITY;
	stInfoLogFont.lfPitchAndFamily	= FF_DONTCARE;
	strcpy(stInfoLogFont.lfFaceName, "Times New Roman");
	cInfoFont.CreateFontIndirect(&stInfoLogFont);

	// Italic Info (currently only for extrapolated warning)
	stItalLogFont.lfHeight			= FONT_POINT(10);
	stItalLogFont.lfWidth			= 0;
	stItalLogFont.lfEscapement		= 0;
	stItalLogFont.lfOrientation		= 0;
	stItalLogFont.lfWeight			= FW_NORMAL;
	stItalLogFont.lfItalic			= TRUE;
	stItalLogFont.lfUnderline		= FALSE;
	stItalLogFont.lfStrikeOut		= FALSE;
	stItalLogFont.lfCharSet			= DEFAULT_CHARSET;
	stItalLogFont.lfOutPrecision	= OUT_DEFAULT_PRECIS;
	stItalLogFont.lfClipPrecision	= CLIP_DEFAULT_PRECIS;
	stItalLogFont.lfQuality			= PROOF_QUALITY;
	stItalLogFont.lfPitchAndFamily	= FF_DONTCARE;
	strcpy(stItalLogFont.lfFaceName, "Times New Roman");
	cItalFont.CreateFontIndirect(&stItalLogFont);

	// Footer
	stFooterLogFont.lfHeight			= FONT_POINT(8);
	stFooterLogFont.lfWidth			= 0;
	stFooterLogFont.lfEscapement		= 0;
	stFooterLogFont.lfOrientation		= 0;
	stFooterLogFont.lfWeight			= FW_NORMAL;
	stFooterLogFont.lfItalic			= FALSE;
	stFooterLogFont.lfUnderline		= FALSE;
	stFooterLogFont.lfStrikeOut		= FALSE;
	stFooterLogFont.lfCharSet			= DEFAULT_CHARSET;
	stFooterLogFont.lfOutPrecision		= OUT_DEFAULT_PRECIS;
	stFooterLogFont.lfClipPrecision	= CLIP_DEFAULT_PRECIS;
	stFooterLogFont.lfQuality			= PROOF_QUALITY;
	stFooterLogFont.lfPitchAndFamily	= FF_DONTCARE;
	strcpy(stFooterLogFont.lfFaceName, "Times New Roman");
	cFooterFont.CreateFontIndirect(&stFooterLogFont);

	CRect rectText;
	CBrush brushRects(COLORREF(0x000000));

	pDC->SetTextColor(COLORREF(0x000000));
//	pDC->SetMapMode(MM_TEXT);

	// Set constants used throughout the report
	const double LEFT_MARGIN(1.0);
	const double RIGHT_MARGIN(8.5 - LEFT_MARGIN);
	const double RIGHT_FOOTER_MARGIN(7.25);
	const double TEXT_HEIGHT(0.20);		// height for cTextFont (adj. for point size, above)
	const double INFO_HEIGHT(0.15);		// height for cInfoFont (adj. for point size, above)
	const double TBLNAME_HEIGHT(0.25);	// height for cTblNameFont ("")
	const double ROW_HEIGHT(0.25);		// height for table rows
	const double MIN_HEADER_HEIGHT(1.75); // min. height for report header, incl. title, tower info. and logo
	const double TOP_MARGIN(0.75);
	const TCHAR  FILENAME_SEPARATOR = _T('\\'); // printed between design & test filenames

	double dblCurrentY(TOP_MARGIN);

	// Init Range Count (depending on # of ranges, we can space the report more nicely ...)
	const int NUM_RANGES(m_designData.m_fnGetRangeCnt());
	const double INTER_TABLE_SPACE(NUM_RANGES < 4 ? 0.20 : 0.15);	// space between tables

	// Title
	double dblDeltaY = 0.30;
	strTemp = "Tower Performance Report";
    rectText.SetRect(INCHX(LEFT_MARGIN), INCHY(dblCurrentY), 
		INCHX(RIGHT_MARGIN), INCHY(dblCurrentY+dblDeltaY));
	m_fnDrawText(pDC, strTemp, &cTitleFont, &rectText, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
	dblCurrentY += dblDeltaY;

	// Subject
	if (m_strTitle.GetLength() > 0)
	{
		strTemp = m_strTitle;
		rectText.SetRect(INCHX(LEFT_MARGIN), INCHY(dblCurrentY), 
			INCHX(RIGHT_MARGIN), INCHY(dblCurrentY+dblDeltaY));
		m_fnDrawText(pDC, strTemp, &cSubjectFont, &rectText, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
		dblCurrentY += dblDeltaY;
	}

	// Footer
	CCTIApp * pApp = static_cast<CCTIApp *>(AfxGetApp());
	strTemp = CString(_T("Produced by ")) + pApp->GetAppCopyright() + 
		_T(" ") + pApp->GetShortVersion();
	strTemp.TrimRight();
    rectText.SetRect(INCHX(LEFT_MARGIN), INCHY(10.25), INCHX(RIGHT_FOOTER_MARGIN), INCHY(10.50));
	m_fnDrawText(pDC, strTemp, &cFooterFont, &rectText, DT_SINGLELINE | DT_RIGHT | DT_BOTTOM);

	// File & test name footer
	strTemp = m_designData.m_fnGetFileName();
	strTemp = strTemp.Right(strTemp.GetLength()-strTemp.ReverseFind(_T('\\'))-1);
	strTemp += FILENAME_SEPARATOR + m_testData.m_fnGetName();
	rectText.SetRect(INCHX(LEFT_MARGIN), INCHY(10.25), INCHX(RIGHT_FOOTER_MARGIN), INCHY(10.50));
	m_fnDrawText(pDC, strTemp, &cFooterFont, &rectText, DT_SINGLELINE | DT_LEFT | DT_BOTTOM);

	// Print Tower Information (Owner, Project Name, etc)
	strTemp = m_designData.m_fnGetTowerOwner();
	if (strTemp.GetLength() > 0)
	{
		strTemp = CString(_T("Owner:  ")) + strTemp;
		rectText.SetRect(INCHX(LEFT_MARGIN), INCHY(dblCurrentY), 
			INCHX(RIGHT_MARGIN), INCHY(dblCurrentY+INFO_HEIGHT));
		m_fnDrawText(pDC, strTemp, &cInfoFont, &rectText, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
		dblCurrentY += INFO_HEIGHT;
	}
	strTemp = m_designData.m_fnGetProjectName();
	if (strTemp.GetLength() > 0)
	{
		strTemp = CString(_T("Project:  ")) + strTemp;
		rectText.SetRect(INCHX(LEFT_MARGIN), INCHY(dblCurrentY), 
			INCHX(RIGHT_MARGIN), INCHY(dblCurrentY+INFO_HEIGHT));
		m_fnDrawText(pDC, strTemp, &cInfoFont, &rectText, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
		dblCurrentY += INFO_HEIGHT;
	}
	strTemp = m_designData.m_fnGetTowerLocation();
	if (strTemp.GetLength() > 0)
	{
		strTemp = CString(_T("Location:  ")) + strTemp;
		rectText.SetRect(INCHX(LEFT_MARGIN), INCHY(dblCurrentY), 
			INCHX(RIGHT_MARGIN), INCHY(dblCurrentY+INFO_HEIGHT));
		m_fnDrawText(pDC, strTemp, &cInfoFont, &rectText, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
		dblCurrentY += INFO_HEIGHT;
	}
	strTemp = m_designData.m_fnGetTowerManufacturer();
	if (strTemp.GetLength() > 0)
	{
		strTemp = CString(_T("Manufacturer:  ")) + strTemp;
		rectText.SetRect(INCHX(LEFT_MARGIN), INCHY(dblCurrentY), 
			INCHX(RIGHT_MARGIN), INCHY(dblCurrentY+INFO_HEIGHT));
		m_fnDrawText(pDC, strTemp, &cInfoFont, &rectText, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
		dblCurrentY += INFO_HEIGHT;
	}
	strTemp = CString(_T("Tower Type: "));
	strTemp += (m_designData.m_fnGetInducedFlow() ? 
		_T("Induced"): _T("Forced"));
	strTemp += _T(" Draft");
	rectText.SetRect(INCHX(LEFT_MARGIN), INCHY(dblCurrentY), 
		INCHX(RIGHT_MARGIN), INCHY(dblCurrentY+INFO_HEIGHT));
	m_fnDrawText(pDC, strTemp, &cInfoFont, &rectText, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
	dblCurrentY += INFO_HEIGHT;

	// Check to make sure first table is below report title & logo
	if (dblCurrentY < MIN_HEADER_HEIGHT)
		dblCurrentY = MIN_HEADER_HEIGHT;

	dblDeltaY = NUM_RANGES < 5 ? 0.15 : 0.10;
	dblCurrentY += dblDeltaY;	// some padding

	// Cooling Tower Design and Test Data
	strTemp = "Cooling Tower Design and Test Data";
    rectText.SetRect(INCHX(LEFT_MARGIN), INCHY(dblCurrentY), 
		INCHX(RIGHT_MARGIN), INCHY(dblCurrentY+TBLNAME_HEIGHT));
	m_fnDrawText(pDC, strTemp, &cTblNameFont, &rectText, DT_SINGLELINE | DT_VCENTER);
	dblCurrentY += TBLNAME_HEIGHT;


	double dblAdjustedFlow;
	double dblDesignAirDensity;
	double dblTestAirDensity;
	double dblDesignSpecificVolume;
	double dblTestSpecificVolume;

	if (m_bIPUnits)
	{
		dblDesignAirDensity		= CalcDensityIP(calcPressureC(m_designData.m_fnGetBarometricPressure(m_bIPUnits)), m_designData.m_fnGetDBT(m_bIPUnits), m_designData.m_fnGetWBT(m_bIPUnits));
		dblTestAirDensity		= CalcDensityIP(calcPressureC(m_testData.m_fnGetBarometricPressure(m_bIPUnits)),   m_testData.m_fnGetDBT(m_bIPUnits),   m_testData.m_fnGetWBT(m_bIPUnits));
		dblDesignSpecificVolume = SpecificVolumeIP(calcPressureC(m_designData.m_fnGetBarometricPressure(m_bIPUnits)), m_designData.m_fnGetDBT(m_bIPUnits), m_designData.m_fnGetWBT(m_bIPUnits));
		dblTestSpecificVolume	= SpecificVolumeIP(calcPressureC(m_testData.m_fnGetBarometricPressure(m_bIPUnits)), m_testData.m_fnGetDBT(m_bIPUnits), m_testData.m_fnGetWBT(m_bIPUnits));
	}
	else
	{
		dblDesignAirDensity		= CalcDensitySI((m_designData.m_fnGetBarometricPressure(m_bIPUnits)), m_designData.m_fnGetDBT(m_bIPUnits), m_designData.m_fnGetWBT(m_bIPUnits));
		dblTestAirDensity		= CalcDensitySI((m_testData.m_fnGetBarometricPressure(m_bIPUnits)), m_testData.m_fnGetDBT(m_bIPUnits), m_testData.m_fnGetWBT(m_bIPUnits));
		dblDesignSpecificVolume = SpecificVolumeSI((m_designData.m_fnGetBarometricPressure(m_bIPUnits)), m_designData.m_fnGetDBT(m_bIPUnits), m_designData.m_fnGetWBT(m_bIPUnits));
		dblTestSpecificVolume	= SpecificVolumeSI((m_testData.m_fnGetBarometricPressure(m_bIPUnits)), m_testData.m_fnGetDBT(m_bIPUnits), m_testData.m_fnGetWBT(m_bIPUnits));
	}

	double dblTestLG;
	double LWBTnew	= 0;
	double DenOutT	= 0;
	double SVOutT	= 0;
	double HLWBT	= 0; 
	dblAdjustedFlow	= DetermineAdjTestFlow(m_bIPUnits, (int)m_designData.m_fnGetInducedFlow(), m_designData.m_fnGetHWT(m_bIPUnits), m_designData.m_fnGetCWT(m_bIPUnits), m_designData.m_fnGetWBT(m_bIPUnits), m_designData.m_fnGetDBT(m_bIPUnits), m_designData.m_fnGetBarometricPressure(m_bIPUnits), m_designData.m_fnGetWaterFlowRate(m_bIPUnits), m_designData.m_fnGetFanDriverPower(m_bIPUnits), m_designData.m_fnGetLG(m_bIPUnits), m_testData.m_fnGetHWT(m_bIPUnits), m_testData.m_fnGetCWT(m_bIPUnits), m_testData.m_fnGetWBT(m_bIPUnits), m_testData.m_fnGetDBT(m_bIPUnits), m_testData.m_fnGetBarometricPressure(m_bIPUnits), m_testData.m_fnGetWaterFlowRate(m_bIPUnits), m_testData.m_fnGetFanDriverPower(m_bIPUnits), LWBTnew, DenOutT, SVOutT, HLWBT, dblAdjustedFlow, dblTestLG);
	//double dblTestLG= CalcTestLG(m_pPerfData->m_dblDesignLG, m_pPerfData->m_dblDesignWaterFlowRate, m_pPerfData->m_arrayTests[m_iTestIndex].m_dblTestWaterFlowRate, m_pPerfData->m_dblDesignFanDriverPower, m_pPerfData->m_arrayTests[m_iTestIndex].m_dblTestFanDriverPower, dblDesignAirDensity, dblTestAirDensity, dblDesignSpecificVolume, dblTestSpecificVolume);

	int	iFlowIndex;
	double dblTblStartX		= LEFT_MARGIN;
	double dblTblStartY		= dblCurrentY;
	double dblColWidth		= 2.0;
	double dblCellHeight	= ROW_HEIGHT;
	int iCol				= 0;
	int iRow				= 0;

	CString strUnitsFlowRate	=	"gpm";
	CString strUnitsFanPower	=	"bhp";
	CString strUnitsPressure	=	"in. Hg.";
	CString strUnitsTemp		=	"°F";

	if (!m_bIPUnits)
	{
		strUnitsFlowRate	=	"l/s";
		strUnitsFanPower	=	"kW";
		strUnitsPressure	=	"kPa";
		strUnitsTemp		=	"°C";
	}

	iCol = 0;
	iRow = 0;
	strTemp = "   Parameters";
	m_fnDrawText(pDC, strTemp, &cTblHdrFont, DT_SINGLELINE | DT_VCENTER, dblTblStartX, dblTblStartY, dblColWidth, dblCellHeight, iRow, iCol);

	iCol = 1;
	iRow = 0;
	strTemp = "   Design";
	m_fnDrawText(pDC, strTemp, &cTblHdrFont, DT_SINGLELINE | DT_VCENTER, dblTblStartX, dblTblStartY, dblColWidth, dblCellHeight, iRow, iCol);

	iCol = 2;
	iRow = 0;
	strTemp = "   Test";
	m_fnDrawText(pDC, strTemp, &cTblHdrFont, DT_SINGLELINE | DT_VCENTER, dblTblStartX, dblTblStartY, dblColWidth, dblCellHeight, iRow, iCol);
	dblCurrentY += dblCellHeight;

	iCol = 0;
	iRow = 1;
	strTemp = "   Water Flow Rate";
	m_fnDrawText(pDC, strTemp, &cTextFont, DT_SINGLELINE | DT_VCENTER, dblTblStartX, dblTblStartY, dblColWidth, dblCellHeight, iRow, iCol);

	iCol = 1;
	iRow = 1;
	strTemp.Format("   %.1f  %s", m_designData.m_fnGetWaterFlowRate(m_bIPUnits), (LPCTSTR)strUnitsFlowRate);
	m_fnDrawText(pDC, strTemp, &cTextFont, DT_SINGLELINE | DT_VCENTER, dblTblStartX, dblTblStartY, dblColWidth, dblCellHeight, iRow, iCol);

	iCol = 2;
	iRow = 1;
	strTemp.Format("   %.1f  %s", m_testData.m_fnGetWaterFlowRate(m_bIPUnits), (LPCTSTR)strUnitsFlowRate);
	m_fnDrawText(pDC, strTemp, &cTextFont, DT_SINGLELINE | DT_VCENTER, dblTblStartX, dblTblStartY, dblColWidth, dblCellHeight, iRow, iCol);
	dblCurrentY += dblCellHeight;


	iCol = 0;
	iRow = 2;
	strTemp = "   Hot Water Temp.";
	m_fnDrawText(pDC, strTemp, &cTextFont, DT_SINGLELINE | DT_VCENTER, dblTblStartX, dblTblStartY, dblColWidth, dblCellHeight, iRow, iCol);

	iCol = 1;
	iRow = 2;
	strTemp.Format("   %.2f  %s", m_designData.m_fnGetHWT(m_bIPUnits), (LPCTSTR)strUnitsTemp);
	m_fnDrawText(pDC, strTemp, &cTextFont, DT_SINGLELINE | DT_VCENTER, dblTblStartX, dblTblStartY, dblColWidth, dblCellHeight, iRow, iCol);

	iCol = 2;
	iRow = 2;
	strTemp.Format("   %.2f  %s", m_testData.m_fnGetHWT(m_bIPUnits), (LPCTSTR)strUnitsTemp);
	m_fnDrawText(pDC, strTemp, &cTextFont, DT_SINGLELINE | DT_VCENTER, dblTblStartX, dblTblStartY, dblColWidth, dblCellHeight, iRow, iCol);
	dblCurrentY += dblCellHeight;


	iCol = 0;
	iRow = 3;
	strTemp = "   Cold Water Temp.";
	m_fnDrawText(pDC, strTemp, &cTextFont, DT_SINGLELINE | DT_VCENTER, dblTblStartX, dblTblStartY, dblColWidth, dblCellHeight, iRow, iCol);

	iCol = 1;
	iRow = 3;
	strTemp.Format("   %.2f  %s", m_designData.m_fnGetCWT(m_bIPUnits), (LPCTSTR)strUnitsTemp);
	m_fnDrawText(pDC, strTemp, &cTextFont, DT_SINGLELINE | DT_VCENTER, dblTblStartX, dblTblStartY, dblColWidth, dblCellHeight, iRow, iCol);

	iCol = 2;
	iRow = 3;
	strTemp.Format("   %.2f  %s", m_testData.m_fnGetCWT(m_bIPUnits), (LPCTSTR)strUnitsTemp);
	m_fnDrawText(pDC, strTemp, &cTextFont, DT_SINGLELINE | DT_VCENTER, dblTblStartX, dblTblStartY, dblColWidth, dblCellHeight, iRow, iCol);
	dblCurrentY += dblCellHeight;


	iCol = 0;
	iRow = 4;
	strTemp = "   Wet Bulb Temp.";
	m_fnDrawText(pDC, strTemp, &cTextFont, DT_SINGLELINE | DT_VCENTER, dblTblStartX, dblTblStartY, dblColWidth, dblCellHeight, iRow, iCol);

	iCol = 1;
	iRow = 4;
	strTemp.Format("   %.2f  %s", m_designData.m_fnGetWBT(m_bIPUnits), (LPCTSTR)strUnitsTemp);
	m_fnDrawText(pDC, strTemp, &cTextFont, DT_SINGLELINE | DT_VCENTER, dblTblStartX, dblTblStartY, dblColWidth, dblCellHeight, iRow, iCol);

	iCol = 2;
	iRow = 4;
	strTemp.Format("   %.2f  %s", m_testData.m_fnGetWBT(m_bIPUnits), (LPCTSTR)strUnitsTemp);
	m_fnDrawText(pDC, strTemp, &cTextFont, DT_SINGLELINE | DT_VCENTER, dblTblStartX, dblTblStartY, dblColWidth, dblCellHeight, iRow, iCol);
	dblCurrentY += dblCellHeight;


	iCol = 0;
	iRow = 5;
	strTemp = "   Dry Bulb Temp.";
	m_fnDrawText(pDC, strTemp, &cTextFont, DT_SINGLELINE | DT_VCENTER, dblTblStartX, dblTblStartY, dblColWidth, dblCellHeight, iRow, iCol);

	iCol = 1;
	iRow = 5;
	strTemp.Format("   %.2f  %s", m_designData.m_fnGetDBT(m_bIPUnits), (LPCTSTR)strUnitsTemp);
	m_fnDrawText(pDC, strTemp, &cTextFont, DT_SINGLELINE | DT_VCENTER, dblTblStartX, dblTblStartY, dblColWidth, dblCellHeight, iRow, iCol);

	iCol = 2;
	iRow = 5;
	strTemp.Format("   %.2f  %s", m_testData.m_fnGetDBT(m_bIPUnits), (LPCTSTR)strUnitsTemp);
	m_fnDrawText(pDC, strTemp, &cTextFont, DT_SINGLELINE | DT_VCENTER, dblTblStartX, dblTblStartY, dblColWidth, dblCellHeight, iRow, iCol);
	dblCurrentY += dblCellHeight;


	iCol = 0;
	iRow = 6;
	strTemp = "   Fan Driver Power";
	m_fnDrawText(pDC, strTemp, &cTextFont, DT_SINGLELINE | DT_VCENTER, dblTblStartX, dblTblStartY, dblColWidth, dblCellHeight, iRow, iCol);

	iCol = 1;
	iRow = 6;
	strTemp.Format("   %.2f  %s", m_designData.m_fnGetFanDriverPower(m_bIPUnits), (LPCTSTR)strUnitsFanPower);
	m_fnDrawText(pDC, strTemp, &cTextFont, DT_SINGLELINE | DT_VCENTER, dblTblStartX, dblTblStartY, dblColWidth, dblCellHeight, iRow, iCol);

	iCol = 2;
	iRow = 6;
	strTemp.Format("   %.2f  %s", m_testData.m_fnGetFanDriverPower(m_bIPUnits), (LPCTSTR)strUnitsFanPower);
	m_fnDrawText(pDC, strTemp, &cTextFont, DT_SINGLELINE | DT_VCENTER, dblTblStartX, dblTblStartY, dblColWidth, dblCellHeight, iRow, iCol);
	dblCurrentY += dblCellHeight;


	iCol = 0;
	iRow = 7;
	strTemp = "   Barometric Pressure";
	m_fnDrawText(pDC, strTemp, &cTextFont, DT_SINGLELINE | DT_VCENTER, dblTblStartX, dblTblStartY, dblColWidth, dblCellHeight, iRow, iCol);

	iCol = 1;
	iRow = 7;
	strTemp.Format("   %.3f  %s", m_designData.m_fnGetBarometricPressure(m_bIPUnits), (LPCTSTR)strUnitsPressure);
	m_fnDrawText(pDC, strTemp, &cTextFont, DT_SINGLELINE | DT_VCENTER, dblTblStartX, dblTblStartY, dblColWidth, dblCellHeight, iRow, iCol);

	iCol = 2;
	iRow = 7;
	strTemp.Format("   %.3f  %s", m_testData.m_fnGetBarometricPressure(m_bIPUnits), (LPCTSTR)strUnitsPressure);
	m_fnDrawText(pDC, strTemp, &cTextFont, DT_SINGLELINE | DT_VCENTER, dblTblStartX, dblTblStartY, dblColWidth, dblCellHeight, iRow, iCol);
	dblCurrentY += dblCellHeight;


	iCol = 0;
	iRow = 8;
	strTemp = "   Liquid to Gas Ratio";
	m_fnDrawText(pDC, strTemp, &cTextFont, DT_SINGLELINE | DT_VCENTER, dblTblStartX, dblTblStartY, dblColWidth, dblCellHeight, iRow, iCol);


	iCol = 1;
	iRow = 8;
	strTemp.Format("   %.3f", m_designData.m_fnGetLG(m_bIPUnits));
	m_fnDrawText(pDC, strTemp, &cTextFont, DT_SINGLELINE | DT_VCENTER, dblTblStartX, dblTblStartY, dblColWidth, dblCellHeight, iRow, iCol);

	iCol = 2;
	iRow = 8;
	strTemp.Format("   %.3f", dblTestLG);
	m_fnDrawText(pDC, strTemp, &cTextFont, DT_SINGLELINE | DT_VCENTER, dblTblStartX, dblTblStartY, dblColWidth, dblCellHeight, iRow, iCol);
	dblCurrentY += dblCellHeight;


	dblCurrentY += INTER_TABLE_SPACE;

	// Cold Water Temperatures vs. Range 
	strTemp = "Cold Water Temperatures vs. Range";
    rectText.SetRect(INCHX(LEFT_MARGIN), INCHY(dblCurrentY), 
		INCHX(RIGHT_MARGIN), INCHY(dblCurrentY+TBLNAME_HEIGHT));
	m_fnDrawText(pDC, strTemp, &cTblNameFont, &rectText, DT_SINGLELINE | DT_VCENTER);
	dblCurrentY += TBLNAME_HEIGHT;

	strTemp.Format("At %.2f %s Test Wet Bulb", m_testData.m_fnGetWBT(m_bIPUnits), (LPCTSTR)strUnitsTemp);
    rectText.SetRect(INCHX(LEFT_MARGIN), INCHY(dblCurrentY), 
		INCHX(7.25), INCHY(dblCurrentY+TEXT_HEIGHT));
	m_fnDrawText(pDC, strTemp, &cTblHdrFont, &rectText, DT_SINGLELINE | DT_VCENTER);
	dblCurrentY += TEXT_HEIGHT;


	dblTblStartX	= LEFT_MARGIN;
	dblTblStartY	= dblCurrentY;
	dblColWidth		= 1.1;
	dblCellHeight	= ROW_HEIGHT;
	iCol			= 0;
	iRow			= 0;


// Init Counts
	int IFlowNo		=	m_designData.m_fnGetFlowCnt();
	int IRangeNo	=	NUM_RANGES; // m_designData.m_fnGetRangeCnt();
	int IWBNo		=	m_designData.m_fnGetWBTCnt(0);


	iCol = 0;
	iRow = 0;
	strTemp = "   Range";
	m_fnDrawText(pDC, strTemp, &cTblHdrFont, DT_SINGLELINE | DT_VCENTER, dblTblStartX, dblTblStartY, dblColWidth, dblCellHeight, iRow, iCol);

	for (iFlowIndex = 0; iFlowIndex < IFlowNo; iFlowIndex++)
	{
		iCol = iFlowIndex+1;
		iRow = 0;
//		strTemp.Format("   %.1f %s", m_designData.m_fnGetFlow(iFlowIndex, m_bIPUnits), (LPCTSTR)strUnitsFlowRate);
		strTemp.Format("%.1f %s", m_designData.m_fnGetFlow(iFlowIndex, m_bIPUnits), (LPCTSTR)strUnitsFlowRate);
		m_fnDrawText(pDC, strTemp, &cTblHdrFont, DT_SINGLELINE | DT_CENTER | DT_VCENTER, dblTblStartX, dblTblStartY, dblColWidth, dblCellHeight, iRow, iCol);
	}
	dblCurrentY += dblCellHeight;

	for (int iRangeIndex = 0; iRangeIndex < IRangeNo; iRangeIndex++)
	{
		iCol = 0;
		iRow = iRangeIndex + 1;
		strTemp.Format("   %.2f %s", m_designData.m_fnGetRange(iRangeIndex, m_bIPUnits), (LPCTSTR)strUnitsTemp);
		m_fnDrawText(pDC, strTemp, &cTextFont, DT_SINGLELINE | DT_VCENTER, dblTblStartX, dblTblStartY, dblColWidth, dblCellHeight, iRow, iCol);
		dblCurrentY += dblCellHeight;
	}



	//
	// Copy data to temp vars and do calcs
	//
	int IFlow;
	int IR;

    double Y[24] /*FLOW[5]*/;
	double YFIT;

    double /*X[24],*/ /*YFIT[24],*/ /*YA[3],*/ /*Y2A[3],*/ Y2[24];
    double /*R[5],*/ /*CWT[5][10][5],*/ CWT_WB[5][5], CWT_WB_R[5];
//'                    (FLOW, WB, RANGE)

// Init Arrays
	double R[MAX_RANGES];
	double FLOW[MAX_FLOWS];
	double WB[MAX_WBTS];
	double WBTemp[MAX_FLOWS][MAX_WBTS];
	double CWT[MAX_FLOWS][MAX_WBTS][MAX_RANGES];

	m_designData.m_fnGetCWTArray(CWT, m_bIPUnits);
	m_designData.m_fnGetRangeArray(R, m_bIPUnits);
	m_designData.m_fnGetFlowArray(FLOW, m_bIPUnits);
	m_designData.m_fnGetWBTArray(WBTemp, m_bIPUnits);

	//WBs may change each flow so put the copy in the flow loop


//Init Others
	double TestWB	= m_testData.m_fnGetWBT(m_bIPUnits);
	double TestR	= m_testData.m_fnGetHWT(m_bIPUnits) - m_testData.m_fnGetCWT(m_bIPUnits);
	double TestCWT	= m_testData.m_fnGetCWT(m_bIPUnits);


//'Interpolate for WB
#ifdef _DEBUG
//    PRINT USING "@ TestWB=##.##"; TestWB
//    PRINT #2, USING "@ TestWB=##.###"; TestWB
	strTemp.Format("@ TestWB=%.03f\r\n", TestWB);
	OutputDebugString(strTemp);
#endif // _DEBUG

    for (IFlow = 0; IFlow < IFlowNo; IFlow++)
	{
		IWBNo =	m_designData.m_fnGetWBTCnt(IFlow);

        for (IR = 0; IR < IRangeNo; IR++)
		{
            for (int IWB = 0; IWB < IWBNo; IWB++)
			{
                Y[IWB] = CWT[IFlow][IWB][IR];
			}
			memcpy(WB, WBTemp[IFlow], sizeof(WB));
            CalcPerfData(IWBNo, WB, Y, TestWB, YFIT, Y2);

            CWT_WB[IFlow][IR] = YFIT;

#ifdef _DEBUG
            //PRINT USING "FLOW=###### RANGE=##.## CWT=##.###"; FLOW(IFlow); R(IR); CWT_WB(IFlow, IR)          
			strTemp.Format("FLOW=%.0f RANGE=%.02f CWT=%.03f\r\n", FLOW[IFlow], R[IR], CWT_WB[IFlow][IR]);
			OutputDebugString(strTemp);
#endif // _DEBUG

			iCol = IFlow+1;
			iRow = IR+1;
			strTemp.Format("   %.2f %s", CWT_WB[IFlow][IR], (LPCTSTR)strUnitsTemp);
			m_fnDrawText(pDC, strTemp, &cTextFont, DT_SINGLELINE | DT_VCENTER, dblTblStartX, dblTblStartY, dblColWidth, dblCellHeight, iRow, iCol);

        }
		TRACE0("\r\n");
    }


//'Interpolate for Range
#ifdef _DEBUG
//    PRINT #2, USING "@ TestWB=##.## and TestR=##.##"; TestWB; TestR
	strTemp.Format("@ TestWB=%.02f and TestR=%.02f\r\n", TestWB, TestR);
	OutputDebugString(strTemp);
#endif // _DEBUG


	// Cold Water Temperature vs. Water Flow 
	dblCurrentY += INTER_TABLE_SPACE;

	strTemp = "Cold Water Temperature vs. Water Flow";
    rectText.SetRect(INCHX(LEFT_MARGIN), INCHY(dblCurrentY), 
		INCHX(RIGHT_MARGIN), INCHY(dblCurrentY+0.25));
	m_fnDrawText(pDC, strTemp, &cTblNameFont, &rectText, DT_SINGLELINE | DT_VCENTER);
	dblCurrentY += 0.25;

	strTemp.Format("At %.2f %s Test Wet Bulb and %.2f %s Test Range", TestWB, (LPCTSTR)strUnitsTemp, TestR, (LPCTSTR)strUnitsTemp);
    rectText.SetRect(INCHX(LEFT_MARGIN), INCHY(dblCurrentY), 
		INCHX(RIGHT_MARGIN), INCHY(dblCurrentY+0.25));
	m_fnDrawText(pDC, strTemp, &cTblHdrFont, &rectText, DT_SINGLELINE | DT_VCENTER);
	dblCurrentY += 0.25;


	dblTblStartX	= LEFT_MARGIN;
	dblTblStartY	= dblCurrentY; // 6.75;
	dblColWidth		= 1.25;
	dblCellHeight	= ROW_HEIGHT;
	iCol			= 0;
	iRow			= 0;

	iCol = 0;
	iRow = 0;

	for (iFlowIndex = 0; iFlowIndex < IFlowNo; iFlowIndex++)
	{
		iCol = iFlowIndex;
		iRow = 0;
		strTemp.Format("   %.1f %s", m_designData.m_fnGetFlow(iFlowIndex, m_bIPUnits), (LPCTSTR)strUnitsFlowRate);
		m_fnDrawText(pDC, strTemp, &cTblHdrFont, DT_SINGLELINE | DT_VCENTER, dblTblStartX, dblTblStartY, dblColWidth, dblCellHeight, iRow, iCol);
	}
	dblCurrentY += dblCellHeight;

    for (IFlow = 0; IFlow < IFlowNo; IFlow++)
	{
        for (IR = 0; IR < IRangeNo; IR++)
		{
            Y[IR] = CWT_WB[IFlow][IR];
        }
        CalcPerfData(IRangeNo, R, Y, TestR, YFIT, Y2);
        CWT_WB_R[IFlow] = YFIT;

#ifdef _DEBUG
        //PRINT USING "FLOW=######  CWT=##.###"; FLOW(IFlow); CWT_WB_R(IFlow)
		strTemp.Format("FLOW=%.0f CWT=%.03f\r\n", FLOW[IFlow], CWT_WB_R[IFlow]);
		OutputDebugString(strTemp);
#endif // _DEBUG

		iCol = IFlow;
		iRow = 1;
		strTemp.Format("   %.2f %s", CWT_WB_R[IFlow], (LPCTSTR)strUnitsTemp);
		m_fnDrawText(pDC, strTemp, &cTextFont, DT_SINGLELINE | DT_VCENTER, dblTblStartX, dblTblStartY, dblColWidth, dblCellHeight, iRow, iCol);
	}
	dblCurrentY += dblCellHeight;
	TRACE0("\r\n");

//'Final interpolation for Flow (Y) vs CWT (X)
#ifdef _DEBUG
//    PRINT #2, USING "@ TestWB=##.## and TestR=##.## and TestCWT=###.##"; TestWB; TestR; TestCWT
	strTemp.Format("@ TestWB=%.02f and TestR=%.02f and TestCWT=%.02f\r\n", TestWB, TestR, TestCWT);
	OutputDebugString(strTemp);
#endif // _DEBUG

    CalcPerfData(IFlowNo, CWT_WB_R, FLOW, TestCWT, YFIT, Y2);
    double PredFlow = YFIT;

//'Rev:1-23-01 Added check for EXTRAPOLATED flow
//'Determine Min and Max Flows
    double MinFlow = FLOW[0];
	double MaxFlow = FLOW[0];
    for (int iExt = 1; iExt < IFlowNo; iExt++)
	{
        if (FLOW[iExt] < MinFlow)
		{
			MinFlow = FLOW[iExt];
		}
        if (FLOW[iExt] > MaxFlow)
		{
			MaxFlow = FLOW[iExt];
		}
    }
    if ((PredFlow < MinFlow) || (PredFlow > MaxFlow))
	{
        //PRINT #2, STRING$(80, 42)
        //PRINT #2, USING "CAUTION: Predicted Flow ####.## is EXTRAPOLATED beyond the"; PredFlow
        //PRINT #2, USING "Supplied Curve Flows of #####.## to #####.##"; MinFlow; MaxFlow
        //PRINT #2, STRING$(80, 42)
		CString strWarning;
		strWarning.Format("CAUTION: Predicted Flow %.02f is EXTRAPOLATED beyond the\r\nSupplied Curve Flows of %.02f to %.02f", PredFlow, MinFlow, MaxFlow);
		//MessageBox(strWarning, "Extrapolation Warning", MB_ICONWARNING);
		bExtrapolated = true;
    }
//'End check for Extrapolation

	
	LWBTnew	= 0;
	DenOutT	= 0;
	SVOutT	= 0;
	HLWBT	= 0; 
	dblAdjustedFlow	= DetermineAdjTestFlow(m_bIPUnits, 
		// Design Input Values
		(int)m_designData.m_fnGetInducedFlow(), 
		m_designData.m_fnGetHWT(m_bIPUnits), m_designData.m_fnGetCWT(m_bIPUnits), 
		m_designData.m_fnGetWBT(m_bIPUnits), m_designData.m_fnGetDBT(m_bIPUnits), 
		m_designData.m_fnGetBarometricPressure(m_bIPUnits), 
		m_designData.m_fnGetWaterFlowRate(m_bIPUnits), m_designData.m_fnGetFanDriverPower(m_bIPUnits), 
		m_designData.m_fnGetLG(m_bIPUnits), 
		// Test Input Values
		m_testData.m_fnGetHWT(m_bIPUnits), m_testData.m_fnGetCWT(m_bIPUnits), 
		m_testData.m_fnGetWBT(m_bIPUnits), m_testData.m_fnGetDBT(m_bIPUnits), 
		m_testData.m_fnGetBarometricPressure(m_bIPUnits), 
		m_testData.m_fnGetWaterFlowRate(m_bIPUnits), m_testData.m_fnGetFanDriverPower(m_bIPUnits), 
		// Output Values
		LWBTnew, DenOutT, SVOutT, HLWBT, dblAdjustedFlow, dblTestLG);
	double AdjTestFlow = dblAdjustedFlow;


	double DesignAdjTestFlow= dblAdjustedFlow;
	double DesignLWBTnew	= 0;
	double DesignDenOutT	= 0;
	double DesignSVOutT		= 0;
	double DesignHLWBT		= 0; 
	DesignAdjTestFlow		= DetermineAdjTestFlow(m_bIPUnits, 
		// Design Input Values		
		(int)m_designData.m_fnGetInducedFlow(), 
		m_designData.m_fnGetHWT(m_bIPUnits), m_designData.m_fnGetCWT(m_bIPUnits), 
		m_designData.m_fnGetWBT(m_bIPUnits), m_designData.m_fnGetDBT(m_bIPUnits), 
		m_designData.m_fnGetBarometricPressure(m_bIPUnits), 
		m_designData.m_fnGetWaterFlowRate(m_bIPUnits), m_designData.m_fnGetFanDriverPower(m_bIPUnits), 
		m_designData.m_fnGetLG(m_bIPUnits), 
		// Test Input Values (these are actually design values)
		m_designData.m_fnGetHWT(m_bIPUnits), m_designData.m_fnGetCWT(m_bIPUnits), 
		m_designData.m_fnGetWBT(m_bIPUnits),m_designData.m_fnGetDBT(m_bIPUnits), 
		m_designData.m_fnGetBarometricPressure(m_bIPUnits), 
		m_designData.m_fnGetWaterFlowRate(m_bIPUnits), m_designData.m_fnGetFanDriverPower(m_bIPUnits), 
		// Output Values
		DesignLWBTnew, DesignDenOutT, DesignSVOutT, DesignHLWBT, DesignAdjTestFlow, dblTestLG);

#ifdef _DEBUG
//    PRINT USING "TestCWT=##.###  PredFlow=######.###"; TestCWT; PredFlow
//    PRINT USING "AdjTestFlow=######.###  PredFlow=######.###  Percent Capability=###.##"; AdjTestFlow; PredFlow; AdjTestFlow / PredFlow * 100
	strTemp.Format("TestCWT=%.02f PredFlow=%.03f\r\n",TestCWT, PredFlow);
	OutputDebugString(strTemp);
	strTemp.Format("AdjTestFlow=%.03f  PredFlow=%.03f  Percent Capability=%.03f", AdjTestFlow, PredFlow, AdjTestFlow / PredFlow * 100.0);
	OutputDebugString(strTemp);
#endif // _DEBUG

	// Cooling Tower Design and Test Data
	dblCurrentY += INTER_TABLE_SPACE;
	if (m_designData.m_fnGetInducedFlow())
		strTemp = "Exit Air Properties";
	else
		strTemp = "Inlet Air Properties";

    rectText.SetRect(INCHX(LEFT_MARGIN), INCHY(dblCurrentY), 
		INCHX(RIGHT_MARGIN), INCHY(dblCurrentY+0.25));
	m_fnDrawText(pDC, strTemp, &cTblNameFont, &rectText, DT_SINGLELINE | DT_VCENTER);
	dblCurrentY += 0.25;

	dblTblStartX	= LEFT_MARGIN;
	dblTblStartY	= dblCurrentY; // 7.75;
	dblColWidth		= 1.25;
	dblCellHeight	= ROW_HEIGHT;
	iCol			= 0;
	iRow			= 0;

	iCol = 0;
	iRow = 0;
	strTemp = "     ";
	m_fnDrawText(pDC, strTemp, &cTblHdrFont, DT_SINGLELINE | DT_VCENTER, dblTblStartX, dblTblStartY, dblColWidth, dblCellHeight, iRow, iCol);

	iCol = 1;
	iRow = 0;
	strTemp = " Wet Bulb Temp";
	m_fnDrawText(pDC, strTemp, &cTblHdrFont, DT_SINGLELINE | DT_VCENTER, dblTblStartX, dblTblStartY, dblColWidth, dblCellHeight, iRow, iCol);

	iCol = 2;
	iRow = 0;
	strTemp = "   Density";
	m_fnDrawText(pDC, strTemp, &cTblHdrFont, DT_SINGLELINE | DT_VCENTER, dblTblStartX, dblTblStartY, dblColWidth, dblCellHeight, iRow, iCol);

	iCol = 3;
	iRow = 0;
	strTemp = "   Sp. Vol.";
	m_fnDrawText(pDC, strTemp, &cTblHdrFont, DT_SINGLELINE | DT_VCENTER, dblTblStartX, dblTblStartY, dblColWidth, dblCellHeight, iRow, iCol);

	iCol = 4;
	iRow = 0;
	strTemp = "   Enthalpy";
	m_fnDrawText(pDC, strTemp, &cTblHdrFont, DT_SINGLELINE | DT_VCENTER, dblTblStartX, dblTblStartY, dblColWidth, dblCellHeight, iRow, iCol);

	dblCurrentY += 0.25;

	iCol = 0;
	iRow = 1;
	strTemp = "   Design";
	m_fnDrawText(pDC, strTemp, &cTblHdrFont, DT_SINGLELINE | DT_VCENTER, dblTblStartX, dblTblStartY, dblColWidth, dblCellHeight, iRow, iCol);

	iCol = 1;
	iRow = 1;
	strTemp.Format("   %.2f", DesignLWBTnew);
	m_fnDrawText(pDC, strTemp, &cTextFont, DT_SINGLELINE | DT_VCENTER, dblTblStartX, dblTblStartY, dblColWidth, dblCellHeight, iRow, iCol);

	iCol = 2;
	iRow = 1;
	strTemp.Format("   %.5f", DesignDenOutT);
	m_fnDrawText(pDC, strTemp, &cTextFont, DT_SINGLELINE | DT_VCENTER, dblTblStartX, dblTblStartY, dblColWidth, dblCellHeight, iRow, iCol);

	iCol = 3;
	iRow = 1;
	strTemp.Format("   %.4f", DesignSVOutT);
	m_fnDrawText(pDC, strTemp, &cTextFont, DT_SINGLELINE | DT_VCENTER, dblTblStartX, dblTblStartY, dblColWidth, dblCellHeight, iRow, iCol);

	iCol = 4;
	iRow = 1;
	strTemp.Format("   %.4f", DesignHLWBT);
	m_fnDrawText(pDC, strTemp, &cTextFont, DT_SINGLELINE | DT_VCENTER, dblTblStartX, dblTblStartY, dblColWidth, dblCellHeight, iRow, iCol);

	dblCurrentY += 0.25;

	iCol = 0;
	iRow = 2;
	strTemp = "   Test";
	m_fnDrawText(pDC, strTemp, &cTblHdrFont, DT_SINGLELINE | DT_VCENTER, dblTblStartX, dblTblStartY, dblColWidth, dblCellHeight, iRow, iCol);

	iCol = 1;
	iRow = 2;
	strTemp.Format("   %.2f", LWBTnew);
	m_fnDrawText(pDC, strTemp, &cTextFont, DT_SINGLELINE | DT_VCENTER, dblTblStartX, dblTblStartY, dblColWidth, dblCellHeight, iRow, iCol);

	iCol = 2;
	iRow = 2;
	strTemp.Format("   %.5f", DenOutT);
	m_fnDrawText(pDC, strTemp, &cTextFont, DT_SINGLELINE | DT_VCENTER, dblTblStartX, dblTblStartY, dblColWidth, dblCellHeight, iRow, iCol);

	iCol = 3;
	iRow = 2;
	strTemp.Format("   %.4f", SVOutT);
	m_fnDrawText(pDC, strTemp, &cTextFont, DT_SINGLELINE | DT_VCENTER, dblTblStartX, dblTblStartY, dblColWidth, dblCellHeight, iRow, iCol);

	iCol = 4;
	iRow = 2;
	strTemp.Format("   %.4f", HLWBT);
	m_fnDrawText(pDC, strTemp, &cTextFont, DT_SINGLELINE | DT_VCENTER, dblTblStartX, dblTblStartY, dblColWidth, dblCellHeight, iRow, iCol);

	dblCurrentY += INTER_TABLE_SPACE;

	double dblCapability = AdjTestFlow / PredFlow * 100.0;

	// Cooling Tower Design and Test Data
	strTemp = "Test Results";
    rectText.SetRect(INCHX(LEFT_MARGIN), INCHY(9.25), INCHX(RIGHT_MARGIN), INCHY(9.5));
	m_fnDrawText(pDC, strTemp, &cTblNameFont, &rectText, DT_SINGLELINE | DT_VCENTER);

	
	dblTblStartX	= LEFT_MARGIN;
	dblTblStartY	= 9.5;
	dblColWidth		= 1.5;
	dblCellHeight	= ROW_HEIGHT;
	iCol			= 0;
	iRow			= 0;

	if (bExtrapolated)
	{
		strTemp = _T("* Indicates predicted flow is extrapolated.");
	    rectText.SetRect(INCHX(LEFT_MARGIN+dblColWidth), INCHY(9.25), 
			INCHX(RIGHT_MARGIN), INCHY(9.5));
		m_fnDrawText(pDC, strTemp, &cItalFont, &rectText, DT_SINGLELINE | DT_VCENTER);
	}

	iCol = 0;
	iRow = 0;
	strTemp = "   Adjusted Flow";
	m_fnDrawText(pDC, strTemp, &cTblHdrFont, DT_SINGLELINE | DT_VCENTER, dblTblStartX, dblTblStartY, dblColWidth, dblCellHeight, iRow, iCol);

	iCol = 1;
	iRow = 0;
	strTemp = "   Predicted Flow";
	m_fnDrawText(pDC, strTemp, &cTblHdrFont, DT_SINGLELINE | DT_VCENTER, dblTblStartX, dblTblStartY, dblColWidth, dblCellHeight, iRow, iCol);

	iCol = 2;
	iRow = 0;
	strTemp = "   CWT Deviation";
	m_fnDrawText(pDC, strTemp, &cTblHdrFont, DT_SINGLELINE | DT_VCENTER, dblTblStartX, dblTblStartY, dblColWidth, dblCellHeight, iRow, iCol);

	iCol = 3;
	iRow = 0;
	strTemp = "   Tower Capability";
	m_fnDrawText(pDC, strTemp, &cTblHdrFont, DT_SINGLELINE | DT_VCENTER, dblTblStartX, dblTblStartY, dblColWidth, dblCellHeight, iRow, iCol);

	iCol = 0;
	iRow = 1;
	strTemp.Format("   %.1f %s", AdjTestFlow, (LPCTSTR)strUnitsFlowRate);
	m_fnDrawText(pDC, strTemp, &cTextFont, DT_SINGLELINE | DT_VCENTER, dblTblStartX, dblTblStartY, dblColWidth, dblCellHeight, iRow, iCol);

	iCol = 1;
	iRow = 1;
	if (bExtrapolated)
	{
//		strTemp.Format("   %.1f %s (Extrapolated)", PredFlow, (LPCTSTR)strUnitsFlowRate);
		strTemp.Format("   %.1f %s *", PredFlow, (LPCTSTR)strUnitsFlowRate);
	}
	else
	{
		strTemp.Format("   %.1f %s", PredFlow, (LPCTSTR)strUnitsFlowRate);
	}
	m_fnDrawText(pDC, strTemp, &cTextFont, DT_SINGLELINE | DT_VCENTER, dblTblStartX, dblTblStartY, dblColWidth, dblCellHeight, iRow, iCol);

	iCol = 2;
	iRow = 1;
	strTemp.Format("   %.2f %s", m_dblCWTDev, (LPCTSTR)strUnitsTemp);
	m_fnDrawText(pDC, strTemp, &cTextFont, DT_SINGLELINE | DT_VCENTER, dblTblStartX, dblTblStartY, dblColWidth, dblCellHeight, iRow, iCol);

	iCol = 3;
	iRow = 1;
	strTemp.Format("   %.1f%s", dblCapability, "%");
	m_fnDrawText(pDC, strTemp, &cTextFont, DT_SINGLELINE | DT_VCENTER, dblTblStartX, dblTblStartY, dblColWidth, dblCellHeight, iRow, iCol);
	
	// Print CTI-requested disclaimer
	strTemp = _T("This test result is only certified by CTI if the test data ");
	strTemp += _T("was collected by a CTI Licensed Testing Agency. ");
	strTemp += _T("See www.cti.org for an agency list.");
	// For some reason, in print preview this disclaimer needs a little more right margin ....
    rectText.SetRect(INCHX(LEFT_MARGIN), INCHY(10.0), INCHX(RIGHT_FOOTER_MARGIN+0.5), INCHY(10.25));
	m_fnDrawText(pDC, strTemp, &cFooterFont, &rectText, DT_SINGLELINE | DT_LEFT | DT_VCENTER);
}


void CPrintPerfReport::m_fnDrawDesignReport(CDC* pDC)
{

	// Note: the report layout depends upon several constants, 
	// set below immediately after font structures.

	CBitmap *poldbmp;
	CDC		memdc;
	memdc.CreateCompatibleDC( pDC );

	CBitmap		tBitmap;
	CPalette 	tPalette;
	
	/***
	switch (NumColors(pDC))
	{
		case 1:
        case 2:
			tBitmap.LoadBitmap(IDB_BW_LOGO);
			break;

		case 16:
			tBitmap.LoadBitmap(IDB_HICOLOR_LOGO);
			break;

		case 256:
			GetBitmapAndPalette(IDB_256COLOR_LOGO, tBitmap, tPalette);
			break;

		default:
			//tBitmap.LoadBitmap(IDB_HICOLOR_LOGO);
			GetBitmapAndPalette(IDB_256COLOR_LOGO, tBitmap, tPalette);  // only print the 256 color bitmap
			break;
	}
	***/

	// only print the 256 color bitmap
	GetBitmapAndPalette(IDB_256COLOR_LOGO, tBitmap, tPalette); 
	// Select the bitmap into the DC
	poldbmp = memdc.SelectObject( &tBitmap );

	BITMAP BitMap;
	tBitmap.GetBitmap(&BitMap);

    double  hpixels = (double)pDC->GetDeviceCaps(LOGPIXELSX);
    double  vpixels = (double)pDC->GetDeviceCaps(LOGPIXELSY);

	int iDataLineWidth = (int)(vpixels * 0.013);
	int iUserLineWidth = (int)(vpixels * 0.017);

    // preview - scale to fit a property page
	/*
    if (!pDC->IsPrinting())
    {
        CRect r;                              
        getApp().tps->GetClientRect(&r);
        double horizontal = r.right;
        double vertical   = r.bottom;
        double horzpage   = hpixels * 8.5;
        double vertpage   = vpixels * 11.0;
        double scalex     = horizontal / horzpage;
        double scaley     = vertical   / vertpage;
        hpixels *= scalex;
        vpixels *= scaley;
        CBrush brush;
        brush.CreateSolidBrush(0xFFFFFF);
        pDC->FillRect(r,&brush);
    }
	*/

	// Paint the Safe on with a transparent background
	pDC->StretchBlt( int(hpixels*.93), int(vpixels*.9), 
		int(hpixels*1.5), int(vpixels*.8), &memdc, 0, 0, BitMap.bmWidth, BitMap.bmHeight, SRCCOPY);

	memdc.SelectObject( poldbmp );


	bool	bExtrapolated = false;
    CString	strTemp;
    CFont	cTitleFont;
    CFont	cSubjectFont;
    CFont   cTblNameFont;
    CFont   cTblHdrFont;
    CFont   cTextFont;
    CFont   cFooterFont;
	LOGFONT	stTitleLogFont;
    LOGFONT stSubLogFont;
    LOGFONT stTblNameLogFont;
    LOGFONT stTblHdrLogFont;
    LOGFONT stTxtLogFont;
    LOGFONT stFooterLogFont;

	// Title
	stTitleLogFont.lfHeight			= FONT_POINT(18);
    stTitleLogFont.lfWidth			= 0;
	stTitleLogFont.lfEscapement		= 0;
	stTitleLogFont.lfOrientation	= 0;
	stTitleLogFont.lfWeight			= FW_BOLD;
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

	// Subject
	stSubLogFont.lfHeight			= FONT_POINT(12);
	stSubLogFont.lfWidth			= 0;
	stSubLogFont.lfEscapement		= 0;
	stSubLogFont.lfOrientation		= 0;
	stSubLogFont.lfWeight			= FW_NORMAL;
	stSubLogFont.lfItalic			= FALSE;
	stSubLogFont.lfUnderline		= FALSE;
	stSubLogFont.lfStrikeOut		= FALSE;
	stSubLogFont.lfCharSet			= DEFAULT_CHARSET;
	stSubLogFont.lfOutPrecision		= OUT_DEFAULT_PRECIS;
	stSubLogFont.lfClipPrecision	= CLIP_DEFAULT_PRECIS;
	stSubLogFont.lfQuality			= PROOF_QUALITY;
	stSubLogFont.lfPitchAndFamily	= FF_DONTCARE;
	strcpy(stSubLogFont.lfFaceName, "Times New Roman");
	cSubjectFont.CreateFontIndirect(&stSubLogFont);

	// Table Name
	stTblNameLogFont.lfHeight			= FONT_POINT(12);
	stTblNameLogFont.lfWidth			= 0;
	stTblNameLogFont.lfEscapement		= 0;
	stTblNameLogFont.lfOrientation		= 0;
	stTblNameLogFont.lfWeight			= FW_BOLD;
	stTblNameLogFont.lfItalic			= TRUE;
	stTblNameLogFont.lfUnderline		= FALSE;
	stTblNameLogFont.lfStrikeOut		= FALSE;
	stTblNameLogFont.lfCharSet			= DEFAULT_CHARSET;
	stTblNameLogFont.lfOutPrecision		= OUT_DEFAULT_PRECIS;
	stTblNameLogFont.lfClipPrecision	= CLIP_DEFAULT_PRECIS;
	stTblNameLogFont.lfQuality			= PROOF_QUALITY;
	stTblNameLogFont.lfPitchAndFamily	= FF_DONTCARE;
	strcpy(stTblNameLogFont.lfFaceName, "Times New Roman");
	cTblNameFont.CreateFontIndirect(&stTblNameLogFont);

	// Table Header
	stTblHdrLogFont.lfHeight			= FONT_POINT(10);
	stTblHdrLogFont.lfWidth				= 0;
	stTblHdrLogFont.lfEscapement		= 0;
	stTblHdrLogFont.lfOrientation		= 0;
	stTblHdrLogFont.lfWeight			= FW_BOLD;
	stTblHdrLogFont.lfItalic			= FALSE;
	stTblHdrLogFont.lfUnderline			= FALSE;
	stTblHdrLogFont.lfStrikeOut			= FALSE;
	stTblHdrLogFont.lfCharSet			= DEFAULT_CHARSET;
	stTblHdrLogFont.lfOutPrecision		= OUT_DEFAULT_PRECIS;
	stTblHdrLogFont.lfClipPrecision		= CLIP_DEFAULT_PRECIS;
	stTblHdrLogFont.lfQuality			= PROOF_QUALITY;
	stTblHdrLogFont.lfPitchAndFamily	= FF_DONTCARE;
	strcpy(stTblHdrLogFont.lfFaceName, "Times New Roman");
	cTblHdrFont.CreateFontIndirect(&stTblHdrLogFont);

	// Text
	stTxtLogFont.lfHeight			= FONT_POINT(10);
	stTxtLogFont.lfWidth			= 0;
	stTxtLogFont.lfEscapement		= 0;
	stTxtLogFont.lfOrientation		= 0;
	stTxtLogFont.lfWeight			= FW_NORMAL;
	stTxtLogFont.lfItalic			= FALSE;
	stTxtLogFont.lfUnderline		= FALSE;
	stTxtLogFont.lfStrikeOut		= FALSE;
	stTxtLogFont.lfCharSet			= DEFAULT_CHARSET;
	stTxtLogFont.lfOutPrecision		= OUT_DEFAULT_PRECIS;
	stTxtLogFont.lfClipPrecision	= CLIP_DEFAULT_PRECIS;
	stTxtLogFont.lfQuality			= PROOF_QUALITY;
	stTxtLogFont.lfPitchAndFamily	= FF_DONTCARE;
	strcpy(stTxtLogFont.lfFaceName, "Times New Roman");
	cTextFont.CreateFontIndirect(&stTxtLogFont);
	
	// Footer
	stFooterLogFont.lfHeight			= FONT_POINT(8);
	stFooterLogFont.lfWidth			= 0;
	stFooterLogFont.lfEscapement		= 0;
	stFooterLogFont.lfOrientation		= 0;
	stFooterLogFont.lfWeight			= FW_NORMAL;
	stFooterLogFont.lfItalic			= FALSE;
	stFooterLogFont.lfUnderline		= FALSE;
	stFooterLogFont.lfStrikeOut		= FALSE;
	stFooterLogFont.lfCharSet			= DEFAULT_CHARSET;
	stFooterLogFont.lfOutPrecision		= OUT_DEFAULT_PRECIS;
	stFooterLogFont.lfClipPrecision	= CLIP_DEFAULT_PRECIS;
	stFooterLogFont.lfQuality			= PROOF_QUALITY;
	stFooterLogFont.lfPitchAndFamily	= FF_DONTCARE;
	strcpy(stFooterLogFont.lfFaceName, "Times New Roman");
	cFooterFont.CreateFontIndirect(&stFooterLogFont);

	CRect rectText;
	CBrush brushRects(COLORREF(0x000000));

	pDC->SetTextColor(COLORREF(0x000000));
//	pDC->SetMapMode(MM_TEXT);

	// Set constants used throughout the report
	const double LEFT_MARGIN(1.0);
	const double RIGHT_MARGIN(8.5 - LEFT_MARGIN);
	const double TEXT_HEIGHT(0.15);		// height for cTextFont (adj. for point size, above)
	const double TBLNAME_HEIGHT(0.20);	// height for cTblNameFont ("")
	const double ROW_HEIGHT(0.15);		// height for table rows
	const double MIN_HEADER_HEIGHT(1.75); // min. height for report header, incl. title, tower info. and logo
	const double TOP_MARGIN(0.75);
	const double RIGHT_FOOTER_MARGIN(7.25);

	double dblCurrentY(TOP_MARGIN);

	// Init Flow Count (depending on # of flows, we can space the report more nicely ...)
	const int NUM_FLOWS(m_designData.m_fnGetFlowCnt());


	// Title
	double dblDeltaY = NUM_FLOWS < 5 ? 0.30 : 0.25;
	strTemp = _T("Tower Design Data Report");
    rectText.SetRect(INCHX(LEFT_MARGIN), INCHY(dblCurrentY), 
		INCHX(RIGHT_MARGIN), INCHY(dblCurrentY+dblDeltaY));
	m_fnDrawText(pDC, strTemp, &cTitleFont, &rectText, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
	dblCurrentY += dblDeltaY;

	// Subject
	if (m_strTitle.GetLength() > 0)
	{
		strTemp = m_strTitle;
		rectText.SetRect(INCHX(dblCurrentY), INCHY(dblCurrentY), 
			INCHX(RIGHT_MARGIN), INCHY(dblCurrentY+dblDeltaY));
		m_fnDrawText(pDC, strTemp, &cSubjectFont, &rectText, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
		dblCurrentY += dblDeltaY;
	}

	// Footer
	CCTIApp * pApp = static_cast<CCTIApp *>(AfxGetApp());
	strTemp = CString(_T("Produced by ")) + pApp->GetAppCopyright() + 
		_T(" ") + pApp->GetShortVersion();
    rectText.SetRect(INCHX(LEFT_MARGIN), INCHY(10.35), INCHX(RIGHT_FOOTER_MARGIN), INCHY(10.50));
	m_fnDrawText(pDC, strTemp, &cFooterFont, &rectText, DT_SINGLELINE | DT_RIGHT | DT_BOTTOM);

	// File name footer
	strTemp = m_designData.m_fnGetFileName();
	strTemp = strTemp.Right(strTemp.GetLength()-strTemp.ReverseFind(_T('\\'))-1);
	rectText.SetRect(INCHX(LEFT_MARGIN), INCHY(10.25), INCHX(RIGHT_FOOTER_MARGIN), INCHY(10.50));
	m_fnDrawText(pDC, strTemp, &cFooterFont, &rectText, DT_SINGLELINE | DT_LEFT | DT_BOTTOM);

	// Print Tower Information (Owner, Project Name, etc)
	strTemp = m_designData.m_fnGetTowerOwner();
	if (strTemp.GetLength() > 0)
	{
		strTemp = CString(_T("Owner:  ")) + strTemp;
		rectText.SetRect(INCHX(LEFT_MARGIN), INCHY(dblCurrentY), 
			INCHX(RIGHT_MARGIN), INCHY(dblCurrentY+TEXT_HEIGHT));
		m_fnDrawText(pDC, strTemp, &cTextFont, &rectText, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
		dblCurrentY += TEXT_HEIGHT;
	}
	strTemp = m_designData.m_fnGetProjectName();
	if (strTemp.GetLength() > 0)
	{
		strTemp = CString(_T("Project:  ")) + strTemp;
		rectText.SetRect(INCHX(LEFT_MARGIN), INCHY(dblCurrentY), 
			INCHX(RIGHT_MARGIN), INCHY(dblCurrentY+TEXT_HEIGHT));
		m_fnDrawText(pDC, strTemp, &cTextFont, &rectText, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
		dblCurrentY += TEXT_HEIGHT;
	}
	strTemp = m_designData.m_fnGetTowerLocation();
	if (strTemp.GetLength() > 0)
	{
		strTemp = CString(_T("Location:  ")) + strTemp;
		rectText.SetRect(INCHX(LEFT_MARGIN), INCHY(dblCurrentY), 
			INCHX(RIGHT_MARGIN), INCHY(dblCurrentY+TEXT_HEIGHT));
		m_fnDrawText(pDC, strTemp, &cTextFont, &rectText, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
		dblCurrentY += TEXT_HEIGHT;
	}
	strTemp = m_designData.m_fnGetTowerManufacturer();
	if (strTemp.GetLength() > 0)
	{
		strTemp = CString(_T("Manufacturer:  ")) + strTemp;
		rectText.SetRect(INCHX(LEFT_MARGIN), INCHY(dblCurrentY), 
			INCHX(RIGHT_MARGIN), INCHY(dblCurrentY+TEXT_HEIGHT));
		m_fnDrawText(pDC, strTemp, &cTextFont, &rectText, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
		dblCurrentY += TEXT_HEIGHT;
	}
	strTemp = CString(_T("Tower Type: "));
	strTemp += (m_designData.m_fnGetInducedFlow() ? 
		_T("Induced"): _T("Forced"));
	strTemp += _T(" Draft");
	rectText.SetRect(INCHX(LEFT_MARGIN), INCHY(dblCurrentY), 
		INCHX(RIGHT_MARGIN), INCHY(dblCurrentY+TEXT_HEIGHT));
	m_fnDrawText(pDC, strTemp, &cTextFont, &rectText, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
	dblCurrentY += TEXT_HEIGHT;
	
	// Check to make sure first table is below report title & logo
	if (dblCurrentY < MIN_HEADER_HEIGHT)
		dblCurrentY = MIN_HEADER_HEIGHT;
	
	// Cooling Tower Design and Test Data
	dblDeltaY = NUM_FLOWS < 5 ? 0.15 : 0.05;
	dblCurrentY += dblDeltaY;
	strTemp = "Cooling Tower Design Data";
    rectText.SetRect(INCHX(LEFT_MARGIN), INCHY(dblCurrentY), 
		INCHX(RIGHT_MARGIN), INCHY(dblCurrentY+TBLNAME_HEIGHT));
	m_fnDrawText(pDC, strTemp, &cTblNameFont, &rectText, DT_SINGLELINE | DT_VCENTER);
	dblCurrentY += TBLNAME_HEIGHT;

	double dblDesignAirDensity;
	double dblTestAirDensity;
	double dblDesignSpecificVolume;
	double dblTestSpecificVolume;

	if (m_bIPUnits)
	{
		dblDesignAirDensity		= CalcDensityIP(calcPressureC(m_designData.m_fnGetBarometricPressure(m_bIPUnits)), m_designData.m_fnGetDBT(m_bIPUnits), m_designData.m_fnGetWBT(m_bIPUnits));
		dblTestAirDensity		= CalcDensityIP(calcPressureC(m_testData.m_fnGetBarometricPressure(m_bIPUnits)),   m_testData.m_fnGetDBT(m_bIPUnits),   m_testData.m_fnGetWBT(m_bIPUnits));
		dblDesignSpecificVolume = SpecificVolumeIP(calcPressureC(m_designData.m_fnGetBarometricPressure(m_bIPUnits)), m_designData.m_fnGetDBT(m_bIPUnits), m_designData.m_fnGetWBT(m_bIPUnits));
		dblTestSpecificVolume	= SpecificVolumeIP(calcPressureC(m_testData.m_fnGetBarometricPressure(m_bIPUnits)), m_testData.m_fnGetDBT(m_bIPUnits), m_testData.m_fnGetWBT(m_bIPUnits));
	}
	else
	{
		dblDesignAirDensity		= CalcDensitySI((m_designData.m_fnGetBarometricPressure(m_bIPUnits)), m_designData.m_fnGetDBT(m_bIPUnits), m_designData.m_fnGetWBT(m_bIPUnits));
		dblTestAirDensity		= CalcDensitySI((m_testData.m_fnGetBarometricPressure(m_bIPUnits)), m_testData.m_fnGetDBT(m_bIPUnits), m_testData.m_fnGetWBT(m_bIPUnits));
		dblDesignSpecificVolume = SpecificVolumeSI((m_designData.m_fnGetBarometricPressure(m_bIPUnits)), m_designData.m_fnGetDBT(m_bIPUnits), m_designData.m_fnGetWBT(m_bIPUnits));
		dblTestSpecificVolume	= SpecificVolumeSI((m_testData.m_fnGetBarometricPressure(m_bIPUnits)), m_testData.m_fnGetDBT(m_bIPUnits), m_testData.m_fnGetWBT(m_bIPUnits));
	}

	double dblTblStartX		= LEFT_MARGIN;
	double dblTblStartY		= dblCurrentY;
	double dblColWidth		= 1.5;
	double dblCellHeight	= ROW_HEIGHT;
	int iCol				= 0;
	int iRow				= 0;

	CString strUnitsFlowRate	=	"gpm";
	CString strUnitsFanPower	=	"bhp";
	CString strUnitsPressure	=	"in. Hg.";
	CString strUnitsTemp		=	"°F";

	if (!m_bIPUnits)
	{
		strUnitsFlowRate	=	"l/s";
		strUnitsFanPower	=	"kW";
		strUnitsPressure	=	"kPa";
		strUnitsTemp		=	"°C";
	}

	iCol = 0;
	// iRow = 0;
	strTemp = "   Water Flow Rate";
	m_fnDrawText(pDC, strTemp, &cTextFont, DT_SINGLELINE | DT_VCENTER, dblTblStartX, dblTblStartY, dblColWidth, dblCellHeight, iRow, iCol);

	iCol = 1;
	strTemp.Format("   %.1f  %s", m_designData.m_fnGetWaterFlowRate(m_bIPUnits), (LPCTSTR)strUnitsFlowRate);
	m_fnDrawText(pDC, strTemp, &cTextFont, DT_SINGLELINE | DT_VCENTER, dblTblStartX, dblTblStartY, dblColWidth, dblCellHeight, iRow, iCol);
	dblCurrentY += ROW_HEIGHT;

	iCol = 0;
	iRow++;	// 1
	strTemp = "   Hot Water Temp.";
	m_fnDrawText(pDC, strTemp, &cTextFont, DT_SINGLELINE | DT_VCENTER, dblTblStartX, dblTblStartY, dblColWidth, dblCellHeight, iRow, iCol);

	iCol = 1;
	strTemp.Format("   %.2f  %s", m_designData.m_fnGetHWT(m_bIPUnits), (LPCTSTR)strUnitsTemp);
	m_fnDrawText(pDC, strTemp, &cTextFont, DT_SINGLELINE | DT_VCENTER, dblTblStartX, dblTblStartY, dblColWidth, dblCellHeight, iRow, iCol);
	dblCurrentY += ROW_HEIGHT;

	iCol = 0;
	iRow++;	// 2
	strTemp = "   Cold Water Temp.";
	m_fnDrawText(pDC, strTemp, &cTextFont, DT_SINGLELINE | DT_VCENTER, dblTblStartX, dblTblStartY, dblColWidth, dblCellHeight, iRow, iCol);

	iCol = 1;
	strTemp.Format("   %.2f  %s", m_designData.m_fnGetCWT(m_bIPUnits), (LPCTSTR)strUnitsTemp);
	m_fnDrawText(pDC, strTemp, &cTextFont, DT_SINGLELINE | DT_VCENTER, dblTblStartX, dblTblStartY, dblColWidth, dblCellHeight, iRow, iCol);
	dblCurrentY += ROW_HEIGHT;

	iCol = 0;
	iRow++;	// 3;
	strTemp = "   Wet Bulb Temp.";
	m_fnDrawText(pDC, strTemp, &cTextFont, DT_SINGLELINE | DT_VCENTER, dblTblStartX, dblTblStartY, dblColWidth, dblCellHeight, iRow, iCol);

	iCol = 1;
	strTemp.Format("   %.2f  %s", m_designData.m_fnGetWBT(m_bIPUnits), (LPCTSTR)strUnitsTemp);
	m_fnDrawText(pDC, strTemp, &cTextFont, DT_SINGLELINE | DT_VCENTER, dblTblStartX, dblTblStartY, dblColWidth, dblCellHeight, iRow, iCol);
	dblCurrentY += ROW_HEIGHT;

	iCol = 0;
	iRow++;	// 4;
	strTemp = "   Dry Bulb Temp.";
	m_fnDrawText(pDC, strTemp, &cTextFont, DT_SINGLELINE | DT_VCENTER, dblTblStartX, dblTblStartY, dblColWidth, dblCellHeight, iRow, iCol);

	iCol = 1;
	strTemp.Format("   %.2f  %s", m_designData.m_fnGetDBT(m_bIPUnits), (LPCTSTR)strUnitsTemp);
	m_fnDrawText(pDC, strTemp, &cTextFont, DT_SINGLELINE | DT_VCENTER, dblTblStartX, dblTblStartY, dblColWidth, dblCellHeight, iRow, iCol);
	dblCurrentY += ROW_HEIGHT;

	iCol = 0;
	iRow++;	// 5
	strTemp = "   Fan Driver Power";
	m_fnDrawText(pDC, strTemp, &cTextFont, DT_SINGLELINE | DT_VCENTER, dblTblStartX, dblTblStartY, dblColWidth, dblCellHeight, iRow, iCol);

	iCol = 1;
	strTemp.Format("   %.2f  %s", m_designData.m_fnGetFanDriverPower(m_bIPUnits), (LPCTSTR)strUnitsFanPower);
	m_fnDrawText(pDC, strTemp, &cTextFont, DT_SINGLELINE | DT_VCENTER, dblTblStartX, dblTblStartY, dblColWidth, dblCellHeight, iRow, iCol);
	dblCurrentY += ROW_HEIGHT;

	iCol = 0;
	iRow++;	// 6
	strTemp = "   Barometric Pressure";
	m_fnDrawText(pDC, strTemp, &cTextFont, DT_SINGLELINE | DT_VCENTER, dblTblStartX, dblTblStartY, dblColWidth, dblCellHeight, iRow, iCol);

	iCol = 1;
	strTemp.Format("   %.3f  %s", m_designData.m_fnGetBarometricPressure(m_bIPUnits), (LPCTSTR)strUnitsPressure);
	m_fnDrawText(pDC, strTemp, &cTextFont, DT_SINGLELINE | DT_VCENTER, dblTblStartX, dblTblStartY, dblColWidth, dblCellHeight, iRow, iCol);
	dblCurrentY += ROW_HEIGHT;

	iCol = 0;
	iRow++;	// 7
	strTemp = "   Liquid to Gas Ratio";
	m_fnDrawText(pDC, strTemp, &cTextFont, DT_SINGLELINE | DT_VCENTER, dblTblStartX, dblTblStartY, dblColWidth, dblCellHeight, iRow, iCol);

	iCol = 1;
	strTemp.Format("   %.3f", m_designData.m_fnGetLG(m_bIPUnits));
	m_fnDrawText(pDC, strTemp, &cTextFont, DT_SINGLELINE | DT_VCENTER, dblTblStartX, dblTblStartY, dblColWidth, dblCellHeight, iRow, iCol);
	dblCurrentY += ROW_HEIGHT;


	// Print tables of flow data 

	// Init Counts 
	int IFlowNo(NUM_FLOWS /* m_designData.m_fnGetFlowCnt() */);
	int IRangeNo(m_designData.m_fnGetRangeCnt());
	int IWBNo(m_designData.m_fnGetWBTCnt(0));
	int iFlowIndex(0);
	int iRangeIndex(0);

	if (IFlowNo < 5)
		dblCurrentY += 0.25;
	else
		dblCurrentY += 0.10;

	strTemp = "Tower Design Curve Data";
    rectText.SetRect(INCHX(LEFT_MARGIN), INCHY(dblCurrentY), 
		INCHX(RIGHT_MARGIN), INCHY(dblCurrentY+TBLNAME_HEIGHT));
	m_fnDrawText(pDC, strTemp, &cTblNameFont, &rectText, DT_SINGLELINE | DT_VCENTER);
	dblCurrentY += TBLNAME_HEIGHT;

	dblColWidth		= 1.00;
	dblCellHeight	= ROW_HEIGHT;
	dblTblStartX	= LEFT_MARGIN;

	// Init Arrays
	double RANGE[MAX_RANGES];
	double FLOW[MAX_FLOWS];
	double WBT[MAX_FLOWS][MAX_WBTS];
	double CWT[MAX_FLOWS][MAX_WBTS][MAX_RANGES];
	m_designData.m_fnGetCWTArray(CWT, m_bIPUnits);
	m_designData.m_fnGetRangeArray(RANGE, m_bIPUnits);
	m_designData.m_fnGetFlowArray(FLOW, m_bIPUnits);
	m_designData.m_fnGetWBTArray(WBT, m_bIPUnits);

	for (iFlowIndex = 0; iFlowIndex < IFlowNo; iFlowIndex++)
	{
		strTemp.Format("Water Flow Rate #%u: %.1f %s", iFlowIndex+1, m_designData.m_fnGetFlow(iFlowIndex, m_bIPUnits), (LPCTSTR)strUnitsFlowRate);
		rectText.SetRect(INCHX(LEFT_MARGIN), INCHY(dblCurrentY), INCHX(RIGHT_MARGIN), INCHY(dblCurrentY+TEXT_HEIGHT));
		m_fnDrawText(pDC, strTemp, &cTblHdrFont, &rectText, DT_SINGLELINE | DT_VCENTER);
		dblCurrentY += TEXT_HEIGHT;

		dblTblStartY	= dblCurrentY;
		iCol			= 0;
		iRow			= 0;

		strTemp = "   WBT";
		m_fnDrawText(pDC, strTemp, &cTblHdrFont, DT_SINGLELINE | DT_VCENTER, dblTblStartX, dblTblStartY, dblColWidth, dblCellHeight, iRow, iCol);

		for (iRangeIndex = 0; iRangeIndex < IRangeNo; iRangeIndex++)
		{
			iCol = iRangeIndex + 1;
			iRow = 0;
			//strTemp.Format("   R%u: %.2f %s", iRangeIndex+1, RANGE[iRangeIndex], (LPCTSTR)strUnitsTemp);
			strTemp.Format("   R%u: %.2f", iRangeIndex+1, RANGE[iRangeIndex]);
			m_fnDrawText(pDC, strTemp, &cTblHdrFont, DT_SINGLELINE | DT_VCENTER, dblTblStartX, dblTblStartY, dblColWidth, dblCellHeight, iRow, iCol);
		}
		dblCurrentY += dblCellHeight;

		//dblCellHeight = 0.25;
		for (int iWBTIndex = 0; iWBTIndex < IWBNo; iWBTIndex++)
		{
			iCol = 0;
			iRow = iWBTIndex+1;
			strTemp.Format("   %.2f %s", WBT[iFlowIndex][iWBTIndex], (LPCTSTR)strUnitsTemp);
			m_fnDrawText(pDC, strTemp, &cTextFont, DT_SINGLELINE | DT_VCENTER, dblTblStartX, dblTblStartY, dblColWidth, dblCellHeight, iRow, iCol);

			for (iRangeIndex = 0; iRangeIndex < IRangeNo; iRangeIndex++)
			{
				iCol = iRangeIndex+1;
				strTemp.Format("   %.2f %s", CWT[iFlowIndex][iWBTIndex][iRangeIndex], (LPCTSTR)strUnitsTemp);
				m_fnDrawText(pDC, strTemp, &cTextFont, DT_SINGLELINE | DT_VCENTER, dblTblStartX, dblTblStartY, dblColWidth, dblCellHeight, iRow, iCol);
			}
			dblCurrentY += dblCellHeight;
		}
		dblCurrentY += 0.15; // space before next flow table
	}

#ifdef _ENOUGH_ROOM		// This could be on the Design Report, but ....

	// Cooling Tower Design and Test Data
	double dblTestLG(0.0);
	double DesignLWBTnew(0.0);
	double DesignDenOutT(0.0);
	double DesignSVOutT(0.0);
	double DesignHLWBT(0.0); 
	double DesignAdjTestFlow = DetermineAdjTestFlow(m_bIPUnits, (int)m_designData.m_fnGetInducedFlow(), m_designData.m_fnGetHWT(m_bIPUnits), m_designData.m_fnGetCWT(m_bIPUnits), m_designData.m_fnGetWBT(m_bIPUnits), m_designData.m_fnGetDBT(m_bIPUnits), m_designData.m_fnGetBarometricPressure(m_bIPUnits), m_designData.m_fnGetWaterFlowRate(m_bIPUnits), m_designData.m_fnGetFanDriverPower(m_bIPUnits), m_designData.m_fnGetLG(m_bIPUnits), m_designData.m_fnGetHWT(m_bIPUnits), m_designData.m_fnGetCWT(m_bIPUnits), m_designData.m_fnGetWBT(m_bIPUnits), m_designData.m_fnGetDBT(m_bIPUnits), m_designData.m_fnGetBarometricPressure(m_bIPUnits), m_designData.m_fnGetWaterFlowRate(m_bIPUnits), m_designData.m_fnGetFanDriverPower(m_bIPUnits), DesignLWBTnew, DesignDenOutT, DesignSVOutT, DesignHLWBT, DesignAdjTestFlow, dblTestLG);

	if (m_designData.m_fnGetInducedFlow())
		strTemp = "Exit Air Properties";
	else
		strTemp = "Inlet Air Properties";

    rectText.SetRect(INCHX(LEFT_MARGIN), INCHY(dblCurrentY), INCHX(RIGHT_MARGIN), INCHY(dblCurrentY+0.25));
	m_fnDrawText(pDC, strTemp, &cTblNameFont, &rectText, DT_SINGLELINE | DT_VCENTER);
	dblCurrentY += 0.25;

	dblTblStartX	= LEFT_MARGIN;
	dblTblStartY	= dblCurrentY;
	dblColWidth		= 1.25;
	dblCellHeight	= .25;
	iCol			= 0;
	iRow			= 0;
/**
	iCol = 0;
	iRow = 0;
	strTemp = "     ";
	m_fnDrawText(pDC, strTemp, &cTblHdrFont, DT_SINGLELINE | DT_VCENTER, dblTblStartX, dblTblStartY, dblColWidth, dblCellHeight, iRow, iCol);
**/
	iCol = 0; // 1;
	iRow = 0;
	strTemp = "   Temp.";
	m_fnDrawText(pDC, strTemp, &cTblHdrFont, DT_SINGLELINE | DT_VCENTER, dblTblStartX, dblTblStartY, dblColWidth, dblCellHeight, iRow, iCol);

	iCol++;
	//iRow = 0;
	strTemp = "   Density";
	m_fnDrawText(pDC, strTemp, &cTblHdrFont, DT_SINGLELINE | DT_VCENTER, dblTblStartX, dblTblStartY, dblColWidth, dblCellHeight, iRow, iCol);

	iCol++;
	//iRow = 0;
	strTemp = "   Sp. Vol.";
	m_fnDrawText(pDC, strTemp, &cTblHdrFont, DT_SINGLELINE | DT_VCENTER, dblTblStartX, dblTblStartY, dblColWidth, dblCellHeight, iRow, iCol);

	iCol++;;
	//iRow = 0;
	strTemp = "   Enthalpy";
	m_fnDrawText(pDC, strTemp, &cTblHdrFont, DT_SINGLELINE | DT_VCENTER, dblTblStartX, dblTblStartY, dblColWidth, dblCellHeight, iRow, iCol);

	dblCurrentY += 0.25;
/**
	iCol = 0;
	iRow = 1;
	strTemp = "   Design";
	m_fnDrawText(pDC, strTemp, &cTblHdrFont, DT_SINGLELINE | DT_VCENTER, dblTblStartX, dblTblStartY, dblColWidth, dblCellHeight, iRow, iCol);
**/
	iCol = 0; //1;
	iRow = 1;
	strTemp.Format("   %.2f", DesignLWBTnew);
	m_fnDrawText(pDC, strTemp, &cTextFont, DT_SINGLELINE | DT_VCENTER, dblTblStartX, dblTblStartY, dblColWidth, dblCellHeight, iRow, iCol);

	iCol++; // = 2;
	//iRow = 1;
	strTemp.Format("   %.5f", DesignDenOutT);
	m_fnDrawText(pDC, strTemp, &cTextFont, DT_SINGLELINE | DT_VCENTER, dblTblStartX, dblTblStartY, dblColWidth, dblCellHeight, iRow, iCol);

	iCol++; // = 3;
	//iRow = 1;
	strTemp.Format("   %.4f", DesignSVOutT);
	m_fnDrawText(pDC, strTemp, &cTextFont, DT_SINGLELINE | DT_VCENTER, dblTblStartX, dblTblStartY, dblColWidth, dblCellHeight, iRow, iCol);

	iCol++; // = 4;
	//iRow = 1;
	strTemp.Format("   %.4f", DesignHLWBT);
	m_fnDrawText(pDC, strTemp, &cTextFont, DT_SINGLELINE | DT_VCENTER, dblTblStartX, dblTblStartY, dblColWidth, dblCellHeight, iRow, iCol);

	dblCurrentY += 0.25;
#endif // _ENOUGH_ROOM
}


/////////////////////////////////////////////////////////////////////////////
// CPrintPerfReport diagnostics

#ifdef _DEBUG
void CPrintPerfReport::AssertValid() const
{
	CView::AssertValid();
}

void CPrintPerfReport::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPrintPerfReport message handlers

BOOL CPrintPerfReport::OnPreparePrinting(CPrintInfo* pInfo) 
{
	//return CView::OnPreparePrinting(pInfo);
	if (pInfo->m_bPreview)
		pInfo->SetMaxPage(1);
	return DoPreparePrinting(pInfo);
}

void CPrintPerfReport::m_fnPrint(TPerformanceDesign* pDesignData, TPerformanceTest* pTestData, bool bIP, LPCTSTR lpszTitle, bool bPerfReport)
{
	m_strTitle		= lpszTitle;
	m_bIPUnits		= bIP;
	m_testData		= *pTestData;
	m_designData	= *pDesignData;
	m_bPerfReport	= bPerfReport;
	OnFilePrint();
}

void CPrintPerfReport::m_fnPrintPreview(TPerformanceDesign* pDesignData, TPerformanceTest* pTestData, bool bIP, LPCTSTR lpszTitle, bool bPerfReport)
{
	m_strTitle		= lpszTitle;
	m_bIPUnits		= bIP;
	m_testData		= *pTestData;
	m_designData	= *pDesignData;
	m_bPerfReport	= bPerfReport;
#ifdef _MFC_PREVIEW
	m_bOk			= TRUE;
	OnFilePrintPreview();
#else
	InvalidateRect(NULL);
#endif
}

#ifdef _MFC_PREVIEW
void CPrintPerfReport::OnEndPrintPreview(CDC* pDC, CPrintInfo* pInfo,
										 POINT point, CPreviewView * pView)
{
	m_bOk = FALSE;
	CView::OnEndPrintPreview(pDC, pInfo, point, pView);
}
#endif

void CPrintPerfReport::m_fnDrawText(CDC *pDC, LPCTSTR lpszText, CFont *pFont, CRect *pRect, UINT uiFormat)
{
	pDC->SelectObject(pFont);
	pDC->DrawText(lpszText, *pRect, uiFormat); 
}

void CPrintPerfReport::m_fnDrawText(CDC *pDC, LPCTSTR lpszText, CFont *pFont, UINT uiFormat, double dblTblStartX, double dblTblStartY, double dblColWidth, double dblCellHeight, int iRow, int iCol)
{
	CRect	rectText;
	CBrush	brushRects(COLORREF(0x000000));
	
	double y1 = (dblTblStartY + (dblCellHeight*iRow));
	double y2 = (dblTblStartY + (dblCellHeight*iRow) + dblCellHeight);
	double x1 = (dblTblStartX + (iCol*dblColWidth));
	double x2 = (dblTblStartX + (iCol*dblColWidth) + dblColWidth);
    rectText.SetRect(INCHX(x1), INCHY(y1), INCHX(x2), INCHY(y2));
	m_fnDrawText(pDC, lpszText, pFont, &rectText, uiFormat);
	pDC->FrameRect(rectText, &brushRects);
}
