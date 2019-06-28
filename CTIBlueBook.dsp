# Microsoft Developer Studio Project File - Name="CTI BlueBook" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=CTI BlueBook - Win32 Demo Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "CTIBlueBook.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "CTIBlueBook.mak" CFG="CTI BlueBook - Win32 Demo Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "CTI BlueBook - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "CTI BlueBook - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "CTI BlueBook - Win32 PerfCurve Debug" (based on "Win32 (x86) Application")
!MESSAGE "CTI BlueBook - Win32 PerfCurve Release" (based on "Win32 (x86) Application")
!MESSAGE "CTI BlueBook - Win32 Demo Debug" (based on "Win32 (x86) Application")
!MESSAGE "CTI BlueBook - Win32 Demo Release" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "orstrich"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "CTI BlueBook - Win32 Release"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Toolkit\Release"
# PROP Intermediate_Dir "Toolkit\Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W4 /GX /Od /I "C:\Program Files\HTML Help Workshop\include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /FR /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo /o"Toolkit\Release\CTIToolkit.bsc"
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 version.lib htmlhelp.lib /nologo /subsystem:windows /machine:I386 /out:"Toolkit\Release\CTIToolkit.exe" /libpath:"C:\Program Files\HTML Help Workshop\lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "CTI BlueBook - Win32 Debug"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Toolkit\Debug"
# PROP Intermediate_Dir "Toolkit\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MTd /W4 /Gm /GX /ZI /Od /I "C:\Program Files\HTML Help Workshop\include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /FR /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo /o"Toolkit\Debug\CTIToolkit.bsc"
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 version.lib htmlhelp.lib /nologo /subsystem:windows /debug /machine:I386 /out:"Toolkit\Debug\CTIToolkit.exe" /pdbtype:sept /libpath:"C:\Program Files\HTML Help Workshop\lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "CTI BlueBook - Win32 PerfCurve Debug"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "CTI_BlueBook___Win32_PerfCurve_Debug"
# PROP BASE Intermediate_Dir "CTI_BlueBook___Win32_PerfCurve_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "PerfCurve\Debug"
# PROP Intermediate_Dir "PerfCurve\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W4 /Gm /GX /ZI /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /FR /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MTd /W4 /Gm /GX /ZI /Od /I "C:\Program Files\HTML Help Workshop\include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_PERF_CURVE_ONLY" /FR /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_PERF_CURVE_ONLY"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo /o"Debug/CTIToolkit.bsc"
# ADD BSC32 /nologo /o"PerfCurve\Debug\CTIPerfCurve.bsc"
LINK32=link.exe
# ADD BASE LINK32 version.lib /nologo /subsystem:windows /debug /machine:I386 /out:"Debug/CTIToolkit.exe" /pdbtype:sept
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 version.lib htmlhelp.lib /nologo /subsystem:windows /debug /machine:I386 /out:"PerfCurve\Debug\CTIPerfCurve.exe" /pdbtype:sept /libpath:"C:\Program Files\HTML Help Workshop\lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "CTI BlueBook - Win32 PerfCurve Release"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "CTI_BlueBook___Win32_PerfCurve_Release"
# PROP BASE Intermediate_Dir "CTI_BlueBook___Win32_PerfCurve_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "PerfCurve\Release"
# PROP Intermediate_Dir "PerfCurve\Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W4 /GX /Od /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W4 /GX /Od /I "C:\Program Files\HTML Help Workshop\include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_PERF_CURVE_ONLY" /FR /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_PERF_CURVE_ONLY"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo /o"Release/CTIToolkit.bsc"
# ADD BSC32 /nologo /o"PerfCurve\Release\CTIPerfCurve.bsc"
LINK32=link.exe
# ADD BASE LINK32 version.lib /nologo /subsystem:windows /machine:I386 /out:"Release/CTIToolkit.exe"
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 version.lib htmlhelp.lib /nologo /subsystem:windows /machine:I386 /out:"PerfCurve\Release\CTIPerfCurve.exe" /libpath:"C:\Program Files\HTML Help Workshop\lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "CTI BlueBook - Win32 Demo Debug"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "CTI_BlueBook___Win32_Demo_Debug"
# PROP BASE Intermediate_Dir "CTI_BlueBook___Win32_Demo_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Demo\Debug"
# PROP Intermediate_Dir "Demo\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W4 /Gm /GX /ZI /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /FR /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MTd /W4 /Gm /GX /ZI /Od /I "C:\Program Files\HTML Help Workshop\include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_DEMO_VERSION" /FR /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo /o"Debug/CTIToolkit.bsc"
# ADD BSC32 /nologo /o"Demo\Debug\CTIDemo.bsc"
LINK32=link.exe
# ADD BASE LINK32 version.lib /nologo /subsystem:windows /debug /machine:I386 /out:"Debug/CTIToolkit.exe" /pdbtype:sept
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 version.lib htmlhelp.lib /nologo /subsystem:windows /debug /machine:I386 /out:"Demo\Debug\CTIDemo.exe" /pdbtype:sept /libpath:"C:\Program Files\HTML Help Workshop\lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "CTI BlueBook - Win32 Demo Release"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "CTI_BlueBook___Win32_Demo_Release"
# PROP BASE Intermediate_Dir "CTI_BlueBook___Win32_Demo_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Demo\Release"
# PROP Intermediate_Dir "Demo\Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W4 /GX /Od /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W4 /GX /Od /I "C:\Program Files\HTML Help Workshop\include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_DEMO_VERSION" /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo /o"Release/CTIToolkit.bsc"
# ADD BSC32 /nologo /o"Demo\Release\CTIDemo.bsc"
LINK32=link.exe
# ADD BASE LINK32 version.lib /nologo /subsystem:windows /machine:I386 /out:"Release/CTIToolkit.exe"
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 htmlhelp.lib version.lib /nologo /subsystem:windows /machine:I386 /out:"Demo\Release\CTIDemo.exe" /libpath:"C:\Program Files\HTML Help Workshop\lib"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "CTI BlueBook - Win32 Release"
# Name "CTI BlueBook - Win32 Debug"
# Name "CTI BlueBook - Win32 PerfCurve Debug"
# Name "CTI BlueBook - Win32 PerfCurve Release"
# Name "CTI BlueBook - Win32 Demo Debug"
# Name "CTI BlueBook - Win32 Demo Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\areaseries.cpp
# End Source File
# Begin Source File

