---------------------------------------------------------------------------
OpenHaptics(TM) Academic Edition Readme 29-JUL-2005
---------------------------------------------------------------------------

The material embodied in this software and use of this software is
subject to the terms and conditions of the clickthrough Development
License Agreement.  


---------------------------------------------------------------------------
System Requirements
---------------------------------------------------------------------------
The OpenHaptics toolkit requires certain hardware and
software components to be able to work properly. They are listed below.

Hardware
-------- 
    Intel Pentium(R) 4 or higher.
    100 MB disk space and 256 MB RAM.
    One of the supported devices from PHANTOM(R) family of haptic devices:
      PHANTOM(R) Omni(TM)
      PHANTOM(R) Desktop(TM)
      PHANTOM(R) Premium 1.5/6DOF (Parallel Port)
      PHANTOM(R) Premium 3.0/6DOF (Parallel Port)
      PHANTOM(R) Premium A 1.0/1.5/3DOF (Parallel Port/PCI Card)
      PHANTOM(R) Premium L 3.0/3DOF (Parallel Port/PCI Card)


Software
---------
  Windows(R) 2000 or XP

Compilers
---------

  Microsoft(R) Visual C++ 6.0 or later (see known issues below if
  using Microsoft Visual Studio .NET 2003).

Drivers
---------
  PHANTOM Device Driver v 4.2.49

INSTALLATION
---------------------------------------------------------------------------

Installation instructions:
Step 1: Uninstall previously installed software using the Add/Remove
        Programs icon in the control panel.
Step 2. Insert the CD into your CD-ROM drive 
Step 3. In Windows Explorer, locate the drive letter associated with your
         CD_ROM Drive and double-click it.
Step 4. Install the device drivers by running the setup program in the
        “PHANTOM Device Drivers” folder. Make sure that you
        select the right type of device on the installation dialog (e.g.
        for PHANTOM Omni device select Omni, etc). If you are asked to reboot,
        please do so. After rebooting, your device will be
        recognized, and you can run “PHANTOM Test” to calibrate it.
Step 5. Install the OpenHaptics (TM) toolkit by running the setup program in the 
        “OpenHaptics toolkit” folder. Our installation program will 
        install GLUT libraries by default, as those are used by our 
        graphics source code examples.  However, you will have the option
        to not install glut.  After installation, you will find extended
        documentation in the doc folder under the installation root.



Changes from OpenHaptics toolkit v1.02
--------------------------------------
- The Viscosity sample program has been removed from the v2.0 release.
- Improved documentation and examples
- HL attributes, such as material properties, can now be managed via an 
attribute stack.  To save the state of the HL attributes, you can call 
hlPushAttrib and then modify HL attributes.  To reset the HL attributes 
to state when hlPushAttrib was called, you can simply call hlPopAttrib.  
This allows for cleaner code in your HL programs.
Now HL effects can be modified while the effect is running.  Previously, 
you had to stop the effect, change the effect parameters and then restart 
the effect. 

- Changing the effects in place
- Many issues addressed with dual Omni configurations
- hlCheckEvents can be used outside of a haptic frame, which 
  facilitates having event driven haptic rendering in your application.  

Changes from OpenHaptics toolkit v1.01
--------------------------------------
- Added an optional HLAPI modelview matrix stack that can be used in place of
the OpenGL modelview matrix stack.  See hlMatrixMode in the API Reference
manual for more information.
- Added ability to query the golden radius and golden sphere position that
represent the bounds of the geomery used for the haptic camera view.  See
hlGetDoublev in the API reference manual for more information.
- Added support to the haptic mouse utility library for querying and changing
the window that the haptic mouse is associated with.  This simplifies using the
haptic mouse with multiple 3D viewports.
- Fixed a problem in the HelloSphereDual source code sample where the devices
were not being initialized in the correct order.
- Fixed a problem where feedback buffer shapes did not feel correct when using
Wildcat graphics cards.
- Fixed a problem where the position reported by the haptic device was not
correct for first couple of servo loop ticks.
- Fixed the incorrect display of the cursor in the coulumb field HDAPI source
code example.
- Fixed a problem where hlPopMatrix did not correctly update the matrix stack.

Changes from OpenHaptics toolkit v1.0
-------------------------------------
- Licensing was added to the HDAPI and HLAPI (see above).
- Fixed bug where the proxy rotation/transform was incorrect when there is
rotation in the modelview matrix.
- Fixed haptic kicking in certain cases with non-manifold surfaces.

