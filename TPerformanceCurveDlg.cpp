// TPerformanceCurveDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CTIToolkit.h"
#include "TPerformanceCurveDlg.h"
#include "TeeChartDefines.h"
#include "aspect.h"
#include "axes.h"
#include "axis.h"
#include "axislabels.h"
#include "canvas.h"
#include "series.h"
#include "titles.h"
#include "pen.h"
#include "panel.h"
#include "axistitle.h"
#include "axislabels.h"
#include "chartfont.h"
#include "strings.h"
#include "printer.h"
#include "legend.h"
#include "walls.h"
#include "teefunction.h"
#include "gradient.h"
#include "export.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// TPerformanceCurveDlg dialog


TPerformanceCurveDlg::TPerformanceCurveDlg(CWnd* pParent /*=NULL*/)
	: cdxCSizingDialog(TPerformanceCurveDlg::IDD, pParent), 
	m_bIPUnits(true), m_bReversedGraph(false)
{
	//{{AFX_DATA_INIT(TPerformanceCurveDlg)
	m_bTestPoint = false;
	//}}AFX_DATA_INIT
}


void TPerformanceCurveDlg::DoDataExchange(CDataExchange* pDX)
{
	cdxCSizingDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(TPerformanceCurveDlg)
	DDX_Control(pDX, IDC_TAB_CURVES, m_wndCurveTabs);
	DDX_Control(pDX, IDC_TCHART_GRAPH, m_wndGraph);
	DDX_Text(pDX, IDC_STATIC_LEGEND, m_strLegend);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(TPerformanceCurveDlg, cdxCSizingDialog)
	//{{AFX_MSG_MAP(TPerformanceCurveDlg)
	ON_NOTIFY(NM_CLICK, IDC_TAB_CURVES, OnClickTabCurves)
	ON_BN_CLICKED(IDC_BUTTON_COPY_GRAPH, OnButtonCopyGraph)
	ON_BN_CLICKED(IDC_BUTTON_REVERSE, OnButtonReverse)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// TPerformanceCurveDlg message handlers

BOOL TPerformanceCurveDlg::OnInitDialog() 
{
	cdxCSizingDialog::OnInitDialog();

	// Set graph to resizable
	AddSzControl( m_wndGraph,  cdxCSizingDialog::mdResize,cdxCSizingDialog::mdResize);

	// Move legend box (unused?) and buttons (to bottom) when resized
	CWnd * wndControl = static_cast<CWnd *>(GetDlgItem(IDC_STATIC_LEGEND));
	AddSzControl(*wndControl, cdxCSizingDialog::mdResize, cdxCSizingDialog::mdRepos);

	wndControl = static_cast<CWnd *>(GetDlgItem(IDOK));
	AddSzControl(*wndControl, cdxCSizingDialog::mdRelative, cdxCSizingDialog::mdRepos);
	wndControl = static_cast<CWnd *>(GetDlgItem(IDC_BUTTON_REVERSE));
	AddSzControl(*wndControl, cdxCSizingDialog::mdRelative, cdxCSizingDialog::mdRepos);
	wndControl = static_cast<CWnd *>(GetDlgItem(IDC_BUTTON_COPY_GRAPH));
	AddSzControl(*wndControl, cdxCSizingDialog::mdRelative, cdxCSizingDialog::mdRepos);

	int iFlowIndex;

	for (iFlowIndex = 0; iFlowIndex < m_designData.m_fnGetFlowCnt(); iFlowIndex++)
	{
		CString strFlow;
		strFlow.Format("Flow = %.02f", m_designData.m_fnGetFlow(iFlowIndex, m_bIPUnits));
		m_wndCurveTabs.InsertItem(iFlowIndex, strFlow);
	}

	if (m_bTestPoint)
	{
		m_wndCurveTabs.InsertItem(iFlowIndex++, "Crossplot 1");
		m_wndCurveTabs.InsertItem(iFlowIndex++, "Crossplot 2");
	}

	m_wndCurveTabs.SetCurSel(0);
	m_fnDrawGraph();

	// The graph can make the dialog very large - reset to (original) size
	ResetWindow();

	return TRUE;
}

void TPerformanceCurveDlg::ResetWindow()
{
	// Reset the sizing dialog to minimum size
	ResetWindowSize();
}

void TPerformanceCurveDlg::OnClickTabCurves(NMHDR* /*pNMHDR*/, LRESULT* pResult) 
{
	m_fnDrawGraph();
	*pResult = 0;
}

void TPerformanceCurveDlg::m_fnDrawGraph()
{
	int iSelTab = m_wndCurveTabs.GetCurSel();

	if (iSelTab < m_designData.m_fnGetFlowCnt())
	{
		//
		// Flow selected
		//
		m_fnDrawFlow(iSelTab, m_bReversedGraph);
	}
	else if (iSelTab == m_designData.m_fnGetFlowCnt())
	{
		//
		// Crossplot 1 Selected
		//
		m_fnDrawCrossPlot1(m_bReversedGraph);
	}
	else
	{
		//
		// Crossplot 2 Selected
		//
		m_fnDrawCrossPlot2(m_bReversedGraph);
	}
}

void TPerformanceCurveDlg::m_fnDrawFlow(int iFlowIndex, bool bWhiteBackground)
{
	CString			strTitle;
	int				iRangeIndex;

	// Default to light on dark background
	unsigned long BACKCOLOR  (0x000000);
	unsigned long GRADCOLOR  (0x000000);
	unsigned long PANELCOLOR (0xFFFFFF);
	unsigned long TICKCOLOR  (0xCCCCCC);
	unsigned long GRIDCOLOR  (0x777777);
	unsigned long AXISCOLOR  (0xE4E4E4);
	unsigned long TITLECOLOR (0xFFFFFF);
	unsigned long LABELCOLOR (0xFFFFFF);
	unsigned long HEADCOLOR  (0xFFFFFF);
	unsigned long DATACOLOR  (0xFFFF);	// yellow

	if (bWhiteBackground)	// dark on light background graph
	{
		BACKCOLOR  = 0xFFFFFF;
		GRADCOLOR  = 0xFFFFFF;
		PANELCOLOR = 0x000000;
		TICKCOLOR  = 0x222222;
		GRIDCOLOR  = 0x777777;
		AXISCOLOR  = 0x666666;
		TITLECOLOR = 0x000000;
		LABELCOLOR = 0x000000;
		HEADCOLOR  = 0x000000;
		DATACOLOR  = 0xFF0000;	// blue
	}

	//---------------------------------------------------------------------
	// Clean up graph
	//---------------------------------------------------------------------
	m_wndGraph.RemoveAllSeries();

	//---------------------------------------------------------------------
	// Setup the graph appearance
	//---------------------------------------------------------------------
	m_wndGraph.GetWalls().SetBackColor(BACKCOLOR);  // black
	m_wndGraph.GetPanel().GetGradient().SetStartColor(GRADCOLOR);
	m_wndGraph.GetPanel().GetGradient().SetEndColor(GRADCOLOR);
	m_wndGraph.GetPanel().GetGradient().SetVisible(true);
	m_wndGraph.GetAspect().SetView3D(false);
	m_wndGraph.SetDragMode(dmManual);

	if (m_wndGraph.GetHeader().GetText().Count() > 0)
	{
		m_wndGraph.GetHeader().GetText().Remove(0);
	}
	m_wndGraph.GetPanel().SetColor(PANELCOLOR);

	m_wndGraph.GetLegend().SetVisible(false);

	m_wndGraph.GetAxis().GetLeft().SetMinMax	(m_designData.m_fnGetCWT(iFlowIndex,0,0,m_bIPUnits)	, m_designData.m_fnGetCWT(iFlowIndex,m_designData.m_fnGetWBTCnt(iFlowIndex)-1,m_designData.m_fnGetRangeCnt()-1,m_bIPUnits));
	m_wndGraph.GetAxis().GetRight().SetMinMax	(m_designData.m_fnGetCWT(iFlowIndex,0,0,m_bIPUnits)	, m_designData.m_fnGetCWT(iFlowIndex,m_designData.m_fnGetWBTCnt(iFlowIndex)-1,m_designData.m_fnGetRangeCnt()-1,m_bIPUnits));
	m_wndGraph.GetAxis().GetTop().SetMinMax		(m_designData.m_fnGetWBT(iFlowIndex,0,m_bIPUnits  )	, m_designData.m_fnGetWBT(iFlowIndex,m_designData.m_fnGetWBTCnt(iFlowIndex)-1,m_bIPUnits));
	m_wndGraph.GetAxis().GetBottom().SetMinMax	(m_designData.m_fnGetWBT(iFlowIndex,0,m_bIPUnits  )	, m_designData.m_fnGetWBT(iFlowIndex,m_designData.m_fnGetWBTCnt(iFlowIndex)-1,m_bIPUnits));

	m_wndGraph.GetAxis().GetLeft().SetLogarithmic(false);
	m_wndGraph.GetAxis().GetRight().SetLogarithmic(false);
	m_wndGraph.GetAxis().GetTop().SetLogarithmic(false);
	m_wndGraph.GetAxis().GetBottom().SetLogarithmic(false);

	// tick color
	m_wndGraph.GetAxis().GetLeft().GetMinorTicks().SetColor(TICKCOLOR);
	m_wndGraph.GetAxis().GetRight().GetMinorTicks().SetColor(TICKCOLOR);
	m_wndGraph.GetAxis().GetTop().GetMinorTicks().SetColor(TICKCOLOR);
	m_wndGraph.GetAxis().GetBottom().GetMinorTicks().SetColor(TICKCOLOR);

	// grid color
	m_wndGraph.GetAxis().GetLeft().GetGridPen().SetColor(GRIDCOLOR);
	m_wndGraph.GetAxis().GetRight().GetGridPen().SetColor(GRIDCOLOR);
	m_wndGraph.GetAxis().GetTop().GetGridPen().SetColor(GRIDCOLOR);
	m_wndGraph.GetAxis().GetBottom().GetGridPen().SetColor(GRIDCOLOR);

	// Axis color
	m_wndGraph.GetAxis().GetLeft().GetAxisPen().SetColor(AXISCOLOR);
	m_wndGraph.GetAxis().GetRight().GetAxisPen().SetColor(AXISCOLOR);
	m_wndGraph.GetAxis().GetTop().GetAxisPen().SetColor(AXISCOLOR);
	m_wndGraph.GetAxis().GetBottom().GetAxisPen().SetColor(AXISCOLOR);

	// grid text
	m_wndGraph.GetAxis().GetLeft().GetTitle().GetFont().SetColor(TITLECOLOR);
	m_wndGraph.GetAxis().GetBottom().GetTitle().GetFont().SetColor(TITLECOLOR);

	// axis labels
	m_wndGraph.GetAxis().GetLeft().GetLabels().GetFont().SetColor(LABELCOLOR);
	m_wndGraph.GetAxis().GetBottom().GetLabels().GetFont().SetColor(LABELCOLOR);

	// set major axis to solid lines - psSolid
	m_wndGraph.GetAxis().GetLeft().GetGridPen().SetStyle(  psSolid );
	m_wndGraph.GetAxis().GetRight().GetGridPen().SetStyle( psSolid );
	m_wndGraph.GetAxis().GetTop().GetGridPen().SetStyle(   psSolid );
	m_wndGraph.GetAxis().GetBottom().GetGridPen().SetStyle(psSolid );

	m_wndGraph.GetAxis().GetLeft().GetGridPen().SetWidth(  1 );
	m_wndGraph.GetAxis().GetRight().GetGridPen().SetWidth( 1 );
	m_wndGraph.GetAxis().GetTop().GetGridPen().SetWidth(   1 );
	m_wndGraph.GetAxis().GetBottom().GetGridPen().SetWidth(1 );

	m_wndGraph.GetAxis().GetTop().GetLabels().SetAngle(90);
	m_wndGraph.GetAxis().GetBottom().GetLabels().SetAngle(90);

	m_wndGraph.GetAxis().GetLeft().GetLabels().GetFont().SetSize(10);
	m_wndGraph.GetAxis().GetRight().GetLabels().GetFont().SetSize(10);
	m_wndGraph.GetAxis().GetTop().GetLabels().GetFont().SetSize(10);
	m_wndGraph.GetAxis().GetBottom().GetLabels().GetFont().SetSize(10);

	m_wndGraph.GetAxis().GetLeft().SetAutomaticMinimum(true);
	m_wndGraph.GetAxis().GetRight().SetAutomaticMinimum(true);
	m_wndGraph.GetAxis().GetTop().SetAutomaticMinimum(true);
	m_wndGraph.GetAxis().GetBottom().SetAutomaticMinimum(true);

	m_wndGraph.GetAxis().GetLeft().SetAutomaticMaximum(true);
	m_wndGraph.GetAxis().GetRight().SetAutomaticMaximum(true);
	m_wndGraph.GetAxis().GetTop().SetAutomaticMaximum(true);
	m_wndGraph.GetAxis().GetBottom().SetAutomaticMaximum(true);

	m_wndGraph.GetAxis().GetLeft().GetTitle().SetCaption("Cold Water Temp");
	m_wndGraph.GetAxis().GetBottom().GetTitle().SetCaption("Wet Bulb Temp");

	m_wndGraph.GetAxis().GetLeft().GetTitle().GetFont().SetSize(10);
	m_wndGraph.GetAxis().GetBottom().GetTitle().GetFont().SetSize(10);
	m_wndGraph.GetAxis().GetLeft().GetTitle().GetFont().SetBold(true);
	m_wndGraph.GetAxis().GetBottom().GetTitle().GetFont().SetBold(true);


	// Add a descriptive title
	if (m_bIPUnits)
		strTitle.Format("Flow = %.0f gpm", m_designData.m_fnGetFlow(iFlowIndex, m_bIPUnits));
	else
		strTitle.Format("Flow = %.0f l/s", m_designData.m_fnGetFlow(iFlowIndex, m_bIPUnits));
	COleVariant var1(strTitle);
	m_wndGraph.GetHeader().GetText().Add(*(LPCVARIANT)var1);
	m_wndGraph.GetHeader().GetFont().SetSize(10);
	m_wndGraph.GetHeader().GetFont().SetBold(true);
	m_wndGraph.GetHeader().GetFont().SetColor(HEADCOLOR);

	//
	// Add series for each Range
	//
	for (iRangeIndex = 0; iRangeIndex < m_designData.m_fnGetRangeCnt(); iRangeIndex++)
	{
		m_wndGraph.AddSeries(scLine);
		strTitle.Format("%.02f", m_designData.m_fnGetRange(iRangeIndex, m_bIPUnits));
		m_wndGraph.GetSeries(iRangeIndex).SetTitle(strTitle);
	}

	//
	// Add series for Test WBT
	//
	if (m_bTestPoint)
	{
		strTitle.Format("Test WBT = %.02f",  m_testData.m_fnGetWBT(m_bIPUnits));

		m_wndGraph.AddSeries(scLine);
		m_wndGraph.GetSeries(m_designData.m_fnGetRangeCnt()).SetTitle(strTitle);

		m_wndGraph.GetSeries(m_designData.m_fnGetRangeCnt()).AddXY(m_testData.m_fnGetWBT(m_bIPUnits), m_designData.m_fnGetCWT(iFlowIndex,0,0,m_bIPUnits), NULL, DATACOLOR);
		m_wndGraph.GetSeries(m_designData.m_fnGetRangeCnt()).AddXY(m_testData.m_fnGetWBT(m_bIPUnits), m_designData.m_fnGetCWT(iFlowIndex,m_designData.m_fnGetWBTCnt(iFlowIndex)-1,m_designData.m_fnGetRangeCnt()-1 ,m_bIPUnits), NULL, DATACOLOR);
	}

	//
	// Add CWT vs WBT Lines
	//
	for (iRangeIndex = 0; iRangeIndex <m_designData.m_fnGetRangeCnt(); iRangeIndex++)
	{
		//
		// setup arrays for SplineTT function
		//
		double YFIT;   // Y value for the queried X value
		double TestX;  // X value for which you want the Y value

		int    iCnt;		// Count of elements in arrays
		double Y[MAX_WBTS];	// Array of Y values	In this case CWTs
		double X[MAX_WBTS];	// Array of X values	In this case WBTs
		double Y2[MAX_WBTS];// Array of Y??? values
		double WBTemp[MAX_FLOWS][MAX_WBTS];

		for (int iY = 0; iY < MAX_WBTS; iY++)
		{
			Y[iY] = m_designData.m_fnGetCWT(iFlowIndex,iY,iRangeIndex,m_bIPUnits);
		}
		m_designData.m_fnGetWBTArray(WBTemp, m_bIPUnits);
		memcpy(X, WBTemp[iFlowIndex], sizeof(X));
		iCnt = m_designData.m_fnGetWBTCnt(iFlowIndex);

		//
		// Iterate through with 50 increments
		//
		double dblIncrement = (m_designData.m_fnGetWBT(iFlowIndex,m_designData.m_fnGetWBTCnt(iFlowIndex)-1,m_bIPUnits) - m_designData.m_fnGetWBT(iFlowIndex,0,m_bIPUnits)) / 50.0; 
		for (double dblWBTIndex = m_designData.m_fnGetWBT(iFlowIndex,0,m_bIPUnits); dblWBTIndex <= m_designData.m_fnGetWBT(iFlowIndex,m_designData.m_fnGetWBTCnt(iFlowIndex)-1,m_bIPUnits); dblWBTIndex += dblIncrement)
		{
			TestX = dblWBTIndex;
            CalcPerfData(iCnt, X, Y, TestX, YFIT, Y2);
			m_wndGraph.GetSeries(iRangeIndex).AddXY(TestX, YFIT, NULL, DATACOLOR);
		}
	}
}

void TPerformanceCurveDlg::m_fnDrawCrossPlot1(bool bWhiteBackground)
{
	CString			strTitle;

	// Default to light on dark background
	unsigned long BACKCOLOR  (0x000000);
	unsigned long GRADCOLOR  (0x000000);
	unsigned long PANELCOLOR (0xFFFFFF);
	unsigned long TICKCOLOR  (0xCCCCCC);
	unsigned long GRIDCOLOR  (0x777777);
	unsigned long AXISCOLOR  (0xE4E4E4);
	unsigned long TITLECOLOR (0xFFFFFF);
	unsigned long LABELCOLOR (0xFFFFFF);
	unsigned long HEADCOLOR  (0xFFFFFF);
	unsigned long DATACOLOR  (0xFFFF);	// yellow

	if (bWhiteBackground)	// dark on light background graph
	{
		BACKCOLOR  = 0xFFFFFF;
		GRADCOLOR  = 0xFFFFFF;
		PANELCOLOR = 0x000000;
		TICKCOLOR  = 0x222222;
		GRIDCOLOR  = 0x777777;
		AXISCOLOR  = 0x666666;
		TITLECOLOR = 0x000000;
		LABELCOLOR = 0x000000;
		HEADCOLOR  = 0x000000;
		DATACOLOR  = 0xFF0000;	// blue
	}

	//---------------------------------------------------------------------
	// Clean up graph
	//---------------------------------------------------------------------
	m_wndGraph.RemoveAllSeries();

	//---------------------------------------------------------------------
	// Setup the graph appearance
	//---------------------------------------------------------------------
	m_wndGraph.GetWalls().SetBackColor(BACKCOLOR);
	m_wndGraph.GetPanel().GetGradient().SetStartColor(GRADCOLOR);
	m_wndGraph.GetPanel().GetGradient().SetEndColor(GRADCOLOR);
	m_wndGraph.GetPanel().GetGradient().SetVisible(true);
	m_wndGraph.GetAspect().SetView3D(false);
	m_wndGraph.SetDragMode(dmManual);

	if (m_wndGraph.GetHeader().GetText().Count() > 0)
	{
		m_wndGraph.GetHeader().GetText().Remove(0);
	}
	m_wndGraph.GetPanel().SetColor(PANELCOLOR);

	m_wndGraph.GetLegend().SetVisible(false);

	m_wndGraph.GetAxis().GetLeft().SetLogarithmic(false);
	m_wndGraph.GetAxis().GetRight().SetLogarithmic(false);
	m_wndGraph.GetAxis().GetTop().SetLogarithmic(false);
	m_wndGraph.GetAxis().GetBottom().SetLogarithmic(false);

	// tick color
	m_wndGraph.GetAxis().GetLeft().GetMinorTicks().SetColor(TICKCOLOR);
	m_wndGraph.GetAxis().GetRight().GetMinorTicks().SetColor(TICKCOLOR);
	m_wndGraph.GetAxis().GetTop().GetMinorTicks().SetColor(TICKCOLOR);
	m_wndGraph.GetAxis().GetBottom().GetMinorTicks().SetColor(TICKCOLOR);

	// grid color
	m_wndGraph.GetAxis().GetLeft().GetGridPen().SetColor(GRIDCOLOR);
	m_wndGraph.GetAxis().GetRight().GetGridPen().SetColor(GRIDCOLOR);
	m_wndGraph.GetAxis().GetTop().GetGridPen().SetColor(GRIDCOLOR);
	m_wndGraph.GetAxis().GetBottom().GetGridPen().SetColor(GRIDCOLOR);

	// Axis color
	m_wndGraph.GetAxis().GetLeft().GetAxisPen().SetColor(AXISCOLOR);
	m_wndGraph.GetAxis().GetRight().GetAxisPen().SetColor(AXISCOLOR);
	m_wndGraph.GetAxis().GetTop().GetAxisPen().SetColor(AXISCOLOR);
	m_wndGraph.GetAxis().GetBottom().GetAxisPen().SetColor(AXISCOLOR);

	// grid text
	m_wndGraph.GetAxis().GetLeft().GetTitle().GetFont().SetColor(TITLECOLOR);
	m_wndGraph.GetAxis().GetBottom().GetTitle().GetFont().SetColor(TITLECOLOR);

	// axis labels
	m_wndGraph.GetAxis().GetLeft().GetLabels().GetFont().SetColor(LABELCOLOR);
	m_wndGraph.GetAxis().GetBottom().GetLabels().GetFont().SetColor(LABELCOLOR);

	// set major axis to solid lines - psSolid
	m_wndGraph.GetAxis().GetLeft().GetGridPen().SetStyle(  psSolid );
	m_wndGraph.GetAxis().GetRight().GetGridPen().SetStyle( psSolid );
	m_wndGraph.GetAxis().GetTop().GetGridPen().SetStyle(   psSolid );
	m_wndGraph.GetAxis().GetBottom().GetGridPen().SetStyle(psSolid );

	m_wndGraph.GetAxis().GetLeft().GetGridPen().SetWidth(  1 );
	m_wndGraph.GetAxis().GetRight().GetGridPen().SetWidth( 1 );
	m_wndGraph.GetAxis().GetTop().GetGridPen().SetWidth(   1 );
	m_wndGraph.GetAxis().GetBottom().GetGridPen().SetWidth(1 );

	m_wndGraph.GetAxis().GetTop().GetLabels().SetAngle(90);
	m_wndGraph.GetAxis().GetBottom().GetLabels().SetAngle(90);

	m_wndGraph.GetAxis().GetLeft().GetLabels().GetFont().SetSize(10);
	m_wndGraph.GetAxis().GetRight().GetLabels().GetFont().SetSize(10);
	m_wndGraph.GetAxis().GetTop().GetLabels().GetFont().SetSize(10);
	m_wndGraph.GetAxis().GetBottom().GetLabels().GetFont().SetSize(10);

	m_wndGraph.GetAxis().GetLeft().SetAutomaticMinimum(true);
	m_wndGraph.GetAxis().GetRight().SetAutomaticMinimum(true);
	m_wndGraph.GetAxis().GetTop().SetAutomaticMinimum(true);
	m_wndGraph.GetAxis().GetBottom().SetAutomaticMinimum(true);

	m_wndGraph.GetAxis().GetLeft().SetAutomaticMaximum(true);
	m_wndGraph.GetAxis().GetRight().SetAutomaticMaximum(true);
	m_wndGraph.GetAxis().GetTop().SetAutomaticMaximum(true);
	m_wndGraph.GetAxis().GetBottom().SetAutomaticMaximum(true);

	m_wndGraph.GetAxis().GetLeft().GetTitle().SetCaption("Cold Water Temp");
	m_wndGraph.GetAxis().GetBottom().GetTitle().SetCaption("Cooling Range");

	m_wndGraph.GetAxis().GetLeft().GetTitle().GetFont().SetSize(10);
	m_wndGraph.GetAxis().GetBottom().GetTitle().GetFont().SetSize(10);
	m_wndGraph.GetAxis().GetLeft().GetTitle().GetFont().SetBold(true);
	m_wndGraph.GetAxis().GetBottom().GetTitle().GetFont().SetBold(true);

	double dblCWTMin	= 200;
	double dblCWTMax	= 0;
	double dblRangeMin	= 200;
	double dblRangeMax	= 0;

	//
	// Add series for each Flow
	//
	for (int iFlowIndex = 0; iFlowIndex < m_designData.m_fnGetFlowCnt(); iFlowIndex++)
	{
		m_wndGraph.AddSeries(scLine);
		strTitle.Format("%.02f", m_designData.m_fnGetFlow(iFlowIndex, m_bIPUnits));
		m_wndGraph.GetSeries(iFlowIndex).SetTitle(strTitle);
	}

	//
	// Add curve Data for plot 1
	//
	double dblDesignAirDensity;
	double dblTestAirDensity;
	double dblDesignSpecificVolume;
	double dblTestSpecificVolume;

	if (m_bIPUnits)
	{
		dblDesignAirDensity		= CalcDensityIP(   calcPressureC(m_designData.m_fnGetBarometricPressure(m_bIPUnits)), m_designData.m_fnGetDBT(m_bIPUnits), m_designData.m_fnGetWBT(m_bIPUnits));
		dblTestAirDensity		= CalcDensityIP(   calcPressureC(m_testData.m_fnGetBarometricPressure(m_bIPUnits))  , m_testData.m_fnGetDBT(m_bIPUnits)  , m_testData.m_fnGetWBT(m_bIPUnits));
		dblDesignSpecificVolume = SpecificVolumeIP(calcPressureC(m_designData.m_fnGetBarometricPressure(m_bIPUnits)), m_designData.m_fnGetDBT(m_bIPUnits), m_designData.m_fnGetWBT(m_bIPUnits));
		dblTestSpecificVolume	= SpecificVolumeIP(calcPressureC(m_testData.m_fnGetBarometricPressure(m_bIPUnits))  , m_testData.m_fnGetDBT(m_bIPUnits)  , m_testData.m_fnGetWBT(m_bIPUnits));
	}
	else
	{
		dblDesignAirDensity		= CalcDensitySI(   m_designData.m_fnGetBarometricPressure(m_bIPUnits), m_designData.m_fnGetDBT(m_bIPUnits), m_designData.m_fnGetWBT(m_bIPUnits));
		dblTestAirDensity		= CalcDensitySI(   m_testData.m_fnGetBarometricPressure(m_bIPUnits)  , m_testData.m_fnGetDBT(m_bIPUnits)  , m_testData.m_fnGetWBT(m_bIPUnits));
		dblDesignSpecificVolume = SpecificVolumeSI(m_designData.m_fnGetBarometricPressure(m_bIPUnits), m_designData.m_fnGetDBT(m_bIPUnits), m_designData.m_fnGetWBT(m_bIPUnits));
		dblTestSpecificVolume	= SpecificVolumeSI(m_testData.m_fnGetBarometricPressure(m_bIPUnits)  , m_testData.m_fnGetDBT(m_bIPUnits)  , m_testData.m_fnGetWBT(m_bIPUnits));
	}

	//
	// Copy data to temp vars and do calcs
	//
	CString strTemp;
	int IFlow;
	int IR;

    double Y[24] /*FLOW[5]*/;
	double YFIT;

    double /*X[24],*/ /*YFIT[24],*/ /*YA[3],*/ /*Y2A[3],*/ Y2[24];
    double /*R[5],*/ /*CWT[5][10][5],*/ /*CWT_WB_R[5],*/ CWT_WB[5][5];
//'                    (FLOW, WB, RANGE)

	double xAxis[MAX_RANGES];
	double yAxis[MAX_RANGES];

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


// Init Counts
	int IFlowNo		=	m_designData.m_fnGetFlowCnt();
	int IRangeNo	=	m_designData.m_fnGetRangeCnt();
	int IWBNo		=	m_designData.m_fnGetWBTCnt(0);

//Init Others
	double TestWB	= m_testData.m_fnGetWBT(m_bIPUnits);
	double TestR (m_testData.m_fnGetHWT(m_bIPUnits) - 
		m_testData.m_fnGetCWT(m_bIPUnits) - 1.0);
//	double TestCWT	= m_testData.m_fnGetCWT;

	// Add a descriptive title
	CString strDegree;
	if (m_bIPUnits)
		strDegree = "°F";
	else
		strDegree = "°C";
	strTitle.Format("At %.02f %s Test Wet Bulb", TestWB, strDegree);
	COleVariant var1(strTitle);
	m_wndGraph.GetHeader().GetText().Add(*(LPCVARIANT)var1);
	m_wndGraph.GetHeader().GetFont().SetSize(10);
	m_wndGraph.GetHeader().GetFont().SetBold(true);
	m_wndGraph.GetHeader().GetFont().SetColor(HEADCOLOR);

	// Try this to make sure test R is on graph:
	if (TestR < dblRangeMin)
		dblRangeMin = TestR;
	else if (TestR > dblRangeMax)
		dblRangeMax = TestR;

//'Interpolate for WB
//    PRINT USING "@ TestWB=##.##"; TestWB
//    PRINT #2, USING "@ TestWB=##.###"; TestWB
	strTemp.Format("@ TestWB=%.03f\r\n", TestWB);
	OutputDebugString(strTemp);

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

			//
			// Add series item
			//
			xAxis[IR] = R[IR];
			yAxis[IR] = CWT_WB[IFlow][IR];

			//
			// Set Min max values
			//
			if (CWT_WB[IFlow][IR] < dblCWTMin)
			{
				dblCWTMin = CWT_WB[IFlow][IR];
			}
			else if (CWT_WB[IFlow][IR] > dblCWTMax)
			{
				dblCWTMax = CWT_WB[IFlow][IR];
			}
			if (R[IR] < dblRangeMin)
			{
				dblRangeMin = R[IR];
			}
			else if (R[IR] > dblRangeMax)
			{
				dblRangeMax = R[IR];
			}
        }
		double	dblIncrement =  (dblRangeMax - dblRangeMin) / 50.0;
		int		iCnt = IRangeNo;
		for (double dblRangeIndex = dblRangeMin; dblRangeIndex <= dblRangeMax; dblRangeIndex += dblIncrement)
		{
			double TestX = dblRangeIndex;
            CalcPerfData(iCnt, xAxis, yAxis, TestX, YFIT, Y2);
			m_wndGraph.GetSeries(IFlow).AddXY(TestX, YFIT, NULL, DATACOLOR);
			if (YFIT < dblCWTMin)
			{
				dblCWTMin = YFIT;
			}
			else if (YFIT > dblCWTMax)
			{
				dblCWTMax = YFIT;
			}
		}

		TRACE0("\r\n");
    }

	//
	// Set Min Max
	//
	m_wndGraph.GetAxis().GetLeft().SetMinMax	(dblCWTMin,dblCWTMax);
	m_wndGraph.GetAxis().GetRight().SetMinMax	(dblCWTMin,dblCWTMax);
	m_wndGraph.GetAxis().GetTop().SetMinMax		(dblRangeMin,dblRangeMax);
	m_wndGraph.GetAxis().GetBottom().SetMinMax	(dblRangeMin,dblRangeMax);

	//
	// Add series for Test Range
	//
	m_wndGraph.AddSeries(scLine);
	strTitle.Format("Test Range = %.02f",  m_testData.m_fnGetHWT(m_bIPUnits) - m_testData.m_fnGetCWT(m_bIPUnits));
	m_wndGraph.GetSeries(m_designData.m_fnGetFlowCnt()).SetTitle(strTitle);
	m_wndGraph.GetSeries(m_designData.m_fnGetFlowCnt()).AddXY(m_testData.m_fnGetHWT(m_bIPUnits) - m_testData.m_fnGetCWT(m_bIPUnits), dblCWTMin, NULL, DATACOLOR);
	m_wndGraph.GetSeries(m_designData.m_fnGetFlowCnt()).AddXY(m_testData.m_fnGetHWT(m_bIPUnits) - m_testData.m_fnGetCWT(m_bIPUnits), dblCWTMax, NULL, DATACOLOR);
}

