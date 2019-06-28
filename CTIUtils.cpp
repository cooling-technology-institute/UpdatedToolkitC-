//---------------------------------------------------------------------
//
// CTIUtils.cpp : 
//
// Copyright c 1997-1998 Cosmo Engineering  -  All Rights Reserved.
//
// $Archive: /Toolkit/CTIUtils.cpp $
// $Revision: 11 $
// $Modtime: 8/07/01 3:30a $
//
//---------------------------------------------------------------------
//
// $Log: /Toolkit/CTIUtils.cpp $
// 
// 12	06/21/03 4:22a Davidl
// Modified DetermineAdjTestFlow function to produce forced draft 
// enthalpy, sp. vol, and density values

// 11    8/09/01 2:21a Davidl
// Changed SPLINT function to just do the linear fit if X is beyond XA
// array range.
// 
// 10    5/03/01 4:39a Davidl
// Update build to 30, change for dynamic perf. curve data sheet captions.
// 
// 9     4/26/01 6:06a Davidl
// Added a lot of divide-by-zero checks.  Also fixed apparently-wrong loop
// invariants in SIDEWPoint().
// 
// 8     4/24/01 5:20a Davidl
// Added preconditions checking to SPLINE and SPLINT.
// 
// 7     4/20/01 3:45a Davidl
// Fixed bounds checking in Enthalpysearch functions.
// 
// 6     2/13/01 3:56p Deanp
// Added the Test L/G to the returned items.
// 
// Also no longer do the funny business with the truncit funtion
// 
// 5     1/26/01 3:08p Deanp
// bug fixes
// 
// 4     12/15/00 8:42a Deanp
// Added new DetermineAdjustedFlow function
// 
// 3     12/11/00 10:35a Deanp
// new calc of corrected flow
// 
// 2     12/05/00 12:15p Deanp
// Found some floating point math bugs and made a version with just the
// Performance Curve tab
// 
// 1     11/06/00 9:45a Deanp
// Initial 2.0 source
// 
// 17    6/15/99 7:52a Deanp
// New version of the source from Rich Harrison.
// 
// 16    2/26/99 9:34a Deanp
// fixed errors in dewpoint calc.
// 
// 15    2/25/99 8:22a Deanp
// removed extra calc that was not needed
// 
// 14    2/25/99 6:50a Deanp
// added barometric pressure to altitude functions
// 
//---------------------------------------------------------------------
#include "stdafx.h"
#include "ctiUtils.h"

//---------------------------------------------------------------------
// Unit conversion functions
//---------------------------------------------------------------------

double Altitude2PSI(double altitude)
{
    double psi = 14.696 * pow( (1.0 - (6.87535E-06 * altitude)), 5.2561 );
    return psi;
}

double PSI2Altitude(double psi)
{
    double altitude = (exp(log(psi / 14.696) / 5.2561) - 1.0) / 6.87535E-06 * -1.0;
    return altitude;
}

double Altitude2KPA(double altitude)
{
	altitude = fnMetersToFeet(altitude);
    double kPa = 101.325 * pow( (1.0 - (6.87535E-06 * altitude)), 5.2561 );
    return kPa;
}

double KPA2Altitude(double KPA)
{
    double altitude = fnFeetToMeters((exp(log(KPA / 101.325)/5.2561) - 1.0) / 6.87535E-06 * -1.0);
    return altitude;
}

double PSIToKPA(double psi)
{
    return 101.325 * psi / 14.696;
}

double KPAToPSI(double kpa)
{
    return kpa * 14.696 / 101.325;
}

double calcPressureC(double p)
{
    return p / 29.921 * 14.696;
}

double calcPressureF(double p)
{
    return p / 14.696 * 29.921;
}

double fnCelcToFar(double dblTemp)
{
	return dblTemp * 1.8 + 32.0;
}

double fnFarToCelc(double dblTemp)
{
	return (dblTemp - 32.0) / 1.8;
}

double fnFeetToMeters(double dblDist)
{
	return dblDist * .3048;
}

double fnMetersToFeet(double dblDist)
{
	return dblDist / .3048;
}

double fnBHPToKW(double dblBHP)
{
	return dblBHP * .7457;
}

double fnKWToBHP(double dblKW)
{
	return dblKW * 1.3410218586563;
}

double fnGalToLiter(double dblGal)
{
	return dblGal * 3.785411784;
}

double fnLiterToGal(double dblLiter)
{
	return dblLiter * 0.264172052358148;
}

double fnGPMToLS(double dblGPM)
{
	return dblGPM * 0.0630902;
}

double fnLSToGPM(double dblLS)
{
	return dblLS / 0.0630902;
}



//---------------------------------------------------------------------
// Data calculations
//---------------------------------------------------------------------

void CalcIpVars(double p, double TDB, double TWB, double& PwsDB, double& PwsWB, double& FsDB, double& FsWB)
{
    PwsDB = IPPws(TDB);
    PwsWB = IPPws(TWB);
    FsDB  = Fs(TDB, p);
    FsWB  = Fs(TWB, p);
}

double HumidityRatioIP(double p, double TDB, double TWB)
{
	double PwsDB;
	double PwsWB;
	double FsDB;
	double FsWB;
	double WsWB(0.0);
    double HumidRatio(0.0);

    CalcIpVars(p,TDB,TWB,PwsDB,PwsWB,FsDB,FsWB);

    // Calculate saturated humidity ratio at twb using saturation pressure (Pws) at twb,
    // and Fs correction factor at twb
	double den(p - PwsWB * FsWB);
	if (den != 0.0)
		WsWB = .62198 * PwsWB * FsWB / den;  //ASHRAE Eq.(21a)

    // Calculate humidity ratio of the mixture
    // Check for thermodynamic capability
	den = (1093.0 + 0.444 * TDB - TWB);
	if (den != 0.0)
		HumidRatio = ((1093. - .556 * TWB) * WsWB - .24 * (TDB - TWB)) / den;  //ASHRAE Eq.(33)
    
    return HumidRatio;
}

void IPEnthalpysearch (int sat, 
                       double p, 
                       double RootEnthalpy, 
                       double& OutputEnthalpy, 
                       double& TWB, 
                       double& TDB, 
                       double HumidRatio, 
                       double RelHumid, 
                       double SpVolume, 
                       double Density, 
                       double DEWPoint)
{
    // Establish tolerance on enthalpy search
    double temptolerance(0.001);
    double Htolerance(0.00005);

    // First need to bracket region of WB/DB to created bisection region
    // High and low values are respectively:
    double Tlower(0.0);
    //DDP double Tupper = 200.0;
    double Tupper(140.0);
    double Hlower;
    double Hupper;
    double hmid;
    double Enthalpy;

    // Calculate low value and compare to program and tolerance limits
    TWB = Tlower;
    if ( sat == 1 ) 
        TDB = TWB;

    CalcIPProperties( p, 
                      TWB, 
                      TDB, 
                      HumidRatio, 
                      RelHumid, 
                      Hlower, 
                      SpVolume, 
                      Density, 
                      DEWPoint
                    );

    // DDP change if ( abs(Tlower - RootEnthalpy) <= Htolerance ) 
    if ( fabs(Hlower - RootEnthalpy) <= Htolerance ) 
        return;
    if ( RootEnthalpy < Hlower ) 
	{
		OutputEnthalpy = -999.; // DDP out of range
        return;
	}

    //Calculate high value and compare to program and tolerance limits
    TWB = Tupper;
    if ( sat == 1 ) 
        TDB = TWB;

    CalcIPProperties( p, 
                      TWB, 
                      TDB, 
                      HumidRatio, 
                      RelHumid, 
                      Hupper, 
                      SpVolume, 
                      Density, 
                      DEWPoint
                    );

    if ( fabs(Hupper - RootEnthalpy) <= Htolerance )
        return;
    if ( RootEnthalpy > Hupper )
	{
		OutputEnthalpy = -999.; // DDP out of range
        return;
	}
      
    // Begin bisection root search procedure from Numerical Recipes in BASIC, p 193
    double trtbis(Tlower);
    double DT(Tupper - Tlower);
    double tmid;
    do
    {
        DT = DT / 2.0;
        tmid = trtbis + DT;
        if ( sat == 1 ) 
            TDB = tmid;
        CalcIPProperties( p, 
                          tmid, 
                          TDB, 
                          HumidRatio, 
                          RelHumid, 
                          Enthalpy, 
                          SpVolume, 
                          Density, 
                          DEWPoint
                        );
        hmid = RootEnthalpy - Enthalpy;
        if ( hmid >= 0.0 )
            trtbis = tmid;
    }
    while ( (fabs(DT) >= temptolerance) && ( hmid != 0.0 ) ); 

    TWB = tmid;
    if ( sat == 1 ) 
        TDB = tmid;
    OutputEnthalpy = Enthalpy;
}

double truncit( double number, int precision )
{
    // truncate number to five decimal places
/*
	double dVal = 1;
	if (precision > 0)
	{
		for (int iIndex = 0; iIndex <= precision; iIndex++)
		{
			dVal = dVal/10;
		}
		number = number + dVal;
	}
*/
    
	CString strValue;
    CString strFormat;
	strFormat.Format("%if", precision);
	strFormat = "%." + strFormat;
	strValue.Format(strFormat, number);

    number = atof(strValue);

    return number;
}

double DegreeOfSaturationIP(double p, double TDB, double TWB)
{
	double PwsDB;
	double PwsWB;
	double FsDB;
	double FsWB;
	double WsDB(0.0);
	double WsWB(0.0);
	double DegofSat(0.0);
    double HumidRatio;

    CalcIpVars(p,TDB,TWB,PwsDB,PwsWB,FsDB,FsWB);

    // Calculate saturated humidity ratio at twb using saturation pressure (Pws) at twb,
    // and Fs correction factor at twb
	double den(p - PwsWB * FsWB);
	if (den != 0.0)
		WsWB = .62198 * PwsWB * FsWB / den;  //ASHRAE Eq.(21a)

    // Calculate humidity ratio of the mixture
    // HumidRatio = ((1093. - .556 * TWB) * WsWB - .24 * (TDB - TWB)) / (1093. + .444 * TDB - TWB);  //ASHRAE Eq.(33)
    HumidRatio = HumidityRatioIP(p,TDB,TWB);
        
    // Calculate saturated humidity ratio at tdb using saturation pressure(Pws) at
    // tdb and correction factor Fs at tdb
	den = (p - PwsDB * FsDB);
	if (den != 0.0)
		WsDB = .62198 * PwsDB * FsDB / den; //ASHRAE Eq.(21a)

    // Calculate degree of saturation
	if (WsDB != 0.0)
		DegofSat = HumidRatio / WsDB;           //ASHRAE Eq.(10)

    return DegofSat;
}

