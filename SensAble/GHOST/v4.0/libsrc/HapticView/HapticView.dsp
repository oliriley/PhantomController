# Microsoft Developer Studio Project File - Name="HapticView" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=HapticView - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "HapticView.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "HapticView.mak" CFG="HapticView - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "HapticView - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "HapticView - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "HapticView - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I ".\include" /I "$(GHOST_BASE)\include" /I "$(GHOST_BASE)\external\stl" /D "NDEBUG" /D "__SGI_STL_INTERNAL_RELOPS" /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "WINNT_USER_MODE" /Yu"StdAfx.h" /FD /c
# SUBTRACT CPP /Fr
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo
# Begin Special Build Tool
TargetPath=.\Release\HapticView.lib
SOURCE="$(InputPath)"
# End Special Build Tool

!ELSEIF  "$(CFG)" == "HapticView - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /GX /Zi /Od /I "$(GHOST_BASE)\include" /I "$(GHOST_BASE)\external\stl" /I ".\include" /D "_DEBUG" /D "__SGI_STL_INTERNAL_RELOPS" /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "WINNT_USER_MODE" /FR /Yu"StdAfx.h" /FD /YlAnyName /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo
# Begin Special Build Tool
TargetPath=.\Debug\HapticView.lib
SOURCE="$(InputPath)"
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "HapticView - Win32 Release"
# Name "HapticView - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\src\HapticApp.cpp
# End Source File
# Begin Source File

SOURCE=.\src\HapticConsoleIO.cpp
# End Source File
# Begin Source File

SOURCE=.\src\HapticDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\src\HapticFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\src\HapticMouse.cpp
# End Source File
# Begin Source File

SOURCE=.\src\HapticMouseBase.cpp
# End Source File
# Begin Source File

SOURCE=.\src\HapticSplash.cpp
# End Source File
# Begin Source File

SOURCE=.\src\HapticView.cpp
# End Source File
# Begin Source File

SOURCE=.\src\StdAfx.cpp
# ADD CPP /Yc"StdAfx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE="$(GHOST_BASE)\include\HapticApp.h"
# End Source File
# Begin Source File

SOURCE=..\..\..\include\HapticConsoleIO.h
# End Source File
# Begin Source File

SOURCE="$(GHOST_BASE)\include\HapticDoc.h"
# End Source File
# Begin Source File

SOURCE="$(GHOST_BASE)\include\HapticFrame.h"
# End Source File
# Begin Source File

SOURCE="$(GHOST_BASE)\include\HapticMouse.h"
# End Source File
# Begin Source File

SOURCE="$(GHOST_BASE)\include\HapticMouseBase.h"
# End Source File
# Begin Source File

SOURCE="$(GHOST_BASE)\include\HapticView.h"
# End Source File
# Begin Source File

SOURCE="$(GHOST_BASE)\include\PHANToMMouseIoCtl.h"
# End Source File
# Begin Source File

SOURCE="$(GHOST_BASE)\include\StdAfx.h"
# End Source File
# End Group
# End Target
# End Project