SOURCE=.\arrowseries.cpp
# End Source File
# Begin Source File

SOURCE=.\aspect.cpp
# End Source File
# Begin Source File

SOURCE=.\axes.cpp
# End Source File
# Begin Source File

SOURCE=.\axis.cpp
# End Source File
# Begin Source File

SOURCE=.\axislabels.cpp
# End Source File
# Begin Source File

SOURCE=.\axistitle.cpp
# End Source File
# Begin Source File

SOURCE=.\barseries.cpp
# End Source File
# Begin Source File

SOURCE=.\brush.cpp
# End Source File
# Begin Source File

SOURCE=.\bubbleseries.cpp
# End Source File
# Begin Source File

SOURCE=.\candleseries.cpp
# End Source File
# Begin Source File

SOURCE=.\canvas.cpp
# End Source File
# Begin Source File

SOURCE=.\cdxCSizingDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\CError.cpp
# End Source File
# Begin Source File

SOURCE=.\chartfont.cpp
# End Source File
# Begin Source File

SOURCE=".\CTI BlueBook.cpp"
# End Source File
# Begin Source File

SOURCE=".\CTI Demo.rc"
# End Source File
# Begin Source File

SOURCE=.\CTIUtils.cpp
# End Source File
# Begin Source File

SOURCE=.\curvefittingfunction.cpp
# End Source File
# Begin Source File