void TPerformanceCurveDlg::m_fnDrawCrossPlot2(bool bWhiteBackground)
{
	CString			strTitle;

	// Default to light on dark background
	unsigned long BACKCOLOR  (0x000000);
	unsigned long GRADCOLOR  (0x000000);
	unsigned long PANELCOLOR (0xFFFFFF);
	unsigned long TICKCOLOR  (0xCCCCCC);
	unsigned long GRIDCOLOR  (0x777777);
	unsigned long AXISCOLOR  (0xE4E4E4);
	unsigned long TITLECOLOR (0xFFFFFF);
	unsigned long LABELCOLOR (0xFFFFFF);
	unsigned long HEADCOLOR  (0xFFFFFF);
	unsigned long DATACOLOR  (0xFFFF);	// yellow

	if (bWhiteBackground)	// dark on light background graph
	{
		BACKCOLOR  = 0xFFFFFF;
		GRADCOLOR  = 0xFFFFFF;
		PANELCOLOR = 0x000000;
		TICKCOLOR  = 0x222222;
		GRIDCOLOR  = 0x777777;
		AXISCOLOR  = 0x666666;
		TITLECOLOR = 0x000000;
		LABELCOLOR = 0x000000;
		HEADCOLOR  = 0x000000;
		DATACOLOR  = 0xFF0000;	// blue
	}

	//---------------------------------------------------------------------
	// Clean up graph
	//---------------------------------------------------------------------
	m_wndGraph.RemoveAllSeries();

	//---------------------------------------------------------------------
	// Setup the graph appearance
	//---------------------------------------------------------------------
	m_wndGraph.GetWalls().SetBackColor(BACKCOLOR);
	m_wndGraph.GetPanel().GetGradient().SetStartColor(GRADCOLOR);
	m_wndGraph.GetPanel().GetGradient().SetEndColor(GRADCOLOR);
	m_wndGraph.GetPanel().GetGradient().SetVisible(true);
	m_wndGraph.GetAspect().SetView3D(false);
	m_wndGraph.SetDragMode(dmManual);

	if (m_wndGraph.GetHeader().GetText().Count() > 0)
	{
		m_wndGraph.GetHeader().GetText().Remove(0);
	}
	m_wndGraph.GetPanel().SetColor(PANELCOLOR);

	m_wndGraph.GetLegend().SetVisible(false);

	m_wndGraph.GetAxis().GetLeft().SetLogarithmic(false);
	m_wndGraph.GetAxis().GetRight().SetLogarithmic(false);
	m_wndGraph.GetAxis().GetTop().SetLogarithmic(false);
	m_wndGraph.GetAxis().GetBottom().SetLogarithmic(false);

	// tick color
	m_wndGraph.GetAxis().GetLeft().GetMinorTicks().SetColor(TICKCOLOR);
	m_wndGraph.GetAxis().GetRight().GetMinorTicks().SetColor(TICKCOLOR);
	m_wndGraph.GetAxis().GetTop().GetMinorTicks().SetColor(TICKCOLOR);
	m_wndGraph.GetAxis().GetBottom().GetMinorTicks().SetColor(TICKCOLOR);

	// grid color
	m_wndGraph.GetAxis().GetLeft().GetGridPen().SetColor(GRIDCOLOR);
	m_wndGraph.GetAxis().GetRight().GetGridPen().SetColor(GRIDCOLOR);
	m_wndGraph.GetAxis().GetTop().GetGridPen().SetColor(GRIDCOLOR);
	m_wndGraph.GetAxis().GetBottom().GetGridPen().SetColor(GRIDCOLOR);

	// Axis color
	m_wndGraph.GetAxis().GetLeft().GetAxisPen().SetColor(AXISCOLOR);
	m_wndGraph.GetAxis().GetRight().GetAxisPen().SetColor(AXISCOLOR);
	m_wndGraph.GetAxis().GetTop().GetAxisPen().SetColor(AXISCOLOR);
	m_wndGraph.GetAxis().GetBottom().GetAxisPen().SetColor(AXISCOLOR);

	// grid text
	m_wndGraph.GetAxis().GetLeft().GetTitle().GetFont().SetColor(TITLECOLOR);
	m_wndGraph.GetAxis().GetBottom().GetTitle().GetFont().SetColor(TITLECOLOR);

	// axis labels
	m_wndGraph.GetAxis().GetLeft().GetLabels().GetFont().SetColor(LABELCOLOR);
	m_wndGraph.GetAxis().GetBottom().GetLabels().GetFont().SetColor(LABELCOLOR);

	// set major axis to solid lines - psSolid
	m_wndGraph.GetAxis().GetLeft().GetGridPen().SetStyle(  psSolid );
	m_wndGraph.GetAxis().GetRight().GetGridPen().SetStyle( psSolid );
	m_wndGraph.GetAxis().GetTop().GetGridPen().SetStyle(   psSolid );
	m_wndGraph.GetAxis().GetBottom().GetGridPen().SetStyle(psSolid );

	m_wndGraph.GetAxis().GetLeft().GetGridPen().SetWidth(  1 );
	m_wndGraph.GetAxis().GetRight().GetGridPen().SetWidth( 1 );
	m_wndGraph.GetAxis().GetTop().GetGridPen().SetWidth(   1 );
	m_wndGraph.GetAxis().GetBottom().GetGridPen().SetWidth(1 );

	m_wndGraph.GetAxis().GetTop().GetLabels().SetAngle(90);
	m_wndGraph.GetAxis().GetBottom().GetLabels().SetAngle(90);

	m_wndGraph.GetAxis().GetLeft().GetLabels().GetFont().SetSize(10);
	m_wndGraph.GetAxis().GetRight().GetLabels().GetFont().SetSize(10);
	m_wndGraph.GetAxis().GetTop().GetLabels().GetFont().SetSize(10);
	m_wndGraph.GetAxis().GetBottom().GetLabels().GetFont().SetSize(10);

	m_wndGraph.GetAxis().GetLeft().SetAutomaticMinimum(true);
	m_wndGraph.GetAxis().GetRight().SetAutomaticMinimum(true);
	m_wndGraph.GetAxis().GetTop().SetAutomaticMinimum(true);
	m_wndGraph.GetAxis().GetBottom().SetAutomaticMinimum(true);

	m_wndGraph.GetAxis().GetLeft().SetAutomaticMaximum(true);
	m_wndGraph.GetAxis().GetRight().SetAutomaticMaximum(true);
	m_wndGraph.GetAxis().GetTop().SetAutomaticMaximum(true);
	m_wndGraph.GetAxis().GetBottom().SetAutomaticMaximum(true);

	m_wndGraph.GetAxis().GetLeft().GetTitle().SetCaption("Cold Water Temp");
	m_wndGraph.GetAxis().GetBottom().GetTitle().SetCaption("Water Flow Rate");

	m_wndGraph.GetAxis().GetLeft().GetTitle().GetFont().SetSize(10);
	m_wndGraph.GetAxis().GetBottom().GetTitle().GetFont().SetSize(10);
	m_wndGraph.GetAxis().GetLeft().GetTitle().GetFont().SetBold(true);
	m_wndGraph.GetAxis().GetBottom().GetTitle().GetFont().SetBold(true);

	double dblCWTMin	= 200;
	double dblCWTMax	= 0;
	double dblFlowMin	= 200000000000;
	double dblFlowMax	= 0;

	//
	// Add series for Predicted Flow
	//
	m_wndGraph.AddSeries(scLine);
	strTitle = "Crossplot2";
	m_wndGraph.GetSeries(0).SetTitle(strTitle);

	//
	// Add curve Data for plot 1
	//
	double dblAdjustedFlow;
	double dblDesignAirDensity;
	double dblTestAirDensity;
	double dblDesignSpecificVolume;
	double dblTestSpecificVolume;

	if (m_bIPUnits)
	{
		dblDesignAirDensity		= CalcDensityIP(   calcPressureC(m_designData.m_fnGetBarometricPressure(m_bIPUnits)), m_designData.m_fnGetDBT(m_bIPUnits), m_designData.m_fnGetWBT(m_bIPUnits));
		dblTestAirDensity		= CalcDensityIP(   calcPressureC(m_testData.m_fnGetBarometricPressure(m_bIPUnits))  , m_testData.m_fnGetDBT(m_bIPUnits)  , m_testData.m_fnGetWBT(m_bIPUnits));
		dblDesignSpecificVolume = SpecificVolumeIP(calcPressureC(m_designData.m_fnGetBarometricPressure(m_bIPUnits)), m_designData.m_fnGetDBT(m_bIPUnits), m_designData.m_fnGetWBT(m_bIPUnits));
		dblTestSpecificVolume	= SpecificVolumeIP(calcPressureC(m_testData.m_fnGetBarometricPressure(m_bIPUnits))  , m_testData.m_fnGetDBT(m_bIPUnits)  , m_testData.m_fnGetWBT(m_bIPUnits));
	}
	else
	{
		dblDesignAirDensity		= CalcDensitySI(   m_designData.m_fnGetBarometricPressure(m_bIPUnits), m_designData.m_fnGetDBT(m_bIPUnits), m_designData.m_fnGetWBT(m_bIPUnits));
		dblTestAirDensity		= CalcDensitySI(   m_testData.m_fnGetBarometricPressure(m_bIPUnits)  , m_testData.m_fnGetDBT(m_bIPUnits)  , m_testData.m_fnGetWBT(m_bIPUnits));
		dblDesignSpecificVolume = SpecificVolumeSI(m_designData.m_fnGetBarometricPressure(m_bIPUnits), m_designData.m_fnGetDBT(m_bIPUnits), m_designData.m_fnGetWBT(m_bIPUnits));
		dblTestSpecificVolume	= SpecificVolumeSI(m_testData.m_fnGetBarometricPressure(m_bIPUnits)  , m_testData.m_fnGetDBT(m_bIPUnits)  , m_testData.m_fnGetWBT(m_bIPUnits));
	}

	double dblTestLG;

	//
	// Copy data to temp vars and do calcs
	//
	CString strTemp;
	int IFlow;
	int IR;

    double Y[24] /*FLOW[5]*/;
	double YFIT;

    double /*X[24],*/ /*YFIT[24],*/ /*YA[3],*/ /*Y2A[3],*/ Y2[24];
    double /*R[5],*/ /*CWT[5][10][5],*/ CWT_WB[5][5], CWT_WB_R[5];
//'                    (FLOW, WB, RANGE)
	double xAxis[MAX_RANGES];
	double yAxis[MAX_RANGES];

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


// Init Counts
	int IFlowNo		=	m_designData.m_fnGetFlowCnt();
	int IRangeNo	=	m_designData.m_fnGetRangeCnt();
	int IWBNo		=	m_designData.m_fnGetWBTCnt(0);

//Init Others
	double TestWB	= m_testData.m_fnGetWBT(m_bIPUnits);
	double TestR	= m_testData.m_fnGetHWT(m_bIPUnits) - m_testData.m_fnGetCWT(m_bIPUnits);
	double TestCWT	= m_testData.m_fnGetCWT(m_bIPUnits);

	// Add a descriptive title
	CString strDegree;
	if (m_bIPUnits)
		strDegree = "°F";
	else
		strDegree = "°C";
	strTitle.Format("At %.02f %s Test Wet Bulb and %.02f %s Test Range", 
		TestWB, strDegree, TestR, strDegree);
	COleVariant var1(strTitle);
	m_wndGraph.GetHeader().GetText().Add(*(LPCVARIANT)var1);
	m_wndGraph.GetHeader().GetFont().SetSize(10);
	m_wndGraph.GetHeader().GetFont().SetBold(true);
	m_wndGraph.GetHeader().GetFont().SetColor(HEADCOLOR);

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
        }
		TRACE0("\r\n");
    }

