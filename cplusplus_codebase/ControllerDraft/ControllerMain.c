/*****************************************************************************

Copyright (c) 2004 SensAble Technologies, Inc. All rights reserved.

OpenHaptics(TM) toolkit. The material embodied in this software and use of
this software is subject to the terms and conditions of the clickthrough
Development License Agreement.

For questions, comments or bug reports, go to forums at: 
    http://dsc.sensable.com

Module Name:

  AnchoredSpringForce.c

Description: 

  This example shows how to generate a spring force effect that is anchored
  at the position where the user clicks.

*******************************************************************************/

#include <stdio.h>

#if defined(WIN32)
# include <conio.h>
#else
# include "conio.h"
#endif

#include <HD/hd.h>
#include <HDU/hduError.h>
#include <HDU/hduVector.h>

static HDdouble gSpringStiffness = 0.1;
static HDdouble gMaxStiffness = 1.0;
static HDdouble gStiffnessIncrement = 0.025;
///////////////////////////////
/////Stuff Oliver has added////
double dPosCurrentX;
double dPosCurrentY;
double dPosCurrentZ;

double dForceCurrentX;
double dForceCurrentY;
double dForceCurrentZ;

int count;
static int MAX_COUNT = 750;
///////////////////////////////
///////////////////////////////

HDSchedulerHandle gCallbackHandle = 0;

void mainLoop();
HDCallbackCode HDCALLBACK SetSpringStiffnessCallback(void *pUserData);
HDCallbackCode HDCALLBACK AnchoredSpringForceCallback(void *pUserData);

/******************************************************************************
 Main function.
******************************************************************************/
int main(int argc, char* argv[])
{  
    HDErrorInfo error;

    HHD hHD = hdInitDevice(HD_DEFAULT_DEVICE);
    if (HD_DEVICE_ERROR(error = hdGetError()))
    {
        hduPrintError(stderr, &error, "Failed to initialize haptic device");
        fprintf(stderr, "\nPress any key to quit.\n");
        getch();
        return -1;
    }

    printf("Anchored Spring Force Example\n");

    /* Schedule the haptic callback function for continuously monitoring the
       button state and rendering the anchored spring force. */
    gCallbackHandle = hdScheduleAsynchronous(
        AnchoredSpringForceCallback, 0, HD_MAX_SCHEDULER_PRIORITY);

    hdEnable(HD_FORCE_OUTPUT);

    /* Query the max closed loop control stiffness that the device
       can handle. Using a value beyond this limit will likely cause the 
       device to buzz. */
    hdGetDoublev(HD_NOMINAL_MAX_STIFFNESS, &gMaxStiffness);

    /* Start the haptic rendering loop. */
    hdStartScheduler();
    if (HD_DEVICE_ERROR(error = hdGetError()))
    {
        hduPrintError(stderr, &error, "Failed to start scheduler");
        fprintf(stderr, "\nPress any key to quit.\n");
        getch();
        return -1;
    }

    /* Start the main application loop. */
    mainLoop();

    /* Cleanup by stopping the haptics loop, unscheduling the asynchronous
       callback, disabling the device. */
    hdStopScheduler();
    hdUnschedule(gCallbackHandle);
    hdDisableDevice(hHD);

    return 0;
}

////////////////////////////////////
////Stuff Oliver has added//////////	
 void updateUser(hduVector3Dd position, hduVector3Dd force)
 {
	if (position[0] != dPosCurrentX || position[1] != dPosCurrentY || position[2] != dPosCurrentZ)
	{
		dPosCurrentX = position[0];
		dPosCurrentY = position[1];
		dPosCurrentZ = position[2];

		printf("x = %f\n",dPosCurrentX);
		printf("y = %f\n",dPosCurrentY);
		printf("z = %f\n",dPosCurrentZ);
		printf("\n");
	}
	if (force[0] != dForceCurrentX || force[1] != dForceCurrentY || force[2] != dForceCurrentZ)
	{
		dForceCurrentX = force[0];
		dForceCurrentY = force[1];
		dForceCurrentZ = force[2];

		printf("x force = %f\n",dForceCurrentX);
		printf("y force = %f\n",dForceCurrentY);
		printf("z force = %f\n",dForceCurrentZ);
		printf("\n");
	}
}
////////////////////////////////////
////////////////////////////////////

