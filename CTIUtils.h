//---------------------------------------------------------------------
//
// CTIUtils.h : 
//
// Copyright c 1997-1998 Cosmo Engineering  -  All Rights Reserved.
//
// $Archive: /BlueBook 2.0/CTIUtils.h $
// $Revision: 5 $
// $Modtime: 2/12/01 1:14p $
//
//---------------------------------------------------------------------
//
// $Log: /BlueBook 2.0/CTIUtils.h $
// 
// 5     2/13/01 3:56p Deanp
// Added the Test L/G to the returned items.
// 
// Also no longer do the funny bussiness with the truncit funtion
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
// 13    5/12/99 9:07a Administrator
// fixes for version 1.0.0.3 to fix IP/SI problems
// 
// 12    5/05/99 7:36a Administrator
// added degrees K for range
// 
// 11    2/25/99 6:50a Deanp
// added barometric pressure to altitude functions
// 
//---------------------------------------------------------------------
#ifndef CTIUTILS_H
#define CTIUTILS_H

#include <math.h>

//---------------------------------------------------------------------
// Unit labels
//---------------------------------------------------------------------
#define L_PERCENT	"%"
#define L_DEGF		"°F"
#define L_DEGC		"°C"
#define L_DEGK		"K"
#define L_FEET		"ft"
#define L_METERS	"m"
#define L_HG		"\"Hg"
#define L_KPA		"kPa"
#define L_BTULBM	"Btu/lbm"
#define L_KJKG		"kJ/kg"
#define L_GPM		"gpm"
#define L_LPS		"l/s"
#define L_BHP		"bhp"
#define L_KW		"kW"


const char pszIPTemp[] = "°F";
const char pszSITemp[] = "°C";
const char pszIPPres[] = "Hg";
const char pszSIPres[] = "kPa";
const char pszPercent[] = "%";
const char pszIPEnthrapy[] = "Btu mixture/lbm dry air";
const char pszSIEnthrapy[] = "kJ mixture/kg dry air";
const char pszIPDensity[] = "lbm mixture/ft³";
const char pszSIDensity[] = "kg mixture/m³";
const char pszIPSPVolume[] = "ft³/lbm dry air";
const char pszSISPVolume[] = "m³/kg dry air";
const char pszIPHumidityRatio[] = "lbm water/lbm dry air";
const char pszSIHumidityRatio[] = "kg water/kg dry air";
const char pszMerkelOut[] = "KaV/L = %.5f";


//---------------------------------------------------------------------
// Unit conversion functions
//---------------------------------------------------------------------
double KPAToPSI(double kpa);
double PSIToKPA(double psi);
double calcPressureF(double p);
double calcPressureC(double p);
double Altitude2PSI(double altitude);
double Altitude2KPA(double altitude);
double PSI2Altitude(double PSI);
double KPA2Altitude(double KPA);
double fnCelcToFar(double dblTemp);
double fnFarToCelc(double dblTemp);
double fnFeetToMeters(double dblDist);
double fnMetersToFeet(double dblDist);
double fnBHPToKW(double dblBHP);
double fnKWToBHP(double dblKW);
double fnGalToLiter(double dblGal);
double fnLiterToGal(double dblLiter);
double fnGPMToLS(double dblGPM);
double fnLSToGPM(double dblLS);



double HumidityRatioIP     (double p, double TDB, double TWB);       
double DegreeOfSaturationIP(double p, double TDB, double TWB);
double RelativeHumidityIP  (double p, double TDB, double TWB);
double SpecificVolumeIP    (double p, double TDB, double TWB);
double CalcDensityIP       (double p, double TDB, double TWB);
double CalcEnthalpyIP      (double p, double TDB, double TWB);
double CalcDEWPointIP      (double p, double TDB, double TWB);

// streamlined IP Enthalpy function for demand curves
double Enthalpy            (double p, double TDB, double TWB); 

double HumidityRatioSI     (double p, double TDB, double TWB);       
double DegreeOfSaturationSI(double p, double TDB, double TWB);
double RelativeHumiditySI  (double p, double TDB, double TWB);
double SpecificVolumeSI    (double p, double TDB, double TWB);
double CalcDensitySI       (double p, double TDB, double TWB);
double CalcEnthalpySI      (double p, double TDB, double TWB);
double CalcDEWPointSI      (double p, double TDB, double TWB);

double truncit( double, int );
void   CalcIPProperties (double p, double TWB, double TDB, double &HumidRatio, double &RelHumid, double &Enthalpy, double &SpVolume, double &Density, double &DEWPoint);
double IPPws (double tair);
double Fs (double t, double p);
double IPDEWPoint (double p, double TWB, double TDB, double HumidRatio);

void CalcSIProperties (double p, double TWB, double TDB, double &HumidRatio, double &RelHumid, double &Enthalpy, double &SpVolume, double &Density, double &DEWPoint);
double SIPws (double tair);
double SIDEWPoint (double p, double TWB, double TDB, double HumidRatio);
double KAVL(double T1, double T2, double WBT, double LG);

//double abs(double dblNum);

double Pstd(double Z);
#define Tboil	(double)212.0
#define Patm	(double)14.696
double Merkel(double Twb,double Ran,double Apr,double LG,double Elev);
double Interpolate(double top, double inc, int nvals, double Twb, double Ran, double Apr, double LG, double Elev);
double Hdbw(double Tdb,double W);
double Wsat(double Ts);
double Pwsat(double Ts);

double IPRelHumidity(double psi, double TWB, double TDB);
double SIRelHumidity( double psi, double TWB, double TDB);
void   IPWBsearch (double psi, double RelHumid, double TDB, double& TWB);
void   SIWBsearch (double psi, double RelHumid, double TDB, double& TWB);

void   IPEnthalpysearch (int sat, 
                       double p, 
                       double RootEnthalpy, 
                       double& OutputEnthalpy, 
                       double& TWB, 
                       double& TDB, 
                       double HumidRatio, 
                       double RelHumid, 
                       double SpVolume, 
                       double Density, 
                       double DEWPoint);

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
                       double DEWPoint);



void CalcPerfData(int INUM, double* X, double* YMEAS, double& XREAL, double& YFIT, double* Y2);
void SPLINE(double* X, double* Y, int INUM, double YP1, double YPN, double* Y2);
void SPLINT(double* XA, double* YA, double* Y2A, int INUM, double& X, double& Y);
double CalcTestLG(double dblDesignLG, double dblDesignFlow, double dblTestFlow, double dblDesignFanPower, double dblTestFanPower, double dblDesignAirDensity, double dblTestAirDensity, double dblDesignSpecificVolume, double dblTestSpecificVolume);
double DetermineAdjTestFlow(int IunitsIP, int IInduced, double EWTd, double LWTd, double EWBd, double EDBd, double BPd, double FLOWd, double BHPd, double LinGD, double EWTt, double LWTt, double EWBt, double EDBt, double BPt, double FLOWt, double BHPt, double &LWBTnew, double &DenOutT, double &SVOutT, double &HLWBT, double &AdjTestFlow, double &LinGt);
void EnthalpysearchIP(int sat, double P, double RootEnthalpy, double &OutputEnthalpy, double &TWB, double &TDB, double &HumidRatio, double &RelHumid, double &SpVolume, double &Density, double &DEWPoint);
void EnthalpysearchSI(int sat, double P, double RootEnthalpy, double &OutputEnthalpy, double &TWB, double &TDB, double &HumidRatio, double &RelHumid, double &SpVolume, double &Density, double &DEWPoint);

#endif
//---------------------------------------------------------------------
// End: CTIUtils.h
//---------------------------------------------------------------------
