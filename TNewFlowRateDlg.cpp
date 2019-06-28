// TNewFlowRateDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CTIToolkit.h"
#include "TNewFlowRateDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// TNewFlowRateDlg dialog


TNewFlowRateDlg::TNewFlowRateDlg(CWnd* pParent /*=NULL*/)
	: CDialog(TNewFlowRateDlg::IDD, pParent)
{
	m_dblClosestFlow = 0.0;
	//{{AFX_DATA_INIT(TNewFlowRateDlg)
	m_dblFlowRate = 0.0;
	//}}AFX_DATA_INIT
	m_bNewFlow = true;
}


void TNewFlowRateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(TNewFlowRateDlg)
	DDX_Text(pDX, IDC_EDIT_FLOW, m_dblFlowRate);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(TNewFlowRateDlg, CDialog)
	//{{AFX_MSG_MAP(TNewFlowRateDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// TNewFlowRateDlg message handlers

void TNewFlowRateDlg::OnOK() 
{
	UpdateData();
	
	if ((m_dblClosestFlow > 0.0) && (m_dblFlowRate > 0.0))
	{
		double dblPercent = 0;
		if (m_dblFlowRate <= m_dblClosestFlow)
		{
			dblPercent = m_dblFlowRate/m_dblClosestFlow*100;
		}
		else
		{
			dblPercent = m_dblClosestFlow/m_dblFlowRate*100;
		}
		if (dblPercent < 40.0)
		{
			if (MessageBox("Flow is not within tolerance of other flows. Do want to keep this flow value?", NULL, MB_ICONQUESTION | MB_YESNO) == IDNO)
			{
				return;
			}
		}
	}
	else if (m_dblFlowRate <= 0)
	{
		MessageBox("Flow must be greater than 0");
		return;
	}
	CDialog::OnOK();
}

BOOL TNewFlowRateDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if (!m_bNewFlow)
	{
		SetWindowText("Edit Flow Rate");
	}
	
	GetDlgItem(IDC_EDIT_FLOW)->SetFocus();

	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
