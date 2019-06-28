//---------------------------------------------------------------------
//
// ListCtrl.cpp : implementation file
//
// Copyright © 1997-2017 Cooling Technology Institute Corporation  -  All Rights Reserved.
//
// $Archive: /BlueBook 2.0/ListCtrl.cpp $
// $Revision: 1 $
// $Modtime: 2/02/99 10:14a $
//
//---------------------------------------------------------------------
//
// $Log: /BlueBook 2.0/ListCtrl.cpp $
// 
// 1     11/06/00 9:45a Deanp
// Initial 2.0 source
// 
// 3     2/02/99 11:14a Administrator
// 
// 2     1/07/99 2:09p Administrator
// 
// 1     12/01/98 9:48a Administrator
// updated baseline
// 
// 1     11/17/98 7:42a Administrator
// baseline
// 
// 1     11/17/98 7:41a Administrator
// baseline
// 
// 1     11/09/98 9:13a Administrator
// base
// 
// 1     11/09/98 9:10a Administrator
// base
// 
// 4     1/02/98 3:54p Lizb
// Took out the OnLButtonDown and fnItemSelected functions
// 
// 3     12/22/97 8:58a Lizb
// 
// 2     12/18/97 9:07a Lizb
// 
// 1     12/15/97 1:56p Admin
//
//---------------------------------------------------------------------

#include "stdafx.h"
#include "ListCtrl.h"
//#include "PrivateMessages.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CListCtrlEx

CListCtrlEx::CListCtrlEx()
{
	m_bCheckBoxes = FALSE;
}

CListCtrlEx::~CListCtrlEx()
{
}

BEGIN_MESSAGE_MAP(CListCtrlEx, CListCtrl)
	//{{AFX_MSG_MAP(CListCtrlEx)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CListCtrlEx message handlers

BOOL CListCtrlEx::AddCheckBoxes()
{
	m_bCheckBoxes = TRUE;

	return SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT, 
			LVS_EX_CHECKBOXES );
}

BOOL CListCtrlEx::AddColumn(int nSubItem, LPCTSTR strItem, int nMargin, int nFmt, int nMask)
{
	LV_COLUMN lvc;

	lvc.mask = nMask | LVS_EX_FULLROWSELECT | LVS_REPORT;
	lvc.fmt = nFmt;
    lvc.pszText = (LPTSTR) strItem;
	lvc.cx = GetStringWidth(lvc.pszText) + nMargin;
	lvc.iSubItem = nSubItem;

	return InsertColumn(nSubItem, &lvc);
}

BOOL CListCtrlEx::SetItem(int nItem, int nSubItem, LPCTSTR strItem, BOOL bChecked, int nImageIndex, 
						  BOOL bAddItem)
{
	LV_ITEM lvi;

	lvi.mask = LVIF_TEXT;
	lvi.iItem = nItem;
	lvi.iSubItem = nSubItem;

	lvi.pszText = (LPTSTR) strItem;

	if (nImageIndex != -1) {
		lvi.mask |= LVIF_IMAGE;
		lvi.iImage = nImageIndex;
	}

	BOOL bReturn = bAddItem ? InsertItem(&lvi) : CListCtrl::SetItem(&lvi);

	if (nSubItem == 0 && m_bCheckBoxes)
		SetChecked(nItem, bChecked);

	return bReturn;
}

BOOL CListCtrlEx::AddItem(int nItem, int nSubItem, LPCTSTR strItem, BOOL bChecked, int nImageIndex)
{	
	return SetItem(nItem, nSubItem, strItem, bChecked, nImageIndex, nSubItem == 0 ? TRUE : FALSE);  
}

BOOL CListCtrlEx::GetChecked(int nItem)
{
	return ListView_GetCheckState(m_hWnd, nItem);
}

BOOL CListCtrlEx::SetChecked(int nItem, BOOL bChecked)
{
	LV_ITEM lvi;

	lvi.mask = LVIF_STATE;
	lvi.iItem = nItem;
	lvi.iSubItem = 0;
	lvi.stateMask = LVIS_STATEIMAGEMASK;
	lvi.state = (bChecked ? 2 : 1) << 12;

	return CListCtrl::SetItem(&lvi);
}

BOOL CListCtrlEx::ToggleChecked(int nItem)
{
	return SetChecked(nItem, !GetChecked(nItem));
}

BOOL CListCtrlEx::ToggleHitTest(CPoint point)
{
	BOOL	bReturn = FALSE;
	UINT	uFlags = 0;

	int nItem = HitTest(point, &uFlags);

	if (uFlags & LVHT_ONITEMSTATEICON) {
		ToggleChecked(nItem);
		bReturn = TRUE;
	}

	return bReturn;
}

void CListCtrlEx::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	if (m_bCheckBoxes) {
		UINT	uFlags;

		int nItem = HitTest(point, &uFlags);

		if (uFlags & LVHT_ONITEMSTATEICON)
			ToggleChecked(nItem);
	}
	
	CListCtrl::OnLButtonDblClk(nFlags, point);
}


