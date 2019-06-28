// TTowerPerformanceFile.cpp: implementation of the TTowerPerformanceFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CTIToolkit.h"
#include "CTIUtils.h"
#include "TTowerPerformanceFile.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


TPerformanceTest::TPerformanceTest()
{
	m_strName	= "";
	m_bIPUnits	= !TPropPageBase::metricflag;

	m_dblTestWaterFlowRate		= 0.0;
	m_dblTestHWT				= 0.0;
	m_dblTestCWT				= 0.0;
	m_dblTestWBT				= 0.0;
	m_dblTestDBT				= 0.0;
	m_dblTestFanDriverPower		= 0.0;
	m_dblTestBarometricPressure	= 0.0;
}
TPerformanceTest::~TPerformanceTest()
{
}

TPerformanceTest& TPerformanceTest::operator =(TPerformanceTest& rPerfTest)
{
	m_strName					= rPerfTest.m_strName;
	m_bIPUnits					= rPerfTest.m_bIPUnits;

	m_dblTestWaterFlowRate		= rPerfTest.m_dblTestWaterFlowRate;
	m_dblTestHWT				= rPerfTest.m_dblTestHWT;
	m_dblTestCWT				= rPerfTest.m_dblTestCWT;
	m_dblTestWBT				= rPerfTest.m_dblTestWBT;
	m_dblTestDBT				= rPerfTest.m_dblTestDBT;
	m_dblTestFanDriverPower		= rPerfTest.m_dblTestFanDriverPower;
	m_dblTestBarometricPressure	= rPerfTest.m_dblTestBarometricPressure;

	return *this;
}

CString TPerformanceTest::m_fnGetName()
{
	return m_strName;
}

double TPerformanceTest::m_fnGetWaterFlowRate(bool bIP)
{
	double dblReturn = m_dblTestWaterFlowRate;
	if (m_bIPUnits != bIP)
	{
		if (bIP)
		{
			dblReturn		= fnLSToGPM(dblReturn);
		}
		else
		{
			dblReturn		= fnGPMToLS(dblReturn);
		}
	}
	return dblReturn;
}

double TPerformanceTest::m_fnGetHWT(bool bIP)
{
	double dblReturn = m_dblTestHWT;
	if (m_bIPUnits != bIP)
	{
		if (bIP)
		{
			dblReturn		= fnCelcToFar(dblReturn);
		}
		else
		{
			dblReturn		= fnFarToCelc(dblReturn);
		}
	}
	return dblReturn;
}

double TPerformanceTest::m_fnGetCWT(bool bIP)
{
	double dblReturn = m_dblTestCWT;
	if (m_bIPUnits != bIP)
	{
		if (bIP)
		{
			dblReturn		= fnCelcToFar(dblReturn);
		}
		else
		{
			dblReturn		= fnFarToCelc(dblReturn);
		}
	}
	return dblReturn;
}

double TPerformanceTest::m_fnGetWBT(bool bIP)
{
	double dblReturn = m_dblTestWBT;
	if (m_bIPUnits != bIP)
	{
		if (bIP)
		{
			dblReturn		= fnCelcToFar(dblReturn);
		}
		else
		{
			dblReturn		= fnFarToCelc(dblReturn);
		}
	}
	return dblReturn;
}

double TPerformanceTest::m_fnGetDBT(bool bIP)
{
	double dblReturn = m_dblTestDBT;
	if (m_bIPUnits != bIP)
	{
		if (bIP)
		{
			dblReturn		= fnCelcToFar(dblReturn);
		}
		else
		{
			dblReturn		= fnFarToCelc(dblReturn);
		}
	}
	return dblReturn;
}

double TPerformanceTest::m_fnGetFanDriverPower(bool bIP)
{
	double dblReturn = m_dblTestFanDriverPower;
	if (m_bIPUnits != bIP)
	{
		if (bIP)
		{
			dblReturn		= fnKWToBHP(dblReturn);
		}
		else
		{
			dblReturn		= fnBHPToKW(dblReturn);
		}
	}
	return dblReturn;
}

double TPerformanceTest::m_fnGetBarometricPressure(bool bIP)
{
	double dblReturn = m_dblTestBarometricPressure;
	if (m_bIPUnits != bIP)
	{
		if (bIP)
		{
			dblReturn		= calcPressureF(KPAToPSI(dblReturn));
		}
		else
		{
			dblReturn		= calcPressureC(PSIToKPA(dblReturn));
		}
	}
	return dblReturn;
}

void TPerformanceTest::m_fnSetName(LPCTSTR lpszName)
{
	m_strName = lpszName;
}

void TPerformanceTest::m_fnSetWaterFlowRate(double dblVal, bool bIP)
{
	if (m_bIPUnits != bIP)
	{
		if (m_bIPUnits)
		{
			m_dblTestWaterFlowRate		= fnLSToGPM(dblVal);
		}
		else
		{
			m_dblTestWaterFlowRate		= fnGPMToLS(dblVal);
		}
	}
	else
	{
		m_dblTestWaterFlowRate = dblVal;
	}
}

void TPerformanceTest::m_fnSetHWT(double dblVal, bool bIP)
{
	if (m_bIPUnits != bIP)
	{
		if (m_bIPUnits)
		{
			m_dblTestHWT				= fnCelcToFar(dblVal);
		}
		else
		{
			m_dblTestHWT				= fnFarToCelc(dblVal);
		}
	}
	else
	{
		m_dblTestHWT = dblVal;
	}
}

void TPerformanceTest::m_fnSetCWT(double dblVal, bool bIP)
{
	if (m_bIPUnits != bIP)
	{
		if (m_bIPUnits)
		{
			m_dblTestCWT				= fnCelcToFar(dblVal);
		}
		else
		{
			m_dblTestCWT				= fnFarToCelc(dblVal);
		}
	}
	else
	{
		m_dblTestCWT = dblVal;
	}
}

void TPerformanceTest::m_fnSetWBT(double dblVal, bool bIP)
{
	if (m_bIPUnits != bIP)
	{
		if (m_bIPUnits)
		{
			m_dblTestWBT				= fnCelcToFar(dblVal);
		}
		else
		{
			m_dblTestWBT				= fnFarToCelc(dblVal);
		}
	}
	else
	{
		m_dblTestWBT = dblVal;
	}
}

void TPerformanceTest::m_fnSetDBT(double dblVal, bool bIP)
{
	if (m_bIPUnits != bIP)
	{
		if (m_bIPUnits)
		{
			m_dblTestDBT				= fnCelcToFar(dblVal);
		}
		else
		{
			m_dblTestDBT				= fnFarToCelc(dblVal);
		}
	}
	else
	{
		m_dblTestDBT = dblVal;
	}
}

void TPerformanceTest::m_fnSetFanDriverPower(double dblVal, bool bIP)
{
	if (m_bIPUnits != bIP)
	{
		if (m_bIPUnits)
		{
			m_dblTestFanDriverPower		= fnKWToBHP(dblVal);
		}
		else
		{
			m_dblTestFanDriverPower		= fnBHPToKW(dblVal);
		}
	}
	else
	{
		m_dblTestFanDriverPower = dblVal;
	}
}