//'Interpolate for Range
#ifdef _DEBUG
//    PRINT #2, USING "@ TestWB=##.## and TestR=##.##"; TestWB; TestR
	strTemp.Format("@ TestWB=%.02f and TestR=%.02f\r\n", TestWB, TestR);
	OutputDebugString(strTemp);
#endif // _DEBUG

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

		//
		// Add series item
		//
		xAxis[IFlow] = FLOW[IFlow];
		yAxis[IFlow] = CWT_WB_R[IFlow];

		//
		// Set Min max values
		//
		if (CWT_WB_R[IFlow] < dblCWTMin)
		{
			dblCWTMin = CWT_WB_R[IFlow];
		}
		else if (CWT_WB_R[IFlow] > dblCWTMax)
		{
			dblCWTMax = CWT_WB_R[IFlow];
		}
		if (FLOW[IFlow] < dblFlowMin)
		{
			dblFlowMin = FLOW[IFlow];
		}
		else if (FLOW[IFlow] > dblFlowMax)
		{
			dblFlowMax = FLOW[IFlow];
		}

	}

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
		MessageBox(strWarning, "Extrapolation Warning", MB_ICONWARNING);
    }
//'End check for Extrapolation


	double LWBTnew	= 0;
	double DenOutT	= 0;
	double SVOutT	= 0;
	double HLWBT	= 0; 
	dblAdjustedFlow	= DetermineAdjTestFlow(m_bIPUnits, (int)m_designData.m_fnGetInducedFlow(), m_designData.m_fnGetHWT(m_bIPUnits), m_designData.m_fnGetCWT(m_bIPUnits), m_designData.m_fnGetWBT(m_bIPUnits), m_designData.m_fnGetDBT(m_bIPUnits), m_designData.m_fnGetBarometricPressure(m_bIPUnits), m_designData.m_fnGetWaterFlowRate(m_bIPUnits), m_designData.m_fnGetFanDriverPower(m_bIPUnits), m_designData.m_fnGetLG(m_bIPUnits), m_testData.m_fnGetHWT(m_bIPUnits), m_testData.m_fnGetCWT(m_bIPUnits), m_testData.m_fnGetWBT(m_bIPUnits), m_testData.m_fnGetDBT(m_bIPUnits), m_testData.m_fnGetBarometricPressure(m_bIPUnits), m_testData.m_fnGetWaterFlowRate(m_bIPUnits), m_testData.m_fnGetFanDriverPower(m_bIPUnits), LWBTnew, DenOutT, SVOutT, HLWBT, dblAdjustedFlow, dblTestLG);
	double AdjTestFlow = dblAdjustedFlow;
