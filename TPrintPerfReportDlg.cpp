// TPrintPerfReportDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CTIToolkit.h"
#include "TPrintPerfReportDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// TPrintPerfReportDlg dialog


TPrintPerfReportDlg::TPrintPerfReportDlg(CWnd* pParent /*=NULL*/)
	: CDialog(TPrintPerfReportDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(TPrintPerfReportDlg)
	m_csDescription = _T("");
	m_nReportType = 0;
	//}}AFX_DATA_INIT
	m_bPerfReport = true;
}


void TPrintPerfReportDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(TPrintPerfReportDlg)
	DDX_Text(pDX, IDC_PRINT_DESC, m_csDescription);
	DDX_Radio(pDX, IDC_RADIO_PERF_REPORT, m_nReportType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(TPrintPerfReportDlg, CDialog)
	//{{AFX_MSG_MAP(TPrintPerfReportDlg)
	ON_BN_CLICKED(IDC_RADIO_DESIGN_DATA, OnRadioDesignData)
	ON_BN_CLICKED(IDC_RADIO_PERF_REPORT, OnRadioPerfReport)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// TPrintPerfReportDlg message handlers

void TPrintPerfReportDlg::OnRadioDesignData() 
{
}

void TPrintPerfReportDlg::OnRadioPerfReport() 
{
}

void TPrintPerfReportDlg::OnOK() 
{
	UpdateData();
	/**
    int button = GetCheckedRadioButton( IDC_RADIO_PERF_REPORT, IDC_RADIO_DESIGN_DATA );
    switch (button)
    {
    case IDC_RADIO_DESIGN_DATA :
		m_bPerfReport = false;
        break;
    case IDC_RADIO_PERF_REPORT :
		m_bPerfReport = true;
        break;
    }
	**/
	m_bPerfReport = (m_nReportType == 0) ? true : false;
	CDialog::OnOK();
}