void TPerformanceTest::m_fnSetBarometricPressure(double dblVal, bool bIP)
{
	if (m_bIPUnits != bIP)
	{
		if (m_bIPUnits)
		{
			m_dblTestBarometricPressure	= calcPressureF(KPAToPSI(dblVal));
		}
		else
		{
			m_dblTestBarometricPressure	= calcPressureC(PSIToKPA(dblVal));
		}
	}
	else
	{
		m_dblTestBarometricPressure = dblVal;
	}
}

bool TPerformanceTest::m_fnGetUnits()
{
	return m_bIPUnits;
}

void TPerformanceTest::m_fnSetUnits(bool bIP)
{
	if (m_bIPUnits != bIP)
	{
		if (bIP)
		{
			m_fnConvertAllToIP();
		}
		else
		{
			m_fnConvertAllToSI();
		}
	}
	m_bIPUnits = bIP;
}

void TPerformanceTest::m_fnConvertAllToIP()
{
	if (!m_bIPUnits)
	{
		m_dblTestHWT				= fnCelcToFar(m_dblTestHWT);
		m_dblTestCWT				= fnCelcToFar(m_dblTestCWT);
		m_dblTestWBT				= fnCelcToFar(m_dblTestWBT);
		m_dblTestDBT				= fnCelcToFar(m_dblTestDBT);
		m_dblTestWaterFlowRate		= fnLSToGPM(m_dblTestWaterFlowRate);
		m_dblTestFanDriverPower		= fnKWToBHP(m_dblTestFanDriverPower);
		m_dblTestBarometricPressure	= calcPressureF(KPAToPSI(m_dblTestBarometricPressure));

		m_bIPUnits = true;
	}
}

void TPerformanceTest::m_fnConvertAllToSI()
{
	if (m_bIPUnits)
	{
		m_dblTestHWT				= fnFarToCelc(m_dblTestHWT);
		m_dblTestCWT				= fnFarToCelc(m_dblTestCWT);
		m_dblTestWBT				= fnFarToCelc(m_dblTestWBT);
		m_dblTestDBT				= fnFarToCelc(m_dblTestDBT);
		m_dblTestWaterFlowRate		= fnGPMToLS(m_dblTestWaterFlowRate);
		m_dblTestFanDriverPower		= fnBHPToKW(m_dblTestFanDriverPower);
		m_dblTestBarometricPressure	= calcPressureC(PSIToKPA(m_dblTestBarometricPressure));

		m_bIPUnits = false;
	}
}






//////////////////////////////////////////////////////////////////////
// Design Data Object
//////////////////////////////////////////////////////////////////////
TPerformanceDesign::TPerformanceDesign()
{
	m_bIPUnits						= !TPropPageBase::metricflag;
	m_bManufacturerData				= false;

	m_strTowerOwner					= "";
	m_strProjectName				= "";
	m_strTowerLocation				= "";
	m_strTowerManufacturer			= "";
	m_strFileName					= "";
	m_bInducedFlow					= true;

	m_dblDesignWaterFlowRate		= 0.0;
	m_dblDesignHWT					= 0.0;
	m_dblDesignCWT					= 0.0;
	m_dblDesignWBT					= 0.0;
	m_dblDesignDBT					= 0.0;
	m_dblDesignFanDriverPower		= 0.0;
	m_dblDesignBarometricPressure	= 0.0;
	m_dblDesignLG					= 0.0;

	m_iFlowCnt						= 0;
	m_iRangeCnt						= 0;

	memset(m_iWBTCnt,	0, MAX_FLOWS * sizeof(double));
	memset(m_dblFlows,	0, MAX_FLOWS * sizeof(double));
	memset(m_dblRanges, 0, MAX_RANGES* sizeof(double));
	memset(m_dblWBTs,	0, MAX_FLOWS * MAX_WBTS * sizeof(double));
	memset(m_dblCWTs,	0, MAX_FLOWS * MAX_WBTS * MAX_RANGES * sizeof(double));
}
TPerformanceDesign::~TPerformanceDesign()
{
}

TPerformanceDesign& TPerformanceDesign::operator =(TPerformanceDesign& rPerfDesign)
{
	m_bIPUnits						=	rPerfDesign.m_bIPUnits;

	m_strTowerOwner					=	rPerfDesign.m_strTowerOwner;
	m_strProjectName				=	rPerfDesign.m_strProjectName;
	m_strTowerLocation				=	rPerfDesign.m_strTowerLocation;
	m_strTowerManufacturer			=	rPerfDesign.m_strTowerManufacturer;
	m_bManufacturerData				=	rPerfDesign.m_bManufacturerData;
	m_strFileName					=   rPerfDesign.m_strFileName;
	m_bInducedFlow					=	rPerfDesign.m_bInducedFlow;

	m_dblDesignWaterFlowRate		=	rPerfDesign.m_dblDesignWaterFlowRate;
	m_dblDesignHWT					=	rPerfDesign.m_dblDesignHWT;
	m_dblDesignCWT					=	rPerfDesign.m_dblDesignCWT;
	m_dblDesignWBT					=	rPerfDesign.m_dblDesignWBT;
	m_dblDesignDBT					=	rPerfDesign.m_dblDesignDBT;
	m_dblDesignFanDriverPower		=	rPerfDesign.m_dblDesignFanDriverPower;
	m_dblDesignBarometricPressure	=	rPerfDesign.m_dblDesignBarometricPressure;
	m_dblDesignLG					=	rPerfDesign.m_dblDesignLG;

	m_iFlowCnt						=	rPerfDesign.m_iFlowCnt;
	m_iRangeCnt						=	rPerfDesign.m_iRangeCnt;

	memcpy(m_iWBTCnt,	rPerfDesign.m_iWBTCnt,		MAX_FLOWS * sizeof(double));
	memcpy(m_dblFlows,	rPerfDesign.m_dblFlows,		MAX_FLOWS * sizeof(double));
	memcpy(m_dblRanges, rPerfDesign.m_dblRanges,	MAX_RANGES* sizeof(double));
	memcpy(m_dblWBTs,	rPerfDesign.m_dblWBTs,		MAX_FLOWS * MAX_WBTS * sizeof(double));
	memcpy(m_dblCWTs,	rPerfDesign.m_dblCWTs,		MAX_FLOWS * MAX_WBTS * MAX_RANGES * sizeof(double));

	return *this;
}

bool TPerformanceDesign::m_fnGetInducedFlow()
{
	return m_bInducedFlow;
}

CString TPerformanceDesign::m_fnGetTowerOwner()
{
	return m_strTowerOwner;
}

CString TPerformanceDesign::m_fnGetProjectName()
{
	return m_strProjectName;
}

CString TPerformanceDesign::m_fnGetTowerLocation()
{
	return m_strTowerLocation;
}

CString TPerformanceDesign::m_fnGetTowerManufacturer()
{
	return m_strTowerManufacturer;
}

CString TPerformanceDesign::m_fnGetFileName()
{
	return m_strFileName;
}

bool TPerformanceDesign::m_fnGetManufacturerData()
{
	return m_bManufacturerData;
}

bool TPerformanceDesign::m_fnCanModify()
{
	return !m_bManufacturerData;
}

void TPerformanceDesign::m_fnSetInducedFlow(bool bVal)
{
	m_bInducedFlow = bVal;
}

void TPerformanceDesign::m_fnSetTowerOwner(LPCTSTR lpszVal)
{
	m_strTowerOwner = lpszVal;
}

void TPerformanceDesign::m_fnSetProjectName(LPCTSTR lpszVal)
{
	m_strProjectName = lpszVal;
}