/******************************************************************************
 Main loop.  
 Detects and interprets keypresses.  Monitors and initiates error recovery if 
 necessary.
******************************************************************************/
void mainLoop()
{
    HDdouble stiffness = gSpringStiffness;
    int keypress;

    printf("\nPress button and drag to feel anchored spring force.\n");
    printf("Press +/- to modify spring stiffness or press Q to quit.\n\n");

    printf("Spring stiffness: %.3f N/mm\n", stiffness);

    while (TRUE)
    {       
        /* Check for keyboard input. */
        if (_kbhit())
        {
            keypress = getch();
            keypress = toupper(keypress);
            
            if (keypress == 'Q')
            {
                return;
            }
            else if (keypress == '+' || keypress == '=')
            {
                stiffness += gStiffnessIncrement;
                if (stiffness > gMaxStiffness)
                    stiffness = gMaxStiffness;
                
                    /* Make a thread-safe call to change the spring stiffness 
                       being used for rendering forces. */
                hdScheduleSynchronous(SetSpringStiffnessCallback, &stiffness,
                    HD_DEFAULT_SCHEDULER_PRIORITY);
                
                printf("Spring stiffness: %.3f N/mm\n", stiffness);
            }
            else if (keypress == '-' || keypress == '_')
            {
                stiffness -= gStiffnessIncrement;
                if (stiffness < 0)
                    stiffness = 0;
                
                    /* Make a thread-safe call to change the spring stiffness 
                       being used for rendering forces. */
                hdScheduleSynchronous(SetSpringStiffnessCallback, &stiffness,
                    HD_DEFAULT_SCHEDULER_PRIORITY);            
                
                printf("Spring stiffness: %.3f N/mm\n", stiffness);
            }
            else
            {
                printf("\nInvalid keypress\n");
                printf("Press +/- to modify spring stiffness or press Q to quit.\n\n");
            }
        }

        /* Check if the main scheduler callback has exited. */
        if (!hdWaitForCompletion(gCallbackHandle, HD_WAIT_CHECK_STATUS))
        {
            fprintf(stderr, "\nThe main scheduler callback has exited\n");
            fprintf(stderr, "\nPress any key to quit.\n");
            getch();
            return;
        }
    }
}

/******************************************************************************
 * Main scheduler callback for rendering the anchored spring force.
 *****************************************************************************/
HDCallbackCode HDCALLBACK AnchoredSpringForceCallback(void *pUserData)
{
    static hduVector3Dd anchor;
    static HDboolean bRenderForce = FALSE;
    HDErrorInfo error;

    HDint nCurrentButtons, nLastButtons;
    hduVector3Dd position;
    hduVector3Dd force = { 0, 0, 0 };

    hdBeginFrame(hdGetCurrentDevice());

    hdGetDoublev(HD_CURRENT_POSITION, position);

	hdGetIntegerv(HD_CURRENT_BUTTONS, &nCurrentButtons);
    hdGetIntegerv(HD_LAST_BUTTONS, &nLastButtons);

    if ((nCurrentButtons & HD_DEVICE_BUTTON_1) != 0 &&
        (nLastButtons & HD_DEVICE_BUTTON_1) == 0)
    {
        /* Detected button down */
        memcpy(anchor, position, sizeof(hduVector3Dd));
        bRenderForce = TRUE;
    }
    else if ((nCurrentButtons & HD_DEVICE_BUTTON_1) == 0 &&
             (nLastButtons & HD_DEVICE_BUTTON_1) != 0)

    {
        /* Detected button up */
        bRenderForce = FALSE;

        /* Send zero force to the device, or else it will just continue
           rendering the last force sent */
        hdSetDoublev(HD_CURRENT_FORCE, force);
    }

    if (bRenderForce)
    {
        /* Compute spring force as F = k * (anchor - pos), which will attract
           the device position towards the anchor position */
        hduVecSubtract(force, anchor, position);
        hduVecScaleInPlace(force, gSpringStiffness);
                
        hdSetDoublev(HD_CURRENT_FORCE, force);
    }
//////////////////////////////////////////////
//More stuff oliver has added: call to update position and force display//
	if (count>MAX_COUNT)
	{
		updateUser(position, force);
		count -= MAX_COUNT;
	}
//////////////////////////////////////////////
//////////////////////////////////////////////

    hdEndFrame(hdGetCurrentDevice());

    /* Check if an error occurred while attempting to render the force */
    if (HD_DEVICE_ERROR(error = hdGetError()))
    {
        if (hduIsForceError(&error))
        {
            bRenderForce = FALSE;
        }
        else if (hduIsSchedulerError(&error))
        {
            return HD_CALLBACK_DONE;
        }
    }
	count++;
    return HD_CALLBACK_CONTINUE;
}

/******************************************************************************
 * Scheduler callback for synchronously changing the stiffness value being
   used by the anchored spring force.
 *****************************************************************************/
HDCallbackCode HDCALLBACK SetSpringStiffnessCallback(void *pUserData)
{
    HDdouble *pStiffness = (HDdouble *) pUserData;
        
    gSpringStiffness = *pStiffness;        

    return HD_CALLBACK_DONE;    
}

/*****************************************************************************/