SOURCE=.\customseries.cpp
# End Source File
# Begin Source File

SOURCE=.\errorbarseries.cpp
# End Source File
# Begin Source File

SOURCE=.\expavgfunction.cpp
# End Source File
# Begin Source File

SOURCE=.\export.cpp
# End Source File
# Begin Source File

SOURCE=.\fastlineseries.cpp
# End Source File
# Begin Source File

SOURCE=.\ganttseries.cpp
# End Source File
# Begin Source File

SOURCE=.\gradient.cpp
# End Source File
# Begin Source File

SOURCE=.\horizbarseries.cpp
# End Source File
# Begin Source File

SOURCE=.\import.cpp
# End Source File
# Begin Source File

SOURCE=.\legend.cpp
# End Source File
# Begin Source File

SOURCE=.\ListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\marks.cpp
# End Source File
# Begin Source File

SOURCE=.\movingavgfunction.cpp
# End Source File
# Begin Source File

SOURCE=.\page.cpp
# End Source File
# Begin Source File

SOURCE=.\panel.cpp
# End Source File
# Begin Source File

SOURCE=.\pen.cpp
# End Source File
# Begin Source File

SOURCE=.\pieseries.cpp
# End Source File
# Begin Source File

SOURCE=.\pointer.cpp
# End Source File
# Begin Source File

SOURCE=.\pointseries.cpp
# End Source File
# Begin Source File

SOURCE=.\polarseries.cpp
# End Source File
# Begin Source File

SOURCE=.\printer.cpp
# End Source File
# Begin Source File

SOURCE=.\PrintFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\PrintList.cpp
# End Source File
# Begin Source File

SOURCE=.\PrintPerfReport.cpp
# End Source File
# Begin Source File

SOURCE=.\PrintView.cpp
# End Source File
# Begin Source File

SOURCE=.\scroll.cpp
# End Source File
# Begin Source File

SOURCE=.\series.cpp
# End Source File
# Begin Source File

SOURCE=.\shapeseries.cpp
# End Source File
# Begin Source File

SOURCE=.\Splash.cpp
# End Source File
# Begin Source File

SOURCE=.\StaticHyperLink.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\strings.cpp
# End Source File
# Begin Source File

SOURCE=.\surfaceseries.cpp
# End Source File
# Begin Source File

SOURCE=.\tchart.cpp
# End Source File
# Begin Source File

SOURCE=.\teefunction.cpp
# End Source File
# Begin Source File

SOURCE=.\titles.cpp
# End Source File
# Begin Source File

SOURCE=.\TMultiEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\TNewDemandCurveDataDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TNewFlowRateDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TNewPerformanceDataFileDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TNewTestDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ToolkitDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\TPerformanceCurveDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TPerformanceDataDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TPrint.cpp
# End Source File
# Begin Source File

SOURCE=.\TPrintPerfReportDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TPropPageBase.cpp
# End Source File
# Begin Source File

SOURCE=.\TPropPageFour.cpp
# End Source File
# Begin Source File

SOURCE=.\TPropPageOne.cpp
# End Source File
# Begin Source File

SOURCE=.\TPropPageThree.cpp
# End Source File
# Begin Source File

SOURCE=.\TPropPageTwo.cpp
# End Source File
# Begin Source File

SOURCE=.\TPropPageZero.cpp

!IF  "$(CFG)" == "CTI BlueBook - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "CTI BlueBook - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "CTI BlueBook - Win32 PerfCurve Debug"

!ELSEIF  "$(CFG)" == "CTI BlueBook - Win32 PerfCurve Release"

!ELSEIF  "$(CFG)" == "CTI BlueBook - Win32 Demo Debug"

!ELSEIF  "$(CFG)" == "CTI BlueBook - Win32 Demo Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TTowerPerformanceFile.cpp
# End Source File
# Begin Source File

SOURCE=.\valuelist.cpp
# End Source File
# Begin Source File

SOURCE=.\volumeseries.cpp
# End Source File
# Begin Source File