void TPerformanceDesign::m_fnSetTowerLocation(LPCTSTR lpszVal)
{
	m_strTowerLocation = lpszVal;
}

void TPerformanceDesign::m_fnSetTowerManufacturer(LPCTSTR lpszVal)
{
	m_strTowerManufacturer = lpszVal;
}

void TPerformanceDesign::m_fnSetFileName(LPCTSTR lpszVal)
{
	m_strFileName = lpszVal;
}

void TPerformanceDesign::m_fnSetManufacturerData(bool bVal)
{
	m_bManufacturerData = bVal;
}

double TPerformanceDesign::m_fnGetWaterFlowRate(bool bIP)
{
	double dblReturn = m_dblDesignWaterFlowRate;
	if (m_bIPUnits != bIP)
	{
		if (bIP)
		{
			dblReturn		= fnLSToGPM(dblReturn);
		}
		else
		{
			dblReturn		= fnGPMToLS(dblReturn);
		}
	}
	return dblReturn;
}

double TPerformanceDesign::m_fnGetHWT(bool bIP)
{
	double dblReturn = m_dblDesignHWT;
	if (m_bIPUnits != bIP)
	{
		if (bIP)
		{
			dblReturn		= fnCelcToFar(dblReturn);
		}
		else
		{
			dblReturn		= fnFarToCelc(dblReturn);
		}
	}
	return dblReturn;
}

double TPerformanceDesign::m_fnGetCWT(bool bIP)
{
	double dblReturn = m_dblDesignCWT;
	if (m_bIPUnits != bIP)
	{
		if (bIP)
		{
			dblReturn		= fnCelcToFar(dblReturn);
		}
		else
		{
			dblReturn		= fnFarToCelc(dblReturn);
		}
	}
	return dblReturn;
}

double TPerformanceDesign::m_fnGetWBT(bool bIP)
{
	double dblReturn = m_dblDesignWBT;
	if (m_bIPUnits != bIP)
	{
		if (bIP)
		{
			dblReturn		= fnCelcToFar(dblReturn);
		}
		else
		{
			dblReturn		= fnFarToCelc(dblReturn);
		}
	}
	return dblReturn;
}

double TPerformanceDesign::m_fnGetDBT(bool bIP)
{
	double dblReturn = m_dblDesignDBT;
	if (m_bIPUnits != bIP)
	{
		if (bIP)
		{
			dblReturn		= fnCelcToFar(dblReturn);
		}
		else
		{
			dblReturn		= fnFarToCelc(dblReturn);
		}
	}
	return dblReturn;
}

double TPerformanceDesign::m_fnGetFanDriverPower(bool bIP)
{
	double dblReturn = m_dblDesignFanDriverPower;
	if (m_bIPUnits != bIP)
	{
		if (bIP)
		{
			dblReturn		= fnKWToBHP(dblReturn);
		}
		else
		{
			dblReturn		= fnBHPToKW(dblReturn);
		}
	}
	return dblReturn;
}

double TPerformanceDesign::m_fnGetBarometricPressure(bool bIP)
{
	double dblReturn = m_dblDesignBarometricPressure;
	if (m_bIPUnits != bIP)
	{
		if (bIP)
		{
			dblReturn		= calcPressureF(KPAToPSI(dblReturn));
		}
		else
		{
			dblReturn		= calcPressureC(PSIToKPA(dblReturn));
		}
	}
	return dblReturn;
}

double TPerformanceDesign::m_fnGetLG(bool /*bIP*/)
{
	double dblReturn = m_dblDesignLG;

	return dblReturn;
}

void TPerformanceDesign::m_fnSetWaterFlowRate(double dblVal, bool bIP)
{
	if (m_bIPUnits != bIP)
	{
		if (m_bIPUnits)
		{
			m_dblDesignWaterFlowRate		= fnLSToGPM(dblVal);
		}
		else
		{
			m_dblDesignWaterFlowRate		= fnGPMToLS(dblVal);
		}
	}
	else
	{
		m_dblDesignWaterFlowRate = dblVal;
	}
}

void TPerformanceDesign::m_fnSetHWT(double dblVal, bool bIP)
{
	if (m_bIPUnits != bIP)
	{
		if (m_bIPUnits)
		{
			m_dblDesignHWT				= fnCelcToFar(dblVal);
		}
		else
		{
			m_dblDesignHWT				= fnFarToCelc(dblVal);
		}
	}
	else
	{
		m_dblDesignHWT = dblVal;
	}
}

void TPerformanceDesign::m_fnSetCWT(double dblVal, bool bIP)
{
	if (m_bIPUnits != bIP)
	{
		if (m_bIPUnits)
		{
			m_dblDesignCWT				= fnCelcToFar(dblVal);
		}
		else
		{
			m_dblDesignCWT				= fnFarToCelc(dblVal);
		}
	}
	else
	{
		m_dblDesignCWT = dblVal;
	}
}

void TPerformanceDesign::m_fnSetWBT(double dblVal, bool bIP)
{
	if (m_bIPUnits != bIP)
	{
		if (m_bIPUnits)
		{
			m_dblDesignWBT				= fnCelcToFar(dblVal);
		}
		else
		{
			m_dblDesignWBT				= fnFarToCelc(dblVal);
		}
	}
	else
	{
		m_dblDesignWBT = dblVal;
	}
}

void TPerformanceDesign::m_fnSetDBT(double dblVal, bool bIP)
{
	if (m_bIPUnits != bIP)
	{
		if (m_bIPUnits)
		{
			m_dblDesignDBT				= fnCelcToFar(dblVal);
		}
		else
		{
			m_dblDesignDBT				= fnFarToCelc(dblVal);
		}
	}
	else
	{
		m_dblDesignDBT = dblVal;
	}
}

void TPerformanceDesign::m_fnSetFanDriverPower(double dblVal, bool bIP)
{
	if (m_bIPUnits != bIP)
	{
		if (m_bIPUnits)
		{
			m_dblDesignFanDriverPower		= fnKWToBHP(dblVal);
		}
		else
		{
			m_dblDesignFanDriverPower		= fnBHPToKW(dblVal);
		}
	}
	else
	{
		m_dblDesignFanDriverPower = dblVal;
	}
}

void TPerformanceDesign::m_fnSetBarometricPressure(double dblVal, bool bIP)
{
	if (m_bIPUnits != bIP)
	{
		if (m_bIPUnits)
		{
			m_dblDesignBarometricPressure	= calcPressureF(KPAToPSI(dblVal));
		}
		else
		{
			m_dblDesignBarometricPressure	= calcPressureC(PSIToKPA(dblVal));
		}
	}
	else
	{
		m_dblDesignBarometricPressure = dblVal;
	}
}

void TPerformanceDesign::m_fnSetLG(double dblVal, bool /*bIP*/)
{
	m_dblDesignLG = dblVal;
}