Changes from OpenHaptics toolkit v0.9 Limited Release
---------------------------------------------------------------------------
- Addition of HapticMouse utility library which allows you to use the 
haptic device to control the mouse cursor.  Full source for this library
is provided.  See the HapticMaterials sample program for example usage.
- Ability to create callback shapes that use lines and points as local
features.  Note that this has changed the interface for the closest feature
callback for callback shapes and will require modification of client code 
to use the new interface.
- Support for surfaces that are changing dynamically from frame to frame,
using the HL_SHAPE_DYNAMIC_SURFACE_CHANGE hint.
- Ability to query and change the haptic device used by a haptic rendering
context using the new hlContextDevice, hlGetCurrentDevice, and 
hlGetCurrentContext functions.
- Changed hlMakeCurrent and hdMakeCurrentDevice to be managed on a per
thread basis.
- Ability to combine HLAPI and HDAPI programs by scheduling HDAPI
servo thread callbacks that bracket the HLAPI servo loop code.
- Ability to have multiple rendering contexts use the same haptic device
concurrently.
- Improvements to the hluFit routines to allow fitting of the haptic
workspace to a subset of the graphics view frustum.
- Ability to query touch state and reaction force on a per shape basis,
using new hlGetShape functions.
- Motion events may now also be handled in the collision thread, allowing
for more frequent sampling of proxy position.
- Additional sample programs: HapticViewer, HapticMaterials, 
SimpleDeformableSurface, and SimpleRigidBody.  See the code sample
descriptions for details.

Changes from HDAPI 1.0
---------------------------------------------------------------------------
- Addition of the new Haptic Library API.
- HD_NOMINAL_MAX_DAMPING accessor has been added as a paramater for
hdGetDoublev.
- A bug has been fixed in the reporting of calibration status for 
PHANTOM Desktop and Omni devices.
- hduRecord functionality has been migrated to the HDU library.
- Several geometry utility classes have been added: hduLine, hduLineSegment, 
hduPlane, and hduQuaternion.
- The workspace mapping technique utilized by hduMapWorkspaceViewEx, 
hduMapWorkspaceView, and hduMapWorkspace has been improved to yield better 
utilization of the haptic workspace and motion within the view.

Using OpenHaptics with C++ .NET
---------------------------------------------------------------------------

OpenHaptics for Windows can be used wiith the Microsoft Visual C++ .NET 2003 
compiler with a little effort.  When you install OpenHaptics, the Haptic Device 
Utility (HDU) and Haptic Libbrary Utilitiy (HLU) libraries have been built for 
Microsoft Visual C++ 6.0.  Due to compatibililty issues with the C++ standard 
library, these libraries need to be recompiled in order to work with 
the .NET C++ compiler.   Fortunately,this is straightforward and only 
has to be done once.     

 

The source code for the HDU and HLU libraries is included with your OpenHaptics 
installation, and so have the Microsoft Visual C++ 6.0 project files.  To convert 
these libraries for use with Visual C++ .NET 2003, perform the following steps:

 

1.)    Go to the OpenHaptics utilities directory.  If you installed
OpenHaptics in C:\Program Files\OpenHaptics, then this directory is
C:\Program Files\OpenHaptics\utilities.    

2.)    Go to the utilities\src\HDU directory and open HDU.dsw with
Visual C++ .NET 2003.  The Visual C++ will ask you if you want to convert the project 
file to the newer .sln format.  Say yes.  

3.)    Rebuild Release and Debug versions of the HDU library. Using the
Build/Batch Build menu item, and then pressing the Rebuild button, is probably easiest 
way to do this.  It won't take long to rebuild the library. 

4.)    Copy the newly rebuilt hdu.lib and hdud.lib files from the Debug
and Release directories in src/HDU to the utilities/lib directory.  This
will overwrite the lib files built for Visual C++ 6.0.   Note: hdud.lib
is the debug version and hdu.lib is the release version.    

5.)    Repeat this process with the HLU library (hlu.lib and hlud.lib) ,
HapticMouse (HapticMouse.lib and HapticMouseD.lib) and for SnapConstraints 
(SnapContraints.lib and SnapContaintsD.lib)  

 

You should be able successfully link the OpenHaptics examples. 



KNOWN ISSUES
---------------------------------------------------------------------------
- For customers using a 1.5 6DOF or 3.0 6DOF device, the gimbal forces in 
the SimpleRigidBody example may be excessively strong when grabbing a block. 
Please use caution when utilizing this sample program with the 6DOF devices.

