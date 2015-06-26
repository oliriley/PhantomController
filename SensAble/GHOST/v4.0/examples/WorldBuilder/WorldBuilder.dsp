# Microsoft Developer Studio Project File - Name="WorldBuilder" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=WorldBuilder - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "WorldBuilder.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "WorldBuilder.mak" CFG="WorldBuilder - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "WorldBuilder - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "WorldBuilder - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "WorldBuilder - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\Release"
# PROP BASE Intermediate_Dir ".\Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\Release"
# PROP Intermediate_Dir ".\Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /I "." /I ".\include" /I "$(GHOST_BASE)\include" /I "$(GHOST_BASE)\external\stl" /D "NDEBUG" /D "_MBCS" /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "__SGI_STL_INTERNAL_RELOPS" /D "WINNT_USER_MODE" /FR /Yu"gstAfx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /i "$(GHOST_BASE)\lib\res" /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 ghostGLManager.lib HapticView.lib ghost$(GHOST_LIB_VERSION).lib gstVRML$(GSTVRML_LIB_VERSION).lib reset_screen.lib opengl32.lib glu32.lib winmm.lib /nologo /subsystem:windows /machine:I386 /libpath:"$(GHOST_BASE)\lib"
# Begin Special Build Tool
TargetPath=.\Release\WorldBuilder.exe
SOURCE="$(InputPath)"
# End Special Build Tool

!ELSEIF  "$(CFG)" == "WorldBuilder - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\Debug"
# PROP BASE Intermediate_Dir ".\Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\Debug"
# PROP Intermediate_Dir ".\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /Gi /GR /GX /Zi /Od /I "." /I ".\include" /I "$(GHOST_BASE)\include" /I "$(GHOST_BASE)\external\stl" /D "_DEBUG" /D "_MBCS" /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "__SGI_STL_INTERNAL_RELOPS" /D "WINNT_USER_MODE" /D "CONSOLE_OUTPUT" /Fr /Yu"gstAfx.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /i "$(GHOST_BASE)\lib\res" /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386
# ADD LINK32 ghostGLManager.lib HapticView.lib ghost$(GHOST_LIB_VERSION).lib gstVRML$(GSTVRML_LIB_VERSION).lib reset_screen.lib opengl32.lib glu32.lib winmm.lib /nologo /subsystem:windows /debug /machine:I386 /libpath:"$(GHOST_BASE)\lib"
# Begin Special Build Tool
TargetPath=.\Debug\WorldBuilder.exe
SOURCE="$(InputPath)"
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "WorldBuilder - Win32 Release"
# Name "WorldBuilder - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90"
# Begin Source File

SOURCE=.\src\ColorWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\src\FlatToolBar.cpp
# End Source File
# Begin Source File

SOURCE=.\src\gstAfx.cpp
# ADD CPP /Yc"gstAfx.h"
# End Source File
# Begin Source File

SOURCE=.\src\gstPHANToMSelect.cpp
# End Source File
# Begin Source File

SOURCE=.\src\HapticFrustum.cpp
# End Source File
# Begin Source File

SOURCE=.\src\MaterialBar.cpp
# End Source File
# Begin Source File

SOURCE=.\src\ObjectState.cpp
# End Source File
# Begin Source File

SOURCE=.\src\ParEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\src\TransformBar.cpp
# End Source File
# Begin Source File

SOURCE=.\WorldBuilder.rc
# End Source File
# Begin Source File

SOURCE=.\src\WorldBuilderApp.cpp
# End Source File
# Begin Source File

SOURCE=.\src\WorldBuilderDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\src\WorldBuilderFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\src\WorldBuilderView.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=.\include\ColorWnd.h
# End Source File
# Begin Source File

SOURCE=.\include\FlatToolBar.h
# End Source File
# Begin Source File

SOURCE=.\include\gstAfx.h
# End Source File
# Begin Source File

SOURCE=.\include\gstPHANToMSelect.h
# End Source File
# Begin Source File

SOURCE=.\include\HapticFrustum.h
# End Source File
# Begin Source File

SOURCE=.\include\MaterialBar.h
# End Source File
# Begin Source File

SOURCE=.\include\ObjectState.h
# End Source File
# Begin Source File

SOURCE=.\include\ParEdit.h
# End Source File
# Begin Source File

SOURCE=.\include\Resource.h
# End Source File
# Begin Source File

SOURCE=.\include\TransformBar.h
# End Source File
# Begin Source File

SOURCE=.\include\WorldBuilderApp.h
# End Source File
# Begin Source File

SOURCE=.\include\WorldBuilderDoc.h
# End Source File
# Begin Source File

SOURCE=.\include\WorldBuilderFrame.h
# End Source File
# Begin Source File

SOURCE=.\include\WorldBuilderView.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\GhostApp.ico
# End Source File
# Begin Source File

SOURCE=.\res\GhostAppDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\Main.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Material.bmp
# End Source File
# Begin Source File

SOURCE=.\res\PHANToM.bmp
# End Source File
# Begin Source File

SOURCE=.\res\PHANToM.ico
# End Source File
# Begin Source File

SOURCE=.\res\Primitvs.bmp
# End Source File
# Begin Source File

SOURCE="$(GHOST_BASE)\lib\res\SensAble.bmp"
# End Source File
# Begin Source File

SOURCE="$(GHOST_BASE)\lib\res\Splash.bmp"
# End Source File
# Begin Source File

SOURCE=.\res\Transform.bmp
# End Source File
# Begin Source File

SOURCE=.\res\WorldBuilder.rc2
# End Source File
# End Group
# End Target
# End Project
# Section WorldBuilder : {00000053-0005-0000-24ED-4A002A2E4F42}
# 	1:10:IDB_SPLASH:100
# 	2:21:SplashScreenInsertKey:4.0
# End Section