void TPerformanceDesign::m_fnGetCWTArray(double pCWTs[MAX_FLOWS][MAX_WBTS][MAX_RANGES], bool bIP)
{
	memcpy(pCWTs, m_dblCWTs, sizeof(m_dblCWTs));

	if (m_bIPUnits != bIP)
	{
		if (bIP)
		{
			for (int iFlowIndex = 0; iFlowIndex < m_iFlowCnt; iFlowIndex++)
			{
				for (int iWBTIndex = 0; iWBTIndex < m_iWBTCnt[iFlowIndex]; iWBTIndex++)
				{
					for (int iCWTIndex = 0; iCWTIndex < m_iRangeCnt; iCWTIndex++)
					{
						pCWTs[iFlowIndex][iWBTIndex][iCWTIndex] = fnCelcToFar(pCWTs[iFlowIndex][iWBTIndex][iCWTIndex]);
					}
				}
			}
		}
		else
		{
			for (int iFlowIndex = 0; iFlowIndex < m_iFlowCnt; iFlowIndex++)
			{
				for (int iWBTIndex = 0; iWBTIndex < m_iWBTCnt[iFlowIndex]; iWBTIndex++)
				{
					for (int iCWTIndex = 0; iCWTIndex < m_iRangeCnt; iCWTIndex++)
					{
						pCWTs[iFlowIndex][iWBTIndex][iCWTIndex] = fnFarToCelc(pCWTs[iFlowIndex][iWBTIndex][iCWTIndex]);
					}
				}
			}
		}
	}
}

void TPerformanceDesign::m_fnGetWBTArray(double pWBTs[MAX_FLOWS][MAX_WBTS], bool bIP)
{
	memcpy(pWBTs, m_dblWBTs, sizeof(m_dblWBTs));

	if (m_bIPUnits != bIP)
	{
		if (bIP)
		{
			for (int iFlowIndex = 0; iFlowIndex < m_iFlowCnt; iFlowIndex++)
			{
				for (int iWBTIndex = 0; iWBTIndex < m_iWBTCnt[iFlowIndex]; iWBTIndex++)
				{
					pWBTs[iFlowIndex][iWBTIndex] = fnCelcToFar(pWBTs[iFlowIndex][iWBTIndex]);
				}
			}
		}
		else
		{
			for (int iFlowIndex = 0; iFlowIndex < m_iFlowCnt; iFlowIndex++)
			{
				for (int iWBTIndex = 0; iWBTIndex < m_iWBTCnt[iFlowIndex]; iWBTIndex++)
				{
					pWBTs[iFlowIndex][iWBTIndex] = fnFarToCelc(pWBTs[iFlowIndex][iWBTIndex]);
				}
			}
		}
	}
}

void TPerformanceDesign::m_fnGetRangeArray(double pRanges[MAX_RANGES], bool bIP)
{
	memcpy(pRanges, m_dblRanges, sizeof(m_dblRanges));

	if (m_bIPUnits != bIP)
	{
		if (bIP)
		{
			for (int iRangeIndex = 0; iRangeIndex < m_iRangeCnt; iRangeIndex++)
			{
				pRanges[iRangeIndex] = pRanges[iRangeIndex] * 1.8;
			}
		}
		else
		{
			for (int iRangeIndex = 0; iRangeIndex < m_iRangeCnt; iRangeIndex++)
			{
				pRanges[iRangeIndex] = pRanges[iRangeIndex] / 1.8;
			}
		}
	}
}

void TPerformanceDesign::m_fnGetFlowArray(double pFlows[MAX_FLOWS], bool bIP)
{
	memcpy(pFlows, m_dblFlows, sizeof(m_dblFlows));

	if (m_bIPUnits != bIP)
	{
		if (bIP)
		{
			for (int iFlowIndex = 0; iFlowIndex < m_iFlowCnt; iFlowIndex++)
			{
				pFlows[iFlowIndex] = fnLSToGPM(pFlows[iFlowIndex]);
			}
		}
		else
		{
			for (int iFlowIndex = 0; iFlowIndex < m_iFlowCnt; iFlowIndex++)
			{
				pFlows[iFlowIndex] = fnGPMToLS(pFlows[iFlowIndex]);
			}
		}
	}
}


int TPerformanceDesign::m_fnGetFlowCnt()
{
	return m_iFlowCnt;
}

int TPerformanceDesign::m_fnGetRangeCnt()
{
	return m_iRangeCnt;
}

int TPerformanceDesign::m_fnGetWBTCnt(int iFlow)
{
	if (iFlow < 0)
	{
		return 0;
	}
	else
	{
		return m_iWBTCnt[iFlow];
	}
}

void TPerformanceDesign::m_fnDeleteFlow(int iDelIndex)
{
	if ((iDelIndex >= 0) && (iDelIndex < MAX_FLOWS))
	{
		// Decrement the flow counter
		m_iFlowCnt--;

		// Move everything over one position
		for (int iFlowIndex = 0; iFlowIndex < MAX_FLOWS; iFlowIndex++)
		{
			if (iDelIndex < iFlowIndex)
			{
				m_dblFlows[iFlowIndex-1] = m_dblFlows[iFlowIndex];
				for (int iWBTIndex = 0; iWBTIndex < MAX_WBTS; iWBTIndex++)
				{
					m_dblWBTs[iFlowIndex-1][iWBTIndex] = m_dblWBTs[iFlowIndex][iWBTIndex];
					for (int iRangeIndex = 0; iRangeIndex < MAX_RANGES; iRangeIndex++)
					{
						m_dblCWTs[iFlowIndex-1][iWBTIndex][iRangeIndex] = m_dblCWTs[iFlowIndex][iWBTIndex][iRangeIndex];
					}
				}
			}
		}

		// Now, zero out everything from the last to the max position
		for (int iFlowIndex = m_iFlowCnt; iFlowIndex < MAX_FLOWS; iFlowIndex++)
		{
			m_dblFlows[iFlowIndex] = 0.0;
			for (int iWBTIndex = 0; iWBTIndex < MAX_WBTS; iWBTIndex++)
			{
				m_dblWBTs[iFlowIndex][iWBTIndex] = 0.0;
				for (int iRangeIndex = 0; iRangeIndex < MAX_RANGES; iRangeIndex++)
				{
					m_dblCWTs[iFlowIndex][iWBTIndex][iRangeIndex] = 0.0;
				}

			}
		}
	}
}

double TPerformanceDesign::m_fnGetFlow(int iFlow, bool bIP)
{
	ASSERT((iFlow >= 0) && (iFlow < MAX_FLOWS));

	if (iFlow < 0) return 0;
	double dblReturn = m_dblFlows[iFlow];
	if (m_bIPUnits != bIP)
	{
		if (bIP)
		{
			dblReturn		= fnLSToGPM(dblReturn);
		}
		else
		{
			dblReturn		= fnGPMToLS(dblReturn);
		}
	}
	return dblReturn;
}

double TPerformanceDesign::m_fnGetRange(int iRange, bool bIP)
{
	ASSERT((iRange >= 0) && (iRange < MAX_RANGES));

	if (iRange < 0) return 0;
	double dblReturn = m_dblRanges[iRange];
	if (m_bIPUnits != bIP)
	{
		if (bIP)
		{
			dblReturn		= dblReturn * 1.8;
		}
		else
		{
			dblReturn		= dblReturn / 1.8;
		}
	}
	return dblReturn;
}

double TPerformanceDesign::m_fnGetWBT(int iFlow, int iWBT, bool bIP)
{
	ASSERT((iFlow >= 0) && (iFlow < MAX_FLOWS));
	ASSERT((iWBT >= 0) && (iWBT < MAX_WBTS));

	if (iFlow < 0) return 0;
	double dblReturn = m_dblWBTs[iFlow][iWBT];
	if (m_bIPUnits != bIP)
	{
		if (bIP)
		{
			dblReturn		= fnCelcToFar(dblReturn);
		}
		else
		{
			dblReturn		= fnFarToCelc(dblReturn);
		}
	}
	return dblReturn;
}