double RelativeHumidityIP(double p, double TDB, double TWB)
{
	double PwsDB;
	double PwsWB;
	double FsDB;
	double FsWB;
	double WsDB(0.0);
	double WsWB(0.0);
	double DegofSat;
    double HumidRatio;
    double RelHumid(0.0);

    // Calculate vapor pressure and Fs factor at wb & db temperature
    CalcIpVars(p,TDB,TWB,PwsDB,PwsWB,FsDB,FsWB);

    // Calculate saturated humidity ratio at twb using saturation pressure (Pws) at twb,
    // and Fs correction factor at twb
    double den(p - PwsWB * FsWB);
	if (den != 0.0)
		WsWB = .62198 * PwsWB * FsWB / den;  //ASHRAE Eq.(21a)

    // Calculate humidity ratio of the mixture
    // HumidRatio = ((1093. - .556 * TWB) * WsWB - .24 * (TDB - TWB)) / (1093. + .444 * TDB - TWB);  //ASHRAE Eq.(33)
    HumidRatio = HumidityRatioIP(p,TDB,TWB);
        
    // Calculate saturated humidity ratio at tdb using saturation pressure(Pws) at
    // tdb and correction factor Fs at tdb
	den = (p - PwsDB * FsDB);
	if (den != 0.0)
		WsDB = .62198 * PwsDB * FsDB / den; //ASHRAE Eq.(21a)

    // Calculate degree of saturation
    DegofSat = DegreeOfSaturationIP(p,TDB,TWB);

    // Calculate relative humidity
	den = p != 0.0 ? (1.0 - (1.0 - DegofSat) * (FsDB * PwsDB / p)) : 0.0;
	if (den != 0.0)
		RelHumid = DegofSat / den;  //ASHRAE Eq.(23a)

    return RelHumid;
}

double SpecificVolumeIP(double p, double TDB, double TWB)
{
	double PwsDB;
	double PwsWB;
	double FsDB;
	double FsWB;
	double WsDB(0.0);
	double WsWB(0.0);
	double DegofSat;
	double Ra;
    double RelHumid;
    double HumidRatio;
    double SpVolume(0.0);

    // Calculate vapor pressure and Fs factor at wb & db temperature
    CalcIpVars(p,TDB,TWB,PwsDB,PwsWB,FsDB,FsWB);

    // Calculate saturated humidity ratio at twb using saturation pressure (Pws) at twb,
    // and Fs correction factor at twb
	double den(p - PwsWB * FsWB);
	if (den != 0.0)
		WsWB = .62198 * PwsWB * FsWB / den;  //ASHRAE Eq.(21a)

    // Calculate humidity ratio of the mixture
    // HumidRatio = ((1093. - .556 * TWB) * WsWB - .24 * (TDB - TWB)) / (1093. + .444 * TDB - TWB);  //ASHRAE Eq.(33)
    HumidRatio = HumidityRatioIP(p,TDB,TWB);
        
    // Calculate saturated humidity ratio at tdb using saturation pressure(Pws) at
    // tdb and correction factor Fs at tdb
	den = (p - PwsDB * FsDB);
	if (den != 0.0)
		WsDB = .62198 * PwsDB * FsDB / den; //ASHRAE Eq.(21a)

    // Calculate degree of saturation
    // DegofSat = HumidRatio / WsDB;           //ASHRAE Eq.(10)
    DegofSat = DegreeOfSaturationIP(p,TDB,TWB);

    // Calculate relative humidity
    // RelHumid = DegofSat / (1. - (1. - DegofSat) * (FsDB * PwsDB / p));  //ASHRAE Eq.(23a)
    RelHumid = RelativeHumidityIP(p,TDB,TWB);

    // Calculate specific volume
    Ra = 53.352 / 144.0;   // to change gas constant to psi per foot
	if (p != 0.0)
		SpVolume = Ra * (TDB + 459.67) * (1.0 + 1.6078 * HumidRatio) / p;   //ASHRAE Eq.(26)

    return SpVolume;
}

double CalcDensityIP(double p, double TDB, double TWB)
{
	double PwsDB;
	double PwsWB;
	double FsDB;
	double FsWB;
	double WsDB(0.0);
	double WsWB(0.0);
    double HumidRatio;
    double RelHumid;
	double DegofSat;
    double Den(0.0);
    double SpVolume;
//	double Ra;

    // Calculate vapor pressure and Fs factor at wb & db temperature
    CalcIpVars(p,TDB,TWB,PwsDB,PwsWB,FsDB,FsWB);

    // Calculate saturated humidity ratio at twb using saturation pressure (Pws) at twb,
    // and Fs correction factor at twb
	double den(p - PwsWB * FsWB);
	if (den != 0.0)
		WsWB = .62198 * PwsWB * FsWB / den;  //ASHRAE Eq.(21a)

    // Calculate humidity ratio of the mixture
    // HumidRatio = ((1093. - .556 * TWB) * WsWB - .24 * (TDB - TWB)) / (1093. + .444 * TDB - TWB);  //ASHRAE Eq.(33)
    HumidRatio = HumidityRatioIP(p,TDB,TWB);
        
    // Calculate saturated humidity ratio at tdb using saturation pressure(Pws) at
    // tdb and correction factor Fs at tdb
	den = (p - PwsDB * FsDB);
	if (den != 0.0)
		WsDB = .62198 * PwsDB * FsDB / den; //ASHRAE Eq.(21a)

    // Calculate degree of saturation
    // DegofSat = HumidRatio / WsDB;           //ASHRAE Eq.(10)
    DegofSat = DegreeOfSaturationIP(p,TDB,TWB);

    // Calculate relative humidity
    // RelHumid = DegofSat / (1. - (1. - DegofSat) * (FsDB * PwsDB / p));  //ASHRAE Eq.(23a)
    RelHumid = RelativeHumidityIP(p,TDB,TWB);

    // Calculate specific volume
    // Ra = 53.352 / 144.;   // to change gas constant to psi per foot
    // SpVolume = Ra * (TDB + 459.67) * (1. + 1.6078 * HumidRatio) / p;   //ASHRAE Eq.(26)
    SpVolume = SpecificVolumeIP(p,TDB,TWB);

    // Calculate density
	if (SpVolume != 0.0)
		Den = (1.0 + HumidRatio) / SpVolume;

    return Den;
}

// streamlined Enthalpy function for demand curves
double Enthalpy( double p, double TDB, double TWB )
{
    double Ra;
	double PwsDB;
	double PwsWB;
	double FsDB;
	double FsWB;
	double WsDB(0.0);
	double WsWB(0.0);
	double DegofSat(0.0);
    double HumidRatio(0.0);
    double RelHumid(0.0);
    double SpVolume(0.0);
    double Density(0.0);
    double Enthalpy;
    
    // CalcIpVars
    PwsDB = IPPws(TDB);
    PwsWB = IPPws(TWB);
    FsDB  = Fs(TDB, p);
    FsWB  = Fs(TWB, p);

    // calculate WsWB
	double den(p - PwsWB * FsWB);
	if (den != 0.0)
		WsWB = .62198 * PwsWB * FsWB / den;;  //ASHRAE Eq.(21a)
    
    // calculate humidity ratio
	den = (1093.0 + .444 * TDB - TWB);
	if (den != 0.0)
		HumidRatio = ((1093.0 - .556 * TWB) * WsWB - .24 * (TDB - TWB)) / den;  //ASHRAE Eq.(33)

    // Calculate saturated humidity ratio at tdb using saturation pressure(Pws) at
    // tdb and correction factor Fs at tdb
	den = (p - PwsDB * FsDB);
	if (den != 0.0)
		WsDB = .62198 * PwsDB * FsDB / den; //ASHRAE Eq.(21a)    

    // Calculate degree of saturation
	if (WsDB != 0.0)
		DegofSat = HumidRatio / WsDB;           //ASHRAE Eq.(10)

    // Calculate relative humidity
	den = p != 0.0 ? (1.0 - (1.0 - DegofSat) * (FsDB * PwsDB / p)) : 0.0;
	if (den != 0.0)
		RelHumid = DegofSat / den;  //ASHRAE Eq.(23a)

    // Calculate specific volume
    Ra = 53.352 / 144.;   // to change gas constant to psi per foot
	if (p != 0.0)
		SpVolume = Ra * (TDB + 459.67) * (1.0 + 1.6078 * HumidRatio) / p;   //ASHRAE Eq.(26)

    // Calculate density
	if (SpVolume != 0.0)
		Density = (1.0 + HumidRatio) / SpVolume;

    // Calculate enthalpy
    Enthalpy = .24 * TDB + HumidRatio * (1061.0 + .444 * TDB);  //ASHRAE Eq.(30)

    return Enthalpy;
}

double CalcEnthalpyIP(double p, double TDB, double TWB)
{
	double PwsDB;
	double PwsWB;
	double FsDB;
	double FsWB;
	double WsDB(0.0);
	double WsWB(0.0);
	double DegofSat;
    double HumidRatio;
    double RelHumid;
    double SpVolume;
    double Density;
    double Enthalpy;
//	double Ra;

    // Calculate vapor pressure and Fs factor at wb & db temperature
    CalcIpVars(p,TDB,TWB,PwsDB,PwsWB,FsDB,FsWB);

    // Calculate saturated humidity ratio at twb using saturation pressure (Pws) at twb,
    // and Fs correction factor at twb
	double den(p - PwsWB * FsWB);
	if (den != 0.0)
		WsWB = .62198 * PwsWB * FsWB / den;  //ASHRAE Eq.(21a)

    // Calculate humidity ratio of the mixture
    // HumidRatio = ((1093. - .556 * TWB) * WsWB - .24 * (TDB - TWB)) / (1093. + .444 * TDB - TWB);  //ASHRAE Eq.(33)
    HumidRatio = HumidityRatioIP(p,TDB,TWB);
        
    // Calculate saturated humidity ratio at tdb using saturation pressure(Pws) at
    // tdb and correction factor Fs at tdb
	den = (p - PwsDB * FsDB);
	if (den != 0.0)
		WsDB = .62198 * PwsDB * FsDB / den; //ASHRAE Eq.(21a)

    // Calculate degree of saturation
    // DegofSat = HumidRatio / WsDB;           //ASHRAE Eq.(10)
    DegofSat = DegreeOfSaturationIP(p,TDB,TWB);

    // Calculate relative humidity
    // RelHumid = DegofSat / (1. - (1. - DegofSat) * (FsDB * PwsDB / p));  //ASHRAE Eq.(23a)
    RelHumid = RelativeHumidityIP(p,TDB,TWB);

    // Calculate specific volume
    // Ra = 53.352 / 144.;   // to change gas constant to psi per foot
    // SpVolume = Ra * (TDB + 459.67) * (1. + 1.6078 * HumidRatio) / p;   //ASHRAE Eq.(26)
    SpVolume = SpecificVolumeIP(p,TDB,TWB);

    // Calculate density
    // Density = (1. + HumidRatio) / SpVolume;
    Density = CalcDensityIP(p,TDB,TWB);

    // Calculate enthalpy
    Enthalpy = .24 * TDB + HumidRatio * (1061.0 + .444 * TDB);  //ASHRAE Eq.(30)

    return Enthalpy;
}

