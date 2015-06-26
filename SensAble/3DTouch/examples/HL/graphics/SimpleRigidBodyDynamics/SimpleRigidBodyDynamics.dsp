# Microsoft Developer Studio Project File - Name="SimpleRigidBodyDynamics" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=SimpleRigidBodyDynamics - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "SimpleRigidBodyDynamics.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SimpleRigidBodyDynamics.mak" CFG="SimpleRigidBodyDynamics - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SimpleRigidBodyDynamics - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "SimpleRigidBodyDynamics - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "SimpleRigidBodyDynamics - Win32 Release"

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
# ADD CPP /nologo /MD /W3 /GX /O2 /I "include" /I "$(3DTOUCH_BASE)\include" /I "$(3DTOUCH_BASE)\utilities\include" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Yu"SimpleRigidBodyDynamicsAfx.h" /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 hl.lib hlu.lib hd.lib hdu.lib glut32.lib /nologo /subsystem:console /profile /machine:I386 /libpath:"$(3DTOUCH_BASE)\lib" /libpath:"$(3DTOUCH_BASE)\utilities\lib"

!ELSEIF  "$(CFG)" == "SimpleRigidBodyDynamics - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "include" /I "$(3DTOUCH_BASE)\include" /I "$(3DTOUCH_BASE)\utilities\include" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FR /Yu"SimpleRigidBodyDynamicsAfx.h" /FD /GZ /c
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

# Name "SimpleRigidBodyDynamics - Win32 Release"
# Name "SimpleRigidBodyDynamics - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Contact.cpp
# End Source File
# Begin Source File

SOURCE=.\draw_string.cpp
# End Source File
# Begin Source File

SOURCE=.\DynamicsWorld.cpp
# End Source File
# Begin Source File

SOURCE=.\DynEdge.cpp
# End Source File
# Begin Source File

SOURCE=.\DynFace.cpp
# End Source File
# Begin Source File

SOURCE=.\DynFreePlane.cpp
# End Source File
# Begin Source File

SOURCE=.\main.cpp
# End Source File
# Begin Source File

SOURCE=.\OdeSolver.cpp
# End Source File
# Begin Source File

SOURCE=.\RigidBody.cpp
# End Source File
# Begin Source File

SOURCE=.\RigidBodyBox.cpp
# End Source File
# Begin Source File

SOURCE=.\RigidBodyWall.cpp
# End Source File
# Begin Source File

SOURCE=.\SimpleRigidBodyDynamicsAfx.cpp
# ADD CPP /Yc"SimpleRigidBodyDynamicsAfx.h"
# End Source File
# Begin Source File

SOURCE=.\TestScenes.cpp
# End Source File
# Begin Source File

SOURCE=.\UnProjectUtilities.cpp
# End Source File
# Begin Source File

SOURCE=.\Witness.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Contact.h
# End Source File
# Begin Source File

SOURCE=.\ContactList.h
# End Source File
# Begin Source File

SOURCE=.\draw_string.h
# End Source File
# Begin Source File

SOURCE=.\DynamicsMath.h
# End Source File
# Begin Source File

SOURCE=.\DynamicsWorld.h
# End Source File
# Begin Source File

SOURCE=.\DynEdge.h
# End Source File
# Begin Source File

SOURCE=.\DynFace.h
# End Source File
# Begin Source File

SOURCE=.\DynFreePlane.h
# End Source File
# Begin Source File

SOURCE=.\DynPlane.h
# End Source File
# Begin Source File

SOURCE=.\OdeSolver.h
# End Source File
# Begin Source File

SOURCE=.\RigidBody.h
# End Source File
# Begin Source File

SOURCE=.\RigidBodyBox.h
# End Source File
# Begin Source File

SOURCE=.\RigidBodyWall.h
# End Source File
# Begin Source File

SOURCE=.\SimpleRigidBodyDynamicsAfx.h
# End Source File
# Begin Source File

SOURCE=.\TestScenes.h
# End Source File
# Begin Source File

SOURCE=.\UnProjectUtilities.h
# End Source File
# Begin Source File

SOURCE=.\Witness.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