double TPerformanceDesign::m_fnGetCWT(int iFlow, int iWBT, int iRange, bool bIP)
{
//	ASSERT((iFlow >= 0) && (iFlow < MAX_FLOWS));
	ASSERT(iFlow < MAX_FLOWS);
	ASSERT((iWBT >= 0) && (iWBT < MAX_WBTS));
	ASSERT((iRange >= 0) && (iRange < MAX_RANGES));

	if (iFlow < 0) return 0;
	double dblReturn = m_dblCWTs[iFlow][iWBT][iRange];
	if (m_bIPUnits != bIP)
	{
		if (bIP)
		{
			dblReturn		= fnCelcToFar(dblReturn);
		}
		else
		{
			dblReturn		= fnFarToCelc(dblReturn);
		}
	}
	return dblReturn;
}

void TPerformanceDesign::m_fnSetFlowCnt(int iVal)
{
	m_iFlowCnt = iVal;
}

void TPerformanceDesign::m_fnSetRangeCnt(int iVal)
{
	m_iRangeCnt = iVal;
}

void TPerformanceDesign::m_fnSetWBTCnt(int iVal, int iFlow)
{
	m_iWBTCnt[iFlow] = iVal;
}

void TPerformanceDesign::m_fnSetFlow(double dblVal, int iFlow, bool bIP)
{
	ASSERT((iFlow >= 0) && (iFlow < MAX_FLOWS));

	if (iFlow < 0) return;
	if (m_bIPUnits != bIP)
	{
		if (m_bIPUnits)
		{
			m_dblFlows[iFlow]	= fnLSToGPM(dblVal);
		}
		else
		{
			m_dblFlows[iFlow]	= fnGPMToLS(dblVal);
		}
	}
	else
	{
		m_dblFlows[iFlow] = dblVal;
	}
}

void TPerformanceDesign::m_fnSetRange(double dblVal, int iRange, bool bIP)
{
	ASSERT((iRange >= 0) && (iRange < MAX_RANGES));

	if (iRange < 0) return;
	if (m_bIPUnits != bIP)
	{
		if (m_bIPUnits)
		{
			m_dblRanges[iRange]				= dblVal * 1.8;
		}
		else
		{
			m_dblRanges[iRange]				= dblVal / 1.8;
		}
	}
	else
	{
		m_dblRanges[iRange] = dblVal;
	}
}

void TPerformanceDesign::m_fnSetWBT(double dblVal, int iFlow, int iWBT, bool bIP)
{
	ASSERT((iFlow >= 0) && (iFlow < MAX_FLOWS));
	ASSERT((iWBT >= 0) && (iWBT < MAX_WBTS));

	if (iFlow < 0) return;
	if (m_bIPUnits != bIP)
	{
		if (m_bIPUnits)
		{
			m_dblWBTs[iFlow][iWBT]				= fnCelcToFar(dblVal);
		}
		else
		{
			m_dblWBTs[iFlow][iWBT]				= fnFarToCelc(dblVal);
		}
	}
	else
	{
		m_dblWBTs[iFlow][iWBT] = dblVal;
	}
}

void TPerformanceDesign::m_fnSetCWT(double dblVal, int iFlow, int iWBT, int iRange, bool bIP)
{
	ASSERT((iFlow >= 0) && (iFlow < MAX_FLOWS));
	ASSERT((iWBT >= 0) && (iWBT < MAX_WBTS));
	ASSERT((iRange >= 0) && (iRange < MAX_RANGES));

	if (iFlow < 0) return;
	if (m_bIPUnits != bIP)
	{
		if (m_bIPUnits)
		{
			m_dblCWTs[iFlow][iWBT][iRange]				= fnCelcToFar(dblVal);
		}
		else
		{
			m_dblCWTs[iFlow][iWBT][iRange]				= fnFarToCelc(dblVal);
		}
	}
	else
	{
		m_dblCWTs[iFlow][iWBT][iRange] = dblVal;
	}
}

bool TPerformanceDesign::m_fnGetUnits()
{
	return m_bIPUnits;
}

void TPerformanceDesign::m_fnSetUnits(bool bIP)
{
	if (m_bIPUnits != bIP)
	{
		if (bIP)
		{
			m_fnConvertAllToIP();
		}
		else
		{
			m_fnConvertAllToSI();
		}
	}
	m_bIPUnits = bIP;
}

void TPerformanceDesign::m_fnConvertAllToIP()
{
	if (!m_bIPUnits)
	{
		m_dblDesignHWT					= fnCelcToFar(m_dblDesignHWT);
		m_dblDesignCWT					= fnCelcToFar(m_dblDesignCWT);
		m_dblDesignWBT					= fnCelcToFar(m_dblDesignWBT);
		m_dblDesignDBT					= fnCelcToFar(m_dblDesignDBT);
		m_dblDesignWaterFlowRate		= fnLSToGPM(m_dblDesignWaterFlowRate);
		m_dblDesignFanDriverPower		= fnKWToBHP(m_dblDesignFanDriverPower);
		m_dblDesignBarometricPressure	= calcPressureF(KPAToPSI(m_dblDesignBarometricPressure));

		for (int iRangeIndex = 0; iRangeIndex < m_iRangeCnt; iRangeIndex++)
		{
			m_dblRanges[iRangeIndex] = m_dblRanges[iRangeIndex] * 1.8;
		}

		for (int iFlowIndex = 0; iFlowIndex < m_iFlowCnt; iFlowIndex++)
		{
			m_dblFlows[iFlowIndex] = fnLSToGPM(m_dblFlows[iFlowIndex]);

			for (int iWBTIndex = 0; iWBTIndex < m_iWBTCnt[iFlowIndex]; iWBTIndex++)
			{
				m_dblWBTs[iFlowIndex][iWBTIndex] = fnCelcToFar(m_dblWBTs[iFlowIndex][iWBTIndex]);

				for (int iCWTIndex = 0; iCWTIndex < m_iRangeCnt; iCWTIndex++)
				{
					m_dblCWTs[iFlowIndex][iWBTIndex][iCWTIndex] = fnCelcToFar(m_dblCWTs[iFlowIndex][iWBTIndex][iCWTIndex]);
				}
			}
		}

		m_bIPUnits = true;
	}
}

void TPerformanceDesign::m_fnConvertAllToSI()
{
	if (m_bIPUnits)
	{
		m_dblDesignHWT					= fnFarToCelc(m_dblDesignHWT);
		m_dblDesignCWT					= fnFarToCelc(m_dblDesignCWT);
		m_dblDesignWBT					= fnFarToCelc(m_dblDesignWBT);
		m_dblDesignDBT					= fnFarToCelc(m_dblDesignDBT);
		m_dblDesignWaterFlowRate		= fnGPMToLS(m_dblDesignWaterFlowRate);
		m_dblDesignFanDriverPower		= fnBHPToKW(m_dblDesignFanDriverPower);
		m_dblDesignBarometricPressure	= calcPressureC(PSIToKPA(m_dblDesignBarometricPressure));

		for (int iRangeIndex = 0; iRangeIndex < m_iRangeCnt; iRangeIndex++)
		{
			m_dblRanges[iRangeIndex] = m_dblRanges[iRangeIndex] / 1.8;
		}

		for (int iFlowIndex = 0; iFlowIndex < m_iFlowCnt; iFlowIndex++)
		{
			m_dblFlows[iFlowIndex] = fnGPMToLS(m_dblFlows[iFlowIndex]);

			for (int iWBTIndex = 0; iWBTIndex < m_iWBTCnt[iFlowIndex]; iWBTIndex++)
			{
				m_dblWBTs[iFlowIndex][iWBTIndex] = fnFarToCelc(m_dblWBTs[iFlowIndex][iWBTIndex]);

				for (int iCWTIndex = 0; iCWTIndex < m_iRangeCnt; iCWTIndex++)
				{
					m_dblCWTs[iFlowIndex][iWBTIndex][iCWTIndex] = fnFarToCelc(m_dblCWTs[iFlowIndex][iWBTIndex][iCWTIndex]);
				}
			}
		}

		m_bIPUnits = false;
	}
}