double CalcDEWPointIP(double p, double TDB, double TWB)
{
	double PwsDB;
	double PwsWB;
	double FsDB;
	double FsWB;
	double WsDB(0.0);
	double WsWB(0.0);
	double DegofSat;
    double HumidRatio;
    double RelHumid;
    double SpVolume;
    double Density;
    double Enthalpy;
    double DEWPoint = 0.0;

    // Calculate vapor pressure and Fs factor at wb & db temperature
    CalcIpVars(p,TDB,TWB,PwsDB,PwsWB,FsDB,FsWB);

    // Calculate saturated humidity ratio at twb using saturation pressure (Pws) at twb,
    // and Fs correction factor at twb
	double den(p - PwsWB * FsWB);
	if (den != 0.0)
		WsWB = .62198 * PwsWB * FsWB / den;  //ASHRAE Eq.(21a)

    // Calculate humidity ratio of the mixture
    // HumidRatio = ((1093. - .556 * TWB) * WsWB - .24 * (TDB - TWB)) / (1093. + .444 * TDB - TWB);  //ASHRAE Eq.(33)
    HumidRatio = HumidityRatioIP(p,TDB,TWB);
        
    // Calculate saturated humidity ratio at tdb using saturation pressure(Pws) at
    // tdb and correction factor Fs at tdb
    den = (p - PwsDB * FsDB);
	if (den != 0.0)
		WsDB = .62198 * PwsDB * FsDB / den; //ASHRAE Eq.(21a)

    // Calculate degree of saturation
    // DegofSat = HumidRatio / WsDB;           //ASHRAE Eq.(10)
    DegofSat = DegreeOfSaturationIP(p,TDB,TWB);

    // Calculate relative humidity
    // RelHumid = DegofSat / (1. - (1. - DegofSat) * (FsDB * PwsDB / p));  //ASHRAE Eq.(23a)
    RelHumid = RelativeHumidityIP(p,TDB,TWB);

    // Calculate specific volume
    // Ra = 53.352 / 144.;   // to change gas constant to psi per foot
    // SpVolume = Ra * (TDB + 459.67) * (1. + 1.6078 * HumidRatio) / p;   //ASHRAE Eq.(26)
    SpVolume = SpecificVolumeIP(p,TDB,TWB);

    // Calculate density
    // Density = (1. + HumidRatio) / SpVolume;
    Density = CalcDensityIP(p,TDB,TWB);

    // Calculate enthalpy
    // Enthalpy = .24 * TDB + HumidRatio * (1061. + .444 * TDB);  //ASHRAE Eq.(30)
    Enthalpy = CalcEnthalpyIP(p,TDB,TWB);

    // Calculate dew point temperature
    DEWPoint = IPDEWPoint(p, TWB, TDB, HumidRatio);

    return DEWPoint;
}

//*******Given WB & DB Calculate IP psycrometric properties routine ********
//
void CalcIPProperties(double p, double TWB, double TDB, double &HumidRatio, double &RelHumid, double &Enthalpy, double &SpVolume, double &Density, double &DEWPoint)
{
	double DegofSat;

    // Calculate humidity ratio of the mixture
    // HumidRatio = ((1093. - .556 * TWB) * WsWB - .24 * (TDB - TWB)) / (1093. + .444 * TDB - TWB);  //ASHRAE Eq.(33)
    HumidRatio = HumidityRatioIP(p,TDB,TWB);
        
    // Calculate degree of saturation
    // DegofSat = HumidRatio / WsDB;           //ASHRAE Eq.(10)
    DegofSat = DegreeOfSaturationIP(p,TDB,TWB);

    // Calculate relative humidity
    // RelHumid = DegofSat / (1. - (1. - DegofSat) * (FsDB * PwsDB / p));  //ASHRAE Eq.(23a)
    RelHumid = RelativeHumidityIP(p,TDB,TWB);

    // Calculate specific volume
    // Ra = 53.352 / 144.;   // to change gas constant to psi per foot
    // SpVolume = Ra * (TDB + 459.67) * (1. + 1.6078 * HumidRatio) / p;   //ASHRAE Eq.(26)
    SpVolume = SpecificVolumeIP(p,TDB,TWB);

    // Calculate density
    // Density = (1. + HumidRatio) / SpVolume;
    Density = CalcDensityIP(p,TDB,TWB);

    // Calculate enthalpy
    // Enthalpy = .24 * TDB + HumidRatio * (1061. + .444 * TDB);  //ASHRAE Eq.(30)
    Enthalpy = CalcEnthalpyIP(p,TDB,TWB);

    // Calculate dew point temperature
    DEWPoint = IPDEWPoint(p, TWB, TDB, HumidRatio);

	if ((HumidRatio < 0.0) && (fabs(HumidRatio) < .000001))
	{
		HumidRatio = 0.0;
	}
}

//************ Function calculates IP Vapor pressure ************************
//*********** Partial Pressure of Water Vapor (-148ø to 32ø) ****************
//*********** Partial Pressure of Water Vapor (32ø-392ø) ********************
//
double IPPws (double tair)
{
	double dblIPPws;
	double C1;
	double C2;
	double C3;
	double C4;
	double C5;
	double C6;
	double C7;
	double C8;
	double C9;
	double C10;
	double C11;
	double C12;
	double C13;
	double LnPws(0.0);
	double t;

// Calculate saturation pressure at t!
    if (tair >= 32)
	{
        C8  = -10440.39708;
        C9  = -11.2946496;
        C10 = -.027022355;
        C11 = .00001289036;
        C12 = -.000000002478068;
        C13 = 6.5459673;
        t = tair + 459.67;
        LnPws = C8 / t + C9 + C10 * t + C11 * t * t + C12 * t * t * t + C13 * log(t);
        dblIPPws = exp(LnPws);    //ASHRAE Eq.(4)
	}
    else
	{
        C1 = -10214.16462;
        C2 = -4.89350301;
        C3 = -.00537657944;
        C4 = .000000192023769;
        C5 = 3.55758316E-10;
        C6 = -9.03446883E-14;
        C7 = 4.1635019;
        t = tair + 459.67;
		if (t != 0.0)
			LnPws = C1 / t + C2 + C3 * t + C4 * t * t + C5 * t * t * t + C6 * t * t * t * t + C7 * log(t);
        dblIPPws = exp(LnPws);   //ASHRAE Eq.(3)
	}

	return dblIPPws;
}

/*
// bluebook version
double Fs ( double T, double P )
{
    return(((-4.55447E-10*T+9.400757E-9*P+1.282159E-7)*T
    -1.762686E-6*P+6.35199E-6)*T+3.18886E-4*P+1.000104);
}
*/
// psych35 version
double Fs (double t, double p)
{
	double C1 = 1.000119;
	double C2 = 9.184907E-06;
	double C3 = 1.286098E-11;
	double C4 = -1.593274E-13;
	double C5 = 2.872637E-04;
	double C6 = -1.618048E-06;
	double C7 = 1.467535E-08;
	double C8 = 2.41896E-12;
	double C9 = -1.371762E-10;
	double C10 = -8.565893E-10;
	double C11 = 1.229524E-10;
	double C12 = -2.336628E-11;

	double C13 = C1 + C2 * t + C3 * pow(t,4.0) + C4 * pow(t,5.0) + C5 * p + C6 * p * t;
	double C14 = C13 + C7 * p * pow(t,2.0) + C8 * p * pow(t,4.0) + C9 * t * pow(p,4.0);
	double dblFs = C14 + C10 * pow(t,2.0) * pow(p,2.0) + C11 * pow(t,2.0) * pow(p,3.0) + C12 * pow(p,2.0) * pow(t,3.0);

	return dblFs;
}

//*** Function to find IPDewPoint
//*** Converges to the same Humidity Ratio as if you had entered
//*** saturated conditions (DB=WB)
double IPDEWPoint (double p, double TWB, double /* TDB */, double HumidRatio)
{       
	double C1 = -10214.16462;
	double C3 = -.00537657944;
	double C4 = .000000192023769;
	double C5 = 3.55758316E-10;
	double C6 = -9.03446883E-14;
	double C7 = 4.1635019;
	double C8 = -10440.39708;
	double C10 = -.027022355;
	double C11 = .00001289036;
	double C12 = -.000000002478068;
	double C13 = 6.5459673;
	double PwsDp;
	double FsDp;
	double WsDp;
	double DEWPoint;
	double PDEW;
	double lnpw;
	double DeltaT;
	double DERPws;
	double t;
	double DERHR;
	int    iLoop;
	double den;

	errno = 0;

// Method to determine Dew Point - Fs varies with temp - Process in iterative passes.
    FsDp = Fs(TWB, p);
    DEWPoint = TWB;

	for (iLoop = 1; iLoop <= 2; iLoop++)      // Makes exactly two passes to get best first estimate
	{
// Calculate dew point pressure
		den = (FsDp * (.62198 + HumidRatio));
		PDEW = den != 0.0 ? (p * HumidRatio / den) : 1.0;  //ASHRAE Eq.(34)
   
// Calculate dew point temperature - check above and below ice point
       if (DEWPoint < 32)
	   {
           lnpw = log(PDEW);
           DEWPoint = 90.12 + 26.142 * lnpw + .8927 * pow(lnpw,2.0);   //ASHRAE Eq.(35)
	   }
       else
	   {
           double C14(100.45);
           double C15(33.193);
           double C16(2.319);
           double C17(0.17074);
           double C18(1.2063);
           lnpw = log(PDEW);

		   double t1(pow(lnpw,2.0));
		   double t2(pow(lnpw,3.0));
		   double t3(pow(PDEW,0.1984));

           DEWPoint = C14 + C15 * lnpw + C16 * t1 + C17 * t2 + C18 * t3;  //ASHRAE Eq.(36)
	   }
       FsDp = Fs(DEWPoint, p);
	}

    PwsDp = IPPws(DEWPoint);
	den = (p - PwsDp * FsDp);
    WsDp = den != 0.0 ? (0.62198 * PwsDp * FsDp / den) : 0.0;
    DeltaT = 1.0;

	while (
		   (WsDp != 0.0) &&
		   ((fabs(HumidRatio / WsDp - 1.0) >= 0.000001) || 
		    (fabs(DeltaT) >= 0.0001))
		  )
	{
		PwsDp = IPPws(DEWPoint);
		FsDp = Fs(DEWPoint, p);
		den = (p - PwsDp * FsDp);
		WsDp = den != 0.0 ? (0.62198 * PwsDp * FsDp / den) : 0.0;

        //Calculate DERivative of Vapor Pressure
		t = DEWPoint + 459.67;
		if (DEWPoint >= 32)
		{
			DERPws = PwsDp * (-C8 / pow(t,2.0) + C10 + 2.0 * C11 * t + 3.0 * C12 * t * t + C13 / t);
		}
		else if (t != 0.0)
		{
			DERPws = PwsDp * (-C1 / pow(t,2.0) + C3 + 2.0 * C4 * t + 3.0 * C5 * t * t + 4.0 * C6 * pow(t,3.0) + C7 / t);
		}
		else
		{
			DERPws = 0.0;
		}

//*****************************************************************************

        //Calculate DERivative of Humidity Ratio
		den = pow((p - FsDp * PwsDp),2.0);
		if (den != 0.0)
			DERHR = ((p - PwsDp * FsDp) * .62198 * FsDp * DERPws - (.62198 * FsDp * PwsDp) * (-FsDp * DERPws)) / den;
		else
			DERHR = 0.0;
  
        //Converge to given HumidRatio using Newton-Raphson Method
        //Yields about one order of magnitude correction per iteration
		DeltaT = DERHR != 0.0 ? ((HumidRatio - WsDp) / DERHR) : 0.0;
		DEWPoint = DEWPoint + DeltaT;
	}

	if (errno != 0)
	{
		DEWPoint = 0.0;
	}

    return DEWPoint;
}

void CalcSiVars(double p, double TDB, double TWB, double& PwsDB, double& PwsWB, double& FsDB, double& FsWB)
{
    double Ppsi;
    double tF;

    PwsDB = SIPws(TDB);
    PwsWB = SIPws(TWB);
    tF = TDB * 1.8 + 32.0;
    Ppsi = 14.696 * p / 101.325;
    FsDB = Fs(tF, Ppsi);
    tF = TWB * 1.8 + 32.0;
    FsWB = Fs(tF, Ppsi);
}

