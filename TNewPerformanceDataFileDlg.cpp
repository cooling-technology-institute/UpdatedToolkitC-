// TNewPerformanceDataFileDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CTIToolkit.h"
#include "TNewPerformanceDataFileDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// TNewPerformanceDataFileDlg dialog


TNewPerformanceDataFileDlg::TNewPerformanceDataFileDlg(CWnd* pParent /*=NULL*/)
	: CDialog(TNewPerformanceDataFileDlg::IDD, pParent)
{
	m_strCopyFile = "";
	//{{AFX_DATA_INIT(TNewPerformanceDataFileDlg)
	m_strNewFileName = _T("");
	//}}AFX_DATA_INIT
}


void TNewPerformanceDataFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(TNewPerformanceDataFileDlg)
	DDX_Control(pDX, IDC_EDIT_NEW_FILE, m_wndNewFileEdit);
	DDX_Control(pDX, IDC_COMBO_FILE_LIST, m_wndDataFileList);
	DDX_Text(pDX, IDC_EDIT_NEW_FILE, m_strNewFileName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(TNewPerformanceDataFileDlg, CDialog)
	//{{AFX_MSG_MAP(TNewPerformanceDataFileDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// TNewPerformanceDataFileDlg message handlers

BOOL TNewPerformanceDataFileDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// Get list of data Files
	//
	CString strPath;

	GetModuleFileName(NULL, strPath.GetBuffer(MAX_PATH+1), MAX_PATH);
	strPath.ReleaseBuffer();
	strPath = strPath.Left(strPath.ReverseFind('\\')+1);
	strPath += "*.bbp";

	WIN32_FIND_DATA fd;
	HANDLE handle = FindFirstFile(strPath, &fd);

	m_wndDataFileList.AddString("None");

	if (handle != INVALID_HANDLE_VALUE) 
	{
		BOOL bFound = true;
		while (bFound)
		{
			m_wndDataFileList.AddString(fd.cFileName);
			bFound = FindNextFile(handle, &fd);
		}
		FindClose(handle);
	}

	m_wndDataFileList.SelectString(0, "None");

	m_wndNewFileEdit.SetFocus();
	m_wndNewFileEdit.SetSel(0, -1);

	return false;
}

void TNewPerformanceDataFileDlg::OnOK() 
{
	UpdateData();
	m_strCopyFile = "";

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
		MessageBox("Please do not use . in the name of the Performance data.");
		m_wndNewFileEdit.SetFocus();
		m_wndNewFileEdit.SetSel(0, -1);
		return;
	}

	try
	{
		m_strNewFileName += ".bbp";
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
	}
	catch(...)
	{
	}

	m_wndDataFileList.GetLBText(m_wndDataFileList.GetCurSel(), m_strCopyFile);

	if (m_strCopyFile == "None")
	{
		m_strCopyFile = "";
	}

	CDialog::OnOK();
	m_strNewFileName += ".bbp";
}