//    PRINT USING "TestCWT=##.###  PredFlow=######.###"; TestCWT; PredFlow
//    PRINT USING "AdjTestFlow=######.###  PredFlow=######.###  Percent Capability=###.##"; AdjTestFlow; PredFlow; AdjTestFlow / PredFlow * 100

	strTemp.Format("TestCWT=%.02f PredFlow=%.03f\r\n",TestCWT, PredFlow);
	OutputDebugString(strTemp);
	strTemp.Format("AdjTestFlow=%.03f  PredFlow=%.03f  Percent Capability=%.03f", AdjTestFlow, PredFlow, 
		PredFlow != 0.0 ? (AdjTestFlow / PredFlow * 100.0) : 0.0);
	TRACE0(strTemp);


//	double dblCapability = AdjTestFlow / PredFlow * 100.0;

	//
	// Add point going through the correct spot
	//
	// DDP 2-9-01 m_wndGraph.GetSeries(0).AddXY(PredFlow, m_testData.m_fnGetCWT, NULL, color);

	//
	// Add series for Test CWT
	//
	// DDP 2-9-01	
	if (dblFlowMin > PredFlow)
	{
		dblFlowMin = PredFlow - (PredFlow*.05);
	}
	else if (dblFlowMax < PredFlow)
	{
		dblFlowMax = PredFlow + (PredFlow*.05);
	}
	m_wndGraph.AddSeries(scLine);
	strTitle.Format("CWT = %.02f",  m_testData.m_fnGetCWT(m_bIPUnits));
	m_wndGraph.GetSeries(1).SetTitle(strTitle);
	m_wndGraph.GetSeries(1).AddXY(dblFlowMin, m_testData.m_fnGetCWT(m_bIPUnits), NULL, DATACOLOR);
	m_wndGraph.GetSeries(1).AddXY(PredFlow  , m_testData.m_fnGetCWT(m_bIPUnits), NULL, DATACOLOR);

	//
	// Add series for Pred Flow
	//
	// DDP 2-9-01
	if (dblCWTMin > m_testData.m_fnGetCWT(m_bIPUnits))
	{
		dblCWTMin = m_testData.m_fnGetCWT(m_bIPUnits) - (m_testData.m_fnGetCWT(m_bIPUnits)*.05);
	}
	else if (dblCWTMax < PredFlow)
	{
		dblCWTMax = m_testData.m_fnGetCWT(m_bIPUnits) + (m_testData.m_fnGetCWT(m_bIPUnits)*.05);
	}
	m_wndGraph.AddSeries(scLine);
	strTitle.Format("Pred Flow = %.02f",  PredFlow);
	m_wndGraph.GetSeries(2).SetTitle(strTitle);
	m_wndGraph.GetSeries(2).AddXY(PredFlow, m_testData.m_fnGetCWT(m_bIPUnits), NULL, DATACOLOR);
	m_wndGraph.GetSeries(2).AddXY(PredFlow, dblCWTMin, NULL, DATACOLOR);

	//
	// Put in line
	//