//*******Given WB & DB Calculate SI psycrometric properties routine ********
//
double HumidityRatioSI(double p, double TDB, double TWB)
{
	double PwsDB;
	double PwsWB;
	double FsDB;
	double FsWB;
	double WsWB(0.0);
    double HumidRatio(0.0);

    CalcSiVars(p,TDB,TWB,PwsDB,PwsWB,FsDB,FsWB);

    // Calculate saturated humidity ratio at twb using saturation pressure (Pws) at twb,
    // and Fs correction factor at twb
	double den(p - PwsWB * FsWB);
	if (den != 0.0)
		WsWB = .62198 * PwsWB * FsWB / den;  //ASHRAE Eq.(21a)

    // Calculate humidity ratio of the mixture
	den = (2501.0 + 1.805 * TDB - 4.186 * TWB);
	if (den != 0.0)
		HumidRatio = ((2501.0 - 2.381 * TWB) * WsWB - (TDB - TWB)) / den;  //ASHRAE Eq.(33)

    return HumidRatio;
}

double DegreeOfSaturationSI(double p, double TDB, double TWB)
{
	double PwsDB;
	double PwsWB;
	double FsDB;
	double FsWB;
	double WsDB(0.0);
    double HumidRatio;
	double DegofSat(0.0);

    CalcSiVars(p,TDB,TWB,PwsDB,PwsWB,FsDB,FsWB);

    // Calculate humidity ratio of the mixture
    //    HumidRatio = ((2501 - 2.381 * TWB) * WsWB - (TDB - TWB)) / (2501 + 1.805 * TDB - 4.186 * TWB);  //ASHRAE Eq.(33)
    HumidRatio = HumidityRatioSI(p,TDB,TWB);

    // Calculate saturated humidity ratio at tdb using saturation pressure(Pws) at
    // tdb and correction factor Fs at tdb
	double den(p - PwsDB * FsDB);
	if (den != 0.0)
		WsDB = .62198 * PwsDB * FsDB / den; //ASHRAE Eq.(21a)

    // Calculate degree of saturation
	if (WsDB != 0.0)
		DegofSat = HumidRatio / WsDB;           //ASHRAE Eq.(10)

    return DegofSat;
}

double RelativeHumiditySI(double p, double TDB, double TWB)
{
	double PwsDB;
	double PwsWB;
	double FsDB;
	double FsWB;
	double DegofSat;
    double HumidRatio;
    double RelHumid(0.0);

    CalcSiVars(p,TDB,TWB,PwsDB,PwsWB,FsDB,FsWB);

    // Calculate humidity ratio of the mixture
    //    HumidRatio = ((2501 - 2.381 * TWB) * WsWB - (TDB - TWB)) / (2501 + 1.805 * TDB - 4.186 * TWB);  //ASHRAE Eq.(33)
    HumidRatio = HumidityRatioSI(p,TDB,TWB);

    // Calculate saturated humidity ratio at tdb using saturation pressure(Pws) at
    // tdb and correction factor Fs at tdb
    // WsDB = .62198 * PwsDB * FsDB / (p - PwsDB * FsDB); //ASHRAE Eq.(21a)

    // Calculate degree of saturation
    // DegofSat = HumidRatio / WsDB;           //ASHRAE Eq.(10)
    DegofSat = DegreeOfSaturationSI(p,TDB,TWB);

// Calculate relative humidity
	double den(p != 0.0 ? 
		(1.0 - (1.0 - DegofSat) * (FsDB * PwsDB / p)) : 0.0);
	if (den != 0.0)
		RelHumid = DegofSat / den;  //ASHRAE Eq.(23a)
    return RelHumid;
}

double SpecificVolumeSI(double p, double TDB, double TWB)
{
	double PwsDB;
	double PwsWB;
	double FsDB;
	double FsWB;
	double DegofSat;
	double Ra;
    double HumidRatio;
    double RelHumid;
    double SpVolume(0.0);

    CalcSiVars(p,TDB,TWB,PwsDB,PwsWB,FsDB,FsWB);

    // Calculate humidity ratio of the mixture
    //    HumidRatio = ((2501 - 2.381 * TWB) * WsWB - (TDB - TWB)) / (2501 + 1.805 * TDB - 4.186 * TWB);  //ASHRAE Eq.(33)
    HumidRatio = HumidityRatioSI(p,TDB,TWB);

    // Calculate degree of saturation
    // DegofSat = HumidRatio / WsDB;           //ASHRAE Eq.(10)
    DegofSat = DegreeOfSaturationSI(p,TDB,TWB);

    // Calculate relative humidity
    // RelHumid = DegofSat / (1 - (1 - DegofSat) * (FsDB * PwsDB / p));  //ASHRAE Eq.(23a)
    RelHumid = RelativeHumiditySI(p,TDB,TWB);

    // Calculate specific volume
    Ra = 287.055 / 1000;   // to change gas constant units convert kPa to Pa
    if (p != 0.0)
		SpVolume = Ra * (TDB + 273.15) * (1.0 + 1.6078 * HumidRatio) / p;   //ASHRAE Eq.(26)

    return SpVolume;
}

double CalcDensitySI(double p, double TDB, double TWB)
{
	double PwsDB;
	double PwsWB;
	double FsDB;
	double FsWB;
	double DegofSat;
    double HumidRatio;
    double RelHumid;
    double SpVolume;
    double Density;

    CalcSiVars(p,TDB,TWB,PwsDB,PwsWB,FsDB,FsWB);

    // Calculate humidity ratio of the mixture
    //    HumidRatio = ((2501 - 2.381 * TWB) * WsWB - (TDB - TWB)) / (2501 + 1.805 * TDB - 4.186 * TWB);  //ASHRAE Eq.(33)
    HumidRatio = HumidityRatioSI(p,TDB,TWB);

    // Calculate degree of saturation
    // DegofSat = HumidRatio / WsDB;           //ASHRAE Eq.(10)
    DegofSat = DegreeOfSaturationSI(p,TDB,TWB);

    // Calculate relative humidity
    // RelHumid = DegofSat / (1 - (1 - DegofSat) * (FsDB * PwsDB / p));  //ASHRAE Eq.(23a)
    RelHumid = RelativeHumiditySI(p,TDB,TWB);

    // Calculate specific volume
    // Ra = 287.055 / 1000;   // to change gas constant units convert kPa to Pa
    // SpVolume = Ra * (TDB + 273.15) * (1 + 1.6078 * HumidRatio) / p;   //ASHRAE Eq.(26)
    SpVolume = SpecificVolumeSI(p,TDB,TWB);

    // Calculate density
    Density = SpVolume != 0.0 ? ((1.0 + HumidRatio) / SpVolume) : 0.0;
    return Density;
}

double CalcEnthalpySI(double p, double TDB, double TWB)
{
	double PwsDB;
	double PwsWB;
	double FsDB;
	double FsWB;
	double DegofSat;
    double HumidRatio;
    double RelHumid;
    double SpVolume;
    double Density;
    double Enthalpy;

    CalcSiVars(p,TDB,TWB,PwsDB,PwsWB,FsDB,FsWB);

    // Calculate humidity ratio of the mixture
    //    HumidRatio = ((2501 - 2.381 * TWB) * WsWB - (TDB - TWB)) / (2501 + 1.805 * TDB - 4.186 * TWB);  //ASHRAE Eq.(33)
    HumidRatio = HumidityRatioSI(p,TDB,TWB);

    // Calculate degree of saturation
    // DegofSat = HumidRatio / WsDB;           //ASHRAE Eq.(10)
    DegofSat = DegreeOfSaturationSI(p,TDB,TWB);

    // Calculate relative humidity
    // RelHumid = DegofSat / (1 - (1 - DegofSat) * (FsDB * PwsDB / p));  //ASHRAE Eq.(23a)
    RelHumid = RelativeHumiditySI(p,TDB,TWB);

    // Calculate specific volume
    // Ra = 287.055 / 1000;   // to change gas constant units convert kPa to Pa
    // SpVolume = Ra * (TDB + 273.15) * (1 + 1.6078 * HumidRatio) / p;   //ASHRAE Eq.(26)
    SpVolume = SpecificVolumeSI(p,TDB,TWB);

    // Calculate density
    // Density = (1 + HumidRatio) / SpVolume;
    Density = CalcDensitySI(p,TDB,TWB);

    // Calculate enthalpy
    Enthalpy = 1.006 * TDB + HumidRatio * (2501.0 + 1.805 * TDB);  //ASHRAE Eq.(30)

    return Enthalpy;
}

double CalcDEWPointSI(double p, double TDB, double TWB)
{
	double PwsDB;
	double PwsWB;
	double FsDB;
	double FsWB;
	double DegofSat;
    double HumidRatio;
    double RelHumid;
    double SpVolume;
    double Density;
    double Enthalpy;
    double DEWPoint = 0;

    CalcSiVars(p,TDB,TWB,PwsDB,PwsWB,FsDB,FsWB);

    // Calculate humidity ratio of the mixture
    //    HumidRatio = ((2501 - 2.381 * TWB) * WsWB - (TDB - TWB)) / (2501 + 1.805 * TDB - 4.186 * TWB);  //ASHRAE Eq.(33)
    HumidRatio = HumidityRatioSI(p,TDB,TWB);

    // Calculate degree of saturation
    // DegofSat = HumidRatio / WsDB;           //ASHRAE Eq.(10)
    DegofSat = DegreeOfSaturationSI(p,TDB,TWB);

    // Calculate relative humidity
    // RelHumid = DegofSat / (1 - (1 - DegofSat) * (FsDB * PwsDB / p));  //ASHRAE Eq.(23a)
    RelHumid = RelativeHumiditySI(p,TDB,TWB);

    // Calculate specific volume
    // Ra = 287.055 / 1000;   // to change gas constant units convert kPa to Pa
    // SpVolume = Ra * (TDB + 273.15) * (1 + 1.6078 * HumidRatio) / p;   //ASHRAE Eq.(26)
    SpVolume = SpecificVolumeSI(p,TDB,TWB);

    // Calculate density
    // Density = (1 + HumidRatio) / SpVolume;
    Density = CalcDensitySI(p,TDB,TWB);

    // Calculate enthalpy
    // Enthalpy = 1.006 * TDB + HumidRatio * (2501 + 1.805 * TDB);  //ASHRAE Eq.(30)
    Enthalpy = CalcEnthalpySI(p,TDB,TWB);

    // Calculate dew point temperature
    DEWPoint = SIDEWPoint(p, TWB, TDB, HumidRatio);

    return DEWPoint;
}

void CalcSIProperties (double p, double TWB, double TDB, double &HumidRatio, double &RelHumid, double &Enthalpy, double &SpVolume, double &Density, double &DEWPoint)
{
	double PwsDB;
	double PwsWB;
	double FsDB;
	double FsWB;
	double DegofSat;

    CalcSiVars(p,TDB,TWB,PwsDB,PwsWB,FsDB,FsWB);

    // Calculate humidity ratio of the mixture
    //    HumidRatio = ((2501 - 2.381 * TWB) * WsWB - (TDB - TWB)) / (2501 + 1.805 * TDB - 4.186 * TWB);  //ASHRAE Eq.(33)
    HumidRatio = HumidityRatioSI(p,TDB,TWB);

    // Calculate degree of saturation
    // DegofSat = HumidRatio / WsDB;           //ASHRAE Eq.(10)
    DegofSat = DegreeOfSaturationSI(p,TDB,TWB);

    // Calculate relative humidity
    // RelHumid = DegofSat / (1 - (1 - DegofSat) * (FsDB * PwsDB / p));  //ASHRAE Eq.(23a)
    RelHumid = RelativeHumiditySI(p,TDB,TWB);

    // Calculate specific volume
    // Ra = 287.055 / 1000;   // to change gas constant units convert kPa to Pa
    // SpVolume = Ra * (TDB + 273.15) * (1 + 1.6078 * HumidRatio) / p;   //ASHRAE Eq.(26)
    SpVolume = SpecificVolumeSI(p,TDB,TWB);

    // Calculate density
    // Density = (1 + HumidRatio) / SpVolume;
    Density = CalcDensitySI(p,TDB,TWB);

    // Calculate enthalpy
    // Enthalpy = 1.006 * TDB + HumidRatio * (2501 + 1.805 * TDB);  //ASHRAE Eq.(30)
    Enthalpy = CalcEnthalpySI(p,TDB,TWB);

    // Calculate dew point temperature
    DEWPoint = SIDEWPoint(p, TWB, TDB, HumidRatio);

	if ((HumidRatio < 0) && (fabs(HumidRatio) < .000001))
	{
		HumidRatio = 0;
	}
}


