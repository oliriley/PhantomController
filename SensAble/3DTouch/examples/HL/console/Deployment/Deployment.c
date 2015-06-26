/*****************************************************************************

Copyright (c) 2004 SensAble Technologies, Inc. All rights reserved.

OpenHaptics(TM) toolkit. The material embodied in this software and use of
this software is subject to the terms and conditions of the clickthrough
Development License Agreement.

For questions, comments or bug reports, go to forums at: 
    http://dsc.sensable.com

Module Name:

  Deployment.c

Description: 

  This application shows how to add deployment licensing to an application.

  The deployment license allows end-users to run your application
  without a developer license.  To obtain a deployment license, you send
  an application identifier string to SensAble.  In return, a code
  snippet for use in your application will be returned.  The code
  snippet will enable the deployment license.

  The application identifier string is defined as:

  vendor application

  where vendor is the name of your organization and application is the name of
  the application you developed.

  For example, a company named ABC Software, Inc. who is developing an 
  application named HapticGold would send the following string to 
  SensAble:

  "ABC Software, Inc." HapticsGold

  * Note that strings with spaces must be surrounded by quotes or the spaces 
  must be replaced with another character or removed.

  The returned code snippet is added to your code as shown below.

******************************************************************************/

#include <stdio.h>
#include <assert.h>

#if defined(WIN32)
# include <conio.h>
#else
# include "conio.h"
#endif

#include <HD/hd.h>
#include <HL/hl.h>

#include <HDU/hduError.h>

void HLCALLBACK buttonCB(HLenum event, HLuint object, HLenum thread, 
                       HLcache *cache, void *userdata);

/*******************************************************************************
 Main function.
*******************************************************************************/
int main(int argc, char *argv[])
{
    HHD hHD;
    HHLRC hHLRC;
    HDErrorInfo error;
    HLuint spring;

    /* The code snippet provided to you by SensAble should be executed near 
       application startup.  Once a deployment license has been validated, 
       it will remain in effect until the application is shutdown.
    
       Be sure to place the HD deployment license code before the call 
       to hdInitDevice, and the HL deployment license code before the call 
       to hlCreateContext.
     
       NOTE THAT THE FOLLOWING IS FOR DEMONSTRATION ONLY. THE LICENSES 
       ARE NOT VALID. */

    hdDeploymentLicense(
        "ABC Software, Inc.",
        "HapticsGold",
        "F1312D97ECCC754D5BE4BEE7E831BC27ACF809E9B850D9576F1A856AF70DD3A879B4D3DC7F922BDB2C639DA4A565CA5FC598D8AF34EA010B13A8C232B78F22C");

    hlDeploymentLicense(
        "ABC Software, Inc.",
        "HapticsGold",
        "A653A1EEAFF7B87C952754672FDB1AA16A9035ADE1CFCA6394FE869BAFECE0B7A32251502DDF220B7BA27979695041AE59DCEA007605027D471801F4BF26C24");

    hHD = hdInitDevice(HD_DEFAULT_DEVICE);
    if (HD_DEVICE_ERROR(error = hdGetError())) 
    {
        hduPrintError(stderr, &error, "Failed to initialize haptic device");
        fprintf(stderr, "\nPress any key to quit.\n");
        getch();
        return -1;
    }
    hdMakeCurrentDevice(hHD);    

    hHLRC = hlCreateContext(hHD);
    hlMakeCurrent(hHLRC);
    
    hlDisable(HL_USE_GL_MODELVIEW);

    spring = hlGenEffects(1);

    // Add a callback to handle button down in the collision thread.
    hlAddEventCallback(HL_EVENT_1BUTTONDOWN, HL_OBJECT_ANY, HL_CLIENT_THREAD, 
                       buttonCB, &spring);
    hlAddEventCallback(HL_EVENT_1BUTTONUP, HL_OBJECT_ANY, HL_CLIENT_THREAD, 
                       buttonCB, &spring);

    printf("Press and hold the primary stylus button to feel the spring effect.\n");
    printf("Press any key to quit.\n\n");

    // Run the main loop.
    while (!_kbhit())
    {
        hlBeginFrame();

        // Poll for events. Note that client thread event callbacks get
        // dispatched from within a frame here, so we can safely start/stop
        // effects from the event callback directly.
        hlCheckEvents();

        hlEndFrame();
    }

    hlDeleteEffects(spring, 1);

    hlDeleteContext(hHLRC);
    hdDisableDevice(hHD);

    return 0;
}

/*******************************************************************************
 Creates a spring upon button press.
*******************************************************************************/
void HLCALLBACK buttonCB(HLenum event, HLuint object, HLenum thread, 
                         HLcache *cache, void *userdata)
{
    if (event == HL_EVENT_1BUTTONDOWN)
    {
        HLuint spring = *((HLuint *) userdata); 

        HDdouble anchor[3];
        hlCacheGetDoublev(cache, HL_PROXY_POSITION, anchor);

        hlEffectd(HL_EFFECT_PROPERTY_GAIN, 0.8);
        hlEffectd(HL_EFFECT_PROPERTY_MAGNITUDE, 1.0);
        hlEffectdv(HL_EFFECT_PROPERTY_POSITION, anchor);
        hlStartEffect(HL_EFFECT_SPRING, spring);
    }
    else if (event == HL_EVENT_1BUTTONUP)
    {
        HLuint spring = *((HLuint *) userdata); 

        hlStopEffect(spring);    
    }
}

/******************************************************************************/