//	double	dblIncrement =  (dblFlowMax - dblFlowMin) / 50.0;
//	int		iCnt = IFlowNo;
//	for (double dblFlowIndex = dblFlowMin; dblFlowIndex <= dblFlowMax; dblFlowIndex += dblIncrement)
//	{
//		double TestX = dblFlowIndex;
//		CalcPerfData(iCnt, xAxis, yAxis, TestX, YFIT, Y2);
//		m_wndGraph.GetSeries(0).AddXY(TestX, YFIT, NULL, color);
//	}
	double	dblIncrement((dblCWTMax - dblCWTMin) / 50.0);
	if (dblIncrement > 0.0)
	{
		int	iCnt(IFlowNo);
		for (double dblCWTIndex = dblCWTMin; dblCWTIndex <= dblCWTMax; 
			dblCWTIndex += dblIncrement)
		{
			double TestY = dblCWTIndex;
			double XFIT;
			CalcPerfData(iCnt, yAxis, xAxis, TestY, XFIT, Y2);
			m_wndGraph.GetSeries(0).AddXY(XFIT, TestY, NULL, DATACOLOR);
		}
	}
	//
	// Set Min Max
	//
	m_wndGraph.GetAxis().GetLeft().SetMinMax	(dblCWTMin,dblCWTMax);
	m_wndGraph.GetAxis().GetRight().SetMinMax	(dblCWTMin,dblCWTMax);
	m_wndGraph.GetAxis().GetTop().SetMinMax		(dblFlowMin,dblFlowMax);
	m_wndGraph.GetAxis().GetBottom().SetMinMax	(dblFlowMin,dblFlowMax);
}

void TPerformanceCurveDlg::OnButtonCopyGraph() 
{
		m_wndGraph.GetExport().CopyToClipboardMetafile(true);
}

void TPerformanceCurveDlg::OnButtonReverse()
{
	m_bReversedGraph = m_bReversedGraph ? false : true;
	m_fnDrawGraph();
}