//************ Function calculates SI Vapor pressure ************************
//*********** Partial Pressure of Water Vapor (-100ø to 0ø) ****************
//*********** Partial Pressure of Water Vapor (0ø-200ø) ********************
//
double SIPws (double tair)
{
	double dblSIPws(0.0);
	double C1;
	double C2;
	double C3;
	double C4;
	double C5;
	double C6;
	double C7;
	double C8;
	double C9;
	double C10;
	double C11;
	double C12;
	double C13;
	double LnPws;
	double t;

// Calculate saturation pressure at t!
    if (tair >= 0)
	{
        C8 = -5800.2206;
        C9 = -5.516256;
        C10 = -.048640239;
        C11 = .000041764768;
        C12 = -.000000014452093;
        C13 = 6.5459673;
        t = tair + 273.15;
        LnPws = C8 / t + C9 + C10 * t + C11 * t * t + C12 * t * t * t + C13 * log(t);
        dblSIPws = exp(LnPws);    //ASHRAE Eq.(4)
	}
    else
	{
        C1 = -5674.5359;
        C2 = -.51523058;
        C3 = -.009677843;
        C4 = .00000062215701;
        C5 = .0000000020747825;
        C6 = -9.484024000000001E-13;
        C7 = 4.1635019;
        t = tair + 273.15;
		if (t != 0.0)
		{
			LnPws = C1 / t + C2 + C3 * t + C4 * t * t + C5 * t * t * t + C6 * t * t * t * t + C7 * log(t);
			dblSIPws = exp(LnPws);   //ASHRAE Eq.(3)
		}
    }

	return dblSIPws;
}


//*** Function to find SIDewPoint
//*** Converges to the same Humidity Ratio as if you had entered
//*** saturated conditions (DB=WB)
double SIDEWPoint (double p, double TWB, double /* TDB */, double HumidRatio)
{
	int		iLoop;
	double PDEW;
	double lnpw;
	double PwsDP;
	double WSDP;
	double DeltaT;
	double t;
	double DERPws;
	double DERHR;
   	double Ppsi = 14.696 * p / 101.325;
       
	double C1 = -5674.5359;
	double C3 = -.009677843;
	double C4 = .00000062215701;
	double C5 = .0000000020747825;
	double C6 = -9.484024000000001E-13;
	double C7 = 4.1635019;

	double C8 = -5800.2206;
	double C10 = -.048640239;
	double C11 = .000041764768;
	double C12 = -.000000014452093;
	double C13 = 6.5459673;
	double C14 = 6.54;
	double C15 = 14.526;
	double C16 = .7389;
	double C17 = .09486;
	double C18 = .4569;

	errno = 0;

// Method to determine Dew Point - Fs varies with temp - Process in iterative passes.
	double DEWPoint = TWB;
	double tF = DEWPoint * 1.8 + 32.0;
	double FsDP = Fs(tF, Ppsi);

	for (iLoop = 1; iLoop <= 2; iLoop++)      // Makes exactly two passes to get best first estimate
	{
	// Calculate dew point pressure
		   PDEW = p * HumidRatio / (FsDP * (.62198 + HumidRatio));  //ASHRAE Eq.(34)
   
	// Calculate dew point temperature - check above and below ice point
		   if (DEWPoint < 0)
		   {
			   lnpw = log(PDEW);
			   DEWPoint = 6.09 + 12.608 * lnpw + .4959 * pow(lnpw,2.0);   //ASHRAE Eq.(36)
		   }
		   else
		   {
			   C14 = 6.54;
			   C15 = 14.526;
			   C16 = .7389;
			   C17 = .09486;
			   C18 = .4569;
			   lnpw = log(PDEW);
			   DEWPoint = C14 + C15 * lnpw + C16 * pow(lnpw,2.0) + C17 * pow(lnpw,3.0) + C18 * pow((PDEW),.1984);  //ASHRAE Eq.(35)
		   }
		   tF = DEWPoint * 1.8 + 32.0;
		   FsDP = Fs(tF, Ppsi);
	}

    PwsDP = SIPws(DEWPoint);
	double den(p - PwsDP * FsDP);
    WSDP = den != 0.0 ? (0.62198 * PwsDP * FsDP / den) : 0.0;
    DeltaT = 1.0;

	// DavidL, 04/26/2001: Fixed the loop conditions to mimic IPDEWPoint()
	while 
		(
		(WSDP != 0.0) &&
//		((abs(HumidRatio / WSDP - 1.0) < .000001) || (abs(DeltaT) < .0001))
		((fabs(HumidRatio / WSDP - 1.0) >= .000001) || (fabs(DeltaT) >= .0001))
		)
	{
		PwsDP = SIPws(DEWPoint);
		tF = DEWPoint * 1.8 + 32.0;
		FsDP = Fs(tF, Ppsi);
		den = (p - PwsDP * FsDP);
		WSDP = den != 0.0 ? (0.62198 * PwsDP * FsDP / den) : 0.0;

	//Calculate DERivative of Vapor Pressure
		t = DEWPoint + 273.15;
		if (DEWPoint >= 0)
		{
		  DERPws = t != 0.0 ? 
			  (PwsDP * (-C8 / pow(t,2.0) + C10 + 2.0 * C11 * t + 3.0 * C12 * t * t + C13 / t)) : 0.0;
		}
		else
		{
		  DERPws = t != 0.0 ?
			  (PwsDP * (-C1 / pow(t,2.0) + C3 + 2.0 * C4 * t + 3.0 * C5 * t * t + 4.0 * C6 * pow(t,3.0) + C7 / t)) : 0.0;
		}
	//*****************************************************************************

	//Calculate DERivative of Humidity Ratio
		den = pow((p - FsDP * PwsDP),2.0);
		if (den != 0.0)
			DERHR = ((p - PwsDP * FsDP) * .62198 * FsDP * DERPws - (.62198 * FsDP * PwsDP) * (-FsDP * DERPws)) / den;
		else
			DERHR = 0.0;
  
	//Converge to given HumidRatio using Newton-Raphson Method
	//Yields about one order of magnitude correction per iteration

		DeltaT = DERHR != 0.0 ? ((HumidRatio - WSDP) / DERHR) : 0.0;
		DEWPoint = DEWPoint + DeltaT;
	}

	if (errno != 0)
	{
		DEWPoint = 0.0;
	}

    return DEWPoint;
}

double Pstd(double Z)                      /* standard atmospheric pressure */
{
    Z/=10000.0;
    return(.491154*((.547462*Z-7.67923)*Z+29.9309)/(.10803*Z+1.0));
}

double Merkel(double Twb,double Ran,double Apr,double LG,double Elev)  // used by demand curve
{
	short i;
	double KaV,Ha,Haex,Hain,Hw,Tcold,Thot,Tw;
    double X[4] = { .9, .6, .4, .1};
    double pressure = 14.696;

//	Hain = Hdbw(Twb,Wsat(Twb));  // produces 33.17 with defaults
//  Tdb = Twb + Ran;
//  Hain = CalcEnthalpyIP(14.696, Twb, Twb);

    if (Elev != 0)
        pressure = Pstd(Elev);
    Hain = Enthalpy(pressure, Twb, Twb);
	Haex = Hain + Ran * LG;
	Tcold= Twb + Apr;
	Thot =  Tcold + Ran;
	if (Thot >= Tboil ) return(999);
	KaV = 0;
	for(i=0; i<4; i++)
	{
		Tw = Tcold + X[i] * Ran;
//		Hw = Hdbw( Tw, Wsat(Tw) );
//      Hw = CalcEnthalpyIP(14.696, Tw, Tw);
        Hw = Enthalpy(pressure, Tw, Tw);
		Ha = Hain + X[i] * (Haex - Hain);
		if ( Hw <= Ha )return(999);
		KaV += .25 / (Hw-Ha);
	}
	return KaV * Ran;
}

double KAVL(double T1, double T2, double WBT, double LG) // used by merkel calculation
{
	double dblKAVL;

	double HumidRatio;
	double RelHumid;
	double SpVolume;
	double Density;
	double DEWPoint;

	double Tt1 = T2 + .1*(T1 - T2);
	double Tt2 = T2 + .4*(T1 - T2);
	double Tt3 = T1 - .4*(T1 - T2);
	double Tt4 = T1 - .1*(T1 - T2);

	double Hw1;
	double Hw2;
	double Hw3;
	double Hw4;
	CalcIPProperties (14.696, Tt1, Tt1, HumidRatio, RelHumid, Hw1, SpVolume, Density, DEWPoint);
	CalcIPProperties (14.696, Tt2, Tt2, HumidRatio, RelHumid, Hw2, SpVolume, Density, DEWPoint);
	CalcIPProperties (14.696, Tt3, Tt3, HumidRatio, RelHumid, Hw3, SpVolume, Density, DEWPoint);
	CalcIPProperties (14.696, Tt4, Tt4, HumidRatio, RelHumid, Hw4, SpVolume, Density, DEWPoint);

	double HaStart;
	double HaEnd;
	CalcIPProperties (14.696, WBT, WBT, HumidRatio, RelHumid, HaStart, SpVolume, Density, DEWPoint);
	HaEnd = HaStart + LG * (T1 - T2);

	double Ha1 = HaStart + .1*LG*(T1 - T2);
	double Ha2 = HaStart + .4*LG*(T1 - T2);
	double Ha3 = HaEnd - .4*LG*(T1 - T2);
	double Ha4 = HaEnd - .1*LG*(T1 - T2);

	double dH1 = Hw1 - Ha1;
	double dH2 = Hw2 - Ha2;
	double dH3 = Hw3 - Ha3;
	double dH4 = Hw4 - Ha4;

	if ((dH1 == 0.0) || (dH2 == 0.0) || (dH3 == 0.0) || (dH4 == 0.0))
		dblKAVL = 0.0;
	else
		dblKAVL = ((T1 - T2)/4.0)*((1.0/dH1) + (1.0/dH2) + (1.0/dH3) + (1.0/dH4));

	return dblKAVL;
}

/*
double abs(double dblNum)
{
	double dblabs = dblNum;

	if (dblNum < 0.0)
	{
		dblabs = dblNum * -1.0;
	}

	return dblabs;
}
*/

double Interpolate(double top, double inc, int nvals, double Twb, double Ran, double Apr, double LG, double Elev)
{
    double next = 0.0;
    double intervals;
    double begin = Merkel(Twb,Ran,Apr,LG-inc,Elev);

    if ( begin > top ) return 0.0;  // Merkel returns 999 on error condition

    // determine where line crosses y-axis
    for (double x=LG-inc; x<=LG+inc; x+=0.01)
    {
        next = Merkel(Twb,Ran,Apr,x,Elev);
        if (next >= top)
        {
            break;
        }
    }

    // determine increment amount
    if (nvals > 0) 
        intervals = (next-begin) / ((double) nvals);
    else
        intervals = (next-begin) / 99.0;
    
    return intervals;   
}

