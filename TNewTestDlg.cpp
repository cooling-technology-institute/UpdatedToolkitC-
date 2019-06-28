// TNewTestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CTIToolkit.h"
#include "TNewTestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// TNewTestDlg dialog


TNewTestDlg::TNewTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(TNewTestDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(TNewTestDlg)
	m_strTestName = _T("");
	//}}AFX_DATA_INIT
}


void TNewTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(TNewTestDlg)
	DDX_Text(pDX, IDC_EDIT_TESTNAME, m_strTestName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(TNewTestDlg, CDialog)
	//{{AFX_MSG_MAP(TNewTestDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// TNewTestDlg message handlers

BOOL TNewTestDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	GetDlgItem(IDC_EDIT_TESTNAME)->SetFocus();
	
	return false;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