//////////////////////////////////////////////////////////////////////
// File object
//////////////////////////////////////////////////////////////////////

TTowerPerformanceFile::TTowerPerformanceFile()
{
	m_fnInit();
}

TTowerPerformanceFile::~TTowerPerformanceFile()
{

}

TTowerPerformanceFile& TTowerPerformanceFile::operator =(TTowerPerformanceFile& fileSrc)
{
	m_fnInit();

	m_strFileName		= fileSrc.m_strFileName;
	m_designData		= fileSrc.m_designData;

	for (int iTestIndex = 0; iTestIndex <= fileSrc.m_arrayTests.GetUpperBound(); iTestIndex++)
	{
		TPerformanceTest stTest = fileSrc.m_arrayTests[iTestIndex];
		m_arrayTests.Add(stTest);
	}

	return *this;
}

//
// File Handling
//
bool TTowerPerformanceFile::m_fnOpenFile(LPCTSTR lpszFileName)
{
	bool bReturn = false;
	CString strSection;
	CString strItem;
	char szValue[256];
	bool bIPUnits;

	m_fnInit();

	m_strFileName = lpszFileName;
	m_designData.m_fnSetFileName(lpszFileName);

	//
	// Tower Data
	//
	strSection = "Tower Info";
	GetPrivateProfileString(strSection, "Owner", "", szValue, 256, m_strFileName);
	m_designData.m_fnSetTowerOwner(szValue);
	GetPrivateProfileString(strSection, "ProjectName", "", szValue, 256, m_strFileName);
	m_designData.m_fnSetProjectName(szValue);
	GetPrivateProfileString(strSection, "Location", "", szValue, 256, m_strFileName);
	m_designData.m_fnSetTowerLocation(szValue);
	GetPrivateProfileString(strSection, "Manufacturer", "", szValue, 256, m_strFileName);
	m_designData.m_fnSetTowerManufacturer(szValue);
	m_designData.m_fnSetManufacturerData(GetPrivateProfileInt(strSection, "ManufacturerData", 0, m_strFileName) != 0);
	m_designData.m_fnSetInducedFlow(GetPrivateProfileInt(strSection, "InducedFlow", 1, m_strFileName) != 0);

	//
	// Design Point data
	//
	strSection = "Tower Design Point";
	GetPrivateProfileString(strSection, "Units", "IP", szValue, 256, m_strFileName);
	if (CString("IP") == szValue)
	{
		bIPUnits = true;
	}
	else
	{
		bIPUnits = false;
	}
	m_designData.m_fnSetUnits(bIPUnits);
	GetPrivateProfileString(strSection, "WaterFlowRate", "0.0", szValue, 256, m_strFileName);
	m_designData.m_fnSetWaterFlowRate(atof(szValue), bIPUnits);
	GetPrivateProfileString(strSection, "HWT", "0.0", szValue, 256, m_strFileName);
	m_designData.m_fnSetHWT(atof(szValue), bIPUnits);
	GetPrivateProfileString(strSection, "CWT", "0.0", szValue, 256, m_strFileName);
	m_designData.m_fnSetCWT(atof(szValue), bIPUnits);
	GetPrivateProfileString(strSection, "WBT", "0.0", szValue, 256, m_strFileName);
	m_designData.m_fnSetWBT(atof(szValue), bIPUnits);
	GetPrivateProfileString(strSection, "DBT", "0.0", szValue, 256, m_strFileName);
	m_designData.m_fnSetDBT(atof(szValue), bIPUnits);
	GetPrivateProfileString(strSection, "FanDriverPower", "0.0", szValue, 256, m_strFileName);
	m_designData.m_fnSetFanDriverPower(atof(szValue), bIPUnits);
	GetPrivateProfileString(strSection, "BarometricPressure", "0.0", szValue, 256, m_strFileName);
	m_designData.m_fnSetBarometricPressure(atof(szValue), bIPUnits);
	GetPrivateProfileString(strSection, "LG", "0.0", szValue, 256, m_strFileName);
	m_designData.m_fnSetLG(atof(szValue), bIPUnits);

	//
	// Curve Data
	//
	strSection = "Curve Data";
	GetPrivateProfileString(strSection, "NumRanges", "0.0", szValue, 256, m_strFileName);
	m_designData.m_fnSetRangeCnt(atoi(szValue));
	GetPrivateProfileString(strSection, "NumFlows", "0.0", szValue, 256, m_strFileName);
	m_designData.m_fnSetFlowCnt(atoi(szValue));
	for (int iRangeIndex = 1; (iRangeIndex <= m_designData.m_fnGetRangeCnt()) && (iRangeIndex <= MAX_RANGES); iRangeIndex++)
	{
		CString strItem;
		strItem.Format("Range%i", iRangeIndex);
		GetPrivateProfileString(strSection, strItem, "0.0", szValue, 256, m_strFileName);
		m_designData.m_fnSetRange(atof(szValue), iRangeIndex-1, bIPUnits);
	}

	//
	// Flow Data
	//
	for (int iFlowIndex = 1; (iFlowIndex <= m_designData.m_fnGetFlowCnt()) && (iFlowIndex <= MAX_FLOWS); iFlowIndex++)
	{
		CString strFlowSection;
		strFlowSection.Format("Flow%i", iFlowIndex);

		GetPrivateProfileString(strFlowSection, "FlowRate", "0.0", szValue, 256, m_strFileName);
		m_designData.m_fnSetFlow(atof(szValue), iFlowIndex-1, bIPUnits);

		GetPrivateProfileString(strFlowSection, "WBTs", "", szValue, 256, m_strFileName);
		CString strWBTTemp;
		int		iIndex = 0;

		//
		// CSV list of WBTs
		//
		for (int iWBTIndex = 0; iWBTIndex < (int)strlen(szValue); iWBTIndex++)
		{
			if (szValue[iWBTIndex] == ',')
			{
				m_designData.m_fnSetWBT(atof(strWBTTemp), iFlowIndex-1, iIndex, bIPUnits);
				iIndex++;
				strWBTTemp.Empty();
			}
			else if (szValue[iWBTIndex+1] == '\0')
			{
				strWBTTemp += szValue[iWBTIndex];
				m_designData.m_fnSetWBT(atof(strWBTTemp), iFlowIndex-1, iIndex, bIPUnits);
				iIndex++;
				strWBTTemp.Empty();
			}
			else
			{
				strWBTTemp += szValue[iWBTIndex];
			}
		}
		m_designData.m_fnSetWBTCnt(iIndex, iFlowIndex-1);

		//
		// Flow CWTs
		//
		for (int iWBTIndex = 0; iWBTIndex < m_designData.m_fnGetWBTCnt(iFlowIndex-1); iWBTIndex++)
		{
			CString strCWTTemp;

			strWBTTemp.Format("%.04f", m_designData.m_fnGetWBT(iFlowIndex-1, iWBTIndex, bIPUnits));
			GetPrivateProfileString(strFlowSection, strWBTTemp, "", szValue, 256, m_strFileName);

			//
			// CSV list of CWTs
			//
			iIndex = 0;
			for (int iCWTIndex = 0; iCWTIndex < (int)strlen(szValue); iCWTIndex++)
			{
				if (szValue[iCWTIndex] == ',')
				{
					m_designData.m_fnSetCWT(atof(strCWTTemp), iFlowIndex-1, iWBTIndex, iIndex, bIPUnits);
					iIndex++;
					strCWTTemp.Empty();
				}
				else if (szValue[iCWTIndex+1] == '\0')
				{
					strCWTTemp += szValue[iCWTIndex];
					m_designData.m_fnSetCWT(atof(strCWTTemp), iFlowIndex-1, iWBTIndex, iIndex, bIPUnits);
					iIndex++;
					strCWTTemp.Empty();
				}
				else
				{
					strCWTTemp += szValue[iCWTIndex];
				}
			}
		}
	}	

	//
	// Test Data
	//
	strSection = "Tests";
	GetPrivateProfileString(strSection, "NumTests", "0", szValue, 256, m_strFileName);
	int iTestCnt = atoi(szValue);

	for (int iTestIndex = 1; iTestIndex <= iTestCnt; iTestIndex++)
	{
		CString strTestSection;
		TPerformanceTest stTest;

		strTestSection.Format("TestPoint%i", iTestIndex);

		GetPrivateProfileString(strTestSection, "Date", "", szValue, 256, m_strFileName);
		stTest.m_fnSetName(szValue);
		GetPrivateProfileString(strTestSection, "Units", "IP", szValue, 256, m_strFileName);
		if (CString("IP") == szValue)
		{
			bIPUnits = true;
		}
		else
		{
			bIPUnits = false;
		}
		stTest.m_fnSetUnits(bIPUnits);
		GetPrivateProfileString(strTestSection, "WaterFlowRate", "0.0", szValue, 256, m_strFileName);
		stTest.m_fnSetWaterFlowRate(atof(szValue), bIPUnits);
		GetPrivateProfileString(strTestSection, "HWT", "0.0", szValue, 256, m_strFileName);
		stTest.m_fnSetHWT(atof(szValue), bIPUnits);
		GetPrivateProfileString(strTestSection, "CWT", "0.0", szValue, 256, m_strFileName);
		stTest.m_fnSetCWT(atof(szValue), bIPUnits);
		GetPrivateProfileString(strTestSection, "WBT", "0.0", szValue, 256, m_strFileName);
		stTest.m_fnSetWBT(atof(szValue), bIPUnits);
		GetPrivateProfileString(strTestSection, "DBT", "0.0", szValue, 256, m_strFileName);
		stTest.m_fnSetDBT(atof(szValue), bIPUnits);
		GetPrivateProfileString(strTestSection, "FanDriverPower", "0.0", szValue, 256, m_strFileName);
		stTest.m_fnSetFanDriverPower(atof(szValue), bIPUnits);
		GetPrivateProfileString(strTestSection, "BarometricPressure", "0.0", szValue, 256, m_strFileName);
		stTest.m_fnSetBarometricPressure(atof(szValue), bIPUnits);

		m_arrayTests.Add(stTest);
	}

	return bReturn;
}