/* from bluebook
dbl Hdbw(dbl Tdb,dbl W)                        
{
  return(.24*Tdb+W*(1061.+.444*Tdb));
}
*/

double Hdbw(double Tdb,double W)
{
	return(.24*Tdb+W*(1061.+.444*Tdb));
}

double Wsat(double Ts)
{
	double Psat;
	Psat=Pwsat(Ts);
	double den(Patm-Psat);
	return(den != 0.0 ? (0.62198*Psat/den) : 0.0);
}

double Pwsat(double Ts)
{
	Ts+=459.67;
	return(Ts != 0.0 ? 
		exp(((-2.478068E-9*Ts+.00001289036)*Ts-.027022355)*Ts-11.2946496-10440.39708/Ts+6.5459673*log(Ts))
		: 0.0);
}

double IPRelHumidity(double psi, double TWB, double TDB)
{
	if (psi == 0.0)
		return 0.0;

    double PwsWB(IPPws(TWB));
    double Pwsdb(IPPws(TDB));
    double FsWB(Fs(TWB, psi));
    double Fsdb(Fs(TDB, psi));
	double den(psi - PwsWB * FsWB);
    double Wswb(den != 0.0 ? (0.62198 * PwsWB * FsWB / den) : 0.0);  // 'ASHRAE Eq.(21a)
    den = (psi - Pwsdb * Fsdb);
	double Wsdb(den != 0.0 ? (0.62198 * Pwsdb * Fsdb / den) : 0.0);  // 'ASHRAE Eq.(21a)
	den = (1093.0 + .444 * TDB - TWB);
    double HumidityRatio(den != 0.0 ? (((1093.0 - .556 * TWB) * Wswb - .24 * (TDB - TWB)) / den) : 0.0); // 'ASHRAE Eq.(33)
    double DegofSat(Wsdb != 0.0 ? (HumidityRatio / Wsdb) : 0.0);
	den = (1.0 - (1.0 - DegofSat) * (Fsdb * Pwsdb / psi));
    double RelHumidity(den != 0.0 ? (DegofSat / den) : 0.0);
    return RelHumidity;
}    

void IPWBsearch (double psi, double RelHumid, double TDB, double& TWB)
{
    double temptolerance = .0005;
    double RHtolerance = .00005;
    double RHhigh;
    double RHmid;

    //Calculate saturation value and compare to program and tolerance limits
    RHhigh = IPRelHumidity(psi, TDB, TDB);
    if ( fabs(RHhigh - RelHumid) <= RHtolerance ) 
    {
        TWB = TDB;
        return;
    }

    // Begin bisection root search procedure from Numerical Recipes in BASIC, p 193   
    double t1 = 0.0;
    double t2 = TDB;
    double trtbis = t1;
    double DT = t2 - t1;
    double tmid;
    int count = 0;
    do
    {
        count++;
        DT = DT / 2;
        tmid = trtbis + DT;
        RHmid = RelHumid - IPRelHumidity(psi, tmid, TDB);
        if ( RHmid >= 0.0 ) trtbis = tmid;
    }
    while ( (fabs(DT) >= temptolerance) && (RHmid != 0.0) );

    // found wet bulb
    TWB = tmid;
}

void SIWBsearch (double psi, double RelHumid, double TDB, double& TWB)
{
    double temptolerance = .0005;
    double RHtolerance = .00005;

    //Calculate saturation value and compare to program and tolerance limits
    double RHhigh = SIRelHumidity(psi, TDB, TDB);
    if ( fabs(RHhigh - RelHumid) <= RHtolerance ) 
    {
        TWB = TDB;
        return;
    }

    //Begin bisection root search procedure from Numerical Recipes in BASIC, p 193
    double t1 = -20;
    double t2 = TDB;
    double trtbis = t1;
    double DT = t2 - t1;
    double RHmid;
    double tmid;

    do
    {
        DT = DT / 2.0;
        tmid = trtbis + DT;
        RHmid = RelHumid - SIRelHumidity(psi, tmid, TDB);
        if ( RHmid >= 0.0 ) trtbis = tmid;
    }
    while ( (fabs(DT) >= temptolerance) && ( RHmid != 0.0 ) );

    TWB = tmid;
}

double SIRelHumidity ( double psi, double TWB, double TDB )
{
	if (psi == 0.0)
		return 0.0;

    double PwsWB(SIPws(TWB));
    double Pwsdb(SIPws(TDB));
    double tF(TDB * 1.8 + 32.0);
    double Ppsi(14.696 * psi / 101.325);
    double Fsdb(Fs(tF, Ppsi));
    tF = TWB * 1.8 + 32.0;
    double FsWB = Fs(tF, Ppsi);

	double den(psi - PwsWB * FsWB);
    double Wswb(den != 0.0 ? (0.62198 * PwsWB * FsWB / den) : 0.0); // 'ASHRAE Eq.(21a)
	den = (psi - Pwsdb * Fsdb);
    double Wsdb(den != 0.0 ? (0.62198 * Pwsdb * Fsdb / den) : 0.0); // 'ASHRAE Eq.(21a)
    den = (2501.0 + 1.805 * TDB - 4.186 * TWB);
	double HumidityRatio(den != 0.0 ? (((2501.0 - 2.381 * TWB) * Wswb - (TDB - TWB))) / den : 0.0); //   'ASHRAE Eq.(33)
    double DegofSat(Wsdb != 0.0 ? (HumidityRatio / Wsdb) : 0.0);
	den = (1.0 - (1.0 - DegofSat) * (Fsdb * Pwsdb / psi));
    double RelHumidity(den != 0.0 ? (DegofSat / den) : 0.0);
    return RelHumidity;
}    

void SIEnthalpysearch (int sat, 
                       double p, 
                       double RootEnthalpy, 
                       double& OutputEnthalpy, 
                       double& TWB, 
                       double& TDB, 
                       double HumidRatio, 
                       double RelHumid, 
                       double SpVolume, 
                       double Density, 
                       double DEWPoint
                       )
{
    // Establish tolerance on enthalpy search
    double temptolerance = .001;
    double Htolerance = .00005;

    // First need to bracket region of WB/DB to created bisection region
    // High and low values are respectively:
    double Tlower = -18.0;
    double Tupper = 93.0;
    double Hlower = 0.0;
    double Hupper = 0.0;
    double hmid;
    double Enthalpy;

    // Calculate low value and compare to program and tolerance limits
    TWB = Tlower;
    if ( sat == 1 ) 
        TDB = TWB;
    CalcSIProperties( p, 
                      TWB, 
                      TDB, 
                      HumidRatio, 
                      RelHumid, 
                      Hlower, 
                      SpVolume, 
                      Density, 
                      DEWPoint
                    );

    if  ( fabs(Hlower - RootEnthalpy ) <= Htolerance )
        return;
    if  ( RootEnthalpy < Hlower ) 
	{
		OutputEnthalpy = -999.; // DDP out of range		
        return;
	}

    // Calculate high value and compare to program and tolerance limits
    TWB = Tupper;
    if (sat == 1 )
        TDB = TWB;
    CalcSIProperties( p, 
                      TWB, 
                      TDB, 
                      HumidRatio, 
                      RelHumid, 
                      Hupper, 
                      SpVolume, 
                      Density, 
                      DEWPoint
                    );

    if ( fabs(Hupper - RootEnthalpy ) <= Htolerance )
        return;
    if ( RootEnthalpy > Hupper )
	{
		OutputEnthalpy = -999.; // DDP out of range
        return;
	}

    // Begin bisection root search procedure from Numerical Recipes in BASIC, p 193
    double trtbis = Tlower;
    double DT = Tupper - Tlower;
    double tmid;
    do
    {
        DT = DT / 2.0;
        tmid = trtbis + DT;
        if ( sat == 1 ) 
            TDB = tmid;
        CalcSIProperties( p, 
                          tmid, 
                          TDB, 
                          HumidRatio, 
                          RelHumid, 
                          Enthalpy, 
                          SpVolume, 
                          Density, 
                          DEWPoint
                        );
        hmid = RootEnthalpy - Enthalpy;
        if (hmid >= 0.0) 
            trtbis = tmid;
    }
    while ( (fabs(DT) >= temptolerance) && (hmid != 0.0) );

    TWB = tmid;
    if ( sat == 1 ) 
        TDB = tmid;
    OutputEnthalpy = Enthalpy;
}


void CalcPerfData(int INUM, double* X, double* YMEAS, double& XREAL, double& YFIT, double* Y2)
{
//'						I			I			I				I				O			O
//'  EXAMPLE:			4			2			112
//'						9			113
//'						16			114
//'						23			115
//'     DIM YMEASP(INUM)
   
//' DETERMINE THE SECOND DERIVITATIVES FOR THE SPLINE INTERPOLATION
    
	SPLINE(X, YMEAS, INUM, 1E+31, 1E+31, Y2);

//' DETERMINE INTERPOLATED VALUES
	SPLINT(X, YMEAS, Y2, INUM, XREAL, YFIT);

//'ERASE YMEASP
}

void SPLINE(double* X, double* Y, int INUM, double YP1, double YPN, double* Y2)
{
//'Cubic Spline subroutine from Numerical Recipes in BASIC (1994), p43
	double	QN;
	double	UN;
	double*	U = new double[INUM];
	int I;

	// Check preconditions
	if (INUM < 2)
	{
		ASSERT(0);
		return;
	}
	for (I = 1; I < INUM; I++)
	{
		if (X[I-1] >= X[I])
		{
			ASSERT(0);
			return;
		}
	}

     if (YP1 > 9.9E+29)
	 {
       Y2[0] = 0.0;
       U[0] = 0.0;
	 }
     else
	 {
       Y2[0] = -.5;
       U[0] = (3.0 / (X[1] - X[0])) * ((Y[1] - Y[0]) / (X[1] - X[0]) - YP1);
     }

     for (I = 1; I < INUM - 1; I++)
	 {
       double SIG = (X[I] - X[I - 1]) / (X[I + 1] - X[I - 1]);
       double P = SIG * Y2[I - 1] + 2.0;
       Y2[I] = (SIG - 1.0) / P;
       double DUM1 = (Y[I + 1] - Y[I]) / (X[I + 1] - X[I]);
       double DUM2 = (Y[I] - Y[I - 1]) / (X[I] - X[I - 1]);
       U[I] = (6.0 * (DUM1 - DUM2) / (X[I + 1] - X[I - 1]) - SIG * U[I - 1]) / P;
     }

     if (YPN > 9.9E+29)
	 {
       QN = 0.0;
       UN = 0.0;
	 }
     else
	 {
       QN = .5;
       UN = (3.0 / (X[INUM-1] - X[INUM - 2])) * (YPN - (Y[INUM - 1] - Y[IN - 2]) / (X[INUM - 1] - X[INUM - 2]));
	 }

     Y2[INUM - 1] = (UN - QN * U[INUM - 2]) / (QN * Y2[INUM - 2] + 1.0);

     for (int K = INUM - 2; K >= 0; K--)
	 {
       Y2[K] = Y2[K] * Y2[K + 1] + U[K];
     }

     delete[] U;
}

