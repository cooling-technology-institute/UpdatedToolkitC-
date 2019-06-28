#if !defined(AFX_TMULTIEDIT_H__FC2424E1_B52C_11D2_A7A3_00C04F73051D__INCLUDED_)
#define AFX_TMULTIEDIT_H__FC2424E1_B52C_11D2_A7A3_00C04F73051D__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// TMultiEdit.h : header file
//
#include "TPropPageThree.h"

/////////////////////////////////////////////////////////////////////////////
// TMultiEdit dialog

class TMultiEdit : public CDialog
{
// Construction
public:
	void m_fnGetData(bool* pabApproach, bool &bCoef, bool &bLG, bool &bKAVL, bool &bTargetApproach, bool &bUserApproach, double &dblUserApproach);
	void m_fnInitData(double *listApp, bool* pabApproach, bool bCoef, bool bLG, bool bKAVL, bool bTargetApproach, bool bUserApproach, double dblUserApproach);
	TMultiEdit(CWnd* pParent = NULL);   // standard constructor
    virtual ~TMultiEdit();
// Dialog Data
	//{{AFX_DATA(TMultiEdit)
	enum { IDD = IDD_MULTIEDIT };
	BOOL	m_bchk1;
	BOOL	m_bchk10;
	BOOL	m_bchk11;
	BOOL	m_bchk12;
	BOOL	m_bchk13;
	BOOL	m_bchk14;
	BOOL	m_bchk15;
	BOOL	m_bchk16;
	BOOL	m_bchk17;
	BOOL	m_bchk18;
	BOOL	m_bchk19;
	BOOL	m_bchk3;
	BOOL	m_bchk4;
	BOOL	m_bchk5;
	BOOL	m_bchk6;
	BOOL	m_bchk7;
	BOOL	m_bchk8;
	BOOL	m_bchk9;
	BOOL	m_bCoef;
	double	m_dblUserApproach;
	BOOL	m_bKAVL;
	BOOL	m_bLG;
	BOOL	m_bTargetApproach;
	BOOL	m_bUserApproach;
	//}}AFX_DATA
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(TMultiEdit)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	double m_listApp[50];

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(TMultiEdit)
	virtual void OnOK();
	afx_msg void OnButtonOff();
	afx_msg void OnButtonOn();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TMULTIEDIT_H__FC2424E1_B52C_11D2_A7A3_00C04F73051D__INCLUDED_)
