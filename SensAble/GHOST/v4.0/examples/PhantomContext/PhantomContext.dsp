# Microsoft Developer Studio Project File - Name="PhantomContext" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=PhantomContext - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "PhantomContext.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "PhantomContext.mak" CFG="PhantomContext - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "PhantomContext - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "PhantomContext - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "PhantomContext - Win32 Release"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "." /I ".\include" /I "$(GHOST_BASE)\include" /I "$(GHOST_BASE)\external\stl" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "__SGI_STL_INTERNAL_RELOPS" /D "WINNT_USER_MODE" /Yu"gstAfx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /i "$(GHOST_BASE)\lib" /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 HapticView.lib ghost$(GHOST_LIB_VERSION).lib ghostGLManager.lib reset_screen.lib quit.lib opengl32.lib glu32.lib /nologo /subsystem:windows /machine:I386 /libpath:"$(GHOST_BASE)\lib"
# Begin Special Build Tool
TargetPath=.\Release\PhantomContext.exe
SOURCE="$(InputPath)"
# End Special Build Tool

!ELSEIF  "$(CFG)" == "PhantomContext - Win32 Debug"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "." /I ".\include" /I "$(GHOST_BASE)\include" /I "$(GHOST_BASE)\external\stl" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "__SGI_STL_INTERNAL_RELOPS" /D "WINNT_USER_MODE" /FR /Yu"gstAfx.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /i "$(GHOST_BASE)\lib" /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 HapticView.lib ghost$(GHOST_LIB_VERSION).lib ghostGLManager.lib reset_screen.lib quit.lib opengl32.lib glu32.lib /nologo /subsystem:windows /incremental:no /debug /machine:I386 /pdbtype:sept /libpath:"$(GHOST_BASE)\lib"
# Begin Special Build Tool
TargetPath=.\Debug\PhantomContext.exe
SOURCE="$(InputPath)"
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "PhantomContext - Win32 Release"
# Name "PhantomContext - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\src\gstAfx.cpp
# ADD CPP /Yc
# End Source File
# Begin Source File

SOURCE=.\src\HapticFrustum.cpp
# End Source File
# Begin Source File

SOURCE=.\src\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\src\PhantomContext.cpp
# End Source File
# Begin Source File

SOURCE=.\src\PhantomContext_graphics.cpp
# End Source File
# Begin Source File

SOURCE=.\src\PhantomContext_haptics.cpp
# End Source File
# Begin Source File

SOURCE=.\src\PhantomContextApp.cpp
# End Source File
# Begin Source File

SOURCE=.\src\PhantomContextDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\src\PhantomContextView.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\include\gstAfx.h
# End Source File
# Begin Source File

SOURCE=.\include\HapticFrustum.h
# End Source File
# Begin Source File

SOURCE=.\include\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\include\PhantomContext.h
# End Source File
# Begin Source File

SOURCE=.\include\PhantomContext_graphics.h
# End Source File
# Begin Source File

SOURCE=.\include\PhantomContext_haptics.h
# End Source File
# Begin Source File

SOURCE=.\include\PhantomContextApp.h
# End Source File
# Begin Source File

SOURCE=.\include\PhantomContextDoc.h
# End Source File
# Begin Source File

SOURCE=.\include\PhantomContextView.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\PhantomContext.ico
# End Source File
# Begin Source File

SOURCE=.\PhantomContext.rc
# End Source File
# Begin Source File

SOURCE=.\res\PhantomContext.rc2
# End Source File
# Begin Source File

SOURCE=.\res\PhantomContextDoc.ico
# End Source File
# Begin Source File

SOURCE=.\include\Resource.h
# End Source File
# Begin Source File

SOURCE=res\SensAble.bmp
# End Source File
# Begin Source File

SOURCE=res\Splash.bmp
# End Source File
# End Group
# End Target
# End Project
# Section GhostApp : {004C9F60-AEE0-004C-A0AE-4C0080A44C00}
# 	1:10:IDB_SPLASH:100
# 	2:21:SplashScreenInsertKey:4.0
# End Section
# Section GhostApp : {66206568-7269-7473-2070-616E65207768}
# 	1:17:CG_IDS_DISK_SPACE:105
# 	1:19:CG_IDS_PHYSICAL_MEM:104
# 	1:25:CG_IDS_DISK_SPACE_UNAVAIL:106
# 	2:14:PhysicalMemory:CG_IDS_PHYSICAL_MEM
# 	2:9:DiskSpace:CG_IDS_DISK_SPACE
# 	2:16:SpaceUnavailable:CG_IDS_DISK_SPACE_UNAVAIL
# 	2:7:NewFunc:1
# 	2:10:SysInfoKey:1234
# End Section
# Section GhostApp : {72442072-7761-4320-6F6E-74726F6C0034}
# 	1:27:CG_IDR_POPUP_GHOST_APP_VIEW:102
# End Section