void SPLINT(double* XA, double* YA, double* Y2A, int INUM, double& X, double& Y)
{
	//' Determine interpolated Y value
	//' Rev: 2-22-99 to handle either Increasing or Decreasing XA array

	if ((INUM < 2) || (XA[0] == XA[1]))
	{
		ASSERT(0);
		return;
	}
    
    int ILO(0);
    int IHI(INUM - 1);
	BOOL bIncreasingX(XA[1] > XA[0]);
 
	while (IHI - ILO > 1)
	{
		int II((IHI + ILO) / 2);
		if (bIncreasingX)
		{
			if (XA[II] > X)
			{
				IHI = II;
			}
			else
			{
				ILO = II;
			}
		}
		else               //X DECREASING
		{
			if (XA[II] > X)
			{
				ILO = II;
			}
			else
			{
				IHI = II;
			}
		}
	}
     
	double DX(XA[IHI] - XA[ILO]);	
	if (DX == 0.0)
	{
		ASSERT(0); //"BAD XA INPUT"
		return;
	}
	double A((XA[IHI] - X) / DX);
	double B((X - XA[ILO]) / DX);
	Y = A * YA[ILO] + B * YA[IHI];

	// Change suggested by Rich Harrison on Aug. 3, 2001:
	// Do just the linear fit (last calc above) if x is beyond array range
	if (((bIncreasingX) && (X > XA[0]) && (X < XA[INUM-1])) ||
		((!bIncreasingX) && (X > XA[INUM-1]) && (X < XA[0])))
		Y += ((pow(A,3.0) - A) * Y2A[ILO] + (pow(B,3.0) - B) * Y2A[IHI]) * (pow(DX,2.0)) / 6.0;
}


double CalcTestLG(double dblDesignLG, double dblDesignFlow, double dblTestFlow, double dblDesignFanPower, double dblTestFanPower, double dblDesignAirDensity, double dblTestAirDensity, double dblDesignSpecificVolume, double dblTestSpecificVolume)
{
	double dblReturn;

	dblReturn = dblDesignLG * (dblTestFlow/dblDesignFlow) * pow((dblDesignFanPower/dblTestFanPower),(1.0/3.0)) * (dblTestAirDensity/dblDesignAirDensity) *pow((dblTestSpecificVolume/dblDesignSpecificVolume),(1.0/3.0));

	return dblReturn;
}


double CalcAdjustedFlow(double dblTestWaterFlowRate, double dblDesignFanDriverPower, double dblTestFanDriverPower, double dblDesignAirDensity, double dblTestAirDensity)
{
	double dblReturn;

	dblReturn = dblTestWaterFlowRate * pow((dblDesignFanDriverPower/dblTestFanDriverPower),(1.0/3.0)) * pow((dblTestAirDensity/dblDesignAirDensity),(1.0/3.0));

	return dblReturn;
}

double DetermineAdjTestFlow(int IunitsIP, int IInduced, double EWTd, double LWTd, double EWBd, double EDBd, double BPd, double FLOWd, double BHPd, double LinGD, double EWTt, double LWTt, double EWBt, double EDBt, double BPt, double FLOWt, double BHPt, double &LWBTnew, double &DenOutT, double &SVOutT, double &HLWBT, double &AdjTestFlow, double &LinGt)
{
	//'                         I             I                I            I             I           I            I           I             I            I             I            I            I            I            I           I             I            O                 O                O               O              O
	//'                                       <------------------------------------------ DESIGN VALUES ------------------------------------------------------>  <------------------------------------ TEST VALUES -------------------------------------->  <------------------------------------ OUTPUT VALUES -------------------------------->
	//' Determine Adjusted Test Flow value based on Design and Test conditions
	//' IInduced =1 Induced Draft units - compute AdjTestFlow on LEAVING air temperatures
	//'          =0 Forced Draft units  - compute AdjTestFlow on ENTERING air temperatures - NO ITERATION REQ'D

	CString strTemp;
	double Cpw = 0;
	double WInD = 0;
	double RHInD = 0;
	double HInD = 0;
	double SVInD = 0;
	double DenInD = 0;
	double DEWPointInD = 0;
	double WInT = 0;
	double RHInT = 0;
	double HInT = 0;
	double SVInT = 0;
	double DenInT = 0;
	double DEWPointInT = 0;
	double HOutD = 0;
	double OutputEnthalpy = 0;
	double LWBD = 0;
	double LDBD = 0;
	double HumidRatio = 0;
	double RelHumid = 0;
	double SVOutD = 0;
	double DenOutD = 0;
	double DEWPoint = 0;
	double DenDesign = 0;
	double SVDesign = 0;
	double LWBT = 0;
	double WOutT = 0;
	double RHOutT = 0;
	double DEWPointOutT = 0;
	double HCalcT = 0;
	double LDBTnew = 0;
	double SpVolume = 0;
	double Density = 0;
	double DenTest = 0;

	LinGt = 0;

	//' Design Entering Air Conditions
	if (IunitsIP == 1)
	{
		BPd = 14.696 * BPd / 29.921;	//'Convert to Psi  Else assumed kPa
		Cpw = 1.0;						//'IP specific heat at constant pressure
		CalcIPProperties(BPd, EWBd, EDBd, WInD, RHInD, HInD, SVInD, DenInD, DEWPointInD);
		//CalcPropertiesIP(BPd, EWBd, EDBd, WInD, RHInD, HInD, SVInD, DenInD, DEWPointInD);
	}
	else //'SI
	{
		Cpw = 4.186;					//'SI specific heat at constant pressure
		CalcSIProperties(BPd, EWBd, EDBd, WInD, RHInD, HInD, SVInD, DenInD, DEWPointInD);
		//CalcPropertiesSI(BPd, EWBd, EDBd, WInD, RHInD, HInD, SVInD, DenInD, DEWPointInD);
	}

    //PRINT #2, USING "Design: Flowd=#####.###  BHPd=###.#### LinGd=#.####"; FLOWd; BHPd; LinGD
    //PRINT #2, USING "    EWTd=###.###  LWTd=###.###  RangeD=##.###  AppD=##.###"; EWTd; LWTd; EWTd - LWTd; LWTd - EWBt
    //PRINT #2, USING "    WBd=###.## DBd=###.## BP=###.###  Win=#.#####"; EWBd; EDBd; BPd; WInD
    //PRINT #2, USING "    HInD=###.#### SVInD=##.##### DenInD=#.##### RHInD=###.##% DEWPointInD=##.##"; HInD; SVInD; DenInD; RHInD * 100!; DEWPointInD
    //PRINT #2,

	//' Test Entering Air Conditions
	// Try using this calculation for Forced Draft enthalpy, sp. vol, and density values
	// (Toolkit v3.0 Build #5, DBL - 05/23/03)
	if (IunitsIP == 1) //'IP
	{
		BPt = 14.696 * BPt / 29.921;		//'Convert to Psi  Else assumed kPa
		CalcIPProperties(BPt, EWBt, EDBt, WInT, RHInT, HInT, SVInT, DenInT, DEWPointInT);
		//CalcPropertiesIP(BPt, EWBt, EDBt, WInT, RHInT, HInT, SVInT, DenInT, DEWPointInT);
	}
	else //'SI
	{
		CalcSIProperties(BPt, EWBt, EDBt, WInT, RHInT, HInT, SVInT, DenInT, DEWPointInT);
		//CalcPropertiesSI(BPt, EWBt, EDBt, WInT, RHInT, HInT, SVInT, DenInT, DEWPointInT);
	}
  
	//   PRINT #2, USING "Test:  Flowt=#####.### BHPt=###.####"; FLOWt; BHPt
	//   PRINT #2, USING "    EWTt=###.###  LWTt=###.###  RangeT=##.###  AppT=##.###"; EWTt; LWTt; EWTt - LWTt; LWTt - EWBt
	//   PRINT #2, USING "    WBt=###.## DBt =###.## BPt=###.### WInT=#.#####"; EWBt; EDBt; BPt; WInT
	//   PRINT #2, USING "    HInT=###.#### SVInT=##.#### DenInT=#.##### RHInT=###.##% DEWPointInT=##.##"; HInT; SVInT; DenInT; RHInT * 100!; DEWPointInT
	//   PRINT #2,


	if (IInduced == 1)		//'compute AdjTestFlow on LEAVING air temperatures
	{
		//'  LEAVING AIR CONDITIONS - Predicted Leaving Wet Bulb

		//'First determine Design Leaving Air Density, DenOutD  *****************************
		//'first step is determine Leaving air Enthalpy Design, HOutD
		HOutD = HInD + LinGD * Cpw * (EWTd - LWTd);

		//'Call Enthalpy Search subroutine with calculated HOutD value
		if (IunitsIP == 1)	//'IP
		{
			EnthalpysearchIP(1, BPd, HOutD, OutputEnthalpy, LWBD, LDBD, HumidRatio, RelHumid, SVOutD, DenOutD, DEWPoint);
		}	//'SI
		else
		{
			EnthalpysearchSI(1, BPd, HOutD, OutputEnthalpy, LWBD, LDBD, HumidRatio, RelHumid, SVOutD, DenOutD, DEWPoint);
		}

		//'Store Density Out as Density Design and SV Out as SV Design
		DenDesign = DenOutD;
		SVDesign = SVOutD;
   
		//    PRINT USING "Design Out: LWBD=###.####   DenOutD=#.#####  SVOutD=##.#####"; LWBD; DenOutD; SVOutD
		//    PRINT #2, USING "Design Out: LWBD=###.####   DenOutD=#.#####  SVOutD=##.#####"; LWBD; DenOutD; SVOutD
		//'***********************************************************************************

		//'Next Iterate to find Test Leaving Wet bulb and DenOutT
		//'Initial guess of Leaving Wet Bulb is average of Test Entering and Leaving temperature
		LWBT = (EWTt + LWTt) / 2.0;

		bool bGoto200 = true;
		while (bGoto200)
		{
			//200 'Top of iteration loop *****************************************************************
			//' Determine conditions of air at guess Leaving Wet Bulb (assumed saturated LDB=LWB)
  
			if (IunitsIP == 1)	//'IP
			{
				CalcIPProperties(BPt, LWBT, LWBT, WOutT, RHOutT, HLWBT, SVOutT, DenOutT, DEWPointOutT);
				//CalcPropertiesIP(BPt, LWBT, LWBT, WOutT, RHOutT, HLWBT, SVOutT, DenOutT, DEWPointOutT);
				//CalcIPProperties(BPt, EWBt, EDBt, WInT, RHInT, HInT, SVInT, DenInT, DEWPointInT);
			}
			else	//'SI
			{
				CalcSIProperties(BPt, LWBT, LWBT, WOutT, RHOutT, HLWBT, SVOutT, DenOutT, DEWPointOutT);
				//CalcPropertiesSI(BPt, LWBT, LWBT, WOutT, RHOutT, HLWBT, SVOutT, DenOutT, DEWPointOutT);
			}

			//PRINT USING "LWBt=###.### WOutT=#.##### HOutT=###.#### SVOutT=##.#### DenOutT=#.#####  "; LWBT; WOutT; HLWBT; SVOutT; DenOutT
			//'     PRINT #2, USING "LWB=###.### W=#.##### H=###.#### SV=##.#### Den=#.#####  "; LWBT; WOutT; HLWBT; SVOutT; DenOutT

			//'Calculate L/G Test
			//'Equation 5.1, Liquid to Gas ratio Test
			if ((FLOWd == 0.0) || (DenOutD == 0.0) || (BHPt == 0.0) || (SVOutD == 0.0))
				LinGt = 0.0;
			else
				LinGt = LinGD * (FLOWt / FLOWd) * pow((DenOutT / DenOutD * BHPd / BHPt) ,(1.0 / 3.0)) * (SVOutT / SVOutD);
			HCalcT = HInT + LinGt * Cpw * (EWTt - LWTt);
			//PRINT USING "LinGt=##.#### HCalcT=###.#### HactT=###.#### R=##.###"; LinGt; HCalcT; HLWBT; (HCalcT / HLWBT - 1!) * 100
			//'    PRINT #2, USING "LinGt=##.#### HOoutT=###.#### Hact=###.#### R=+##.###%"; LinGT; HCalcT; HLWBT; (HCalcT / HLWBT - 1!) * 100
			//'    PRINT #2,
			//PRINT

			//'Call Enthalpy Search subroutine for calculated Hout value
			if (IunitsIP == 1)	//'IP
			{
				//'       CALL EnthalpysearchIP(sat%, P!, RootEnthalpy!, OutputEnthalpy!, TWB!, TDB!, HumidRatio!, RelHumid!, SpVolume!, Density!, DEWPoint!)
				EnthalpysearchIP(1, BPt, HCalcT, OutputEnthalpy, LWBTnew, LDBTnew, HumidRatio, RelHumid, SpVolume, Density, DEWPoint);
			}
			else	//'SI
			{
				EnthalpysearchSI(1, BPt, HCalcT, OutputEnthalpy, LWBTnew, LDBTnew, HumidRatio, RelHumid, SpVolume, Density, DEWPoint);
			}
			//PRINT USING "HCalcT=###.#### HactT=###.#### dif=+##.#### LWBT=###.#### LWBTnew=###.####"; HCalcT; HLWBT; HCalcT - HLWBT; LWBT; LWBTnew
			//'    PRINT #2, USING "HCalcT=###.#### HactT=###.#### dif=+##.#### LWBT=###.#### PLWBTnew=###.####"; HCalcT; HLWBT; HCalcT - HLWBT; LWBT; LWBTnew

			//'Check to see if Enthalpy  of Leaving Wet Bulb Test (HLWBT) converged to calculated value (HCalcT)
			if (fabs(HCalcT - HLWBT) > .0002)
			{
				LWBT = LWBTnew;
				bGoto200 = true;
			}
			else
			{
				bGoto200 = false;
			}
		}//'******** BOTTOM OF ITERATION LOOP ********************************************

#ifdef _DEBUG
		//PRINT USING "Test Out:HCalcT=###.#### HactT=###.#### dif=+##.#### LWB=###.#### LWBnew=###.####"; HCalcT; HLWBT; HCalcT - HLWBT; LWBT; LWBTnew
		//PRINT #2,
		//PRINT #2, USING "Test Out:HCalcT=###.#### Hact=###.#### dif=+##.#### LWB=###.#### LWBnew=###.####"; HCalcT; HLWBT; HCalcT - HLWBT; LWBT; LWBTnew
		//PRINT #2, USING "             LinGt=##.####  DenOutT=#.#####  SVOutT=##.#####"; LinGt; DenOutT; SVOutT
		//PRINT #2,

		strTemp.Format("Test Out:HCalcT=%.04f Hact=%.04f dif=%.04f LWB=%.04f LWBnew=%.04f\r\n", HCalcT, HLWBT, HCalcT - HLWBT, LWBT, LWBTnew);
		TRACE0(strTemp);
		strTemp.Format("             LinGt=%.04f  DenOutT=%.05f  SVOutT=%.05f\r\n\r\n", LinGt, DenOutT, SVOutT);
		TRACE0(strTemp);
#endif // _DEBUG

		//'Save convergered Density Out Test as Density Design
		DenTest = DenOutT;
	}
	else	//'Forced Draft NO ITERATION
	{
		//'        ENTERING air conditions  Test and Design
		DenTest = DenInT;
		DenDesign = DenInD;
		
		// Try these values for Forced Draft option
		// (Toolkit v3.0 Build #5, DBL - 05/23/03)
		HLWBT	= HInT; 
		SVOutT	= SVInT; 
		DenOutT = DenInT;
		LWBTnew = EWBt;
	}

	//'Equation 6.1, Adjusted TestFlow
	if ((BHPt == 0.0) || (DenDesign == 0.0))
		AdjTestFlow = 0.0;
	else
		AdjTestFlow = FLOWt * pow((BHPd / BHPt * DenTest / DenDesign), (1.0 / 3.0));


#ifdef _DEBUG
	strTemp.Format("AdjTestFlow = FLOWt   *  (BHPd / BHPt  * DenTest/DenDesign) ^ (1 / 3)\r\n");
	OutputDebugString(strTemp);
	strTemp.Format("AdjTestFlow = %.01f * (%.02f/%.02f * %.05f / %.05f) ^ (1 / 3)\r\n\r\n", FLOWt, BHPd, BHPt, DenTest, DenDesign);
	OutputDebugString(strTemp);

	if (AdjTestFlow >= 1000.0)
	{
		//PRINT #2, USING "Adjusted Test Flow=######.#"; AdjTestFlow
	}
	else
	{
		//PRINT #2, USING "Adjusted Test Flow=###.####"; AdjTestFlow
	}
	//PRINT #2,
#endif // _DEBUG
	return AdjTestFlow;
}

