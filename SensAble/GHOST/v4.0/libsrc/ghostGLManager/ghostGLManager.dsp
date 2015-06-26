# Microsoft Developer Studio Project File - Name="ghostGLManager" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=ghostGLManager - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ghostGLManager.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ghostGLManager.mak" CFG="ghostGLManager - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ghostGLManager - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "ghostGLManager - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ghostGLManager - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\ghost\include" /I "..\ghostGLDraw\include" /I "$(GHOST_BASE)\include" /I ".\include" /I "$(GHOST_BASE)\external\stl" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
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
TargetPath=.\Release\ghostGLManager.lib
SOURCE="$(InputPath)"
# End Special Build Tool

!ELSEIF  "$(CFG)" == "ghostGLManager - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /GX /Zi /Od /I "..\ghost\include" /I "..\ghostGLDraw\include" /I "$(GHOST_BASE)\include" /I ".\include" /I "$(GHOST_BASE)\external\stl" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /Fr /YX /FD /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo
# Begin Special Build Tool
TargetPath=.\Debug\ghostGLManager.lib
SOURCE="$(InputPath)"
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "ghostGLManager - Win32 Release"
# Name "ghostGLManager - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\src\ghostGLCameraBase.cpp
# End Source File
# Begin Source File

SOURCE=.\src\ghostGLManager.cpp
# End Source File
# Begin Source File

SOURCE=.\src\ghostGLPinchXForm.cpp
# End Source File
# Begin Source File

SOURCE=.\src\ghostGLSyncCamera.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE="$(GHOST_BASE)\include\ghostGLActionObject.h"
# End Source File
# Begin Source File

SOURCE="$(GHOST_BASE)\include\ghostGLCameraBase.h"
# End Source File
# Begin Source File

SOURCE="$(GHOST_BASE)\include\ghostGLManager.h"
# End Source File
# Begin Source File

SOURCE="$(GHOST_BASE)\include\ghostGLPinchXForm.h"
# End Source File
# Begin Source File

SOURCE="$(GHOST_BASE)\include\ghostGLSyncCamera.h"
# End Source File
# End Group
# Begin Group "Library Files"

# PROP Default_Filter "lib"
# Begin Source File

SOURCE="$(GHOST_BASE)\lib\ghostGLDraw.lib"
# End Source File
# End Group
# End Target
# End Project
