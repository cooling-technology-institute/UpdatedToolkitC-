//---------------------------------------------------------------------
//
// ListCtrl.h : header file
//
// Copyright © 1997-2017 Cooling Technology Institute Corporation  -  All Rights Reserved.
//
// $Archive: /Toolkit/ListCtrl.h $
// $Revision: 2 $
// $Modtime: 6/22/01 2:18a $
//
//---------------------------------------------------------------------
//
// $Log: /Toolkit/ListCtrl.h $
// 
// 2     7/04/01 4:02a Davidl
// Change include statement.
// 
// 1     11/06/00 9:45a Deanp
// Initial 2.0 source
// 
// 2     1/07/99 2:15p Administrator
// 
// 1     12/01/98 9:50a Administrator
// updated baseline
// 
// 1     11/17/98 7:47a Administrator
// baseline
// 
// 1     11/09/98 9:16a Administrator
// base
// 
// 1     11/09/98 9:10a Administrator
// base
// 
// 4     1/02/98 3:53p Lizb
// Took out the OnLButtonDown and fnItemSelected functions
// 
// 3     12/22/97 8:58a Lizb
// 
// 2     12/18/97 9:04a Lizb
// 
// 1     12/15/97 1:55p Admin
//
//---------------------------------------------------------------------
#if !defined(AFX_LISTCTRL_H__261FA6AE_608D_11D1_8704_00A0C906B782__INCLUDED_)
#define AFX_LISTCTRL_H__261FA6AE_608D_11D1_8704_00A0C906B782__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <afxcmn.h>

/////////////////////////////////////////////////////////////////////////////
// CListCtrlEx window

class CListCtrlEx : public CListCtrl
{
// Construction
public:
	CListCtrlEx();

// Attributes
public:

	BOOL		m_bCheckBoxes;

// Operations
public:
	BOOL AddCheckBoxes();
	CImageList* SetImageList(CImageList* pil, int nImageListType = TVSIL_NORMAL)
		{	return CListCtrl::SetImageList(pil, nImageListType);  }
	BOOL AddColumn(int nSubItem, LPCTSTR strItem, int nMargin = 32, int nFmt = LVCFMT_LEFT,
			int nMask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM);
	BOOL SetItem(int nItem, int nSubItem, LPCTSTR strItem, BOOL bChecked = FALSE, int 
			nImageIndex = -1, BOOL bAddItem = FALSE);
	BOOL AddItem(int nItem, int nSubItem, LPCTSTR strItem, BOOL bChecked = FALSE, int 
			nImageIndex = -1);
	BOOL GetChecked(int nItem);
	BOOL SetChecked(int nItem, BOOL bChecked);
	BOOL IsChecked(int nItem)
		{	return GetChecked(nItem);  }
	BOOL ToggleChecked(int nItem);
	BOOL ToggleHitTest(CPoint point);
//	virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListCtrlEx)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CListCtrlEx();

	// Generated message map functions
protected:
	//{{AFX_MSG(CListCtrlEx)
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LISTCTRL_H__261FA6AE_608D_11D1_8704_00A0C906B782__INCLUDED_)
