// PrintView.cpp : implementation file
//

#include "stdafx.h"
#include "CTIToolkit.h"
#include "PrintView.h"
#include "series.h"
#include "valuelist.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int NumColors ( CDC *pDC )
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


BOOL GetBitmapAndPalette(UINT nIDResource, CBitmap &bitmap, CPalette &pal)
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
		pLP->palNumEntries = WORD(nColors);
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
// CPrintView

IMPLEMENT_DYNCREATE(CPrintView, CView)

CPrintView::CPrintView()
{
}

CPrintView::~CPrintView()
{
}


BEGIN_MESSAGE_MAP(CPrintView, CView)
	//{{AFX_MSG_MAP(CPrintView)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPrintView drawing

void CPrintView::OnDraw(CDC* pDC)
{
    // handle logo

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

	double	iTop	= vpixels * 2.17;
	double	iLeft	= hpixels * .93;
	double	iBottom	= vpixels * 9.75;
	double	iRight	= hpixels * 7.40;

	CPen	cMajorGridPen(PS_SOLID, 2, COLORREF(0x00cc00));
	CPen	cMinorGridPen(PS_SOLID, 1, COLORREF(0x00cc00));
	CPen	cLabelPen(PS_SOLID, 4, COLORREF(0x00cc00));
	CPen	cDataPen(PS_SOLID,   iDataLineWidth, COLORREF(0x000000));
	CPen	cTargetDataPen(PS_SOLID, iUserLineWidth, COLORREF(0x0022cc));
	CFont	cTickFont;
	CFont	cApproachFont;
	LOGFONT	stTickLogFont;
	CString	strTemp;
	CFont	cDataFont;
	LOGFONT	stDataLogFont;
	CFont	cTitleFont;
	LOGFONT	stTitleLogFont;

	pDC->SetTextColor(COLORREF(0x00cc00));

	// Title
	stTitleLogFont.lfHeight			= (LONG)(vpixels*.25);
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

    // title
	CRect rectTitleText(int(hpixels*1), 
                        int(vpixels/4), 
                        int(hpixels*7),
                        int(vpixels/4 + vpixels*.25));
	strTemp = "Cooling Technology Institute";
	pDC->DrawText(strTemp, rectTitleText, DT_CENTER | DT_VCENTER);

    // subtitle
	CRect rectTitleDesc(int(hpixels*1), 
                        int(vpixels/2), 
                        int(hpixels*7),
                        int(vpixels/2 + vpixels*.25));
	strTemp = m_csDesc;
	pDC->DrawText(strTemp, rectTitleDesc, DT_CENTER | DT_VCENTER);    

	// Title
	stDataLogFont.lfHeight			= (LONG)(vpixels*.25);
	stDataLogFont.lfWidth			= 0;
	stDataLogFont.lfEscapement		= 0;
	stDataLogFont.lfOrientation		= 0;
	stDataLogFont.lfWeight			= FW_NORMAL;
	stDataLogFont.lfItalic			= FALSE;
	stDataLogFont.lfUnderline		= FALSE;
	stDataLogFont.lfStrikeOut		= FALSE;
	stDataLogFont.lfCharSet			= DEFAULT_CHARSET;
	stDataLogFont.lfOutPrecision	= OUT_DEFAULT_PRECIS;
	stDataLogFont.lfClipPrecision	= CLIP_DEFAULT_PRECIS;
	stDataLogFont.lfQuality			= PROOF_QUALITY;
	stDataLogFont.lfPitchAndFamily	= FF_DONTCARE;
	strcpy(stDataLogFont.lfFaceName, "Times New Roman");
	cDataFont.CreateFontIndirect(&stDataLogFont);
	pDC->SelectObject(&cDataFont);

    int height = stDataLogFont.lfHeight;
	CRect rectData1Text((int)(hpixels*5.5), 
                        (int)(vpixels-height), 
                        (int)(hpixels*7.5),
                        (int)(vpixels)); 
    if (!TPropPageBase::metricflag)
	    strTemp = "WET BULB (°F)";
    else
        strTemp = "WET BULB (°C)";
	pDC->DrawText(strTemp, rectData1Text, DT_RIGHT | DT_VCENTER);

	CRect rectData2Text( (int)(hpixels*5.5), 
                         (int)(vpixels),
                         (int)(hpixels*7.5),
                         (int)(vpixels+height) );
    if (!TPropPageBase::metricflag)
	    strTemp = "RANGE (°F)";
    else
        strTemp = "RANGE (K)";
	pDC->DrawText(strTemp, rectData2Text, DT_RIGHT | DT_VCENTER);

	CRect rectData3Text((int)(hpixels*5.5), 
                        (int)(vpixels+height),
                        (int)(hpixels*7.5),
                        (int)(vpixels+height*2)); 
    if (!TPropPageBase::metricflag)
        strTemp = "ALTITUDE (ft)";
    else
        strTemp = "ALTITUDE (m)";
	pDC->DrawText(strTemp, rectData3Text, DT_RIGHT | DT_VCENTER);

    CRect rectData3aText((int)(hpixels*5.5), 
        (int)(vpixels+height*2),
        (int)(hpixels*7.5),
        (int)(vpixels+height*3)); 
    if (!TPropPageBase::metricflag)
	    strTemp = "PRESSURE (\"Hg)";
    else
	    strTemp = "PRESSURE (kPa)";
	pDC->DrawText(strTemp, rectData3aText, DT_RIGHT | DT_VCENTER);

	if (m_dblCurveC1 && m_dblCurveC2)
    {
		CRect rectData6bText(int(hpixels*1), 
			                int(iTop - vpixels/4), 
				            int(hpixels*7),
					        int(iTop));
	    strTemp.Format("KaV/L = %.4f * (L/G) ^ %.4f", m_dblCurveC1, m_dblCurveC2);
        pDC->DrawText(strTemp, rectData6bText, DT_CENTER | DT_VCENTER);
    }

	pDC->SetTextColor(COLORREF(0x000000));

	CRect rectData4Text( int(hpixels*4), 
                         int(vpixels)-height, 
                         int(hpixels*5.5),
                         int(vpixels) );
    strTemp.Format("%.2f", m_dblWBT);
	pDC->DrawText(strTemp, rectData4Text, DT_RIGHT | DT_VCENTER);

	CRect rectData5Text( int(hpixels*4), 
                         int(vpixels),
                         int(hpixels*5.5),
                         int(vpixels)+height );
	strTemp.Format("%.2f", m_dblCurveRange);
	pDC->DrawText(strTemp, rectData5Text, DT_RIGHT | DT_VCENTER);

	CRect rectData6Text( int(hpixels*4), 
                         int(vpixels)+height,
                         int(hpixels*5.5),
                         int(vpixels)+height*2); 
	strTemp.Format("%.2f", m_dblAltitude);
	pDC->DrawText(strTemp, rectData6Text, DT_RIGHT | DT_VCENTER);

    CRect rectData7Text((int)(hpixels*4), 
        (int)(vpixels+height*2),
        (int)(hpixels*5.5),
        (int)(vpixels+height*3)); 
    if (!TPropPageBase::metricflag)
		strTemp.Format("%.3f", calcPressureF(Altitude2PSI(m_dblAltitude)));
	else
		strTemp.Format("%.3f", Altitude2KPA(m_dblAltitude));
	pDC->DrawText(strTemp, rectData7Text, DT_RIGHT | DT_VCENTER);


	// Major grid lines
	int		iIndex;
	int		iIndex2;
	double dTemp;
	double aYAxisList[] = {.1, .15, .2, .25, .3, .4, .5, .6, .7, .8, .9, 1.0, 1.5, 2.0, 2.5, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10};
	double aXAxisList[] = {.1, .15, .2, .25, .3, .4, .5, .6, .7, .8, .9, 1.0, 1.5, 2.0, 2.5, 3.0, 4.0, 5.0};
	double dXFactor = (iRight - iLeft) / log10(50.0);
	double dYFactor = (iBottom - iTop) / log10(100.0);

	pDC->SetTextColor(COLORREF(0x00cc00));
	pDC->SelectObject(&cMajorGridPen);
	pDC->MoveTo((int)iLeft, (int)iTop);
 	pDC->LineTo((int)iRight, (int)iTop);
 	pDC->LineTo((int)iRight, (int)iBottom);
 	pDC->LineTo((int)iLeft, (int)iBottom);
 	pDC->LineTo((int)iLeft, (int)iTop);


	stTickLogFont.lfHeight			= long(vpixels*.1);
	stTickLogFont.lfWidth			= 0;
	stTickLogFont.lfEscapement		= 0;
	stTickLogFont.lfOrientation		= 0;
	stTickLogFont.lfWeight			= FW_NORMAL;
	stTickLogFont.lfItalic			= FALSE;
	stTickLogFont.lfUnderline		= FALSE;
	stTickLogFont.lfStrikeOut		= FALSE;
	stTickLogFont.lfCharSet			= DEFAULT_CHARSET;
	stTickLogFont.lfOutPrecision	= OUT_DEFAULT_PRECIS;
	stTickLogFont.lfClipPrecision	= CLIP_DEFAULT_PRECIS;
	stTickLogFont.lfQuality			= PROOF_QUALITY;
	stTickLogFont.lfPitchAndFamily	= FF_DONTCARE;
	strcpy(stTickLogFont.lfFaceName, "Times New Roman");
	cTickFont.CreateFontIndirect(&stTickLogFont);
	stTickLogFont.lfWeight			= FW_BOLD;
	cApproachFont.CreateFontIndirect(&stTickLogFont);
	pDC->SelectObject(&cTickFont);

	pDC->SelectObject(&cMinorGridPen);
	// Print grid lines
	double aYGridList[] = {0.1,0.105,0.11,0.115,0.12,0.125,0.13,0.135,0.14,0.145,0.15,0.155,0.16,0.165,0.17,0.175,0.18,0.185,0.19,0.195,0.2,0.205,0.21,0.215,0.22,0.225,0.23,0.235,0.24,0.245,0.25,0.255,0.26,0.265,0.27,0.275,0.28,0.285,0.29,0.295,0.3,0.31,0.32,0.33,0.34,0.35,0.36,0.37,0.38,0.39,0.4,0.41,0.42,0.43,0.44,0.45,0.46,0.47,0.48,0.49,0.5,0.51,0.52,0.53,0.54,0.55,0.56,0.57,0.58,0.59,0.6,0.62,0.64,0.66,0.68,0.7,0.72,0.74,0.76,0.78,0.8,0.82,0.84,0.86,0.88,0.9,0.92,0.94,0.96,0.98,1,1.05,1.1,1.15,1.2,1.25,1.3,1.35,1.4,1.45,1.5,1.55,1.6,1.65,1.7,1.75,1.8,1.85,1.9,1.95,2,2.05,2.1,2.15,2.2,2.25,2.3,2.35,2.4,2.45,2.5,2.55,2.6,2.65,2.7,2.75,2.8,2.85,2.9,2.95,3,3.1,3.2,3.3,3.4,3.5,3.6,3.7,3.8,3.9,4,4.1,4.2,4.3,4.4,4.5,4.6,4.7,4.8,4.9,5,5.1,5.2,5.3,5.4,5.5,5.6,5.7,5.8,5.9,6,6.2,6.4,6.6,6.8,7,7.2,7.4,7.6,7.8,8,8.2,8.4,8.6,8.8,9,9.2,9.4,9.6,9.8,10};
	double aXGridList[] = {0.1,0.105,0.11,0.115,0.12,0.125,0.13,0.135,0.14,0.145,0.15,0.155,0.16,0.165,0.17,0.175,0.18,0.185,0.19,0.195,0.2,0.205,0.21,0.215,0.22,0.225,0.23,0.235,0.24,0.245,0.25,0.255,0.26,0.265,0.27,0.275,0.28,0.285,0.29,0.295,0.3,0.31,0.32,0.33,0.34,0.35,0.36,0.37,0.38,0.39,0.4,0.41,0.42,0.43,0.44,0.45,0.46,0.47,0.48,0.49,0.5,0.51,0.52,0.53,0.54,0.55,0.56,0.57,0.58,0.59,0.6,0.62,0.64,0.66,0.68,0.7,0.72,0.74,0.76,0.78,0.8,0.82,0.84,0.86,0.88,0.9,0.92,0.94,0.96,0.98,1,1.05,1.1,1.15,1.2,1.25,1.3,1.35,1.4,1.45,1.5,1.55,1.6,1.65,1.7,1.75,1.8,1.85,1.9,1.95,2,2.05,2.1,2.15,2.2,2.25,2.3,2.35,2.4,2.45,2.5,2.55,2.6,2.65,2.7,2.75,2.8,2.85,2.9,2.95,3,3.1,3.2,3.3,3.4,3.5,3.6,3.7,3.8,3.9,4,4.1,4.2,4.3,4.4,4.5,4.6,4.7,4.8,4.9,5};
	for (iIndex = 0; iIndex < sizeof(aYGridList)/sizeof(double); iIndex++)
	{
		dTemp = log10(aYGridList[iIndex] * 10.0);
		dTemp = dTemp * dYFactor;
		dTemp = iBottom - dTemp;
		pDC->MoveTo((int)iLeft, (int)dTemp);
 		pDC->LineTo((int)iRight, (int)dTemp);
	}
	for (iIndex = 0; iIndex < sizeof(aXGridList)/sizeof(double); iIndex++)
	{
		dTemp = log10(aXGridList[iIndex] * 10.0);
		dTemp = dTemp * dXFactor;
		dTemp = iLeft + dTemp;
		pDC->MoveTo((int)dTemp, (int)iTop);
 		pDC->LineTo((int)dTemp, (int)iBottom);
	}

	pDC->SelectObject(&cMajorGridPen);
	for (iIndex = 0; iIndex < sizeof(aYAxisList)/sizeof(double); iIndex++)
	{
		dTemp = log10(aYAxisList[iIndex] * 10.0);
		dTemp = dTemp * dYFactor;
		dTemp = iBottom - dTemp;
		pDC->MoveTo((int)iLeft-4, (int)dTemp);
 		pDC->LineTo((int)iRight, (int)dTemp);
		strTemp.Format("%.2f", aYAxisList[iIndex]);
		if (strTemp[0] == '0')
		{
			strTemp = strTemp.Mid(1);
		}
		while (strTemp[strTemp.GetLength()-1] == '0')
		{
			strTemp = strTemp.Left(strTemp.GetLength()-1);
		}
		if (strTemp[strTemp.GetLength()-1] == '.')
		{
			strTemp = strTemp.Left(strTemp.GetLength()-1);
		}
		CRect rectText(int(iLeft-hpixels*.2) - 10, int(dTemp-(vpixels*.05)), 
			int(iLeft - 10), int(dTemp+(vpixels*.05)));
		pDC->DrawText(strTemp, rectText, DT_RIGHT | DT_VCENTER);
	}
	for (iIndex = 0; iIndex < sizeof(aXAxisList)/sizeof(double); iIndex++)
	{
		dTemp = log10(aXAxisList[iIndex] * 10.0);
		dTemp = dTemp * dXFactor;
		dTemp = iLeft + dTemp;
		pDC->MoveTo((int)dTemp, (int)iTop);
 		pDC->LineTo((int)dTemp, (int)iBottom+4);
		strTemp.Format("%.2f", aYAxisList[iIndex]);
		if (strTemp[0] == '0')
		{
			strTemp = strTemp.Mid(1);
		}
		while (strTemp[strTemp.GetLength()-1] == '0')
		{
			strTemp = strTemp.Left(strTemp.GetLength()-1);
		}
		if (strTemp[strTemp.GetLength()-1] == '.')
		{
			strTemp = strTemp.Left(strTemp.GetLength()-1);
		}
		CRect rectText(int(dTemp-(vpixels*.1)), int(iBottom+10), 
			int(dTemp+(vpixels*.1)), int(iBottom+(vpixels*.1)+10));
		pDC->DrawText(strTemp, rectText, DT_CENTER | DT_TOP);
	}


	// Label
    int incAmount = stTitleLogFont.lfHeight;
    pDC->SelectObject(&cTitleFont);

    // left label
    CRect rectLeftLabel(int(hpixels*.15), 
                        int(iTop + (iBottom - iTop)/2 - (incAmount*.1+incAmount)), 
                        int(iLeft - hpixels*.30), 
                        int(iTop + (iBottom - iTop)/2 - incAmount*.1));
    strTemp = "KaV";
	pDC->DrawText(strTemp, rectLeftLabel, DT_CENTER | DT_VCENTER);
    rectLeftLabel.SetRect(int(hpixels*.15),
                          int(iTop + (iBottom - iTop)/2 + incAmount*.1),
                          int(iLeft - vpixels*.30), 
                          int(iTop + (iBottom - iTop)/2 + (incAmount*.1+incAmount)));
    strTemp = "L";
    pDC->DrawText(strTemp, rectLeftLabel, DT_CENTER | DT_VCENTER);

    // right label
    CRect rectRightLabel(int(iRight + (hpixels*.20 / 2)),
                        int(iTop + (iBottom - iTop)/2 - (incAmount*.1+incAmount)),
                        int(iRight + (hpixels*.25) + (hpixels*.42)),
                        int(iTop + (iBottom - iTop)/2 - incAmount*.1));
    strTemp = "KaV";
	pDC->DrawText(strTemp, rectRightLabel, DT_CENTER | DT_VCENTER);
    rectRightLabel.SetRect(int(iRight + (hpixels*.20 / 2)), 
                          int(iTop + (iBottom - iTop)/2 + incAmount*.1),
                          int(iRight + (hpixels*.25) + (hpixels*.42)), 
                          int(iTop + (iBottom - iTop)/2 + (incAmount*.1+incAmount)));
    strTemp = "L";
    pDC->DrawText(strTemp, rectRightLabel, DT_CENTER | DT_VCENTER);

    CRect rectBottomLabel(int((iRight - iLeft)/2 + iLeft - (hpixels*.1)), 
                        int(iBottom + stTickLogFont.lfHeight*2), 
                        int((iRight - iLeft)/2 + iLeft + (hpixels*.4)), 
                        int(iBottom + stTickLogFont.lfHeight*2 + incAmount));

    strTemp = "L/G";
	pDC->DrawText(strTemp, rectBottomLabel, DT_CENTER | DT_VCENTER);

    // draw line between letters
	pDC->SelectObject(&cLabelPen);
	pDC->MoveTo(rectLeftLabel.left, 
               (rectLeftLabel.top)-int(incAmount*.1));
	pDC->LineTo(rectLeftLabel.right, 
               (rectLeftLabel.top)-int(incAmount*.1));
	pDC->MoveTo(rectRightLabel.left, 
               (rectRightLabel.top)-int(incAmount*.1));
	pDC->LineTo(rectRightLabel.right, 
               (rectRightLabel.top)-int(incAmount*.1));

	//Draw Data Lines
    pDC->SelectObject(&cDataPen);
	pDC->SetTextColor(COLORREF(0x000000));


	//---------------------------------------------------------------------
	// Draw the curves
	//---------------------------------------------------------------------
	for (iIndex = 0; iIndex < m_pwndGraph->GetSeriesCount(); iIndex++)
	{
		CSeries cSeries;
		CValueList cXVals;
		CValueList cYVals;

		cSeries = m_pwndGraph->GetSeries(iIndex);
		cXVals = cSeries.GetXValues();
		cYVals = cSeries.GetYValues();

		for (iIndex2=0; iIndex2 < cXVals.GetCount(); iIndex2++)
		{
            int n = cXVals.GetCount();
			double x = cXVals.GetValue(iIndex2);
			double y = cYVals.GetValue(iIndex2);

			y = log10(y * 10.0);
			y = y * dYFactor;
			y = iBottom - y;

			x = log10(x * 10.0);
			x = x * dXFactor;
			x = iLeft + x;

			if (iIndex2 == 0)                 
			{
				//---------------------------------------------------------------------
				// Print Lable
				//---------------------------------------------------------------------
				strTemp = m_pwndGraph->GetSeries(iIndex).GetTitle();
                if (atof(strTemp) > 0.00001)  // If non text label - approach curve
                {
					//Select Pen and font for the job
					pDC->SelectObject(&cDataPen);
					pDC->SetTextColor(COLORREF(0x000000));
					pDC->SelectObject(&cTickFont);

					pDC->MoveTo(int(x+100),int(y));
					CRect rText(int(x + (hpixels * .02)),
								int(y - (vpixels * .1)),
								int(x + (hpixels * .31)),
								int(y));
					pDC->SetBkMode(OPAQUE);
					pDC->DrawText(strTemp, rText, DT_RIGHT | DT_BOTTOM);

					if ((iIndex == m_iTargetIndex) || (iIndex == m_iUserIndex))
					{
						pDC->SelectObject(&cTargetDataPen);
					}
					else
					{
						pDC->SelectObject(&cDataPen);
					}
                }
				else
				{
					//Select Pen for the job
					pDC->SelectObject(&cTargetDataPen);
				}
                // move pen
                pDC->MoveTo(int(x),int(y));
			}
			else
			{
				pDC->LineTo(int(x),int(y));
			}
		}
	}
	//---------------------------------------------------------------------
	// Draw the circle at the L/G and KaV/L intersection
	//---------------------------------------------------------------------
	if ((m_dblKaVL > 0.0) && (m_dblLG > 0.0))
	{
		CPen	cTargetPoint(PS_SOLID, 4, COLORREF(0x0000ff));
		double	x = m_dblLG;
		double	y = m_dblKaVL;

		y = log10(y * 10.0);
		y = y * dYFactor;
		y = iBottom - y;

		x = log10(x * 10.0);
		x = x * dXFactor;
		x = iLeft + x;

		//CRect rectCircle(x - (hpixels*.1), y - (vpixels*.1), (hpixels*.2), (vpixels*.2));
		CRect rectCircle(int(x - 2), int(y - 2), int(x + 2), int(y + 2));
		pDC->SelectObject(&cTargetPoint);
		pDC->Ellipse(rectCircle);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CPrintView diagnostics

#ifdef _DEBUG
void CPrintView::AssertValid() const
{
	CView::AssertValid();
}

void CPrintView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPrintView message handlers

void CPrintView::m_fnPrint(CString csDescription, double dblCurveRange, double dblCurveWBT, double dblAltitude, double dblC1, double dblC2, double dblKavL, double dblLG, int iTargetIndex, int iUserIndex, CTChart *pGraph )
{
	m_dblWBT		= dblCurveWBT;
	m_dblCurveRange = dblCurveRange;
    m_dblAltitude	= dblAltitude;
	m_dblCurveC1	= dblC1;
	m_dblCurveC2	= dblC2;
	m_dblKaVL		= dblKavL;
	m_dblLG			= dblLG;
	m_iTargetIndex	= iTargetIndex;
	m_iUserIndex	= iUserIndex;
	m_pwndGraph = pGraph;
    m_csDesc = csDescription;
	OnFilePrint();
}

void CPrintView::m_fnPrintPreview(CString csDescription, double dblCurveRange, double dblCurveWBT, double dblAltitude, double dblC1, double dblC2, double dblKavL, double dblLG, int iTargetIndex, int iUserIndex, CTChart *pGraph)
{
	m_dblWBT		= dblCurveWBT;
	m_dblCurveRange = dblCurveRange;
    m_dblAltitude	= dblAltitude;
	m_dblCurveC1	= dblC1;
	m_dblCurveC2	= dblC2;
	m_dblKaVL		= dblKavL;
	m_dblLG			= dblLG;
	m_iTargetIndex	= iTargetIndex;
	m_iUserIndex	= iUserIndex;
	m_pwndGraph = pGraph;
    m_csDesc = csDescription;
	InvalidateRect(NULL);
}

BOOL CPrintView::OnPreparePrinting(CPrintInfo* pInfo) 
{
	return DoPreparePrinting(pInfo);
}

void CPrintView::OnTimer(UINT nIDEvent) 
{
	KillTimer(nIDEvent);

	OnFilePrint();
	
	CView::OnTimer(nIDEvent);
}

