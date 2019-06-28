// TNewDemandCurveDataDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CTIToolkit.h"
#include "TNewDemandCurveDataDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// TNewDemandCurveDataDlg dialog


TNewDemandCurveDataDlg::TNewDemandCurveDataDlg(CWnd* pParent /*=NULL*/)
	: CDialog(TNewDemandCurveDataDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(TNewDemandCurveDataDlg)
	m_strNewFileName = _T("");
	//}}AFX_DATA_INIT
}


void TNewDemandCurveDataDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(TNewDemandCurveDataDlg)
	DDX_Control(pDX, IDC_EDIT_DATANAME, m_wndNewFileEdit);
	DDX_Text(pDX, IDC_EDIT_DATANAME, m_strNewFileName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(TNewDemandCurveDataDlg, CDialog)
	//{{AFX_MSG_MAP(TNewDemandCurveDataDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// TNewDemandCurveDataDlg message handlers

void TNewDemandCurveDataDlg::OnOK() 
{
	UpdateData();

	if (m_strNewFileName.GetLength() == 0)
	{
		MessageBox("Please enter the name for the file");
		m_wndNewFileEdit.SetFocus();
		m_wndNewFileEdit.SetSel(0, -1);
		return;
	}

	int iExt = m_strNewFileName.ReverseFind('.');
	if (iExt != -1)
	{
		MessageBox("Please do not use . in the name of the Demand Curve data.");
		m_wndNewFileEdit.SetFocus();
		m_wndNewFileEdit.SetSel(0, -1);
		return;
	}

	try
	{
		m_strNewFileName += ".bbd";
		CFileStatus stStatus;
		CString strPath;

		GetModuleFileName(NULL, strPath.GetBuffer(MAX_PATH+1), MAX_PATH);
		strPath.ReleaseBuffer();
		strPath = strPath.Left(strPath.ReverseFind('\\')+1);
		strPath += m_strNewFileName;
		
		if (CFile::GetStatus(strPath, stStatus) == TRUE)
		{
			MessageBox("This file already exists please choose another file name.");
			m_wndNewFileEdit.SetFocus();
			m_wndNewFileEdit.SetSel(0, -1);
			return;
		}
		UpdateData(false);
	}
	catch(...)
	{
	}
	
	CDialog::OnOK();
}
