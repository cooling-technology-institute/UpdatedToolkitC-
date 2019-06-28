// TTowerPerformanceFile.h: interface for the TTowerPerformanceFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TTOWERPERFORMANCEFILE_H__72BC8DFA_620D_46E1_A11D_EC013BB901E1__INCLUDED_)
#define AFX_TTOWERPERFORMANCEFILE_H__72BC8DFA_620D_46E1_A11D_EC013BB901E1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MIN_FLOWS	3
#define MAX_FLOWS	5
#define MIN_RANGES	3
#define MAX_RANGES	5
#define MIN_WBTS	5
#define MAX_WBTS	6


//////////////////////////////////////////////////////////////////////
// Test Data Object
//////////////////////////////////////////////////////////////////////
class TPerformanceTest
{
public:
	void m_fnSetBarometricPressure(double dblVal, bool bIP);
	void m_fnSetFanDriverPower(double dblVal, bool bIP);
	void m_fnSetDBT(double dblVal, bool bIP);
	void m_fnSetWBT(double dblVal, bool bIP);
	void m_fnSetCWT(double dblVal, bool bIP);
	void m_fnSetHWT(double dblVal, bool bIP);
	void m_fnSetWaterFlowRate(double dblVal, bool bIP);
	void m_fnSetName(LPCTSTR lpszName);

	
	double m_fnGetBarometricPressure(bool bIP);
	double m_fnGetFanDriverPower(bool bIP);
	double m_fnGetDBT(bool bIP);
	double m_fnGetWBT(bool bIP);
	double m_fnGetCWT(bool bIP);
	double m_fnGetHWT(bool bIP);
	double m_fnGetWaterFlowRate(bool bIP);
	CString m_fnGetName();


public:
	void m_fnConvertAllToSI();
	void m_fnConvertAllToIP();
	void m_fnSetUnits(bool bIP);
	bool m_fnGetUnits();
	TPerformanceTest();
	virtual ~TPerformanceTest();

	TPerformanceTest& operator =(TPerformanceTest& rPerfTest);



private:
	CString m_strName;
	bool	m_bIPUnits;

	double	m_dblTestWaterFlowRate;
	double	m_dblTestHWT;
	double	m_dblTestCWT;
	double	m_dblTestWBT;
	double	m_dblTestDBT;
	double	m_dblTestFanDriverPower;
	double	m_dblTestBarometricPressure;
};

//////////////////////////////////////////////////////////////////////
// Design Data Object
//////////////////////////////////////////////////////////////////////
class TPerformanceDesign
{
public:
	void m_fnGetFlowArray(double pFlows[MAX_FLOWS], bool bIP);
	void m_fnGetRangeArray(double pRanges[MAX_RANGES], bool bIP);
	void m_fnGetWBTArray(double pWBTs[MAX_FLOWS][MAX_WBTS], bool bIP);
	void m_fnGetCWTArray(double pCWTs[MAX_FLOWS][MAX_WBTS][MAX_RANGES], bool bIP);

	void m_fnSetCWT(double dblVal, int iFlow, int iWBT, int iRange, bool bIP);
	void m_fnSetWBT(double dblVal, int iFlow, int iWBT, bool bIP);
	void m_fnSetRange(double dblVal, int iRange, bool bIP);
	void m_fnSetFlow(double dblVal, int iFlow, bool bIP);
	void m_fnSetWBTCnt(int iVal, int iFlow);
	void m_fnSetRangeCnt(int iVal);
	void m_fnSetFlowCnt(int iVal);

	double m_fnGetCWT(int iFlow, int iWBT, int iRange, bool bIP);
	double m_fnGetWBT(int iFlow, int iWBT, bool bIP);
	double m_fnGetRange(int iRange, bool bIP);
	double m_fnGetFlow(int iFlow, bool bIP);
	int m_fnGetWBTCnt(int iFlow);
	int m_fnGetRangeCnt();
	int m_fnGetFlowCnt();

	
	void m_fnSetLG(double dblVal, bool bIP);
	void m_fnSetBarometricPressure(double dblVal, bool bIP);
	void m_fnSetFanDriverPower(double dblVal, bool bIP);
	void m_fnSetDBT(double dblVal, bool bIP);
	void m_fnSetWBT(double dblVal, bool bIP);
	void m_fnSetCWT(double dblVal, bool bIP);
	void m_fnSetHWT(double dblVal, bool bIP);
	void m_fnSetWaterFlowRate(double dblVal, bool bIP);

	
	double m_fnGetLG(bool bIP);
	double m_fnGetBarometricPressure(bool bIP);
	double m_fnGetFanDriverPower(bool bIP);
	double m_fnGetDBT(bool bIP);
	double m_fnGetWBT(bool bIP);
	double m_fnGetCWT(bool bIP);
	double m_fnGetHWT(bool bIP);
	double m_fnGetWaterFlowRate(bool bIP);


