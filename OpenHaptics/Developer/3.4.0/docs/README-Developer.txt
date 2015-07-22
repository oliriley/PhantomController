---------------------------------------------------------------------------
OpenHaptics(R) Developer Edition v3.4.0 Release (Build 0) Readme 05-May-2015
---------------------------------------------------------------------------

The material embodied in this software and use of this software is
subject to the terms and conditions of the click-through Development
License Agreement. 

For customer support: 
  http://dsc.sensable.com/  


---------------------------------------------------------------------------
System Requirements
---------------------------------------------------------------------------
The OpenHaptics toolkit requires certain hardware and
software components to be able to work properly. They are listed below.

Hardware
-------- 
    Intel Pentium(R) 4 or higher.
    512 MB disk space and 2 GB RAM.
    One of the supported devices from 3D Systems's Geomagic Touch or Touch(R) family of haptic devices.


Software
---------
  Windows(R) XP or XP x64 (SP2) or Vista or Vista 64 or Windows(R) 7 or Windows(R) 7 64 or Windows(R) 8.1 or Windows(R) 8.1 64


Compilers
---------
   Microsoft Visual Studio versions 2005 / 2008 / 2010 are supported. 


Drivers
---------
  Geomagic Touch Device Driver v 2014.20 or later

INSTALLATION
---------------------------------------------------------------------------

For installation instructions, please refer to the install guide that came with
the software. 

---------------------------------------------------------------------------
Changes from OpenHaptics toolkit v3.0
--------------------------------------

- Brand new unified installer for both 32-bit and 64-bit SDK. 

- All projects were updated and built to VC 2010, VC 2008 and VC 2005.Utilities libs from 
installer are now built for VC 2010 compiler. Utilities need to be rebuilt to use VC 2008 
or VC 2005 compilers as developer environments. 
 
- The installer copies both 32-bit and 64-bit libs under 64-bit OS and only installs
 32-bit under 32-bit OS. This allows for both 32-bit / 64-bit development on 64-bit OS 
 and only 32-bit development on 32-bit OS. 

- Floating development license scheme has been incorporated in to OH licensing code.
 (Only valid for commercial edition). 

- All examples are tagged separately for VC 2005, VC 2008 and VC 2010. 

- Environment variable has been updated to OH_SDK_BASE from 3DTOUCH_BASE. 

- Default installation folder is now just C drive or windows drive. 
This was done to give users access to example files which were being copied in to program
 files folder in the earlier versions. 

- Adds searchable Doxygen generated API reference documentation for QuickHaptics API. 

- Support for ethernet and USB interface devices. 


Changes from OpenHaptics toolkit v2.01
--------------------------------------
- new QuickHaptics(TM) MicroApi layer ontop of HD and HL.
- New parameters for setting/getting Joint Torques. Especially useful for 6-DOF
devices.

- New parameter for querying the world-space Depth of Penetration from the Surface
Contact Point.

- Support for Vista, Vista 64 and SUSE 11


Changes from OpenHaptics toolkit v2.00
--------------------------------------
- Now supports Windows XP x64 platform
- Additions for support of the Pinch (Thumbpad) and Scissors end effectors for 6DOF devices.



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


Using OpenHaptics with C++ .NET 2003
---------------------------------------------------------------------------

OpenHaptics for Windows can be used with the Microsoft Visual C++ .NET 2003
or .NET 2005 compilers with a little effort.  When you install OpenHaptics,
the Haptic Device Utility (HDU) and Haptic Library Utility (HLU) libraries
have been built for Microsoft Visual C++ 2005.  Due to compatibility issues
with the C++ standard library, these libraries need to be recompiled in order
to work with the .NET C++ 2003 compiler.   Fortunately, this is straightforward
and only has to be done once.     

The source code for the HDU and HLU libraries is included with your OpenHaptics 
installation, and so have the Visual Studio .NET 2003 and 2005 project files.
To convert these libraries for use with Visual C++ .NET 2003, perform the following
steps:

1.)    Go to the OpenHaptics utilities directory.  If you installed
OpenHaptics in C:\Program Files\OpenHaptics, then this directory is
C:\Program Files\OpenHaptics\utilities.    

2.)    In the utilities\src directory, open UtilitiesVS7.sln with
Visual C++ .NET 2003.  

3.)    Rebuild Release and Debug versions of the HDU, HLU and other libraries.
Using the Build/Batch Build menu item; select All Projects; and then pressing the
Rebuild button, is probably easiest way to do this.  It won't take long to rebuild
the library. 
 
You should now be able to successfully link the OpenHaptics examples using
Visual Studio .NET 2003. 

KNOWN ISSUES in OpenHaptics v 3.4.0
---------------------------------------------------------------------------


KNOWN ISSUES in OpenHaptics v 3.0
---------------------------------------------------------------------------


KNOWN ISSUES in OpenHaptics v 2.0.1
---------------------------------------------------------------------------

- For customers using a 1.5 6DOF or 3.0 6DOF device, the gimbal forces in 
the SimpleRigidBody example may be excessively strong when grabbing a block. 
Please use caution when utilizing this sample program with the 6DOF devices.

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




---------------------------------------------------------------------------
©1993-2008 SensAble Technologies, Inc. All rights reserved. ClayTools, 
FreeForm, FreeForm Modeling, FreeForm Modeling Plus, GHOST, HapticExtender,
HapticSound, NetTouch, OpenHaptics, PHANTOM, PHANTOM Desktop, PHANTOM Omni,
QuickHaptics, SensAble, SensAble Technologies, Inc., TextureKiln, and WebTouch 
are trademarks or registered trademarks of SensAble Technologies, Inc. 
Other brand and product names are trademarks of their respective holders.

The system is protected by one or more of the following U.S. Patents:  5,587,937;
5,625,576; 5,898,599; 6,084,587; 6,111,577; 6,191,796; 6,369,834; 6,405,158;
6,417,638; 6,421,048; 6,552,722; 6,671,651; 6,792,398; 6,831,640; 6,853,965; 
6,867,770; and 6,879,315.  Other patents pending.


---------------------------------------------------------------------------