bool TTowerPerformanceFile::m_fnSaveFile(LPCTSTR lpszFileName)
{
	bool bReturn = false;

	if (lpszFileName)
	{
		m_strFileName = lpszFileName;
		m_designData.m_fnSetFileName(lpszFileName);
	}

	try
	{
		CFile::Remove(m_strFileName);
	}
	catch(...)
	{
	}

	CString strSection;

	//if (m_fnCanModify())
	{
		//
		// Tower Data
		//
		strSection = "Tower Info";
		WritePrivateProfileString(strSection, "Owner", m_designData.m_fnGetTowerOwner(), m_strFileName);
		WritePrivateProfileString(strSection, "ProjectName", m_designData.m_fnGetProjectName(), m_strFileName);
		WritePrivateProfileString(strSection, "Location", m_designData.m_fnGetTowerLocation(), m_strFileName);
		WritePrivateProfileString(strSection, "Manufacturer", m_designData.m_fnGetTowerManufacturer(), m_strFileName);
		WritePrivateProfileInt(strSection, "ManufacturerData", m_designData.m_fnGetManufacturerData(), m_strFileName);
		WritePrivateProfileInt(strSection, "InducedFlow", (int)m_designData.m_fnGetInducedFlow(), m_strFileName);

		//
		// Design Point data
		//
		strSection = "Tower Design Point";
		WritePrivateProfileDouble(strSection, "WaterFlowRate", m_designData.m_fnGetWaterFlowRate(m_designData.m_fnGetUnits()), m_strFileName);
		WritePrivateProfileDouble(strSection, "HWT", m_designData.m_fnGetHWT(m_designData.m_fnGetUnits()), m_strFileName);
		WritePrivateProfileDouble(strSection, "CWT", m_designData.m_fnGetCWT(m_designData.m_fnGetUnits()), m_strFileName);
		WritePrivateProfileDouble(strSection, "WBT", m_designData.m_fnGetWBT(m_designData.m_fnGetUnits()), m_strFileName);
		WritePrivateProfileDouble(strSection, "DBT", m_designData.m_fnGetDBT(m_designData.m_fnGetUnits()), m_strFileName);
		WritePrivateProfileDouble(strSection, "FanDriverPower", m_designData.m_fnGetFanDriverPower(m_designData.m_fnGetUnits()), m_strFileName);
		WritePrivateProfileDouble(strSection, "BarometricPressure", m_designData.m_fnGetBarometricPressure(m_designData.m_fnGetUnits()), m_strFileName);
		WritePrivateProfileDouble(strSection, "LG", m_designData.m_fnGetLG(m_designData.m_fnGetUnits()), m_strFileName);
		if (m_designData.m_fnGetUnits())
		{
			WritePrivateProfileString(strSection, "Units", "IP", m_strFileName);
		}
		else
		{
			WritePrivateProfileString(strSection, "Units", "SI", m_strFileName);
		}

		//
		// Curve Data
		//
		strSection = "Curve Data";
		WritePrivateProfileInt(strSection, "NumRanges", m_designData.m_fnGetRangeCnt(), m_strFileName);
		WritePrivateProfileInt(strSection, "NumFlows", m_designData.m_fnGetFlowCnt(), m_strFileName);
		for (int iRangeIndex = 1; (iRangeIndex <= m_designData.m_fnGetRangeCnt()) && (iRangeIndex <= MAX_RANGES); iRangeIndex++)
		{
			CString strItem;
			strItem.Format("Range%i", iRangeIndex);
			WritePrivateProfileDouble(strSection, strItem, m_designData.m_fnGetRange(iRangeIndex-1, m_designData.m_fnGetUnits()), m_strFileName);
		}

		//
		// Flow Data
		//
		for (int iFlowIndex = 1; (iFlowIndex <= m_designData.m_fnGetFlowCnt()) && (iFlowIndex <= MAX_FLOWS); iFlowIndex++)
		{
			CString strFlowSection;
			strFlowSection.Format("Flow%i", iFlowIndex);

			WritePrivateProfileDouble(strFlowSection, "FlowRate", m_designData.m_fnGetFlow(iFlowIndex-1, m_designData.m_fnGetUnits()), m_strFileName);

			CString strWBTTemp = "";
			int		iIndex = 0;

			//
			// CSV list of WBTs
			//
			for (int iWBTIndex = 0; iWBTIndex < m_designData.m_fnGetWBTCnt(iFlowIndex-1); iWBTIndex++)
			{
				CString strWBT;
				strWBT.Format("%.04f", m_designData.m_fnGetWBT(iFlowIndex-1, iWBTIndex, m_designData.m_fnGetUnits()));
				if (strWBTTemp.GetLength())
				{
					strWBTTemp += ",";
				}
				strWBTTemp += strWBT;
			}
			WritePrivateProfileString(strFlowSection, "WBTs", strWBTTemp, m_strFileName);

			//
			// Flow CWTs
			//
			for (int iWBTIndex = 0; iWBTIndex < m_designData.m_fnGetWBTCnt(iFlowIndex-1); iWBTIndex++)
			{
				CString strCWTTemp;

				strWBTTemp.Format("%.04f", m_designData.m_fnGetWBT(iFlowIndex-1, iWBTIndex, m_designData.m_fnGetUnits()));

				//
				// CSV list of CWTs
				//
				iIndex = 0;
				for (int iCWTIndex = 0; iCWTIndex < m_designData.m_fnGetRangeCnt(); iCWTIndex++)
				{
					CString strCWT;
					strCWT.Format("%.04f", m_designData.m_fnGetCWT(iFlowIndex-1, iWBTIndex, iCWTIndex, m_designData.m_fnGetUnits()));
					if (strCWTTemp.GetLength())
					{
						strCWTTemp += ",";
					}
					strCWTTemp += strCWT;
				}
				WritePrivateProfileString(strFlowSection, strWBTTemp, strCWTTemp, m_strFileName);
			}
		}	
	}

	//
	// Test Data
	//
	strSection = "Tests";
	WritePrivateProfileInt(strSection, "NumTests", m_arrayTests.GetUpperBound()+1, m_strFileName);
	int iTestCnt = m_arrayTests.GetUpperBound()+1;

	for (int iTestIndex = 1; iTestIndex <= iTestCnt; iTestIndex++)
	{
		CString strTestSection;

		strTestSection.Format("TestPoint%i", iTestIndex);

		WritePrivateProfileString(strTestSection, "Date", m_arrayTests[iTestIndex-1].m_fnGetName(), m_strFileName);
		if (m_arrayTests[iTestIndex-1].m_fnGetUnits())
		{
			WritePrivateProfileString(strTestSection, "Units", "IP", m_strFileName);
		}
		else
		{
			WritePrivateProfileString(strTestSection, "Units", "SI", m_strFileName);
		}
		WritePrivateProfileDouble(strTestSection, "WaterFlowRate", m_arrayTests[iTestIndex-1].m_fnGetWaterFlowRate(m_arrayTests[iTestIndex-1].m_fnGetUnits()), m_strFileName);
		WritePrivateProfileDouble(strTestSection, "HWT", m_arrayTests[iTestIndex-1].m_fnGetHWT(m_arrayTests[iTestIndex-1].m_fnGetUnits()), m_strFileName);
		WritePrivateProfileDouble(strTestSection, "CWT", m_arrayTests[iTestIndex-1].m_fnGetCWT(m_arrayTests[iTestIndex-1].m_fnGetUnits()), m_strFileName);
		WritePrivateProfileDouble(strTestSection, "WBT", m_arrayTests[iTestIndex-1].m_fnGetWBT(m_arrayTests[iTestIndex-1].m_fnGetUnits()), m_strFileName);
		WritePrivateProfileDouble(strTestSection, "DBT", m_arrayTests[iTestIndex-1].m_fnGetDBT(m_arrayTests[iTestIndex-1].m_fnGetUnits()), m_strFileName);
		WritePrivateProfileDouble(strTestSection, "FanDriverPower", m_arrayTests[iTestIndex-1].m_fnGetFanDriverPower(m_arrayTests[iTestIndex-1].m_fnGetUnits()), m_strFileName);
		WritePrivateProfileDouble(strTestSection, "BarometricPressure", m_arrayTests[iTestIndex-1].m_fnGetBarometricPressure(m_arrayTests[iTestIndex-1].m_fnGetUnits()), m_strFileName);
	}


	return bReturn;
}