void EnthalpysearchIP(int sat, double P, double RootEnthalpy, double &OutputEnthalpy, double &TWB, double &TDB, double &HumidRatio, double &RelHumid, double &SpVolume, double &Density, double &DEWPoint)
{
	//'****** Procedure finds IP WB, DB & properties given enthalpy & pressure **
	//'****** Uses bisection method to search for roots.  Limits 0-140 øF *******
	//'

	//'Establish tolerance on enthalpy search
	double temptolerance = .0001;
	double Htolerance = .00005;
	double H0;
	double H140;
	double Enthalpy(0.0);

	//' First need to bracket region of WB/DB to created bisection region
	//' High and low values are 0ø and 140ø.

	//'Calculate low value and compare to program and tolerance limits

	TWB = 0.0;
	if (sat == 1)
	{
		TDB = TWB;
	}
	CalcIPProperties(P, TWB, TDB, HumidRatio, RelHumid, H0, SpVolume, Density, DEWPoint);
	//CalcPropertiesIP(P, TWB, TDB, HumidRatio, RelHumid, H0, SpVolume, Density, DEWPoint);

	if (fabs(H0 - RootEnthalpy) <= Htolerance)
	{
		return;
	}
	if (RootEnthalpy < H0)
	{
		ASSERT(0);
		//LOCATE 17, 10: PRINT "Enthalpy out of range of program"
		return;
	}

	//'Calculate high value and compare to program and tolerance limits

	TWB = 140.0;
	if (sat == 1)
	{
		TDB = TWB;
	}
	CalcIPProperties(P, TWB, TDB, HumidRatio, RelHumid, H140, SpVolume, Density, DEWPoint);
	//CalcPropertiesIP(P!, TWB!, TDB!, HumidRatio!, RelHumid!, H140, SpVolume!, Density!, DEWPoint!)

	if (fabs(H140 - RootEnthalpy) <= Htolerance)
	{
		return;
	}
	if (RootEnthalpy > H140)
	{
		ASSERT(0);
		//LOCATE 17, 10: PRINT "Enthalpy out of range of program"
		return;
	}

	//'Begin bisection root search procedure from Numerical Recipes in BASIC, p 193

	double t1 = 0.0;
	double t2 = 140.0;
	double trtbis = t1;
	double DT = t2 - t1;
	double tmid(0.0);
	double hmid;
	while (true)
	{
		DT = DT / 2.0;
		tmid = trtbis + DT;
		if (sat == 1) 
		{
			TDB = tmid;
		}
		CalcIPProperties(P, tmid, TDB, HumidRatio, RelHumid, Enthalpy, SpVolume, Density, DEWPoint);
		//CalcPropertiesIP(P!, tmid, TDB!, HumidRatio!, RelHumid!, Enthalpy!, SpVolume!, Density!, DEWPoint!)
		hmid = RootEnthalpy - Enthalpy;
		if (hmid >= 0.0)
		{
			trtbis = tmid;
		}
		if ((fabs(DT) < temptolerance) || (hmid == 0.0))
		{
			break;
		}
	}
	TWB = tmid;
	if (sat == 1)
	{
		TDB = tmid;
	}
	OutputEnthalpy = Enthalpy;
}

void EnthalpysearchSI(int sat, double P, double RootEnthalpy, double &OutputEnthalpy, double &TWB, double &TDB, double &HumidRatio, double &RelHumid, double &SpVolume, double &Density, double &DEWPoint)
{
	//'****** Procedure finds SI WB, DB & properties given enthalpy & pressure **
	//'****** Uses bisection method to search for roots.  Limits -20 to 60 øC ****

	//'Establish tolerance on enthalpy search

	double temptolerance = .00001;
	double Htolerance = .00005;
	double H20;
	double H60;
	double Enthalpy(0.0);

	//' First need to bracket region of WB/DB to created bisection region
	//' High and low values are -20ø and 60ø.

	//'Calculate low value and compare to program and tolerance limits

	TWB = -20.0;
	if (sat == 1) 
	{
		TDB = TWB;
	}
	CalcSIProperties(P, TWB, TDB, HumidRatio, RelHumid, H20, SpVolume, Density, DEWPoint);
	//CalcPropertiesSI(P!, TWB!, TDB!, HumidRatio!, RelHumid!, H20, SpVolume!, Density!, DEWPoint!)

	if (fabs(H20 - RootEnthalpy) <= Htolerance)
	{
		return;
	}
	if (RootEnthalpy < H20)
	{
		ASSERT(0);
		//LOCATE 17, 10: PRINT "Enthalpy out of range of program"
		return;
	}

	//'Calculate high value and compare to program and tolerance limits

	TWB = 60.0;
	if (sat == 1)
	{
		TDB = TWB;
	}
	CalcSIProperties(P, TWB, TDB, HumidRatio, RelHumid, H60, SpVolume, Density, DEWPoint);
	//CalcPropertiesSI(P!, TWB!, TDB!, HumidRatio!, RelHumid!, H60, SpVolume!, Density!, DEWPoint!)

	if (fabs(H60 - RootEnthalpy) <= Htolerance)
	{
		return;
	}
	if (RootEnthalpy > H60)
	{
		ASSERT(0);
		//LOCATE 17, 10: PRINT "Enthalpy out of range of program"
		return;
	}

	//'Begin bisection root search procedure from Numerical Recipes in BASIC, p 193

	double t1 = -20.0;
	double t2 = 60.0;
	double trtbis = t1;
	double DT = t2 - t1;
	double tmid(0.0);
	double hmid;
	while (true)
	{
		DT = DT / 2.0;
		tmid = trtbis + DT;
		if (sat == 1) 
		{
			TDB = tmid;
		}
		CalcSIProperties(P, tmid, TDB, HumidRatio, RelHumid, Enthalpy, SpVolume, Density, DEWPoint);
		//CalcPropertiesSI(P!, tmid, TDB!, HumidRatio!, RelHumid!, Enthalpy!, SpVolume!, Density!, DEWPoint!)
		hmid = RootEnthalpy - Enthalpy;
		if (hmid >= 0.0)
		{
			trtbis = tmid;
		}
		if ((fabs(DT) < temptolerance) || (hmid == 0.0))
		{
			break;
		}
	}
	TWB = tmid;
	if (sat == 1)
	{
		TDB = tmid;
	}
	OutputEnthalpy = Enthalpy;
}

//---------------------------------------------------------------------
// End: CTIUtils.cpp
//---------------------------------------------------------------------