SOURCE=.\wall.cpp
# End Source File
# Begin Source File

SOURCE=.\walls.cpp
# End Source File
# Begin Source File

SOURCE=.\zoom.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\cdxCSizingDialog.h
# End Source File
# Begin Source File

SOURCE=".\CTI BlueBook.h"
# End Source File
# Begin Source File

SOURCE=.\CTIUtils.h
# End Source File
# Begin Source File

SOURCE=.\ListCtrl.h
# End Source File
# Begin Source File

SOURCE=.\PrintFrame.h
# End Source File
# Begin Source File

SOURCE=.\PrintList.h
# End Source File
# Begin Source File

SOURCE=.\PrintPerfReport.h
# End Source File
# Begin Source File

SOURCE=.\PrintView.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\Splash.h
# End Source File
# Begin Source File

SOURCE=.\StaticHyperLink.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\TMultiEdit.h
# End Source File
# Begin Source File

SOURCE=.\TNewDemandCurveDataDlg.h
# End Source File
# Begin Source File

SOURCE=.\TNewFlowRateDlg.h
# End Source File
# Begin Source File

SOURCE=.\TNewPerformanceDataFileDlg.h
# End Source File
# Begin Source File

SOURCE=.\TNewTestDlg.h
# End Source File
# Begin Source File

SOURCE=.\ToolkitDoc.h
# End Source File
# Begin Source File

SOURCE=.\TPerformanceCurveDlg.h
# End Source File
# Begin Source File

SOURCE=.\TPerformanceDataDlg.h
# End Source File
# Begin Source File

SOURCE=.\TPrint.h
# End Source File
# Begin Source File

SOURCE=.\TPrintPerfReportDlg.h
# End Source File
# Begin Source File

SOURCE=.\TPropPageBase.h
# End Source File
# Begin Source File

SOURCE=.\TPropPageFour.h
# End Source File
# Begin Source File

SOURCE=.\TPropPageOne.h
# End Source File
# Begin Source File

SOURCE=.\TPropPageThree.h
# End Source File
# Begin Source File

SOURCE=.\TPropPageTwo.h
# End Source File
# Begin Source File

SOURCE=.\TPropPageZero.h
# End Source File
# Begin Source File

SOURCE=.\TTowerPerformanceFile.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\Res\256logo.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Bwlogo.bmp
# End Source File
# Begin Source File

SOURCE=.\res\colorlogo.bmp
# End Source File
# Begin Source File

SOURCE=".\res\CTI Demo.ico"
# End Source File
# Begin Source File

SOURCE=".\res\CTI Demo.rc2"
# End Source File
# Begin Source File

SOURCE=".\res\CTI DemoDoc.ico"
# End Source File
# Begin Source File

SOURCE=.\res\cursor1.cur
# End Source File
# Begin Source File

SOURCE=.\Res\graph.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Hand.cur
# End Source File
# Begin Source File

SOURCE=.\res\install.ico
# End Source File
# Begin Source File

