// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


#include "stdafx.h"
#include "fastlineseries.h"

// Dispatch interfaces referenced by this interface
#include "pen.h"


/////////////////////////////////////////////////////////////////////////////
// CFastLineSeries properties

/////////////////////////////////////////////////////////////////////////////
// CFastLineSeries operations

CPen1 CFastLineSeries::GetLinePen()
{
	LPDISPATCH pDispatch;
	InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&pDispatch, NULL);
	return CPen1(pDispatch);
}

long CFastLineSeries::AddRealTime(double X, double Y, LPCTSTR ALabel, unsigned long AColor)
{
	long result;
	static BYTE parms[] =
		VTS_R8 VTS_R8 VTS_BSTR VTS_I4;
	InvokeHelper(0x2, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		X, Y, ALabel, AColor);
	return result;
}