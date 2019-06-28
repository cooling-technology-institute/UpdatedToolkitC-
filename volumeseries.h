#if !defined(AFX_VOLUMESERIES_H__4BE02FC8_0229_11D2_AE48_00400141862D__INCLUDED_)
#define AFX_VOLUMESERIES_H__4BE02FC8_0229_11D2_AE48_00400141862D__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


// Dispatch interfaces referenced by this interface
class CValueList;
class CPen1;

/////////////////////////////////////////////////////////////////////////////
// CVolumeSeries wrapper class

class CVolumeSeries : public COleDispatchDriver
{
public:
	CVolumeSeries() {}		// Calls COleDispatchDriver default constructor
	CVolumeSeries(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CVolumeSeries(const CVolumeSeries& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	CValueList GetVolumeValues();
	CPen1 GetLinePen();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VOLUMESERIES_H__4BE02FC8_0229_11D2_AE48_00400141862D__INCLUDED_)