double TTowerPerformanceFile::m_fnGetItemValue(LPCTSTR lpszSection, LPCTSTR lpszItem, double dblDefault)
{
	CString strDefault;
	strDefault.Format("%.4f", dblDefault);
	strDefault = m_fnGetItemValue(lpszSection, lpszItem, strDefault);
	return atof(strDefault);
}

CString TTowerPerformanceFile::m_fnGetItemValue(LPCTSTR lpszSection, LPCTSTR lpszItem, LPCTSTR lpszDefault)
{
	CString strReturn;

	GetPrivateProfileString(lpszSection, lpszItem, lpszDefault, strReturn.GetBuffer(256), 255, m_strFileName);

	return strReturn;
}

void TTowerPerformanceFile::m_fnWriteItemValue(LPCTSTR lpszSection, LPCTSTR lpszItem, double dblValue)
{
	CString strValue;
	strValue.Format("%.4f", dblValue);
	m_fnWriteItemValue(lpszSection, lpszItem, strValue);
}

void TTowerPerformanceFile::m_fnWriteItemValue(LPCTSTR lpszSection, LPCTSTR lpszItem, LPCTSTR lpszValue)
{
	WritePrivateProfileString(lpszSection, lpszItem, lpszValue, m_strFileName);
}

void TTowerPerformanceFile::m_fnInit()
{
	m_strFileName			= "";

	TPerformanceDesign designTemp;
	m_designData = designTemp;
	m_arrayTests.RemoveAll();
}

void TTowerPerformanceFile::m_fnSetUnits(bool bIP)
{
	m_designData.m_fnSetUnits(bIP);

	for (int iTestIndex = 0; iTestIndex <= m_arrayTests.GetUpperBound(); iTestIndex++)
	{
		m_arrayTests[iTestIndex].m_fnSetUnits(bIP);
	}
}

void TTowerPerformanceFile::WritePrivateProfileDouble(LPCTSTR lpszSection, LPCTSTR lpszItem, double dblValue, LPCTSTR lpszFile, int iDecimal)
{
	CString strFormat;
	CString strValue;

	strFormat.Format("%s.0%if", "%", iDecimal);
	strValue.Format(strFormat, dblValue);

	WritePrivateProfileString(lpszSection, lpszItem, strValue, lpszFile);
}

void TTowerPerformanceFile::WritePrivateProfileInt(LPCTSTR lpszSection, LPCTSTR lpszItem, int iValue, LPCTSTR lpszFile)
{
	CString strValue;

	strValue.Format("%i", iValue);

	WritePrivateProfileString(lpszSection, lpszItem, strValue, lpszFile);
}


bool TTowerPerformanceFile::m_fnCanModify()
{
	bool bReturn = m_designData.m_fnCanModify();

	return bReturn;
}

