// TMultiEdit.cpp : implementation file
//

#include "stdafx.h"
#include "CTIToolkit.h"
#include "TMultiEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// TMultiEdit dialog


TMultiEdit::TMultiEdit(CWnd* pParent /*=NULL*/)
    : CDialog(TMultiEdit::IDD, pParent)
{
	//{{AFX_DATA_INIT(TMultiEdit)
	m_bchk1 = TRUE;
	m_bchk3 = TRUE;
	m_bchk4 = TRUE;
	m_bchk5 = TRUE;
	m_bchk6 = TRUE;
	m_bchk7 = TRUE;
	m_bchk8 = TRUE;
	m_bchk9 = TRUE;
	m_bchk10 = TRUE;
	m_bchk11 = TRUE;
	m_bchk12 = TRUE;
	m_bchk13 = TRUE;
	m_bchk14 = TRUE;
	m_bchk15 = TRUE;
	m_bchk16 = TRUE;
	m_bchk17 = TRUE;
	m_bchk18 = TRUE;
	m_bchk19 = TRUE;
	m_bCoef = TRUE;
	m_bKAVL = TRUE;
	m_bLG = TRUE;
	m_bTargetApproach = TRUE;
	m_bUserApproach = TRUE;
	m_dblUserApproach = 0.0;
	//}}AFX_DATA_INIT
}

TMultiEdit::~TMultiEdit()
{
}

void TMultiEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(TMultiEdit)
	DDX_Check(pDX, IDC_CHECK1, m_bchk1);
	DDX_Check(pDX, IDC_CHECK3, m_bchk3);
	DDX_Check(pDX, IDC_CHECK4, m_bchk4);
	DDX_Check(pDX, IDC_CHECK5, m_bchk5);
	DDX_Check(pDX, IDC_CHECK6, m_bchk6);
	DDX_Check(pDX, IDC_CHECK7, m_bchk7);
	DDX_Check(pDX, IDC_CHECK8, m_bchk8);
	DDX_Check(pDX, IDC_CHECK9, m_bchk9);
	DDX_Check(pDX, IDC_CHECK10, m_bchk10);
	DDX_Check(pDX, IDC_CHECK11, m_bchk11);
	DDX_Check(pDX, IDC_CHECK12, m_bchk12);
	DDX_Check(pDX, IDC_CHECK13, m_bchk13);
	DDX_Check(pDX, IDC_CHECK14, m_bchk14);
	DDX_Check(pDX, IDC_CHECK15, m_bchk15);
	DDX_Check(pDX, IDC_CHECK16, m_bchk16);
	DDX_Check(pDX, IDC_CHECK17, m_bchk17);
	DDX_Check(pDX, IDC_CHECK18, m_bchk18);
	DDX_Check(pDX, IDC_CHECK19, m_bchk19);
	DDX_Check(pDX, IDC_COEF, m_bCoef);
	DDX_Text(pDX, IDC_EDIT_USER1, m_dblUserApproach);
    DDV_MinMaxDouble(pDX,m_dblUserApproach, 0, 100.0);
	DDX_Check(pDX, IDC_KAVL, m_bKAVL);
	DDX_Check(pDX, IDC_LG, m_bLG);
	DDX_Check(pDX, IDC_TARGET_APPROACH, m_bTargetApproach);
	DDX_Check(pDX, IDC_USER1, m_bUserApproach);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(TMultiEdit, CDialog)
	//{{AFX_MSG_MAP(TMultiEdit)
	ON_BN_CLICKED(IDC_BUTTON_OFF, OnButtonOff)
	ON_BN_CLICKED(IDC_BUTTON_ON, OnButtonOn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// TMultiEdit message handlers

void TMultiEdit::OnOK() 
{
	UpdateData(true);
	CDialog::OnOK();
}


void TMultiEdit::OnButtonOff() 
{
	m_bchk1		= false;
	m_bchk3		= false;
	m_bchk4		= false;
	m_bchk5		= false;
	m_bchk6		= false;
	m_bchk7		= false;
	m_bchk8		= false;
	m_bchk9		= false;
	m_bchk10	= false;
	m_bchk11	= false;
	m_bchk12	= false;
	m_bchk13	= false;
	m_bchk14	= false;
	m_bchk15	= false;
	m_bchk16	= false;
	m_bchk17	= false;
	m_bchk18	= false;
	m_bchk19	= false;
	m_bCoef		= false;
	m_bLG		= false;
	m_bKAVL		= false;
	m_bTargetApproach	= false;
	m_bUserApproach		= false;
	UpdateData(false);
}

void TMultiEdit::OnButtonOn() 
{
	m_bchk1		= true;
	m_bchk3		= true;
	m_bchk4		= true;
	m_bchk5		= true;
	m_bchk6		= true;
	m_bchk7		= true;
	m_bchk8		= true;
	m_bchk9		= true;
	m_bchk10	= true;
	m_bchk11	= true;
	m_bchk12	= true;
	m_bchk13	= true;
	m_bchk14	= true;
	m_bchk15	= true;
	m_bchk16	= true;
	m_bchk17	= true;
	m_bchk18	= true;
	m_bchk19	= true;
	m_bCoef		= true;
	m_bLG		= true;
	m_bKAVL		= true;
	m_bTargetApproach	= true;
	m_bUserApproach		= true;
	UpdateData(false);
}

void TMultiEdit::m_fnInitData(double *listApp, bool* pabApproach, bool bCoef, bool bLG, bool bKAVL, bool bTargetApproach, bool bUserApproach, double dblUserApproach)
{
	memcpy(m_listApp, listApp, sizeof(m_listApp));

	m_bchk1		= pabApproach[0];
	m_bchk3		= pabApproach[1];
	m_bchk4		= pabApproach[2];
	m_bchk5		= pabApproach[3];
	m_bchk6		= pabApproach[4];
	m_bchk7		= pabApproach[5];
	m_bchk8		= pabApproach[6];
	m_bchk9		= pabApproach[7];
	m_bchk10	= pabApproach[8];
	m_bchk11	= pabApproach[9];
	m_bchk12	= pabApproach[10];
	m_bchk13	= pabApproach[11];
	m_bchk14	= pabApproach[12];
	m_bchk15	= pabApproach[13];
	m_bchk16	= pabApproach[14];
	m_bchk17	= pabApproach[15];
	m_bchk18	= pabApproach[16];
	m_bchk19	= pabApproach[17];
	m_bCoef		= bCoef;
	m_bLG		= bLG;
	m_bKAVL		= bKAVL;
	m_bTargetApproach	= bTargetApproach;
	m_bUserApproach		= bUserApproach;
	m_dblUserApproach	= dblUserApproach;

	if (IsWindow(m_hWnd))
	{
		UpdateData(false);

		//
		// Cycle through Check boxes and set thier text.
		//
		for (int iIndex = IDC_CHECK1; iIndex <= IDC_CHECK19; iIndex++)
		{
			if ((int)m_listApp[iIndex - IDC_CHECK1] > 0)
			{
				CString strTitle;
				strTitle.Format("%i", (int)m_listApp[iIndex - IDC_CHECK1]);
				GetDlgItem(iIndex)->SetWindowText(strTitle);
			}
		}
	}
}

void TMultiEdit::m_fnGetData(bool* pabApproach, bool &bCoef, bool &bLG, bool &bKAVL, bool &bTargetApproach, bool &bUserApproach, double &dblUserApproach)
{
	pabApproach[0]  = (m_bchk1 != 0);
	pabApproach[1]  = (m_bchk3 != 0);
	pabApproach[2]  = (m_bchk4 != 0);
	pabApproach[3]  = (m_bchk5 != 0);
	pabApproach[4]  = (m_bchk6 != 0);
	pabApproach[5]  = (m_bchk7 != 0);
	pabApproach[6]  = (m_bchk8 != 0);
	pabApproach[7]  = (m_bchk9 != 0);
	pabApproach[8]  = (m_bchk10 != 0);
	pabApproach[9]  = (m_bchk11 != 0);
	pabApproach[10] = (m_bchk12 != 0);
	pabApproach[11] = (m_bchk13 != 0);
	pabApproach[12] = (m_bchk14 != 0);
	pabApproach[13] = (m_bchk15 != 0);
	pabApproach[14] = (m_bchk16 != 0);
	pabApproach[15] = (m_bchk17 != 0);
	pabApproach[16] = (m_bchk18 != 0);
	pabApproach[17] = (m_bchk19 != 0);
	bCoef			= (m_bCoef != 0);
	bLG				= (m_bLG != 0);
	bKAVL			= (m_bKAVL != 0);
	bTargetApproach = (m_bTargetApproach != 0);
	bUserApproach	= (m_bUserApproach != 0);
	dblUserApproach = m_dblUserApproach;
}

BOOL TMultiEdit::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	//
	// Cycle through Check boxes and set thier text.
	//
	for (int iIndex = IDC_CHECK1; iIndex <= IDC_CHECK19; iIndex++)
	{
		if ((int)m_listApp[iIndex - IDC_CHECK1] > 0)
		{
			CString strTitle;
			strTitle.Format("%i", (int)m_listApp[iIndex - IDC_CHECK1]);
			GetDlgItem(iIndex)->SetWindowText(strTitle);
		}
		else
		{
			GetDlgItem(iIndex)->SetWindowText("");
			GetDlgItem(iIndex)->EnableWindow(false);
		}
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