SOURCE=.\res\splash.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# End Group
# End Target
# End Project
# Section CTI BlueBook : {B6352504-45FD-11D1-AB84-00400141862D}
# 	2:5:Class:CAxisLabels
# 	2:10:HeaderFile:axislabels.h
# 	2:8:ImplFile:axislabels.cpp
# End Section
# Section CTI BlueBook : {E8B97A90-3E69-11D1-AB59-00400141862D}
# 	2:5:Class:CAspect
# 	2:10:HeaderFile:aspect.h
# 	2:8:ImplFile:aspect.cpp
# End Section
# Section CTI BlueBook : {00561F60-EFA9-11D0-9F64-DE033FB8E6C0}
# 	2:5:Class:CGanttSeries
# 	2:10:HeaderFile:ganttseries.h
# 	2:8:ImplFile:ganttseries.cpp
# End Section
# Section CTI BlueBook : {07A65C40-3DA6-11D1-AB57-00400141862D}
# 	2:5:Class:CZoom
# 	2:10:HeaderFile:zoom.h
# 	2:8:ImplFile:zoom.cpp
# End Section
# Section CTI BlueBook : {6F617F12-CFA9-11D0-BEED-00AA00C9A74F}
# 	2:5:Class:CPointSeries
# 	2:10:HeaderFile:pointseries.h
# 	2:8:ImplFile:pointseries.cpp
# End Section
# Section CTI BlueBook : {6F617F0E-CFA9-11D0-BEED-00AA00C9A74F}
# 	2:5:Class:CSeries
# 	2:10:HeaderFile:series.h
# 	2:8:ImplFile:series.cpp
# End Section
# Section CTI BlueBook : {A9752906-3FA0-11D1-9F64-444553540000}
# 	2:5:Class:CMovingAvgFunction
# 	2:10:HeaderFile:movingavgfunction.h
# 	2:8:ImplFile:movingavgfunction.cpp
# End Section
# Section CTI BlueBook : {008BBE7C-C096-11D0-B4E3-00A0C901D681}
# 	2:5:Class:CTChart
# 	2:10:HeaderFile:tchart.h
# 	2:8:ImplFile:tchart.cpp
# End Section
# Section CTI BlueBook : {F3A0ED82-3C01-11D1-AB52-00400141862D}
# 	2:5:Class:CWalls
# 	2:10:HeaderFile:walls.h
# 	2:8:ImplFile:walls.cpp
# End Section
# Section CTI BlueBook : {7D423BA1-D053-11D0-9F64-837B7E193D48}
# 	2:5:Class:CGradient
# 	2:10:HeaderFile:gradient.h
# 	2:8:ImplFile:gradient.cpp
# End Section
# Section CTI BlueBook : {6F617F11-CFA9-11D0-BEED-00AA00C9A74F}
# 	2:5:Class:CAreaSeries
# 	2:10:HeaderFile:areaseries.h
# 	2:8:ImplFile:areaseries.cpp
# End Section
# Section CTI BlueBook : {DD96FA40-D15C-11D0-9F64-9B8561B59916}
# 	2:5:Class:CAxes
# 	2:10:HeaderFile:axes.h
# 	2:8:ImplFile:axes.cpp
# End Section
# Section CTI BlueBook : {495A9400-D2B9-11D0-9F64-9FE810E73741}
# 	2:5:Class:CLegend
# 	2:10:HeaderFile:legend.h
# 	2:8:ImplFile:legend.cpp
# End Section
# Section CTI BlueBook : {C648F0C8-B5C8-11D0-9F64-B10C6D3C1C4F}
# 	2:5:Class:CPage
# 	2:10:HeaderFile:page.h
# 	2:8:ImplFile:page.cpp
# End Section
# Section CTI BlueBook : {6F617F41-CFA9-11D0-BEED-00AA00C9A74F}
# 	2:5:Class:CFastLineSeries
# 	2:10:HeaderFile:fastlineseries.h
# 	2:8:ImplFile:fastlineseries.cpp
# End Section
# Section CTI BlueBook : {B6352502-45FD-11D1-AB84-00400141862D}
# 	2:5:Class:CImport
# 	2:10:HeaderFile:import.h
# 	2:8:ImplFile:import.cpp
# End Section
# Section CTI BlueBook : {8B625A00-3A9A-11D1-AB4E-00400141862D}
# 	2:5:Class:CCanvas
# 	2:10:HeaderFile:canvas.h
# 	2:8:ImplFile:canvas.cpp
# End Section
# Section CTI BlueBook : {133FE602-D1CD-11D0-84B8-006097386599}
# 	2:5:Class:CAxisTitle
# 	2:10:HeaderFile:axistitle.h
# 	2:8:ImplFile:axistitle.cpp
# End Section
# Section CTI BlueBook : {9B02A321-CDF4-11D0-84B8-006097386599}
# 	2:5:Class:CAxis
# 	2:10:HeaderFile:axis.h
# 	2:8:ImplFile:axis.cpp
# End Section
# Section CTI BlueBook : {2800ECA0-D098-11D0-9F64-BF71EF49FEF2}
# 	2:5:Class:CTitles
# 	2:10:HeaderFile:titles.h
# 	2:8:ImplFile:titles.cpp
# End Section
# Section CTI BlueBook : {EE05DFE2-5549-11D0-9EA9-0020AF3D82DA}
# 	2:5:Class:CStrings
# 	2:10:HeaderFile:strings.h
# 	2:8:ImplFile:strings.cpp
# End Section
# Section CTI BlueBook : {49006AC3-099B-11D1-9F64-444553540000}
# 	2:5:Class:CVolumeSeries
# 	2:10:HeaderFile:volumeseries.h
# 	2:8:ImplFile:volumeseries.cpp
# End Section
# Section CTI BlueBook : {A9752900-3FA0-11D1-9F64-444553540000}
# 	2:5:Class:CCurveFittingFunction
# 	2:10:HeaderFile:curvefittingfunction.h
# 	2:8:ImplFile:curvefittingfunction.cpp
# End Section
# Section CTI BlueBook : {6F617F15-CFA9-11D0-BEED-00AA00C9A74F}
# 	2:5:Class:CHorizBarSeries
# 	2:10:HeaderFile:horizbarseries.h
# 	2:8:ImplFile:horizbarseries.cpp
# End Section
# Section CTI BlueBook : {6F617F40-CFA9-11D0-BEED-00AA00C9A74F}
# 	2:5:Class:CPieSeries
# 	2:10:HeaderFile:pieseries.h
# 	2:8:ImplFile:pieseries.cpp
# End Section
# Section CTI BlueBook : {D151BD25-0900-11D1-ABC8-006097385FF5}
# 	2:5:Class:CSurfaceSeries
# 	2:10:HeaderFile:surfaceseries.h
# 	2:8:ImplFile:surfaceseries.cpp
# End Section
# Section CTI BlueBook : {BEF6E002-A874-101A-8BBA-00AA00300CAB}
# 	2:5:Class:CChartFont
# 	2:10:HeaderFile:chartfont.h
# 	2:8:ImplFile:chartfont.cpp
# End Section
# Section CTI BlueBook : {A9752904-3FA0-11D1-9F64-444553540000}
# 	2:5:Class:CExpAvgFunction
# 	2:10:HeaderFile:expavgfunction.h
# 	2:8:ImplFile:expavgfunction.cpp
# End Section
# Section CTI BlueBook : {D151BD20-0900-11D1-ABC8-006097385FF5}
# 	2:5:Class:CPolarSeries
# 	2:10:HeaderFile:polarseries.h
# 	2:8:ImplFile:polarseries.cpp
# End Section
# Section CTI BlueBook : {00561F62-EFA9-11D0-9F64-DE033FB8E6C0}
# 	2:5:Class:CArrowSeries
# 	2:10:HeaderFile:arrowseries.h
# 	2:8:ImplFile:arrowseries.cpp
# End Section
# Section CTI BlueBook : {07A65C42-3DA6-11D1-AB57-00400141862D}
# 	2:5:Class:CScroll
# 	2:10:HeaderFile:scroll.h
# 	2:8:ImplFile:scroll.cpp
# End Section
# Section CTI BlueBook : {6F617F14-CFA9-11D0-BEED-00AA00C9A74F}
# 	2:5:Class:CBarSeries
# 	2:10:HeaderFile:barseries.h
# 	2:8:ImplFile:barseries.cpp
# End Section
# Section CTI BlueBook : {6F617F39-CFA9-11D0-BEED-00AA00C9A74F}
# 	2:5:Class:CPointer
# 	2:10:HeaderFile:pointer.h
# 	2:8:ImplFile:pointer.cpp
# End Section
# Section CTI BlueBook : {6F617F1B-CFA9-11D0-BEED-00AA00C9A74F}
# 	2:5:Class:CBrush1
# 	2:10:HeaderFile:brush.h
# 	2:8:ImplFile:brush.cpp
# End Section
# Section CTI BlueBook : {6F617F44-CFA9-11D0-BEED-00AA00C9A74F}
# 	2:5:Class:CTeeFunction
# 	2:10:HeaderFile:teefunction.h
# 	2:8:ImplFile:teefunction.cpp
# End Section
# Section CTI BlueBook : {008BBE7E-C096-11D0-B4E3-00A0C901D681}
# 	2:21:DefaultSinkHeaderFile:tchart.h
# 	2:16:DefaultSinkClass:CTChart
# End Section
# Section CTI BlueBook : {F3A0ED84-3C01-11D1-AB52-00400141862D}
# 	2:5:Class:CPrinter
# 	2:10:HeaderFile:printer.h
# 	2:8:ImplFile:printer.cpp
# End Section
# Section CTI BlueBook : {D389A20A-C6E4-11D0-84B8-006097386599}
# 	2:5:Class:CPen1
# 	2:10:HeaderFile:pen.h
# 	2:8:ImplFile:pen.cpp
# End Section
# Section CTI BlueBook : {00561F61-EFA9-11D0-9F64-DE033FB8E6C0}
# 	2:5:Class:CBubbleSeries
# 	2:10:HeaderFile:bubbleseries.h
# 	2:8:ImplFile:bubbleseries.cpp
# End Section
# Section CTI BlueBook : {66833F80-EF3F-11D0-9F64-A6119FA8432F}
# 	2:5:Class:CShapeSeries
# 	2:10:HeaderFile:shapeseries.h
# 	2:8:ImplFile:shapeseries.cpp
# End Section
# Section CTI BlueBook : {6F617F0F-CFA9-11D0-BEED-00AA00C9A74F}
# 	2:5:Class:CCustomSeries
# 	2:10:HeaderFile:customseries.h
# 	2:8:ImplFile:customseries.cpp
# End Section
# Section CTI BlueBook : {B6352500-45FD-11D1-AB84-00400141862D}
# 	2:5:Class:CExport
# 	2:10:HeaderFile:export.h
# 	2:8:ImplFile:export.cpp
# End Section
# Section CTI BlueBook : {6F617F18-CFA9-11D0-BEED-00AA00C9A74F}
# 	2:5:Class:CMarks
# 	2:10:HeaderFile:marks.h
# 	2:8:ImplFile:marks.cpp
# End Section
# Section CTI BlueBook : {49006AC1-099B-11D1-9F64-444553540000}
# 	2:5:Class:CErrorBarSeries
# 	2:10:HeaderFile:errorbarseries.h
# 	2:8:ImplFile:errorbarseries.cpp
# End Section
# Section CTI BlueBook : {6F617F43-CFA9-11D0-BEED-00AA00C9A74F}
# 	2:5:Class:CValueList
# 	2:10:HeaderFile:valuelist.h
# 	2:8:ImplFile:valuelist.cpp
# End Section
# Section CTI BlueBook : {4C33D2E0-D291-11D0-9F64-BCC543D9C168}
# 	2:5:Class:CWall
# 	2:10:HeaderFile:wall.h
# 	2:8:ImplFile:wall.cpp
# End Section
# Section CTI BlueBook : {D151BD23-0900-11D1-ABC8-006097385FF5}
# 	2:5:Class:CCandleSeries
# 	2:10:HeaderFile:candleseries.h
# 	2:8:ImplFile:candleseries.cpp
# End Section
# Section CTI BlueBook : {297DACC0-D044-11D0-9F64-C8FD4AEAB040}
# 	2:5:Class:CPanel
# 	2:10:HeaderFile:panel.h
# 	2:8:ImplFile:panel.cpp
# End Section
