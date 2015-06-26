# Microsoft Developer Studio Project File - Name="SimpleDeformableSurface" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=SimpleDeformableSurface - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "SimpleDeformableSurface.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SimpleDeformableSurface.mak" CFG="SimpleDeformableSurface - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SimpleDeformableSurface - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "SimpleDeformableSurface - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "SimpleDeformableSurface - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /Ob2 /I "include" /I "$(3DTOUCH_BASE)\include" /I "$(3DTOUCH_BASE)\utilities\include" /D "NDEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D "FLOAT_DYNREAL" /FR /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 hl.lib hlu.lib hd.lib hdu.lib glut32.lib opengl32.lib kernel32.lib user32.lib gdi32.lib winspool.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /profile /machine:I386 /libpath:"$(3DTOUCH_BASE)\lib" /libpath:"$(3DTOUCH_BASE)\utilities\lib"

!ELSEIF  "$(CFG)" == "SimpleDeformableSurface - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "SimpleDeformableSurface___Win32_Debug"
# PROP BASE Intermediate_Dir "SimpleDeformableSurface___Win32_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "include" /I "$(3DTOUCH_BASE)\include" /I "$(3DTOUCH_BASE)\utilities\include" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D "FLOAT_DYNREAL" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 hl.lib hlud.lib hd.lib hdud.lib glut32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept /libpath:"$(3DTOUCH_BASE)\lib" /libpath:"$(3DTOUCH_BASE)\utilities\lib"

!ENDIF 

# Begin Target

# Name "SimpleDeformableSurface - Win32 Release"
# Name "SimpleDeformableSurface - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\draw_string.cpp
# End Source File
# Begin Source File

SOURCE=.\HapticDeviceConstraint.cpp
# End Source File
# Begin Source File

SOURCE=.\main.cpp
# End Source File
# Begin Source File

SOURCE=.\MouseSpringConstraint.cpp
# End Source File
# Begin Source File

SOURCE=.\NailConstraint.cpp
# End Source File
# Begin Source File

SOURCE=.\OdeSolver.cpp
# End Source File
# Begin Source File

SOURCE=.\Particle.cpp
# End Source File
# Begin Source File

SOURCE=.\ParticleSystem.cpp
# End Source File
# Begin Source File

SOURCE=.\SpringConstraint.cpp
# End Source File
# Begin Source File

SOURCE=.\Surface.cpp
# End Source File
# Begin Source File

SOURCE=.\UnProjectUtilities.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Constraint.h
# End Source File
# Begin Source File

SOURCE=.\draw_string.h
# End Source File
# Begin Source File

SOURCE=.\DynamicsMath.h
# End Source File
# Begin Source File

SOURCE=.\HapticDeviceConstraint.h
# End Source File
# Begin Source File

SOURCE=.\MouseSpringConstraint.h
# End Source File
# Begin Source File

SOURCE=.\NailConstraint.h
# End Source File
# Begin Source File

SOURCE=.\OdeSolver.h
# End Source File
# Begin Source File

SOURCE=.\Particle.h
# End Source File
# Begin Source File

SOURCE=.\ParticleSystem.h
# End Source File
# Begin Source File

SOURCE=.\SpringConstraint.h
# End Source File
# Begin Source File

SOURCE=.\Surface.h
# End Source File
# Begin Source File

SOURCE=.\UnProjectUtilities.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
