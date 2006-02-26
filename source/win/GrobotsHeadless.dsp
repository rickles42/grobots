# Microsoft Developer Studio Project File - Name="GrobotsHeadless" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=GrobotsHeadless - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "GrobotsHeadless.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "GrobotsHeadless.mak" CFG="GrobotsHeadless - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "GrobotsHeadless - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "GrobotsHeadless - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "GrobotsHeadless - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseHeadless"
# PROP Intermediate_Dir "ReleaseHeadless"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GR /GX /I "../support" /I "../sides" /I "../simulation" /I "../brains" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D HEADLESS=1 /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386

!ELSEIF  "$(CFG)" == "GrobotsHeadless - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DebugHeadless"
# PROP Intermediate_Dir "DebugHeadless"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GR /GX /ZI /Od /I "../support" /I "../sides" /I "../simulation" /I "../brains" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D HEADLESS=1 /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "GrobotsHeadless - Win32 Release"
# Name "GrobotsHeadless - Win32 Debug"
# Begin Group "support"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\support\GBColor.cpp
# End Source File
# Begin Source File

SOURCE=..\support\GBColor.h
# End Source File
# Begin Source File

SOURCE=..\support\GBDeletionReporter.cpp
# End Source File
# Begin Source File

SOURCE=..\support\GBDeletionReporter.h
# End Source File
# Begin Source File

SOURCE=..\support\GBErrors.cpp
# End Source File
# Begin Source File

SOURCE=..\support\GBErrors.h
# End Source File
# Begin Source File

SOURCE=..\support\GBFinePoint.cpp
# End Source File
# Begin Source File

SOURCE=..\support\GBFinePoint.h
# End Source File
# Begin Source File

SOURCE=..\support\GBGraphics.cpp
# End Source File
# Begin Source File

SOURCE=..\support\GBGraphics.h
# End Source File
# Begin Source File

SOURCE=..\support\GBLongNumber.cpp
# End Source File
# Begin Source File

SOURCE=..\support\GBLongNumber.h
# End Source File
# Begin Source File

SOURCE=..\support\GBMilliseconds.cpp
# End Source File
# Begin Source File

SOURCE=..\support\GBMilliseconds.h
# End Source File
# Begin Source File

SOURCE=..\support\GBModel.cpp
# End Source File
# Begin Source File

SOURCE=..\support\GBModel.h
# End Source File
# Begin Source File

SOURCE=..\support\GBNumber.cpp
# End Source File
# Begin Source File

SOURCE=..\support\GBNumber.h
# End Source File
# Begin Source File

SOURCE=..\support\GBPlatform.h
# End Source File
# Begin Source File

SOURCE=..\support\GBRandomState.cpp
# End Source File
# Begin Source File

SOURCE=..\support\GBRandomState.h
# End Source File
# Begin Source File

SOURCE=..\support\GBSound.cpp
# End Source File
# Begin Source File

SOURCE=..\support\GBSound.h
# End Source File
# Begin Source File

SOURCE=..\support\GBStringUtilities.cpp
# End Source File
# Begin Source File

SOURCE=..\support\GBStringUtilities.h
# End Source File
# Begin Source File

SOURCE=..\support\GBTypes.h
# End Source File
# End Group
# Begin Group "sides"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\sides\GBHardwareSpec.cpp
# End Source File
# Begin Source File

SOURCE=..\sides\GBHardwareSpec.h
# End Source File
# Begin Source File

SOURCE=..\sides\GBRobotType.cpp
# End Source File
# Begin Source File

SOURCE=..\sides\GBRobotType.h
# End Source File
# Begin Source File

SOURCE=..\sides\GBScores.cpp
# End Source File
# Begin Source File

SOURCE=..\sides\GBScores.h
# End Source File
# Begin Source File

SOURCE=..\sides\GBSide.cpp
# End Source File
# Begin Source File

SOURCE=..\sides\GBSide.h
# End Source File
# Begin Source File

SOURCE=..\sides\GBSideReader.cpp
# End Source File
# Begin Source File

SOURCE=..\sides\GBSideReader.h
# End Source File
# End Group
# Begin Group "brains"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\brains\GBBrain.cpp
# End Source File
# Begin Source File

SOURCE=..\brains\GBBrain.h
# End Source File
# Begin Source File

SOURCE=..\brains\GBBrainSpec.cpp
# End Source File
# Begin Source File

SOURCE=..\brains\GBBrainSpec.h
# End Source File
# Begin Source File

SOURCE=..\brains\GBStackBrain.cpp
# End Source File
# Begin Source File

SOURCE=..\brains\GBStackBrain.h
# End Source File
# Begin Source File

SOURCE=..\brains\GBStackBrainOpcodes.cpp
# End Source File
# Begin Source File

SOURCE=..\brains\GBStackBrainOpcodes.h
# End Source File
# Begin Source File

SOURCE=..\brains\GBStackBrainPrimitives.cpp
# End Source File
# Begin Source File

SOURCE=..\brains\GBStackBrainSpec.cpp
# End Source File
# Begin Source File

SOURCE=..\brains\GBStackBrainSpec.h
# End Source File
# End Group
# Begin Group "simulation"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\simulation\GBDecorations.cpp
# End Source File
# Begin Source File

SOURCE=..\simulation\GBDecorations.h
# End Source File
# Begin Source File

SOURCE=..\simulation\GBFood.cpp
# End Source File
# Begin Source File

SOURCE=..\simulation\GBFood.h
# End Source File
# Begin Source File

SOURCE=..\simulation\GBHardwareState.cpp
# End Source File
# Begin Source File

SOURCE=..\simulation\GBHardwareState.h
# End Source File
# Begin Source File

SOURCE=..\simulation\GBMessages.cpp
# End Source File
# Begin Source File

SOURCE=..\simulation\GBMessages.h
# End Source File
# Begin Source File

SOURCE=..\simulation\GBObject.cpp
# End Source File
# Begin Source File

SOURCE=..\simulation\GBObject.h
# End Source File
# Begin Source File

SOURCE=..\simulation\GBObjectWorld.cpp
# End Source File
# Begin Source File

SOURCE=..\simulation\GBObjectWorld.h
# End Source File
# Begin Source File

SOURCE=..\simulation\GBRobot.cpp
# End Source File
# Begin Source File

SOURCE=..\simulation\GBRobot.h
# End Source File
# Begin Source File

SOURCE=..\simulation\GBSensorShot.cpp
# End Source File
# Begin Source File

SOURCE=..\simulation\GBSensorShot.h
# End Source File
# Begin Source File

SOURCE=..\simulation\GBShot.cpp
# End Source File
# Begin Source File

SOURCE=..\simulation\GBShot.h
# End Source File
# Begin Source File

SOURCE=..\simulation\GBWorld.cpp
# End Source File
# Begin Source File

SOURCE=..\simulation\GBWorld.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\GBHeadless.cpp
# End Source File
# End Target
# End Project
