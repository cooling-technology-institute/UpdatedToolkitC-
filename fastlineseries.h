#if !defined(AFX_FASTLINESERIES_H__4BE02FC1_0229_11D2_AE48_00400141862D__INCLUDED_)
#define AFX_FASTLINESERIES_H__4BE02FC1_0229_11D2_AE48_00400141862D__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


// Dispatch interfaces referenced by this interface
class CPen1;

/////////////////////////////////////////////////////////////////////////////
// CFastLineSeries wrapper class

class CFastLineSeries : public COleDispatchDriver
{
public:
	CFastLineSeries() {}		// Calls COleDispatchDriver default constructor
	CFastLineSeries(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CFastLineSeries(const CFastLineSeries& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	CPen1 GetLinePen();
	long AddRealTime(double X, double Y, LPCTSTR ALabel, unsigned long AColor);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FASTLINESERIES_H__4BE02FC1_0229_11D2_AE48_00400141862D__INCLUDED_)