	void m_fnSetTowerOwner(LPCTSTR lpszVal);
	void m_fnSetProjectName(LPCTSTR lpszVal);
	void m_fnSetTowerLocation(LPCTSTR lpszVal);
	void m_fnSetTowerManufacturer(LPCTSTR lpszVal);
	void m_fnSetManufacturerData(bool bVal);
	void m_fnSetFileName(LPCTSTR lpszVal);
	void m_fnSetInducedFlow(bool bVal);

	bool m_fnGetInducedFlow();
	CString m_fnGetTowerOwner();
	CString m_fnGetProjectName();
	CString m_fnGetTowerLocation();
	CString m_fnGetTowerManufacturer();
	CString m_fnGetFileName();
	bool m_fnGetManufacturerData();


	void m_fnConvertAllToSI();
	void m_fnConvertAllToIP();
	void m_fnSetUnits(bool bIP);
	bool m_fnGetUnits();

public:
	void m_fnDeleteFlow(int iDelIndex);
	bool m_fnCanModify();
	TPerformanceDesign();
	virtual ~TPerformanceDesign();

	TPerformanceDesign& operator =(TPerformanceDesign& rPerfTest);

private:
	bool	m_bIPUnits;
	//
	// Tower info
	// 
	bool	m_bManufacturerData;
	CString	m_strTowerOwner;
	CString	m_strProjectName;
	CString	m_strTowerLocation;
	CString	m_strTowerManufacturer;
	CString m_strFileName;
	bool	m_bInducedFlow;

	//
	// Design Point
	// 
	double	m_dblDesignWaterFlowRate;
	double	m_dblDesignHWT;
	double	m_dblDesignCWT;
	double	m_dblDesignWBT;
	double	m_dblDesignDBT;
	double	m_dblDesignFanDriverPower;
	double	m_dblDesignBarometricPressure;
	double	m_dblDesignLG;

	//
	// Curve Data
	//
	int m_iFlowCnt;
	int m_iRangeCnt;
	int m_iWBTCnt[MAX_FLOWS];

	double m_dblFlows[MAX_FLOWS];
	double m_dblRanges[MAX_RANGES];
	double m_dblWBTs[MAX_FLOWS][MAX_WBTS];
	double m_dblCWTs[MAX_FLOWS][MAX_WBTS][MAX_RANGES];
};




//////////////////////////////////////////////////////////////////////
// File Object
//////////////////////////////////////////////////////////////////////
class TTowerPerformanceFile
{
public:
	bool m_fnCanModify();
	void m_fnSetUnits(bool bIP);
	void m_fnInit();
	void m_fnWriteItemValue(LPCTSTR lpszSection, LPCTSTR lpszItem, LPCTSTR lpszValue);
	void m_fnWriteItemValue(LPCTSTR lpszSection, LPCTSTR lpszItem, double dblValue);
	CString m_fnGetItemValue(LPCTSTR lpszSection, LPCTSTR lpszItem, LPCTSTR lpszDefault);
	double m_fnGetItemValue(LPCTSTR lpszSection, LPCTSTR lpszItem, double dblDefault = 0.0);
	bool m_fnSaveFile(LPCTSTR lpszFileName = NULL);
	bool m_fnOpenFile(LPCTSTR lpszFileName);
	TTowerPerformanceFile();
	virtual ~TTowerPerformanceFile();


	TTowerPerformanceFile& operator =(TTowerPerformanceFile& fileSrc);

	//
	// Support Functions
	//
	void WritePrivateProfileDouble(LPCTSTR lpszSection, LPCTSTR lpszItem, double dblValue, LPCTSTR lpszFile, int iDecimal = 4);
	void WritePrivateProfileInt(LPCTSTR lpszSection, LPCTSTR lpszItem, int iValue, LPCTSTR lpszFile);

	//
	// Data
	//
	CString m_strFileName;

	TPerformanceDesign m_designData;
	CArray<TPerformanceTest, TPerformanceTest&>	m_arrayTests;
};


#endif // !defined(AFX_TTOWERPERFORMANCEFILE_H__72BC8DFA_620D_46E1_A11D_EC013BB901E1__INCLUDED_)