- Building the source code samples with Microsoft Visual Studio .NET 2003:
In order to build the HDAPI and HLAPI source code samples with the
Visual Studio .NET 2003 C++ compiler, you must first rebuild the utility
libraries.  The prebuilt versions of the HDU, HLU, HapticMouse and
SnapConstraints libraries were built using Microsoft Developer Studio 6.0.
If you build any of the source code samples, you will get a link error
when linking to these libraries.  This is due to a change in the Microsoft 
C++ compiler in the way C++ symbols using templates are exported from static
libraries.  The workaround is to rebuild the HDU, HLU, HapticMouse and 
SnapConstraints libraries.  Project files for these libraries may be found 
in the 3DTouch/utilities/source folder.  Make sure to do a full rebuild 
(rebuild solution) for both the release and debug configurations of 
the above projects. 

- Compatability with HDAPI version 1.0:
The HDAPI library version 1.01 is not binary or source compatible with
version 1.0.  In order to allow future support for languages other than
C/C++, the calling convention of scheduler callback functions was changed 
to stdcall.  This will require programmers to add HDCALLBACK to the
declarations of all callback functions passed to hdScheduleSynchronous
and hdScheduleAsynchronous.  For example the declaration:

HDCallbackCode MyCallback(void *pUserData)	

would need to be changed to:

HDCallbackCode HDCALLBACK MyCallback(void *pUserData)

- Compatability with HLAPI 0.9:
HLAPI v0.9 programs which used the callback shape 
HL_SHAPE_CLOSEST_POINT callback function must be changed to use the
new HL_SHAPE_CLOSEST_FEATURES callback function.  Specifically, calls
of the form:
hlCallback(HL_SHAPE_CLOSEST_POINT, 
	  (HLcallbackProc) closestSurface, 
	  &myObj);
must be changed to:
hlCallback(HL_SHAPE_CLOSEST_FEATURES, 
          (HLcallbackProc) closestSurface, 
	  &myObj);
and the interface for the callback function must be changed from:
HLboolean HLCALLBACK closestSurface(const HLdouble *queryPt, 
                                    HLdouble *closestPt,
                                    HLdouble *closestNormal,
                                    void* userData)
to
HLboolean HLCALLBACK closestSurface(const HLdouble *queryPt, 
                                    const HLdouble *targetPt,
				    HLgeom* geom,
                                    HLdouble *closestPt,
                                    void* userData)
In addition local features should be specified in the callback
using the new hlLocalFeature routines.

- Dynamically changing depth buffer shapes.
Using depth buffer shapes which are changing dynamically, are front 
and back touchable, and have the haptic camera view enabled will allow
the proxy to fall through the surface of the shape when you are not in
contact with the shape.  Potential workarounds are to use front or back
only touchability, disable the haptic camera view or use a feedback 
buffer shape.

- Haptic camera view and fast moving dynamic objects.
Using haptic camera view with shapes which are moving quickly may
allow those objects to pass through the proxy without generating any
forces.  This is because objects which are moving very quickly may
not be visible in the haptic camera view.  Disabling haptic camera view
will prevent this.

- The versions of the HD.DLL and HL.DLL files installed into the Windows system32
directory with the installation of 3D Touch(TM) SDK - OpenHaptics(TM) toolkit v1.02.50
are shown as version 1.0.3.50.  This is the version of the DLL but not of the toolkit.
The versions will be synchronized in a future release.



---------------------------------------------------------------------------
©1993-2005 SensAble Technologies, Inc. All rights reserved. 3D Touch, ClayTools, 
FreeForm, ClayTools system for Rhinoceros, FreeForm Modeling, FreeForm Modeling Plus, 
FreeForm Mold, GHOST, HapticExtender, HapticSound, OpenHaptics, PHANTOM, PHANTOM Desktop, 
PHANTOM Omni, SensAble, SensAble Technologies, Inc., Splodge, Splodge design, TextureKiln and WebTouch 
are trademarks or registered trademarks of SensAble Technologies, Inc. Other brand and 
product names are trademarks of their respective holders.

The system is protected by one or more of the following U.S. Patents:  5,587,937; 5,625,576; 
5,898,599; 6,084,587; 6,111,577; 6,191,796; 6,369,834; 6,405,158; 6,417,638; 6,421,048; 6,552,722; 
6,671,651; 6,792,398; 6,831,640; 6,853,965; 6,867,770; and 6,879,315.  Other patents pending.


---------------------------------------------------------------------------


