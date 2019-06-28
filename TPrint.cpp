// TPrint.cpp : implementation file
//

#include "stdafx.h"
#include "CTIToolkit.h"
#include "TPrint.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// TPrint dialog


TPrint::TPrint(CWnd* pParent /*=NULL*/)
	: CDialog(TPrint::IDD, pParent)
{
	//{{AFX_DATA_INIT(TPrint)
	m_csDescription = _T("");
	//}}AFX_DATA_INIT
    m_csDescription = "";
}


void TPrint::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(TPrint)
	DDX_Text(pDX, IDC_DESC, m_csDescription);
	DDV_MaxChars(pDX, m_csDescription, 50);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(TPrint, CDialog)
	//{{AFX_MSG_MAP(TPrint)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// TPrint message handlers

BOOL TPrint::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	GetDlgItem(IDC_PRINT_DESC)->SetFocus();
	
	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
